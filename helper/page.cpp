#include <QDebug>
#include "page.h"

Page::Page(QQuickItem *parent) : QQuickItem (parent)
{
    server = nullptr;

    QFile file(QCoreApplication::applicationDirPath() + "/config.cfg");
    if (file.open(QIODevice::ReadOnly)) {
        auto doc = QJsonDocument::fromJson(file.readAll());
        file.close();
        if (!doc.isEmpty()) {
            this->settings_ = doc.object();
        }
    }

}

Page::~Page()
{
    if (server) {
        auto stt = settings();
        auto doc = QJsonDocument(stt);

        QFile file(QCoreApplication::applicationDirPath() + "/config.cfg");
        if (file.open(QIODevice::WriteOnly)) {
            file.write(doc.toJson(QJsonDocument::Indented));
            file.close();
        }

        delete server;
        server = nullptr;
    }
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

QString Page::jsonSettings()
{
    QJsonDocument doc(settings_);
    return doc.toJson(QJsonDocument::Compact);
}

void Page::setJsonSettings(QString stt)
{
    auto doc = QJsonDocument::fromJson(stt.toLocal8Bit());
    if (!doc.isEmpty()) {
        auto objStt = JsonObject(doc.object());
        this->settings_.update(objStt);
        emit settingsChanged();
        qDebug() << settings_;
    }
}

const JsonObject &Page::settings()
{
    return this->settings_;
}

void Page::setSettings(const JsonObject &settings)
{

    auto stt = settings;
    this->settings_.update(stt);
    emit settingsChanged();
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


