#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QHttpServer>
#include <QJsonObject>
#include <QSerialPort>

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

signals:
    void serverStarted(QString serverInfo);
    void serverStopped(QString serverInfo);
    void clientConnected(QString clientInfo);
    void clientDisconnected(QString clientInfo);

    void dataReceived(QString data);

public slots:

private: 

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

class HttpServer : public AbstractServer
{
    Q_OBJECT
public:
    explicit HttpServer(QObject *parent = nullptr);
    ~HttpServer() override;

    void start() override;
    void stop() override;

    Response sendData(QString data) override;
    bool isValid() override;
    bool started() override;

signals:

public slots:

private:
    QString _patternText;
    QHttpServer * server;
    bool _started = false;
};

class CommServer : public AbstractServer
{
    Q_OBJECT
public:
    explicit CommServer(QObject *parent = nullptr);
    ~CommServer() override;

    void start() override;
    void stop() override;

    Response sendData(QString data) override;
    bool isValid() override;
    bool started() override;

signals:

public slots:
    void readyRead();

private:
    QSerialPort * server;
};

#endif // SERVER_H
