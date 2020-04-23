/****************************************************************************
**
**
****************************************************************************/

#ifndef COLLECTIONS_WIDGET_H
#define COLLECTIONS_WIDGET_H

#include <QWidget>

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
    CollectionsWidget(QWidget *parent = nullptr);

private slots:
    void selectionChanged(const QItemSelection &newSelection, const QItemSelection &oldSelection);
    void newFile();
    void openFile();
    void closeFile();

private:
    QTreeView *treeView;
    QStandardItemModel *standardItemModel;

};

#endif
