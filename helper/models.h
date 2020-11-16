#ifndef MODELS_H
#define MODELS_H

#include <QAbstractListModel>

class NetworkListModel : public QAbstractListModel
{
public:
    enum Roles {
        Ip = Qt::UserRole + 1,
        ModelData
    };

    NetworkListModel(QObject *parent = nullptr);

    virtual int rowCount(const QModelIndex &parent) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual QHash<int, QByteArray> roleNames() const;

private:
    QList<QString> m_data;
};

#endif // MODELS_H
