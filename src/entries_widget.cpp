/****************************************************************************
**
**
****************************************************************************/

#include "constants.h"
#include "entries_widget.h"
#include "entry_details_widget.h"
//#include "treeitem.h"
//#include "treemodel.h"
#include "mtree_model.h"
#include "mainwindow.h"

#include <QtWidgets>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

EntriesWidget::EntriesWidget(QWidget *parent)
    : QWidget(parent),
      treeView(new MTreeView(this))
{
    collection_id = 0;
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

    /*
    QVector<QVariant> headers = {"Title", "id", "parent", "cid", "Info", "Note"};
    QList<QVector<QVariant>> emptyData =  QList<QVector<QVariant>>();
    mtreeModel = new MTreeModel(headers, emptyData);

    treeView->setModel(mtreeModel);
    //treeView->setHeaderHidden(true);
    for (int column = 1; column < mtreeModel->columnCount(); ++column)
        treeView->setColumnHidden(column, true);
    treeView->resizeColumnToContents(0);

    connect(treeView->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &EntriesWidget::updateActions);

    updateActions();
    */

    auto layout = new QVBoxLayout();
    layout->setMenuBar(tb);
    layout->addWidget(treeView);
    layout->setContentsMargins(0,0,0,0);
    setLayout(layout);
    setObjectName(REF_ENTRIES_WIDGET_NAME);
}

void EntriesWidget::update(MTreeItem *mtreeItem){
    collection_id = mtreeItem->itemId();
    QSqlDatabase db = QSqlDatabase::database(DATABASE_NAME);
    QList<QVector<QVariant>> modelData;
    QSqlQuery query(db);
    // order must be title, id, parent, rest...
    query.prepare("select title, id, parent, icode, collection_id, info from ref_entries where collection_id=:cid");
    query.bindValue(":cid", collection_id);

    if(!query.exec()){
        qDebug() << query.lastError().text();
    }else{
        while (query.next()) {
            QVector<QVariant> result;
            result.push_back(query.value(0).toString()); // title
            result.push_back(query.value(1).toInt()); // id
            result.push_back(query.value(2).toInt()); // parent
            result.push_back(query.value(3).toInt()); // collection_id
            result.push_back(query.value(4).toString()); // icode
            result.push_back(query.value(5).toString()); // info
            modelData.push_back(result);
        }
    }
    qDebug() << "modelData size=" << modelData.size();

    auto oldModel = treeView->model();
    QVector<QVariant> headers = {"Title", "id", "cid", "parent", "icode", "Note"};
    MTreeModel *mtreeModel = new MTreeModel(headers, modelData);
    treeView->setModel(mtreeModel);
    delete oldModel;
    qDebug() << "treeview model has " << mtreeModel->rowCount() << "rows.";
    for (int column = 1; column < mtreeModel->columnCount(); ++column)
        treeView->setColumnHidden(column, true);
    treeView->resizeColumnToContents(0);
    connect(treeView->selectionModel(), &QItemSelectionModel::selectionChanged, this, &EntriesWidget::updateActions);
    connect(treeView->model(), &QAbstractItemModel::dataChanged, this, &EntriesWidget::handleEdit);

    QModelIndex idxFirst = treeView->model()->index(0,0);
    treeView->selectionModel()->setCurrentIndex(idxFirst, QItemSelectionModel::SelectCurrent);
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
    MTreeModel *model = (MTreeModel *)treeView->model();
    qDebug() << "index.row()=" << index.row() << " index.parent()=" << index.parent();

    if (!model->insertRow(index.row() + 1, index.parent()))
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
    //MTreeModel *model = (MTreeModel *) treeView->model();
    const bool hasSelection = !treeView->selectionModel()->selection().isEmpty();
    qDebug() << "hasSelection:" << hasSelection;
    //removeRowAction->setEnabled(hasSelection);

    const bool hasCurrent = treeView->selectionModel()->currentIndex().isValid();
    qDebug() << "hasCurrent:" << hasCurrent;
    //insertRowAction->setEnabled(hasCurrent);

    if (hasCurrent) {
        QModelIndex idx = treeView->selectionModel()->currentIndex();
        MTreeModel *model = (MTreeModel *)treeView->model();
        //QVector<QVariant> rowData = model->getRowData(idx);
        MTreeItem *treeItem = model->getItem(idx);
        //TODO combine
        QSplitter *pw = (QSplitter*)this->parentWidget();
        EntryDetailsWidget *detailsWidget = (EntryDetailsWidget*)(pw->widget(1));
        qDebug() << detailsWidget;
        //detailsWidget->updateDetail(rowData);
        detailsWidget->updateDetail(treeItem);
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

void EntriesWidget::handleEdit()
{
        QModelIndex idx = treeView->selectionModel()->currentIndex();
        MTreeModel *model = (MTreeModel *)treeView->model();
        MTreeItem *item = model->getItem(idx);
        QString newTitle = treeView->model()->data(idx).toString();
        QSqlDatabase db = QSqlDatabase::database(DATABASE_NAME);
        //QList<QVector<QVariant>> modelData;
        QSqlQuery query(db);

        if(item->itemId() == -1){
            MainWindow * mainWindow = qobject_cast<MainWindow *>(QApplication::activeWindow());
            QString icode = mainWindow->getNextICode();
            query.prepare("insert into ref_entries (collection_id, parent, icode, title, info, created)"
                          "values (:ci, :parent, :icode, :title, \"\", :created)");
            query.bindValue(":ci", collection_id);
            // TODO: actually set parent
            query.bindValue(":icode", icode);
            query.bindValue(":parent", 0);
            query.bindValue(":title", newTitle);
            query.bindValue(":created", QDateTime::currentDateTime().toString(Qt::ISODate));
            if(!query.exec()){
                qDebug() << query.lastError().text();
            }else{
                qDebug() << "lastInsertId()=" << query.lastInsertId();
                item->setItemId(query.lastInsertId().toInt());
            }
        }else{
            query.prepare("update ref_entries set title=:newTitle where id=:iid");
            query.bindValue(":iid", item->itemId());
            query.bindValue(":newTitle", newTitle);
            if(!query.exec()){
                qDebug() << query.lastError().text();
            }
        }
}
