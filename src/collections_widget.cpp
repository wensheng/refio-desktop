/****************************************************************************
**
**
****************************************************************************/

#include "constants.h"
#include "collections_widget.h"
#include "mainwindow.h"
#include "mtree_model.h"
#include "entries_widget.h"

#include <QtWidgets>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

CollectionsWidget::CollectionsWidget(int lib_id, QWidget *parent)
    : QWidget(parent),
      treeView(new QTreeView(this)),
      standardItemModel(new QStandardItemModel(this)),
      lib_id(lib_id)
{
    setObjectName(REF_COLLECTIONS_WIDGET_NAME);
    /*
    QSqlDatabase db = QSqlDatabase::database(DATABASE_NAME);
    QList<QVector<QVariant>> modelData;
    QSqlQuery query(db);
    query.prepare("SELECT name, id, parent FROM ref_collections"
                  " where lib_id=:lib_id ORDER BY id DESC");
    query.bindValue(":lib_id", lib_id);
    if(!query.exec()){
        qDebug() << query.lastError().text();
    }else{
        while (query.next()) {
            QVector<QVariant> result;
            result.push_back(query.value(0).toString()); // name
            result.push_back(query.value(1).toInt()); // id
            result.push_back(query.value(2).toInt()); // parent
            modelData.push_back(result);
        }
    }
    qDebug() << modelData;

    QVector<QVariant> headers = {"name", 0, 0};
    mtreeModel = new MTreeModel(headers, modelData);
    treeView->setModel(mtreeModel);
    treeView->setHeaderHidden(true);
    treeView->expandAll();
    treeView->resizeColumnToContents(0);
    for (int column = 1; column < mtreeModel->columnCount(); ++column)
        treeView->setColumnHidden(column, true);

    //selection changes shall trigger a slot
    connect(treeView->selectionModel(), &QItemSelectionModel::selectionChanged,
                this, &CollectionsWidget::selectionChanged);
    connect(treeView->model(), &QAbstractItemModel::dataChanged, this, &CollectionsWidget::handleEdit);
    */
    setupModel(lib_id);

    auto tb = new QToolBar();

    const QIcon newIcon = QIcon::fromTheme("document-new", QIcon(":/images/new.png"));
    QAction *newAct = new QAction(newIcon, tr("&New"), this);
    newAct->setShortcuts(QKeySequence::New);
    connect(newAct, &QAction::triggered, this, &CollectionsWidget::insertRow);
    tb->addAction(newAct);

    const QIcon openIcon = QIcon::fromTheme("document-open", QIcon(":/images/open.png"));
    QAction *openAct = new QAction(openIcon, tr("&Open..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    connect(openAct, &QAction::triggered, this, &CollectionsWidget::openFile);
    tb->addAction(openAct);

    const QIcon saveIcon = QIcon::fromTheme("document-save", QIcon(":/images/save.png"));
    QAction *saveAct = new QAction(saveIcon, tr("&Save"), this);
    saveAct->setShortcuts(QKeySequence::Save);
    connect(saveAct, &QAction::triggered, this, &CollectionsWidget::closeFile);
    tb->addAction(saveAct);


    // here selectionChanged will be invoked, in there will update entriesWidget
    // but entriesWidget is not instantiated at this time, so program will crash here
    // so we use showEvent
    //QModelIndex idxFirst = treeView->model()->index(0,0);
    //treeView->selectionModel()->setCurrentIndex(idxFirst, QItemSelectionModel::SelectCurrent);

    auto layout = new QVBoxLayout();
    layout->setMenuBar(tb);
    layout->addWidget(treeView);
    layout->setContentsMargins(0,0,0,0);
    setLayout(layout);
    qDebug() << metaObject()->className();
}

void CollectionsWidget::setupModel(int lib_id)
{
    QSqlDatabase db = QSqlDatabase::database(DATABASE_NAME);
    QList<QVector<QVariant>> modelData;
    QSqlQuery query(db);
    query.prepare("SELECT name, id, parent FROM ref_collections"
                  " where lib_id=:lib_id ORDER BY id DESC");
    query.bindValue(":lib_id", lib_id);
    if(!query.exec()){
        qDebug() << query.lastError().text();
    }else{
        while (query.next()) {
            QVector<QVariant> result;
            result.push_back(query.value(0).toString()); // name
            result.push_back(query.value(1).toInt()); // id
            result.push_back(query.value(2).toInt()); // parent
            modelData.push_back(result);
        }
    }
    qDebug() << modelData;
    QVector<QVariant> headers = {"name", 0, 0};
    MTreeModel *mtreeModel = new MTreeModel(headers, modelData);
    auto oldModel = treeView->model();
    treeView->setModel(mtreeModel);
    delete oldModel;
    treeView->setHeaderHidden(true);
    treeView->expandAll();
    treeView->resizeColumnToContents(0);
    for (int column = 1; column < mtreeModel->columnCount(); ++column)
        treeView->setColumnHidden(column, true);

    //selection changes shall trigger a slot
    connect(treeView->selectionModel(), &QItemSelectionModel::selectionChanged,
                this, &CollectionsWidget::selectionChanged);
    connect(treeView->model(), &QAbstractItemModel::dataChanged, this, &CollectionsWidget::handleEdit);
}

void CollectionsWidget::newFile()
{

}

void CollectionsWidget::openFile()
{

}

void CollectionsWidget::closeFile()
{

}

void CollectionsWidget::selectionChanged(const QItemSelection &newSelection, const QItemSelection &oldSelection)
{
    //get the text of the selected item
    const QModelIndex index = treeView->selectionModel()->currentIndex();
    MTreeModel *model = qobject_cast<MTreeModel*>(treeView->model());
    const MTreeItem *item = model->getItem(index);
    //EntriesWidget *entriesWidget = parentWidget()->findChild<EntriesWidget *>(REF_ENTRIES_WIDGET_NAME);
    EntriesWidget *entriesWidget = MainWindow::entriesWidget();
    entriesWidget->update(item);

    QString selectedText = index.data(Qt::DisplayRole).toString();
    //find out the hierarchy level of the selected item
    qDebug() << selectedText;
    int hierarchyLevel = 1;
    QModelIndex seekRoot = index;
    while (seekRoot.parent() != QModelIndex()) {
        seekRoot = seekRoot.parent();
        hierarchyLevel++;
    }
    QString showString = QString("%1, Level %2").arg(selectedText)
                         .arg(hierarchyLevel);
    setWindowTitle(showString);
}

void CollectionsWidget::handleEdit()
{
        QModelIndex idx = treeView->selectionModel()->currentIndex();
        if(!idx.isValid()){
            qDebug() << "current collection idx:" << idx;
            return;
        }
        MTreeModel *model = (MTreeModel *)treeView->model();
        MTreeItem *item = model->getItem(idx);
        QString newTitle = treeView->model()->data(idx).toString();
        QSqlDatabase db = QSqlDatabase::database(DATABASE_NAME);
        //QList<QVector<QVariant>> modelData;
        QSqlQuery query(db);
        int parent_id = item->parent()->itemId();
        qDebug() << "parent_id=" << parent_id;

        if(item->itemId() == -1){
            query.prepare("insert into ref_collections (lib_id, parent, name, created) values (:lib_id, :parent, :title, :created)");
            query.bindValue(":lib_id", lib_id);
            query.bindValue(":parent", parent_id);
            query.bindValue(":title", newTitle);
            query.bindValue(":created", QDateTime::currentDateTime().toString(Qt::ISODate));
            if(!query.exec()){
                qDebug() << query.lastError().text();
            }else{
                qDebug() << "lastInsertId()=" << query.lastInsertId();
                item->setItemId(query.lastInsertId().toInt());
            }
        }else{
            query.prepare("update ref_collections set title=:newTitle where id=:iid");
            query.bindValue(":iid", item->itemId());
            query.bindValue(":newTitle", newTitle);
            if(!query.exec()){
                qDebug() << query.lastError().text();
            }
        }
}

void CollectionsWidget::insertRow()
{
    MTreeModel *model = (MTreeModel *)treeView->model();
    QItemSelectionModel *selection = treeView->selectionModel();
    const QModelIndex index = selection->currentIndex();

    //qDebug() << "index.row()=" << index.row() << " index.parent()=" << index.parent();
    // if no selection index.row() will be -1
    // below is still valid
    if (!model->insertRow(index.row() + 1, index.parent())){
        return;
    }

    //updateActions();

    for (int column = 0; column < model->columnCount(index.parent()); ++column) {
        const QModelIndex child = model->index(index.row() + 1, column, index.parent());
        model->setData(child, QVariant(tr("[No data]")), Qt::EditRole);
        selection->select(child, QItemSelectionModel::Select);
    }

}

void CollectionsWidget::showEvent(QShowEvent *event)
{
    //select the first row
    QWidget::showEvent(event);
    QModelIndex idxFirst = treeView->model()->index(0,0);
    treeView->selectionModel()->setCurrentIndex(idxFirst, QItemSelectionModel::SelectCurrent);
}
