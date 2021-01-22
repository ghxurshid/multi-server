#include <QDebug>
#include "page.h"
#include "helper/printer.h"

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
    oprintf(QString(), {});
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
        case 2: server = new CommServer(this); break;
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

QObject *Page::commPortList()
{
    return &m_commPortList;
}

QObject *Page::networkList()
{
    return &m_networklist;
}

QObject *Page::leftArgList()
{
    return &m_leftArgList;
}

QObject *Page::rightArgList()
{
    return &m_rightArgList;
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

void Page::sendData(QString data, QString end)
{
    if (server && server->isValid()) {
        QString mData = oprintf(data, m_rightArgList.listOfArgs());
        char c = static_cast<char>(end.toUInt());
        mData.append(QChar(c));
        auto resp = server->sendData(mData);
        sendText_ += resp.message + "<br>";
        emit sendTextChanged();
    } 
}

void Page::serverStarted(QString serverInfo)
{    
    //recvText_ += serverInfo + "<br>";
    emit recvTextChanged();
    emit connectionStateChanged();
}

void Page::serverStopped(QString serverInfo)
{    
    //recvText_ += serverInfo + "<br>";
    emit recvTextChanged();
    emit connectionStateChanged();
}

void Page::clientConnected(QString clientInfo)
{    
    //recvText_ += clientInfo + "<br>";
    emit recvTextChanged();
}

void Page::clientDisconnected(QString clientInfo)
{    
    //recvText_ += clientInfo + "<br>";
    emit recvTextChanged();
}

void Page::dataReceived(QString data)
{
    m_rightArgList.match(data);
    recvText_ += data + "<br>";
    emit recvTextChanged();
}


#pragma mark - protected functions

QString Page::oprintf(QString format, QStringList args)
{
    Printer prn;
//    prn.printf ("Message: %i %c %s", {"123", "65", "Hello"});
//    prn.printf ("Characters: %c %c", {"97", "65"});
//    prn.printf ("Decimals: %d %ld", {"1977", "650000L"});
//    prn.printf ("Preceding with blanks: %10d", {"1977"});
//    prn.printf ("Preceding with zeros: %010d", {"1977"});
//    prn.printf ("Some different radices: %d %x %o %#x %#o", {"100", "100", "100", "100", "100"});
//    prn.printf ("floats: %4.2f %+.0e %E", {"3.1416", "3.1416", "3.1416"});
//    prn.printf ("Width trick: %*d", {"5", "10"});
//    prn.printf ("%s", {"A string"});
//    prn.printf ("Precision: %9.5i", {"5"});
    return prn.sprintf(format.toLocal8Bit().data(), args);
}
