#ifndef JSONOBJECT_H
#define JSONOBJECT_H

#include <QObject>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>

class JsonObject : public QJsonObject
{
public:
    JsonObject();
    JsonObject(const QJsonObject &other);

    void update(JsonObject & json);

private:
    QJsonValue helper(QJsonValue &val1, QJsonValue &val2);
};

#endif // JSONOBJECT_H
