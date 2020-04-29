/****************************************************************************
**
**
****************************************************************************/

#ifndef COLLECTIONS_WIDGET_H
#define COLLECTIONS_WIDGET_H

#include <QWidget>
#include <QSqlDatabase>
#include "mtree_model.h"

QT_BEGIN_NAMESPACE
class QTreeView;
class QStandardItemModel;
class QSortFilterProxyModel;
class QItemSelection;
class QItemSelectionModel;
QT_END_NAMESPACE

class CollectionsWidget : public QWidget
{
    Q_OBJECT

public:
    CollectionsWidget(int lib_id, QWidget *parent = nullptr);

public slots:
    void handleEdit();
    void insertRow();

private slots:
    void selectionChanged(const QItemSelection &newSelection, const QItemSelection &oldSelection);
    void newFile();
    void openFile();
    void closeFile();

private:
    QTreeView *treeView;
    QStandardItemModel *standardItemModel;
    MTreeModel *mtreeModel;
    int lib_id;
};

#endif
