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
                    resp.message = QString("<b>Sended data[%1]</b>: ").arg(QString::number(size)) + data;
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

HttpServer::HttpServer(QObject *parent) : AbstractServer (parent)
{
    server = new QHttpServer();

    server->route("/", [this]() {        
        return _patternText;
    });

    server->route("/", [this]() {
        return _patternText;
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
            resp.message = QString("<b>Will send data[%1]</b>: ").arg(QString::number(data.size())) + data;
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


#pragma mark - bluetooth server
