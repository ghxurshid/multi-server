#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QJsonObject>

class AbstractServer : public QObject
{
    Q_OBJECT
public:
    explicit AbstractServer(QObject *parent = nullptr);

    struct Response {
        bool status = false;
        QString message = QString("");
    };

    enum class Type {
        TCP,
        HTTP,
        COMM,
        Bluetooth,
        unknown
    };

    virtual void start() = 0;
    virtual void stop()  = 0;

    virtual Response sendData(QString data) = 0;
    virtual bool isValid() = 0;
    virtual bool started() = 0; 

    const QJsonObject &settings();
    void setSettings(const QJsonObject & settings);



signals:
    void serverStarted(QString serverInfo);
    void serverStopped(QString serverInfo);
    void clientConnected(QString clientInfo);
    void clientDisconnected(QString clientInfo);

    void dataReceived(QString data);

public slots:

private:
    QJsonObject settings_;

};

class TcpServer : public AbstractServer
{
    Q_OBJECT
public:
    explicit TcpServer(QObject *parent = nullptr);
    ~TcpServer() override;

    void start() override;
    void stop() override;

    Response sendData(QString data) override;
    bool isValid() override;
    bool started() override;

signals:

public slots:
    void newConnection();
    void readyRead();
    void slotDisconnected();

private:
    QTcpServer * server;
    QTcpSocket * socket;
};

#endif // SERVER_H
