/****************************************************************************
**
**
****************************************************************************/

#include "entries_widget.h"
#include "treemodel.h"

#include <QtWidgets>

EntriesWidget::EntriesWidget(QWidget *parent)
    : QWidget(parent),
      treeView(new QTreeView(this))
{
    auto tb = new QToolBar();

    const QIcon newIcon = QIcon::fromTheme("document-new", QIcon(":/images/new.png"));
    QAction *newAct = new QAction(newIcon, tr("&Insert"), this);
    newAct->setStatusTip(tr("Insert"));
    connect(newAct, &QAction::triggered, this, &EntriesWidget::insertRow);
    tb->addAction(newAct);

    const QIcon openIcon = QIcon::fromTheme("document-open", QIcon(":/images/open.png"));
    QAction *openAct = new QAction(openIcon, tr("&Insert Child"), this);
    openAct->setStatusTip(tr("Insert a child entry"));
    connect(openAct, &QAction::triggered, this, &EntriesWidget::insertChild);
    tb->addAction(openAct);

    const QIcon saveIcon = QIcon::fromTheme("document-save", QIcon(":/images/save.png"));
    QAction *saveAct = new QAction(saveIcon, tr("&Remove"), this);
    saveAct->setStatusTip(tr("Remove selected entry"));
    connect(saveAct, &QAction::triggered, this, &EntriesWidget::removeRow);
    tb->addAction(saveAct);


    const QStringList headers({tr("Title"), tr("Description")});

    QFile file(":/default.txt");
    file.open(QIODevice::ReadOnly);
    TreeModel *model = new TreeModel(headers, file.readAll());
    file.close();

    treeView->setModel(model);
    treeView->setHeaderHidden(true);
    for (int column = 1; column < model->columnCount(); ++column)
        treeView->setColumnHidden(column, true);
    treeView->resizeColumnToContents(0);

    connect(treeView->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &EntriesWidget::updateActions);

    updateActions();

    auto layout = new QVBoxLayout();
    layout->setMenuBar(tb);
    layout->addWidget(treeView);
    layout->setContentsMargins(0,0,0,0);
    setLayout(layout);
}

void EntriesWidget::newFile()
{

}

void EntriesWidget::openFile()
{

}

void EntriesWidget::closeFile()
{

}

void EntriesWidget::insertChild()
{
    const QModelIndex index = treeView->selectionModel()->currentIndex();
    QAbstractItemModel *model = treeView->model();

    if (model->columnCount(index) == 0) {
        if (!model->insertColumn(0, index))
            return;
    }

    if (!model->insertRow(0, index))
        return;

    for (int column = 0; column < model->columnCount(index); ++column) {
        const QModelIndex child = model->index(0, column, index);
        model->setData(child, QVariant(tr("[No data]")), Qt::EditRole);
        if (!model->headerData(column, Qt::Horizontal).isValid())
            model->setHeaderData(column, Qt::Horizontal, QVariant(tr("[No header]")), Qt::EditRole);
    }

    treeView->selectionModel()->setCurrentIndex(model->index(0, 0, index),
                                            QItemSelectionModel::ClearAndSelect);
    updateActions();
}

void EntriesWidget::insertRow()
{
    const QModelIndex index = treeView->selectionModel()->currentIndex();
    QAbstractItemModel *model = treeView->model();

    if (!model->insertRow(index.row()+1, index.parent()))
        return;

    updateActions();

    for (int column = 0; column < model->columnCount(index.parent()); ++column) {
        const QModelIndex child = model->index(index.row() + 1, column, index.parent());
        model->setData(child, QVariant(tr("[No data]")), Qt::EditRole);
    }
}

void EntriesWidget::removeRow()
{
    const QModelIndex index = treeView->selectionModel()->currentIndex();
    QAbstractItemModel *model = treeView->model();
    if (model->removeRow(index.row(), index.parent()))
        updateActions();
}

void EntriesWidget::updateActions()
{
    const bool hasSelection = !treeView->selectionModel()->selection().isEmpty();
    //removeRowAction->setEnabled(hasSelection);

    const bool hasCurrent = treeView->selectionModel()->currentIndex().isValid();
    //insertRowAction->setEnabled(hasCurrent);

    if (hasCurrent) {
        treeView->closePersistentEditor(treeView->selectionModel()->currentIndex());

        const int row = treeView->selectionModel()->currentIndex().row();
        const int column = treeView->selectionModel()->currentIndex().column();
        if (treeView->selectionModel()->currentIndex().parent().isValid()){
            //statusBar()->showMessage(tr("Position: (%1,%2)").arg(row).arg(column));
        }
        else{
            //statusBar()->showMessage(tr("Position: (%1,%2) in top level").arg(row).arg(column));
        }
    }
}
