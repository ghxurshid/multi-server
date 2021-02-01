#include <QRegExp>
#include "server.h"
#include "page.h"

#pragma mark - abstract server

AbstractServer::AbstractServer(QObject *parent) : QObject(parent)
{
    connect(this, SIGNAL(serverStarted(QString)), parent, SLOT(serverStarted(QString)));
    connect(this, SIGNAL(serverStopped(QString)), parent, SLOT(serverStopped(QString)));
    connect(this, SIGNAL(clientConnected(QString)), parent, SLOT(clientConnected(QString)));
    connect(this, SIGNAL(clientDisconnected(QString)), parent, SLOT(clientDisconnected(QString)));

    connect(this, SIGNAL(dataReceived(QString)), parent, SLOT(dataReceived(QString)));
}


#pragma mark - tcp server

TcpServer::TcpServer(QObject *parent) : AbstractServer (parent)
{    
    server = new QTcpServer();
    server->setMaxPendingConnections(1);
    socket = nullptr;

    connect(server, SIGNAL(newConnection()), this, SLOT(newConnection()));
}

TcpServer::~TcpServer()
{
    stop();
    if (server) {
        delete server;
        server = nullptr;
    }
}

void TcpServer::start()
{    
    if (server)
    {
        auto parent_ = reinterpret_cast<Page*>(parent());
        auto srv = parent_->settings()["server"];
        auto tcp = srv.isObject() ? srv["tcp"] : JsonObject();
        auto adr = tcp.isObject() ? tcp["ip"  ].toString() : QString("");
        auto prt = tcp.isObject() ? tcp["port"].toString() : QString("");

        QRegExp ipReg("\\d{1,3}.\\d{1,3}.\\d{1,3}.\\d{1,3}");
        QRegExp portReg("\\d+");

        auto address = ipReg.exactMatch(adr) ? QHostAddress(adr) : QHostAddress(QHostAddress::Any);
        auto port    = portReg.exactMatch(prt) ? quint16(prt.toLong()) : quint16(0);

        if (server->listen(address, port))
        {
            auto message = QString("<font color=\"#00E300\"><b>TcpServer</b> is started: ") + server->serverAddress().toString() + ":" + QString::number(server->serverPort()) + QString("</font>");
            emit serverStarted(message);
        }
    }
}

void TcpServer::stop()
{
    if (socket && socket->state() == QAbstractSocket::ConnectedState) {
        socket->abort();
    }

    if (server && server->isListening()) {
        server->close();
        auto message = QString("<font color=\"#FFFF00\"><b>TcpServer</b> stopped!</font>");
        emit serverStopped(message);
    }
}

AbstractServer::Response TcpServer::sendData(QString data)
{
    Response resp;

    if (!server) {
        resp.message = QString("<font color=\"#FFFF00\">Can't send data:( - TcpServer not valid!</font>");
    } else {
        if (!server->isListening()) {
            resp.message = QString("<font color=\"#FFFF00\">Can't send data:( - TcpServer is not started!</font>");
        } else {
            if (socket)
            {                
                qint64 size = socket->write(data.toLocal8Bit());                
                if (size == data.size())
                {
                    resp.status = true;
                    resp.message = /*QString("<b>Sended data[%1]</b>: ").arg(QString::number(size)) +*/ data;
                }
                else {
                    resp.message = QString("<font color=\"#FFFF00\">Can't send data:( - ") + socket->errorString() + QString("</font>");
                }
            }
            else
            {
                resp.message = QString("<font color=\"#FFFF00\">Can't send data:( - No one client connected!</font>");
            }
        }
    }

    return resp;
}

bool TcpServer::isValid()
{
    return !server ? false : true;
}

bool TcpServer::started()
{
    bool _started = false;
    if (server && server->isListening()) _started = true;
    return _started;
}

void TcpServer::newConnection()
{
    if (server && server->hasPendingConnections())
    {
        socket = server->nextPendingConnection();

        connect(socket, SIGNAL(disconnected()), this, SLOT(slotDisconnected()));
        connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));

        emit clientConnected(QString("<font color=\"#00E300\">Client connected: [%1:%2]</font>").arg(socket->peerAddress().toString(), QString::number(socket->peerPort())));
    }
}

void TcpServer::readyRead()
{
    if (!socket || !socket->isReadable()) return;

    emit dataReceived(QString(socket->readAll()));
}

