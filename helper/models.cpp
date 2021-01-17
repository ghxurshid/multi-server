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


#pragma mark - CommPortListModel definitions

CommPortListModel::CommPortListModel(QObject *parent) : QAbstractListModel (parent)
{
    m_data = QSerialPortInfo::availablePorts();
}

QHash<int, QByteArray> CommPortListModel::roleNames() const
{
    QHash<int, QByteArray> roles = QAbstractListModel::roleNames();
    roles[ModelData] = "modelData";
    return roles;
}

int CommPortListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_data.size() + 1;
}

QVariant CommPortListModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    int size = m_data.size() + 1;
    if (!index.isValid() || row >= size) {
        return QVariant();
    }

    if (row == 0) {
        return QVariant("None");
    } else {
        if (role == Roles::ModelData) return QVariant(m_data[row - 1].portName());
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
{
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
        else if (role == Roles::Selected)
        {
            return  QVariant(m_data[row].selected);
        }
    }
    return QVariant();
}

bool ArgumentsListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
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
    beginInsertRows(QModelIndex(), index, index);
    m_data.insert(index, {data, false});
    endInsertRows();
}

void ArgumentsListModel::remove(int index)
{
    beginRemoveRows(QModelIndex(), index, index); //notify views and proxy models that a line will be deleted
    m_data.removeAt(index); // do the modification to the model data
    endRemoveRows();
}

QList<char> ArgumentsListModel::listOfArgs()
{
    QList<char> list;
    for (auto item : m_data) {
        char c = static_cast<char>(item.title.toUInt());
        list.append(c);
    }
    return list;
}

void ArgumentsListModel::match(const QString data)
{
    auto pattern = data;

    for (int i = 0; i < m_data.size(); i ++)
    {
        m_data[i].selected = (pattern.indexOf(m_data[i].title) > -1);
        auto index = createIndex(i, 0);
        emit dataChanged(index, index, {Selected});
    }
}
