#include "server.h"
#include <QRegExp>



#pragma mark - abstract server

AbstractServer::AbstractServer(QObject *parent) : QObject(parent)
{

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
}

TcpServer::~TcpServer()
{
    stop();
    if (server) server->deleteLater();
}

void TcpServer::start()
{
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
            auto message = QString("TcpServer is started.");
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
        auto message = QString("TcpServer closed successfully");
        emit serverStopped(message);
    }
}

AbstractServer::Response TcpServer::sendData(QString data)
{
    Response resp;
    if (socket)
    {
        qint64 size = socket->write(data.toLocal8Bit());
        if (size == data.size())
        {
            resp.status = true;
            resp.message = QString("Sended data[%1]: ").arg(size) + data;            
        }
        else {
            resp.message = QString("Can't send data:(");
        }
    }
    else
    {        
        resp.message = QString("Can't send data:( - No one client connected!");
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

        connect(socket, SIGNAL(clientDisconnected()), this, SLOT(slotDisconnected()));
        connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));

        emit clientConnected(QString("Client connected!"));
    }
}

void TcpServer::readyRead()
{
    if (!socket || !socket->isReadable()) return;

    emit dataReceived(QString(socket->readAll()));
}

void TcpServer::slotDisconnected()
{
    if (socket) socket->deleteLater();
    emit clientDisconnected(QString("Client disconnected"));
}


#pragma mark - http server


#pragma mark - comm server


#pragma mark - bluetooth server
