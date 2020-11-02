#include "server.h"
#include <QRegExp>



#pragma mark - abstract server

AbstractServer::AbstractServer(QObject *parent) : QObject(parent)
{
    connect(this, SIGNAL(serverStarted(QString)), parent, SLOT(serverStarted(QString)));
    connect(this, SIGNAL(serverStopped(QString)), parent, SLOT(serverStopped(QString)));
    connect(this, SIGNAL(clientConnected(QString)), parent, SLOT(clientConnected(QString)));
    connect(this, SIGNAL(clientDisconnected(QString)), parent, SLOT(clientDisconnected(QString)));

    connect(this, SIGNAL(dataReceived(QString)), parent, SLOT(dataReceived(QString)));
}

const QJsonObject &AbstractServer::settings()
{
    return this->settings_;
}

void AbstractServer::setSettings(const QJsonObject &settings)
{
    this->settings_ = settings;
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
    qDebug() << "Hello";
    if (server)
    {
        auto tcp = settings()["tcp"];
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
                 qDebug() << "socket is" << (socket->isReadable() ? " " : " not ") << "readable";
                 qDebug() << "socket is" << (socket->isWritable() ? " " : " not ") << "writable";
                qint64 size = socket->write(data.toLocal8Bit());
                qDebug() << size;
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
{   qDebug() << Q_FUNC_INFO;
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


#pragma mark - comm server


#pragma mark - bluetooth server
