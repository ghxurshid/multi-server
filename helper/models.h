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

class ArgumentsListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum Roles {
        Title = Qt::UserRole + 1,
        Selected
    };

    struct Item {
        QString title;
        bool selected;
    };

    ArgumentsListModel(QObject *parent = nullptr);
    ~ArgumentsListModel();

    virtual int rowCount(const QModelIndex &parent) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    virtual QHash<int, QByteArray> roleNames() const;

    Q_INVOKABLE void move(int from, int to);
    Q_INVOKABLE void insert(int index, QString data);
    Q_INVOKABLE void remove(int index);

    QList<char> listOfArgs();
    void match(const QString data);
private:
    QList<Item> m_data;
};

#endif // MODELS_H
