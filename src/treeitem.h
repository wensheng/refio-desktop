/****************************************************************************
**
** initially modified from:
** Examples/Qt-5.14.2/widgets/itemviews/editabletreemodel treeitem.h
****************************************************************************/

#ifndef TREEITEM_H
#define TREEITEM_H

#include <QVariant>
#include <QVector>

//! [0]
class TreeItem
{
public:
    explicit TreeItem(const QVector<QVariant> &data, TreeItem *parent = nullptr);
    ~TreeItem();

    TreeItem *child(int number);
    int childCount() const;
    int columnCount() const;
    QVariant data(int column) const;
    bool insertChildren(int position, int count, int columns);
    TreeItem *parent();
    bool removeChildren(int position, int count);
    int childNumber() const;
    bool setData(int column, const QVariant &value);

private:
    QVector<TreeItem*> childItems;
    QVector<QVariant> itemData;
    TreeItem *parentItem;
};
//! [0]

#endif // TREEITEM_H
