#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QIODevice>
#include <QFile>
#include <QJsonDocument>

#include "helper/engine.h"
#include "helper/jsonobject.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    Engine engine;
    engine.load(QUrl(QStringLiteral("qrc:/resources/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
