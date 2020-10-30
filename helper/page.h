#ifndef PAGE_H
#define PAGE_H

#include <QQuickItem> 

#include "server.h"

class Page : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(int type READ type WRITE setType NOTIFY typeChanged)
    Q_PROPERTY(bool connectionState READ connectionState WRITE setConnectionState NOTIFY connectionStateChanged)
    Q_PROPERTY(QString recvText READ recvText NOTIFY recvTextChanged)
    Q_PROPERTY(QString sendText READ sendText NOTIFY sendTextChanged)
public:
    Page(QQuickItem *parent = nullptr);

    int type();
    void setType(int type);

    bool connectionState();
    void setConnectionState(bool state);

    QString recvText();
    QString sendText();

    Q_INVOKABLE bool startServer();

signals:
    void typeChanged();    
    void recvTextChanged();
    void sendTextChanged();
    void connectionStateChanged();

public slots:   
    void serverStarted(QString serverInfo);
    void serverStopped(QString serverInfo);
    void clientConnected(QString clientInfo);
    void clientDisconnected(QString clientInfo);

    void dataReceived(QString data);

private:
    int type_ = 0;
    bool connectionState_ = false;

    QString recvText_;
    QString sendText_;

    AbstractServer * server;
};

#endif // PAGE_H
