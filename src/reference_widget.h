/****************************************************************************
**
**
****************************************************************************/

#ifndef REFERENCE_WIDGET_H
#define REFERENCE_WIDGET_H

#include "addresswidget.h"
#include "collections_widget.h"
#include "entries_widget.h"
#include "entry_details_widget.h"
#include <QWidget>
#include <QBoxLayout>
#include <QSplitter>

QT_BEGIN_NAMESPACE
class QSortFilterProxyModel;
class QItemSelectionModel;
QT_END_NAMESPACE

class ReferenceWidget : public QWidget
{
    Q_OBJECT

public:
    ReferenceWidget(int lib_id, QWidget *parent = nullptr);
    CollectionsWidget *collectionsWidget;
    QWidget *rightWidget;
    EntriesWidget *entriesWidget;
    EntryDetailsWidget *entryDetailsWidget;
    void toggleRightView();


public slots:
    void newFile();
    void openFile();
    void closeFile();

private:
    QSplitter *innerSplitter;

/*
signals:
    void selectionChanged (const QItemSelection &selected);

private:
    void setupTabs();

    TableModel *table;
    NewAddressTab *newAddressTab;
*/
};

#endif // REFERENCE_WIDGET_H
