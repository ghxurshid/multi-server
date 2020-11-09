#include "jsonobject.h"

JsonObject::JsonObject() : QJsonObject ()
{

}

JsonObject::JsonObject(const QJsonObject &other) : QJsonObject (other)
{

}

void JsonObject::append(JsonObject &json)
{
    for (auto key : json.keys()) {
        QJsonValue v1 = value(key);
        QJsonValue v2 = json.value(key);

        insert(key, helper(v1, v2));
    }
}

QJsonValue JsonObject::helper(QJsonValue & val1, QJsonValue & val2)
{
    auto t1 = val1.type();
    auto t2 = val2.type();

    if (t1 != t2) return val2;
    else
    if (t1 == QJsonValue::Array &&
            t2 == QJsonValue::Array) {
        auto arr1 = val1.toArray();
        auto arr2 = val2.toArray();
        for (int i = 0; i < arr2.count(); i ++) {
            arr1.append(arr2[i]);
        }
        return QJsonValue(arr1);
    }
    else
    if (t1 == QJsonValue::Object &&
            t2 == QJsonValue::Object) {
        auto obj1 = val1.toObject();
        auto obj2 = val2.toObject();

        for (auto key : obj2.keys()) {
            QJsonValue v1 = obj1[key];
            QJsonValue v2 = obj2[key];

            obj1[key] = helper(v1, v2);
        }
        return QJsonValue(obj1);
    }
    return QJsonValue(val2);
}
