/****************************************************************************
**
**
****************************************************************************/

#ifndef ENTRIES_WIDGET_H
#define ENTRIES_WIDGET_H

#include "addresswidget.h"
#include <QWidget>
#include <QTreeView>
#include "treemodel.h"
#include "mtree_model.h"
#include "mtree_item.h"
#include "entry_details_widget.h"

QT_BEGIN_NAMESPACE
class QSortFilterProxyModel;
class QItemSelectionModel;
QT_END_NAMESPACE

class EntriesWidget : public QWidget
{
    Q_OBJECT

public:
    EntriesWidget(QWidget *parent = nullptr);
    //AddressWidget *addressWidget;
    QTreeView *treeView;
    void update(MTreeItem *mtreeItem);

public slots:
    void updateActions();
    void handleEdit();

private:
    MTreeModel *mtreeModel;
    EntryDetailsWidget *detailWidget;

private slots:
    void newFile();
    void openFile();
    void closeFile();
    void insertChild();
    void insertRow();
    void removeRow();
};

#endif
