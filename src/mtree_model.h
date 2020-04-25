/****************************************************************************
**
** initially modified from:
** Examples/Qt-5.14.2/widgets/itemviews/editabletreemodel treeitem.h
****************************************************************************/

#ifndef MTREE_MODEL_H
#define MTREE_MODEL_H

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>
#include "mtree_item.h"

//! [0]
class MTreeModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    MTreeModel(const QVector<QVariant> &headers, const QList<QVector<QVariant>> &data,
              QObject *parent = nullptr);
    ~MTreeModel();

    QVariant data(const QModelIndex &index, int role) const override;
    QVector<QVariant> getRowData(const QModelIndex &index) const;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;

    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;
    bool setHeaderData(int section, Qt::Orientation orientation,
                       const QVariant &value, int role = Qt::EditRole) override;

    bool removeRows(int position, int rows,
                    const QModelIndex &parent = QModelIndex()) override;
    MTreeItem *getItem(const QModelIndex &index) const;
    void clearModel();
    void setupModelData(const QList<QVector<QVariant>> &data, MTreeItem *parent);

private:

    MTreeItem *rootItem;
};
//! [2]

#endif // TREEMODEL_H
