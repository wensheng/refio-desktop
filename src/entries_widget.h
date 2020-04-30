/****************************************************************************
**
**
****************************************************************************/

#ifndef ENTRIES_WIDGET_H
#define ENTRIES_WIDGET_H

#include "addresswidget.h"
#include <QWidget>
//#include <QTreeView>
#include "treemodel.h"
#include "mtreeview.h"
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
    MTreeView *treeView;
    void update(MTreeItem *mtreeItem);

public slots:
    void updateActions();
    void handleEdit();
    void insertRow();

private:
    EntryDetailsWidget *detailWidget;
    int collection_id;
    bool eventFilter(QObject *, QEvent*) override;
    void dropEvent(QDropEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override{
        event->accept();
    }
    void dragEnterEvent(QDragEnterEvent *event) override;
    void handlePastedData(const QMimeData *data);

private slots:
    void newFile();
    void openFile();
    void closeFile();
    void insertChild();
    void removeRow();
};

#endif
