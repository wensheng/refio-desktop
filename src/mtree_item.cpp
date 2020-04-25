/****************************************************************************
**
**
****************************************************************************/

/*
    treeitem.cpp

    A container for items of data supplied by the simple tree model.
*/

#include "mtree_item.h"

MTreeItem::MTreeItem(int idx, const QVector<QVariant> &data, MTreeItem *parent)
    : iid(idx),
      itemData(data),
      parentItem(parent)
{}

MTreeItem::~MTreeItem()
{
    qDeleteAll(childItems);
}

MTreeItem *MTreeItem::child(int number)
{
    if (number < 0 || number >= childItems.size())
        return nullptr;
    return childItems.at(number);
}

//! [3]
int MTreeItem::childCount() const
{
    return childItems.count();
}
//! [3]

//! [4]
int MTreeItem::childNumber() const
{
    if (parentItem)
        return parentItem->childItems.indexOf(const_cast<MTreeItem*>(this));
    return 0;
}
//! [4]

//! [5]
int MTreeItem::columnCount() const
{
    return itemData.count();
}
//! [5]

//! [6]
QVariant MTreeItem::data(int column) const
{
    if (column < 0 || column >= itemData.size())
        return QVariant();
    return itemData.at(column);
}
//! [6]

/*
bool MTreeItem::insertChildren(int position, int count, int columns)
{
    if (position < 0 || position > childItems.size())
        return false;

    for (int row = 0; row < count; ++row) {
        QVector<QVariant> data(columns);
        MTreeItem *item = new MTreeItem(data, this);
        childItems.insert(position, item);
    }

    return true;
}*/

void MTreeItem::insertChild(MTreeItem *const &child)
{
    childItems.append(child);
}

MTreeItem *MTreeItem::parent()
{
    return parentItem;
}

//! [10]
bool MTreeItem::removeChildren(int position, int count)
{
    if (position < 0 || position + count > childItems.size())
        return false;

    for (int row = 0; row < count; ++row)
        delete childItems.takeAt(position);

    return true;
}

void MTreeItem::removeAllChildren()
{
    for (int row = 0; row < childItems.size(); ++row){
        delete childItems.takeAt(row);
    }
}

//! [11]
bool MTreeItem::setData(int column, const QVariant &value)
{
    if (column < 0 || column >= itemData.size())
        return false;

    itemData[column] = value;
    return true;
}
//! [11]
