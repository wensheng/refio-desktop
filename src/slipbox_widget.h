/****************************************************************************
**
**
****************************************************************************/

#ifndef SLIPBOX_WIDGET_H
#define SLIPBOX_WIDGET_H

#include "addresswidget.h"
#include <QWidget>

QT_BEGIN_NAMESPACE
class QSortFilterProxyModel;
class QItemSelectionModel;
QT_END_NAMESPACE

class SlipboxWidget : public QWidget
{
    Q_OBJECT

public:
    SlipboxWidget(QWidget *parent = nullptr);
    AddressWidget *addressWidget;

public slots:
    void newFile();
    void openFile();
    void closeFile();

/*
signals:
    void selectionChanged (const QItemSelection &selected);

private:
    void setupTabs();

    TableModel *table;
    NewAddressTab *newAddressTab;
*/
};

#endif // SLIPBOX_WIDGET_H
