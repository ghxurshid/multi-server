#include "models.h"
#include <QNetworkInterface>

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
