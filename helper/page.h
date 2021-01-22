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
    Q_PROPERTY(QObject * commPortList READ commPortList NOTIFY commPortListChanged)
    Q_PROPERTY(QObject * networkList READ networkList NOTIFY networkListChanged)
    Q_PROPERTY(QObject * leftArgList READ leftArgList NOTIFY leftArgListChanged)
    Q_PROPERTY(QObject * rightArgList READ rightArgList NOTIFY rightArgListChanged)
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

    QObject *commPortList();
    QObject *networkList();
    QObject *leftArgList();
    QObject *rightArgList();

    Q_INVOKABLE bool startServer();
    Q_INVOKABLE void sendData(QString data, QString end);

signals:
    void typeChanged();    
    void recvTextChanged();
    void sendTextChanged();
    void settingsChanged();
    void commPortListChanged();
    void networkListChanged();
    void leftArgListChanged();
    void rightArgListChanged();
    void connectionStateChanged();


public slots:   
    void serverStarted(QString serverInfo);
    void serverStopped(QString serverInfo);
    void clientConnected(QString clientInfo);
    void clientDisconnected(QString clientInfo);

    void dataReceived(QString data);


protected:
    QString oprintf(QString format, QStringList args);

private:
    int type_ = -1;
    bool connectionState_ = false;

    QString recvText_;
    QString sendText_;

    AbstractServer * server;
    JsonObject settings_;

    NetworkListModel m_networklist;
    CommPortListModel m_commPortList;
    ArgumentsListModel m_leftArgList;
    ArgumentsListModel m_rightArgList;
};

#endif // PAGE_H
