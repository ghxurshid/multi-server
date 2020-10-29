#include <QDebug>
#include "page.h"

Page::Page(QQuickItem *parent) : QQuickItem (parent)
{

}


#pragma mark - qproperty definitions

int Page::type()
{
    return type_;
}

void Page::setType(int type)
{    
    type_ = type;

    if (server && server->started()) {
        server->stop();
        server->deleteLater();
    }

    switch (type) {
        case 0: server = new TcpServer(); break;
        case 1: break;
        case 2: break;
        case 3: break;
    }

    emit typeChanged();
}

bool Page::connectionState()
{
    return connectionState_;
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


}


