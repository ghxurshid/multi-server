#include <QDebug>
#include "page.h"

Page::Page(QQuickItem *parent) : QQuickItem (parent)
{
    server = nullptr;
}


#pragma mark - qproperty definitions

int Page::type()
{
    return type_;
}

void Page::setType(int type)
{    
    type_ = type;

    if (server)
    {
        if (server->started())
        {
            server->stop();
        }

        delete server;
        server = nullptr;
    }

    switch (type) {
        case 0: server = new TcpServer(this);  break;
        case 1: server = new HttpServer(this); break;
        case 2: break;
        case 3: break;
    }

    emit typeChanged();
}

bool Page::connectionState()
{
//    return connectionState_;
    bool conn = false;
    if (server) conn = server->started();
    return conn;
}

void Page::setConnectionState(bool state)
{
    connectionState_ = state;
    emit connectionStateChanged();
}

QString Page::recvText()
{
    return recvText_;
}

QString Page::sendText()
{
    return sendText_;
}

QString Page::settings()
{
    QString res = "{}";
    if (server) {
        auto doc = QJsonDocument(server->settings());
        res = QString(doc.toJson());
    }
    return res;
}

void Page::setSettings(QString settings)
{

}


#pragma mark - public functions

bool Page::startServer()
{
    if (server && server->isValid()) {
        if (server->started()) {
            server->stop();
        } else {
            recvText_.clear();
            sendText_.clear();
            server->start();
        }
    }
    return true;
}

void Page::sendData(QString data)
{qDebug() << Q_FUNC_INFO << data;
    if (server && server->isValid()) {
        auto resp = server->sendData(data);
        sendText_ += resp.message + "<br>";
        emit sendTextChanged();
    }
}

void Page::serverStarted(QString serverInfo)
{
    qDebug() << "serverStarted: " << serverInfo;
    recvText_ += serverInfo + "<br>";
    emit recvTextChanged();
    emit connectionStateChanged();
}

void Page::serverStopped(QString serverInfo)
{
    qDebug() << "serverStopped: " << serverInfo;    
    recvText_ += serverInfo + "<br>";
    emit recvTextChanged();
    emit connectionStateChanged();
}

void Page::clientConnected(QString clientInfo)
{
    qDebug() << "clientConnected: " << clientInfo;
    recvText_ += clientInfo + "<br>";
    emit recvTextChanged();
}

void Page::clientDisconnected(QString clientInfo)
{
    qDebug() << "clientDisconnected: " << clientInfo;
    recvText_ += clientInfo + "<br>";
    emit recvTextChanged();
}

void Page::dataReceived(QString data)
{
    qDebug() << "dataReceived: " << data;
    recvText_ += data + "<br>";
    emit recvTextChanged();
}


