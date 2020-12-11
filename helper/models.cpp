#include "models.h"
#include <QNetworkInterface>


#pragma mark - NetworkListModel definitions

NetworkListModel::NetworkListModel(QObject *parent) : QAbstractListModel (parent)
{
    auto addrList = QNetworkInterface::allAddresses();
    if (addrList.size() > 0) {
        for (auto addr : addrList) {
            auto addr4 = QHostAddress(addr.toIPv4Address());
            if (addr4 != QHostAddress("0.0.0.0"))
                m_data.append(addr4.toString());
        }
    }
}

QHash<int, QByteArray> NetworkListModel::roleNames() const
{
    QHash<int, QByteArray> roles = QAbstractListModel::roleNames();
    roles[Ip] = "ip";
    roles[ModelData] = "modelData";
    return roles;
}

int NetworkListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_data.size() + 1;
}

QVariant NetworkListModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    int size = m_data.size() + 1;
    if (!index.isValid() || row >= size) {
        return QVariant();
    }

    if (row == 0) {
        return QVariant("All");
    } else {
        if (role == Roles::ModelData)
        {
            return QVariant(m_data[row - 1]);
        }
    }
    return QVariant();
}


#pragma mark - ArgumentsListModel definitions

ArgumentsListModel::ArgumentsListModel(QObject *parent) : QAbstractListModel (parent)
{

}

ArgumentsListModel::~ArgumentsListModel()
{

}

QHash<int, QByteArray> ArgumentsListModel::roleNames() const
{
    QHash<int, QByteArray> roles = QAbstractListModel::roleNames();
    roles[Title] = "title";
    roles[Selected] = "selected";
    return roles;
}

int ArgumentsListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_data.size() + 1;
}

QVariant ArgumentsListModel::data(const QModelIndex &index, int role) const
{qDebug() << Q_FUNC_INFO;
    int row = index.row();
    int size = m_data.size() + 1;
    if (!index.isValid() || row >= size) {
        return QVariant();
    }

    if (row == m_data.size()) {
        return QVariant("");
    } else {
        if (role == Roles::Title)
        {
            return QVariant(m_data[row].title);
        }
    }
    return QVariant();
}

bool ArgumentsListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    Q_UNUSED(index)
    Q_UNUSED(value)
    Q_UNUSED(role)
    qDebug() << value.toString();
    if (!hasIndex(index.row(), index.column(), index.parent()) || !value.isValid())
        return false;

    Item &item = m_data[index.row()];
    if (role == Title) item.title = value.toString();
    else if (role == Selected) item.selected = value.toBool();
    else return false;

    emit dataChanged(index, index, { role } );

    return true ;
}

void ArgumentsListModel::move(int from, int to)
{   
    QModelIndex index;
    if (beginMoveRows(index, from, from, index, to > from ? to + 1 : to)) {
            m_data.move(from, to);
            endMoveRows();
    }
}

void ArgumentsListModel::insert(int index, QString data)
{
    qDebug() << data;
    beginInsertRows(QModelIndex(), index, index); //notify views and proxy models that a line will be inserted
    m_data.insert(index, {data, false}); // do the modification to the model data
    endInsertRows();

}

void ArgumentsListModel::remove(int index)
{
    beginRemoveRows(QModelIndex(), index, index); //notify views and proxy models that a line will be deleted
    m_data.removeAt(index); // do the modification to the model data
    endRemoveRows();
}
