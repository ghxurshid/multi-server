#ifndef PAGE_H
#define PAGE_H

#include <QQuickItem>
#include <QJsonObject>
#include <QJsonDocument>

#include "server.h"
#include "models.h"
#include "jsonobject.h"

class Page : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(int type READ type WRITE setType NOTIFY typeChanged)
    Q_PROPERTY(bool connectionState READ connectionState WRITE setConnectionState NOTIFY connectionStateChanged)
    Q_PROPERTY(QString recvText READ recvText NOTIFY recvTextChanged)
    Q_PROPERTY(QString sendText READ sendText NOTIFY sendTextChanged)
    Q_PROPERTY(QString jsonSettings READ jsonSettings NOTIFY settingsChanged WRITE setJsonSettings)
    Q_PROPERTY(QObject * networkList READ networkList NOTIFY networkListChanged)
public:
    Page(QQuickItem *parent = nullptr);
    ~Page();

    int type();
    void setType(int type);

    bool connectionState();
    void setConnectionState(bool state);

    QString recvText();
    QString sendText();

    QString jsonSettings();
    void setJsonSettings(QString stt);

    const JsonObject &settings();
    void setSettings(const JsonObject & settings);

    QObject *networkList();

    Q_INVOKABLE bool startServer();
    Q_INVOKABLE void sendData(QString data, QString end);

signals:
    void typeChanged();    
    void recvTextChanged();
    void sendTextChanged();
    void settingsChanged();
    void networkListChanged();
    void connectionStateChanged();

public slots:   
    void serverStarted(QString serverInfo);
    void serverStopped(QString serverInfo);
    void clientConnected(QString clientInfo);
    void clientDisconnected(QString clientInfo);

    void dataReceived(QString data);


protected:
    QString oprintf(QString format, QList<char> args);
    QList<char> listOfArgs();

private:
    int type_ = -1;
    bool connectionState_ = false;

    QString recvText_;
    QString sendText_;

    AbstractServer * server;
    JsonObject settings_;

    NetworkListModel nlModel;
};

#endif // PAGE_H
