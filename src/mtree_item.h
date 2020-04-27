/****************************************************************************
**
** initially modified from:
** Examples/Qt-5.14.2/widgets/itemviews/editabletreemodel treeitem.h
****************************************************************************/

#ifndef MTREE_ITEM_H
#define MTREE_ITEM_H

#include <QVariant>
#include <QVector>

class MTreeItem
{
public:
    explicit MTreeItem(int idx, const QVector<QVariant> &data, MTreeItem *parent = nullptr);
    ~MTreeItem();

    MTreeItem *child(int number);
    int childCount() const;
    int columnCount() const;
    QVariant data(int column) const;
    void insertChild(MTreeItem *const &);
    MTreeItem *parent();
    int itemId() const{
        return iid;
    }
    void setItemId(int itemId){
        iid = itemId;
    }
    bool insertChildren(int position, int count);
    bool removeChildren(int position, int count);
    int childNumber() const;
    bool setData(int column, const QVariant &value);
    void removeAllChildren();

private:
    int iid;  //db id
    QVector<MTreeItem*> childItems;
    QVector<QVariant> itemData;
    MTreeItem *parentItem;
};

#endif 