void TcpServer::slotDisconnected()
{
    if (socket) {
        socket->deleteLater();
        socket = nullptr;
    }
    emit clientDisconnected(QString("<font color=\"#FFFF00\">Client disconnected</font>"));
}


#pragma mark - http server

static inline QString host(const QHttpServerRequest &request)
{
    return request.headers()[QStringLiteral("Host")].toString();
}

HttpServer::HttpServer(QObject *parent) : AbstractServer (parent)
{
    server = new QHttpServer();

    auto requestHnd = [] (const QHttpServerRequest &request) {
        qDebug() <<  request.value(QByteArray("batv"));
        qDebug() <<  request.url();
        qDebug() <<  request.query().toString();
        qDebug() <<  request.method();
        qDebug() <<  request.headers();
        qDebug() <<  request.body();
        qDebug() <<  request.remoteAddress();
    };

    server->route("/", [this, requestHnd](const QHttpServerRequest &request) {
        requestHnd (request);
        return _patternText + "A";
    });

    server->route("/v1.0/operator/device/location", [this, requestHnd](const QHttpServerRequest &request) {
        QString response = QString("\n- - - - - - - - - - ") + QTime::currentTime().toString() + QString(" - - - - - - - - - -<br>");
        response.append(QString("<b>URL:</b> ") + request.url().toString() + QString("<br>"));
        response.append(QString("<b>Query:</b> ") + request.query().toString(QUrl::FullyDecoded) + QString("<br>"));
        response.append(QString("<b>Client address:</b> ") + request.remoteAddress().toString() + QString("<br>"));
        response.append(QString("<b>Method:</b> ") + QVariant::fromValue(request.method()).toString() + QString("<br>"));
        response.append(QString("<b>Body:</b> ") + QString(request.body()) + QString("<br>"));
        response.append(QString("<b>Headers:</b><ul>"));

        QMapIterator<QString, QVariant> head(request.headers());
        while (head.hasNext()) {
            head.next();
            response.append(QString("<li>") + head.key() + ": <i>" + head.value().toString() + "</i></li>");
        }

        response.append(QString("</ul>- - - - - - - - - - - - - - - - - - - - - - -<br>"));
        requestHnd (request);
        emit this->dataReceived(response);

        return _patternText;
    });

    server->route("/query", [requestHnd] (const QHttpServerRequest &request) {
        requestHnd (request);
        return QString("%1/query/").arg(host(request));
    });

    server->route("/query/", [] (qint32 id, const QHttpServerRequest &request) {
        return QString("%1/query/%2").arg(host(request)).arg(id);
    });

    server->route("/query/<arg>/log", [] (qint32 id, const QHttpServerRequest &request) {
        return QString("%1/query/%2/log").arg(host(request)).arg(id);
    });

    server->route("/query/<arg>/log/", [] (qint32 id, float threshold,
                                              const QHttpServerRequest &request) {
        return QString("%1/query/%2/log/%3").arg(host(request)).arg(id).arg(threshold);
    });

    server->route("/user/", [] (const qint32 id) {
        return QString("User %1").arg(id);
    });

    server->route("/user/<arg>/detail", [] (const qint32 id) {
        return QString("User %1 detail").arg(id);
    });

    server->route("/user/<arg>/detail/", [] (const qint32 id, const qint32 year) {
        return QString("User %1 detail year - %2").arg(id).arg(year);
    });

    server->route("/json/", [] {
        return QJsonObject{
            {
                {"key1", "1"},
                {"key2", "2"},
                {"key3", "3"}
            }
        };
    });

    server->route("/assets/<arg>", [] (const QUrl &url) {
        return QHttpServerResponse::fromFile(QStringLiteral(":/assets/%1").arg(url.path()));
    });

    server->route("/remote_address", [](const QHttpServerRequest &request) {
        return request.remoteAddress().toString();
    });
}

HttpServer::~HttpServer()
{
    stop();
    if (server) {
        delete server;
        server = nullptr;
    }
}

