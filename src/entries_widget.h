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

public slots:
    void updateActions();

private:
    TreeModel *treeModel;
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
