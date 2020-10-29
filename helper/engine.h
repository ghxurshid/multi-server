#ifndef ENGINE_H
#define ENGINE_H

#include <QQuickItem>
#include <QQmlApplicationEngine>

class Engine : public QQmlApplicationEngine
{
    Q_OBJECT
public:
    Engine(QObject *parent = nullptr);

signals:

public slots:
};

#endif // ENGINE_H
