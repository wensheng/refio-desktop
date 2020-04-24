/****************************************************************************
**
**
****************************************************************************/

#include "mtree_model.h"
#include <QtWidgets>

//! [0]
MTreeModel::MTreeModel(const QVector<QVariant> &headers,
                       const QList<QVector<QVariant>> &data,
                       QObject *parent)
    : QAbstractItemModel(parent)
{
    QVector<QVariant> rootData;
    for (const auto &header : headers)
        rootData << header;

    rootItem = new MTreeItem(0, rootData);
    setupModelData(data, rootItem);
}

MTreeModel::~MTreeModel()
{
    delete rootItem;
}

int MTreeModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return rootItem->columnCount();
}

QVariant MTreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole && role != Qt::EditRole)
        return QVariant();

    MTreeItem *item = getItem(index);

    return item->data(index.column());
}

QVector<QVariant> MTreeModel::getRowData(const QModelIndex &index) const
{
    if (!index.isValid())
        return QVector<QVariant>();

    MTreeItem *item = getItem(index);
    QVector<QVariant> result(item->columnCount());
    for(int i=0; i<item->columnCount(); i++){
        result.push_back(item->data(i));
    }
    return result;
}

Qt::ItemFlags MTreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEditable | QAbstractItemModel::flags(index);
}

MTreeItem *MTreeModel::getItem(const QModelIndex &index) const
{
    if (index.isValid()) {
        MTreeItem *item = static_cast<MTreeItem*>(index.internalPointer());
        if (item)
            return item;
    }
    return rootItem;
}

QVariant MTreeModel::headerData(int section, Qt::Orientation orientation,
                               int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return rootItem->data(section);

    return QVariant();
}

QModelIndex MTreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (parent.isValid() && parent.column() != 0)
        return QModelIndex();

    MTreeItem *parentItem = getItem(parent);
    if (!parentItem)
        return QModelIndex();

    MTreeItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    return QModelIndex();
}

QModelIndex MTreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    MTreeItem *childItem = getItem(index);
    MTreeItem *parentItem = childItem ? childItem->parent() : nullptr;

    if (parentItem == rootItem || !parentItem)
        return QModelIndex();

    return createIndex(parentItem->childNumber(), 0, parentItem);
}

bool MTreeModel::removeRows(int position, int rows, const QModelIndex &parent)
{
    MTreeItem *parentItem = getItem(parent);
    if (!parentItem)
        return false;

    beginRemoveRows(parent, position, position + rows - 1);
    const bool success = parentItem->removeChildren(position, rows);
    endRemoveRows();

    return success;
}

int MTreeModel::rowCount(const QModelIndex &parent) const
{
    const MTreeItem *parentItem = getItem(parent);

    return parentItem ? parentItem->childCount() : 0;
}

bool MTreeModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role != Qt::EditRole)
        return false;

    MTreeItem *item = getItem(index);
    bool result = item->setData(index.column(), value);

    if (result)
        emit dataChanged(index, index, {Qt::DisplayRole, Qt::EditRole});

    return result;
}

bool MTreeModel::setHeaderData(int section, Qt::Orientation orientation,
                              const QVariant &value, int role)
{
    if (role != Qt::EditRole || orientation != Qt::Horizontal)
        return false;

    const bool result = rootItem->setData(section, value);

    if (result)
        emit headerDataChanged(orientation, section, section);

    return result;
}

void MTreeModel::setupModelData(const QList<QVector<QVariant>> &lines, MTreeItem *parent)
{
    if(lines.isEmpty()){
        return;
    }

    QVector<MTreeItem*> parents;
    QHash<int, MTreeItem*> hash;
    hash[0] = parent;
    QVector<int> indentations;
    parents << parent;
    indentations << 0;

    int number = 0;
    /*
     * we can do this because item id's are always in ascending order
     * child id's are always after parent's id.
     * */

    while (number < lines.size()) {
        //qDebug() << number << ":" << lines.at(number);
        /*
        QStringList cidss = query.value(3).toString().split( "," );
        QList<int> cids;
        for(const auto& s: cidss){
            if(!s.isEmpty()){
                cids.push_back(s.toInt());
            }
        }
        result.push_back(cids);
        qDebug() << cids;
        */
        QVector<QVariant> line = lines.at(number);
        int idx = line.at(0).toInt();
        int pid = line.at(2).toInt();
        if(pid==0){
            //root item
            MTreeItem *item = new MTreeItem(idx, {line.at(1)}, parent);
            hash[idx] = item;
            parent->insertChild(item);
        }else{
            //child item
            if(hash.contains(pid)){
                MTreeItem *item = new MTreeItem(idx, {line.at(1)}, hash[pid]);
                hash[idx] = item;
                hash[pid]->insertChild(item);
            }
        }
        ++number;
    }
}
