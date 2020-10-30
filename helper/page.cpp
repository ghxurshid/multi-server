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
        case 0: server = new TcpServer(this); break;
        case 1: break;
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


#pragma mark - public functions

bool Page::startServer()
{
    recvText_.clear();
    sendText_.clear();

    if (server) server->start();
}

void Page::serverStarted(QString serverInfo)
{
    qDebug() << "serverStarted: " << serverInfo;
    emit connectionStateChanged();
}

void Page::serverStopped(QString serverInfo)
{
    qDebug() << "serverStopped: " << serverInfo;
}

void Page::clientConnected(QString clientInfo)
{
    qDebug() << "clientConnected: " << clientInfo;
}

void Page::clientDisconnected(QString clientInfo)
{
    qDebug() << "clientDisconnected: " << clientInfo;
}

void Page::dataReceived(QString data)
{
    qDebug() << "dataReceived: " << data;
}


