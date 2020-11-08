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
    auto contentPath = QString(PROJECT_PATH);
    qDebug() << contentPath;
    QFile jsonFile1(contentPath + QString("/json1.txt"));
    QFile jsonFile2(contentPath + QString("/json2.txt"));

    if (jsonFile1.open(QIODevice::ReadOnly)) {
        if (jsonFile2.open(QIODevice::ReadOnly)) {
            QJsonDocument doc1 = QJsonDocument::fromJson(jsonFile1.readAll());
            QJsonDocument doc2 = QJsonDocument::fromJson(jsonFile2.readAll());

            JsonObject obj1(doc1.object());
            JsonObject obj2(doc2.object());

            qDebug() << obj1;
            qDebug() << obj2;
            qDebug() << "- - - - - - - - - - ";
            obj1.append(obj2);
            qDebug() << obj1;
        } else {
            qDebug() << "json2.txt can't open!";
        }
    } else {
        qDebug() << "json1.txt can't open!";
    }



    return app.exec();
}