void HttpServer::start()
{
    if (server)
    {
        auto parent_ = reinterpret_cast<Page*>(parent());
        auto srv = parent_->settings()["server"];
        auto tcp = srv.isObject() ? srv["http"] : JsonObject();
        auto adr = tcp.isObject() ? tcp["ip"  ].toString() : QString("");
        auto prt = tcp.isObject() ? tcp["port"].toString() : QString("");

        QRegExp ipReg("\\d{1,3}.\\d{1,3}.\\d{1,3}.\\d{1,3}");
        QRegExp portReg("\\d+");

        auto address = ipReg.exactMatch(adr) ? QHostAddress(adr) : QHostAddress(QHostAddress::Any);
        auto port    = portReg.exactMatch(prt) ? quint16(prt.toLong()) : quint16(0);

        if (server->listen(address, port))
        {
            auto message = QString("<font color=\"#00E300\"><b>HttpServer</b> is started: ") + address.toString() + ":" + QString::number(port) + QString("</font>");
            _started = true;
            emit serverStarted(message);


        }
    }
}

void HttpServer::stop()
{
    if (server && _started) {
        for (auto srv : server->servers()) {
            if (srv && srv->isListening()) {
                srv->close();
            }
        }
        _started = false;
        auto message = QString("<font color=\"#FFFF00\"><b>HttpServer</b> stopped!</font>");
        emit serverStopped(message);
    }
}

AbstractServer::Response HttpServer::sendData(QString data)
{
    Response resp;

    if (!server) {
        resp.message = QString("<font color=\"#FFFF00\">Can't send data:( - HttpServer not valid!</font>");
    } else {
        if (!_started) {
            resp.message = QString("<font color=\"#FFFF00\">Can't send data:( - HttpServer is not started!</font>");
        } else {
            _patternText = data;
            resp.status = true;
            resp.message = /*QString("<b>Will send data[%1]</b>: ").arg(QString::number(data.size())) +*/ data;
        }
    }

    return resp;
}

bool HttpServer::isValid()
{
    return !server ? false : true;
}

bool HttpServer::started()
{
    return _started;
}


#pragma mark - comm server

CommServer::CommServer(QObject *parent) : AbstractServer (parent)
{
    server = new QSerialPort();
    connect(server, SIGNAL(readyRead()), this, SLOT(readyRead()));
}

CommServer::~CommServer()
{
    stop();
    if (server) {
        delete server;
        server = nullptr;
    }
}

void CommServer::start()
{
    if (server)
    {
        auto parent_ = reinterpret_cast<Page*>(parent());
        auto serv = parent_->settings()["server"];
        auto comm = serv.isObject() ? serv["comm"] : JsonObject();
        auto name = comm.isObject() ? comm["portName"].toString() : QString("");
        auto bdrt = comm.isObject() ? comm["portBoudRate"].toString() : QString("");

        QRegExp nameReg("COM\\d{1,3}");
        QRegExp bdrtReg("\\d+");

        auto portName     = nameReg.exactMatch(name) ? name : QString("");
        auto portBoudRate = bdrtReg.exactMatch(bdrt) ? qint32(bdrt.toLong()) : qint32(9600);

        server->setPortName(portName);
        server->setBaudRate(portBoudRate); 

        if (server->open(QIODevice::ReadWrite))
        {
            auto message = QString("<font color=\"#00E300\"><b>COMM Server</b> is started: [") + portName + ":" + QString::number(portBoudRate) + QString("]</font>");
            emit serverStarted(message);
        }
        else
        {
            auto message = QString("<font color=\"#E30000\"><b>COMM Server</b> is not started: [" + server->errorString() + "]</font>");
            emit serverStarted(message);
        }
    }
}

void CommServer::stop()
{
    if (server && server->isOpen()) {
        server->close();
        auto message = QString("<font color=\"#FFFF00\"><b>Comm Server</b> stopped!</font>");
        emit serverStopped(message);
    }
}

AbstractServer::Response CommServer::sendData(QString data)
{
    Response resp;

    if (!server) {
        resp.message = QString("<font color=\"#FFFF00\">Can't send data:( - Comm server not valid!</font>");
    } else {
        if (!started()) {
            resp.message = QString("<font color=\"#FFFF00\">Can't send data:( - Comm server is not started!</font>");
        } else {
            server->write(data.toLocal8Bit());

            resp.status = true;
            resp.message = /*QString("<b>Sended data[%1]</b>: ").arg(QString::number(data.size())) +*/ data;
        }
    }

    return resp;
}

bool CommServer::isValid()
{
    return !server ? false : true;
}

bool CommServer::started()
{
    qDebug() << server->isOpen();
    return server && server->isOpen();
}

void CommServer::readyRead()
{
    if (!server || !server->isReadable()) return;

    emit dataReceived(QString(server->readAll()));
}

#pragma mark - bluetooth server
