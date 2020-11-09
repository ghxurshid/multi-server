#ifndef PAGE_H
#define PAGE_H

#include <QQuickItem>
#include <QJsonObject>
#include <QJsonDocument>

#include "server.h"

class Page : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(int type READ type WRITE setType NOTIFY typeChanged)
    Q_PROPERTY(bool connectionState READ connectionState WRITE setConnectionState NOTIFY connectionStateChanged)
    Q_PROPERTY(QString recvText READ recvText NOTIFY recvTextChanged)
    Q_PROPERTY(QString sendText READ sendText NOTIFY sendTextChanged)
    Q_PROPERTY(QString settings READ settings NOTIFY settingsChanged WRITE setSettings)
public:
    Page(QQuickItem *parent = nullptr);
    ~Page();

    int type();
    void setType(int type);

    bool connectionState();
    void setConnectionState(bool state);

    QString recvText();
    QString sendText();

    QString settings();
    void setSettings(QString settings);

    Q_INVOKABLE bool startServer();
    Q_INVOKABLE void sendData(QString data);



signals:
    void typeChanged();    
    void recvTextChanged();
    void sendTextChanged();
    void settingsChanged();
    void connectionStateChanged();



public slots:   
    void serverStarted(QString serverInfo);
    void serverStopped(QString serverInfo);
    void clientConnected(QString clientInfo);
    void clientDisconnected(QString clientInfo);

    void dataReceived(QString data);

private:
    int type_ = -1;
    bool connectionState_ = false;

    QString recvText_;
    QString sendText_;

    AbstractServer * server;
};

#endif // PAGE_H
