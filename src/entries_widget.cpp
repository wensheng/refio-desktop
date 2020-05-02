/****************************************************************************
**
**
****************************************************************************/

#include "constants.h"
#include "entries_widget.h"
#include "entry_details_widget.h"
#include "mainwindow.h"

#include <QtWidgets>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QUrl>

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

    auto layout = new QVBoxLayout();
    layout->setMenuBar(tb);
    layout->addWidget(treeView);
    layout->setContentsMargins(0,0,0,0);
    setLayout(layout);
    setObjectName(REF_ENTRIES_WIDGET_NAME);
    setFocusPolicy(Qt::StrongFocus);
    setAcceptDrops(true);
    installEventFilter(this);
    qDebug() << metaObject()->className();
}

/*
 * called by CollectionsWidget when click on a collection
 */
void EntriesWidget::update(const MTreeItem *mtreeItem){
    collection_id = mtreeItem->itemId();
    QSqlDatabase db = QSqlDatabase::database(DATABASE_NAME);
    QList<QVector<QVariant>> modelData;
    QSqlQuery query(db);
    // order must be title, id, parent, rest...
    query.prepare("select title, id, parent, icode, collection_id, info, created from ref_entries where collection_id=:cid");
    query.bindValue(":cid", collection_id);

    if(!query.exec()){
        qDebug() << query.lastError().text();
    }else{
        while (query.next()) {
            QVector<QVariant> result;
            result.push_back(query.value(0).toString()); // title
            result.push_back(query.value(1).toInt()); // id
            result.push_back(query.value(2).toInt()); // parent
            result.push_back(query.value(3).toString()); // icode
            result.push_back(query.value(4).toInt()); // collection_id
            result.push_back(query.value(5).toString()); // info
            result.push_back(query.value(6).toString()); // created
            modelData.push_back(result);
        }
    }
    //qDebug() << "modelData size=" << modelData.size();

    auto oldModel = treeView->model();
    QVector<QVariant> headers = {"Title", "id", "cid", "icode", "parent", "Note", "Created"};
    MTreeModel *mtreeModel = new MTreeModel(headers, modelData);
    treeView->setModel(mtreeModel);
    delete oldModel;
    //qDebug() << "treeview model has " << mtreeModel->rowCount() << "rows.";
    for(int i=1;i<mtreeModel->columnCount();i++){
        treeView->setColumnHidden(i, true); // id
    }
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
    const bool hasSelection = !treeView->selectionModel()->selection().isEmpty();
    const bool hasCurrent = treeView->selectionModel()->currentIndex().isValid();
    QModelIndex idx = treeView->selectionModel()->currentIndex();

    if (idx.isValid()) {
        MTreeModel *model = (MTreeModel *)treeView->model();
        //QVector<QVariant> rowData = model->getRowData(idx);
        MTreeItem *treeItem = model->getItem(idx);
        EntryDetailsWidget *detailsWidget = parentWidget()->findChild<EntryDetailsWidget*>(REF_ENTRY_DETAILS_WIDGET_NAME);
        detailsWidget->updateDetail(treeItem);
        //treeView->closePersistentEditor(treeView->selectionModel()->currentIndex());

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
            query.prepare("INSERT INTO ref_entries (collection_id, parent, icode, type, title, info, created)"
                          "values (:ci, :parent, :icode, :type, :title, \"\", :created)");
            query.bindValue(":ci", collection_id);
            // TODO: actually set parent
            query.bindValue(":parent", 0);
            query.bindValue(":icode", icode);
            // TODO: real type
            query.bindValue(":type", "Note");
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

bool EntriesWidget::eventFilter(QObject *obj, QEvent* event)
{
    if(event->type() == QEvent::KeyPress || event->type() == QEvent::ShortcutOverride){
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        //QWidget::keyPressEvent(keyEvent);
        if (keyEvent->type() == QKeyEvent::KeyPress || keyEvent->type()== QEvent::ShortcutOverride){
            if (keyEvent->matches(QKeySequence::Paste)){
                QClipboard *clipboard = QGuiApplication::clipboard();
                const QMimeData *data = clipboard->mimeData();
                //QString cText = clipboard->text();
                //qDebug() << cText;
                handlePastedData(data);
                return true;
            }
            return false;
        }
        return false;
    }
    return false;
}

void EntriesWidget::dropEvent(QDropEvent * event)
{
    handlePastedData(event->mimeData());
    QWidget::dropEvent(event);
}

void EntriesWidget::dragEnterEvent(QDragEnterEvent *event)
{
    event->acceptProposedAction();
}

void EntriesWidget::handlePastedData(const QMimeData *data)
{
    if(data->hasText()){
        qDebug() << data->text();
    }
    if (data->hasUrls()){
        QStringList filePathList;
        foreach (QUrl url, data->urls()){
            filePathList << url.toLocalFile();
        }
        qDebug() << filePathList;
    }
}
