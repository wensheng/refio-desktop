/****************************************************************************
**
**
****************************************************************************/

#include "collections_widget.h"
#include "mainwindow.h"
#include "mtree_model.h"

#include <QtWidgets>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

CollectionsWidget::CollectionsWidget(QWidget *parent)
    : QWidget(parent),
      treeView(new QTreeView(this)),
      standardItemModel(new QStandardItemModel(this))
{
    setup_db();
    QList<QVector<QVariant>> modelData;
    QSqlQuery query(db);
    query.prepare("SELECT id, title, parent, children FROM ref_collections");
    if(!query.exec()){
        qDebug() << query.lastError().text();
    }else{
        while (query.next()) {
            QVector<QVariant> result;
            result.push_back(query.value(0).toInt()); // id
            result.push_back(query.value(1).toString()); // title
            result.push_back(query.value(2).toInt()); // parent
            result.push_back(query.value(3).toString()); // children
            modelData.push_back(result);
        }
    }

    QVector<QVariant> headers = {0, "title", 0};
    MTreeModel *mtreeModel = new MTreeModel(headers, modelData);

    auto tb = new QToolBar();

    const QIcon newIcon = QIcon::fromTheme("document-new", QIcon(":/images/new.png"));
    QAction *newAct = new QAction(newIcon, tr("&New"), this);
    newAct->setShortcuts(QKeySequence::New);
    newAct->setStatusTip(tr("Create a new file"));
    connect(newAct, &QAction::triggered, this, &CollectionsWidget::newFile);
    tb->addAction(newAct);

    const QIcon openIcon = QIcon::fromTheme("document-open", QIcon(":/images/open.png"));
    QAction *openAct = new QAction(openIcon, tr("&Open..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open an existing file"));
    connect(openAct, &QAction::triggered, this, &CollectionsWidget::openFile);
    tb->addAction(openAct);

    const QIcon saveIcon = QIcon::fromTheme("document-save", QIcon(":/images/save.png"));
    QAction *saveAct = new QAction(saveIcon, tr("&Save"), this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip(tr("Save the document to disk"));
    connect(saveAct, &QAction::triggered, this, &CollectionsWidget::closeFile);
    tb->addAction(saveAct);


    /* copied from example/widgets/tutorials/7_selections */
    /*
    standardItemModel->setHeaderData(0, Qt::Horizontal, "My Library");
    QStandardItem *rootNode = standardItemModel->invisibleRootItem();

    //defining a couple of items
    QStandardItem *americaItem = new QStandardItem("America");
    americaItem->setIcon(QIcon(":/images/open.png"));
    QStandardItem *mexicoItem =  new QStandardItem("Canada");
    QStandardItem *usaItem =     new QStandardItem("USA");
    QStandardItem *bostonItem =  new QStandardItem("Boston");
    QStandardItem *europeItem =  new QStandardItem("Europe");
    QStandardItem *italyItem =   new QStandardItem("Italy");
    QStandardItem *romeItem =    new QStandardItem("Rome");
    QStandardItem *veronaItem =  new QStandardItem("Verona");

    //building up the hierarchy
    rootNode->    appendRow(americaItem);
    rootNode->    appendRow(europeItem);
    americaItem-> appendRow(mexicoItem);
    americaItem-> appendRow(usaItem);
    usaItem->     appendRow(bostonItem);
    europeItem->  appendRow(italyItem);
    italyItem->   appendRow(romeItem);
    italyItem->   appendRow(veronaItem);

    //register the model
    treeView->setModel(standardItemModel);
    treeView->expandAll();
    */
    treeView->setModel(mtreeModel);
    treeView->setHeaderHidden(true);
    treeView->expandAll();
    treeView->resizeColumnToContents(0);

    //selection changes shall trigger a slot
    QItemSelectionModel *selectionModel = treeView->selectionModel();
    connect(selectionModel, &QItemSelectionModel::selectionChanged,
                this, &CollectionsWidget::selectionChanged);

    auto layout = new QVBoxLayout();
    layout->setMenuBar(tb);
    layout->addWidget(treeView);
    layout->setContentsMargins(0,0,0,0);
    setLayout(layout);
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
    QString selectedText = index.data(Qt::DisplayRole).toString();
    //find out the hierarchy level of the selected item
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

void CollectionsWidget::setup_db()
{
    QString appDataDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir dir(appDataDir);
    if(!dir.exists()){
        dir.mkpath(".");
    }
    qDebug() << "appDataDir - " << appDataDir;
    QString path(appDataDir);
    path.append(QDir::separator()).append("data.db");
    path = QDir::toNativeSeparators(path);

    db = QSqlDatabase::addDatabase("QSQLITE", "refio");
    db.setDatabaseName(path);
    if (!db.open()) {
        qDebug() << "Can't connect to DB!";
    }else{
        qDebug() << "DB opened successfully!";
    }
    //TODO: create tables if not exist

}
