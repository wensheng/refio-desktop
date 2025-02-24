/****************************************************************************
**
**
****************************************************************************/

#include "constants.h"
#include "mainwindow.h"

#include <QAction>
#include <QFileDialog>
#include <QMenuBar>
#include <QDesktopWidget>
#include <QScreen>
#include <QDebug>
#include <QGuiApplication>
#include <QSqlQuery>
#include <QSqlError>
#include <QDateTime>
#include <QMessageBox>
#include <vector>
#include <numeric>
#include <algorithm>
#include <random>
#include <chrono>

#include "about_refio_dialog.h"
#include "settings_dialog.h"

MainWindow::MainWindow() : QMainWindow()
{
    lib_id = 1;
    qApp->setProperty("mainWindow", QVariant::fromValue<MainWindow *>(this));
    setup_db(); // lib_id and iCodeSeq should be reset here

    m_updater = QSimpleUpdater::getInstance();

    referenceWidget = new ReferenceWidget(lib_id, this);
    slipboxWidget = new SlipboxWidget(this);
    readSettings();

    slipboxWidget->hide();

    setCentralWidget(referenceWidget);
    createMenus();
    QDesktopWidget desktopWidget;
    QScreen *mainScreen = QGuiApplication::primaryScreen();
    QRect mainScreenSize = mainScreen->availableGeometry();
    resize(mainScreenSize.width() * 0.7f, mainScreenSize.height() * 0.7f);
    setWindowTitle("Refio");
    setObjectName(MAIN_WINDOW_NAME);

    if(isStacked){
        standardLayoutAct->setFont(menuItemRegularFont);
        stackedLayoutAct->setFont(menuItemBoldFont);
        referenceWidget->toggleRightView();
    }
}

MainWindow::~MainWindow(){
    QSqlDatabase db = QSqlDatabase::database(DATABASE_NAME);
    db.close();
    QSqlDatabase::removeDatabase( QSqlDatabase::defaultConnection );
}

MainWindow *MainWindow::instance() {
    return qApp ? qApp->property("mainWindow").value<MainWindow *>() : nullptr;
}

CollectionsWidget *MainWindow::collectionWidget(){
    MainWindow *mainWin =  qApp->property("mainWindow").value<MainWindow *>();
    if(mainWin){
        return mainWin->findChild<CollectionsWidget *>(REF_COLLECTIONS_WIDGET_NAME);
    }
    return nullptr;
}

EntriesWidget *MainWindow::entriesWidget(){
    MainWindow *mainWin =  qApp->property("mainWindow").value<MainWindow *>();
    if(mainWin){
        return mainWin->findChild<EntriesWidget *>(REF_ENTRIES_WIDGET_NAME);
    }
    return nullptr;
}

void MainWindow::readSettings()
{
    QSettings settings;
    settings.beginGroup("MainWindow");
    isSlipbox = settings.value("isSlipbox", false).toBool();
    isStacked = settings.value("isStacked", false).toBool();
    settings.endGroup();
}

void MainWindow::saveSettings()
{
    QSettings settings;
    settings.beginGroup("MainWindow");
    settings.setValue("isSlipbox", isSlipbox);
    settings.setValue("isStacked", isStacked);
    settings.endGroup();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    saveSettings();
    event->accept();
}

void MainWindow::createMenus()
{
    /*************/
    /* File Menu */
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));

    QAction *newCollectionAct = new QAction(tr("&New Collection"), this);
    fileMenu->addAction(newCollectionAct);
    connect(newCollectionAct, &QAction::triggered, this, &MainWindow::addNewCollection);

    QAction *openAct = new QAction(tr("&Open..."), this);
    fileMenu->addAction(openAct);
    connect(openAct, &QAction::triggered, this, &MainWindow::openFile);

    QAction *saveAct = new QAction(tr("&Save As..."), this);
    fileMenu->addAction(saveAct);
    connect(saveAct, &QAction::triggered, this, &MainWindow::saveFile);

    QAction *exportAct = new QAction(tr("&Export Library"), this);
    fileMenu->addAction(exportAct);
    connect(exportAct, &QAction::triggered, this, &MainWindow::exportLibrary);

    fileMenu->addSeparator();

    QAction *exitAct = new QAction(tr("E&xit"), this);
    fileMenu->addAction(exitAct);
    connect(exitAct, &QAction::triggered, this, &QWidget::close);

    /*************/
    /* Edit Menu */
    QMenu *editMenu = menuBar()->addMenu(tr("&Edit"));

    QAction *undoAct = new QAction(tr("&Undo"));
    undoAct->setShortcut(QKeySequence::Undo);
    editMenu->addAction(undoAct);
    connect(undoAct, &QAction::triggered, this, &MainWindow::undo);

    QAction *redoAct = new QAction(tr("&Redo"));
    redoAct->setShortcut(QKeySequence::Redo);
    editMenu->addAction(redoAct);
    connect(redoAct, &QAction::triggered, this, &MainWindow::redo);

    editMenu->addSeparator();

    QAction *cutAct = new QAction(tr("Cu&t"));
    cutAct->setShortcut(QKeySequence::Cut);
    editMenu->addAction(cutAct);
    connect(cutAct, &QAction::triggered, this, &MainWindow::cut);

    QAction *copyAct = new QAction(tr("&Copy"));
    copyAct->setShortcut(QKeySequence::Copy);
    editMenu->addAction(copyAct);
    connect(copyAct, &QAction::triggered, this, &MainWindow::copy);

    QAction *pasteAct = new QAction(tr("&Paste"));
    pasteAct->setShortcut(QKeySequence::Paste);
    editMenu->addAction(pasteAct);
    connect(pasteAct, &QAction::triggered, this, &MainWindow::paste);

    QAction *deleteAct = new QAction(tr("&Delete"));
    deleteAct->setShortcut(QKeySequence::Delete);
    editMenu->addAction(deleteAct);
    connect(deleteAct, &QAction::triggered, this, &MainWindow::deleteItem);

    editMenu->addSeparator();

    QAction *findAct = new QAction(tr("&Find"));
    findAct->setShortcut(QKeySequence::Find);
    editMenu->addAction(findAct);
    connect(findAct, &QAction::triggered, this, &MainWindow::findItem);

    QAction *advancedSearchAct = new QAction(tr("&Advanced Search"));
    editMenu->addAction(advancedSearchAct);
    connect(advancedSearchAct, &QAction::triggered, this, &MainWindow::advancedSearch);

    /*************/
    /* Add Menu */
    QMenu *addMenu = menuBar()->addMenu(tr("&Add"));

    QAction *addWebpageAct = new QAction(tr("&Webpage"));
    addMenu->addAction(addWebpageAct);
    connect(addWebpageAct, &QAction::triggered, this, &MainWindow::addWebpage);

    QAction *addArticleAct = new QAction(tr("&Article"));
    addMenu->addAction(addArticleAct);
    connect(addArticleAct, &QAction::triggered, this, &MainWindow::addArticle);

    QAction *addBookAct = new QAction(tr("&Book"));
    addMenu->addAction(addBookAct);
    connect(addBookAct, &QAction::triggered, this, &MainWindow::addBook);

    QAction *addDocumentAct = new QAction(tr("&Document"));
    addMenu->addAction(addDocumentAct);
    connect(addDocumentAct, &QAction::triggered, this, &MainWindow::addDocument);


    /*************/
    /* Tools Menu */
    QMenu *toolMenu = menuBar()->addMenu(tr("&Tools"));

    QAction *addAct = new QAction(tr("&Add Entry..."), this);
    toolMenu->addAction(addAct);
    //connect(addAct, &QAction::triggered, referenceWidget->collectionsWidget->addressWidget, &AddressWidget::showAddEntryDialog);

    editAct = new QAction(tr("&Edit Entry..."), this);
    editAct->setEnabled(false);
    toolMenu->addAction(editAct);
    //connect(editAct, &QAction::triggered, referenceWidget->collectionsWidget->addressWidget, &AddressWidget::editEntry);

    toolMenu->addSeparator();

    removeAct = new QAction(tr("&Remove Entry"), this);
    removeAct->setEnabled(false);
    toolMenu->addAction(removeAct);
    //connect(removeAct, &QAction::triggered, referenceWidget->collectionsWidget->addressWidget, &AddressWidget::removeEntry);

    //connect(referenceWidget->collectionsWidget->addressWidget, &AddressWidget::selectionChanged, this, &MainWindow::updateActions);

    toolMenu->addSeparator();

    QAction *settingsAct = new QAction(tr("&Preferences"));
    toolMenu->addAction(settingsAct);
    connect(settingsAct, &QAction::triggered, this, &MainWindow::showSettings);

    /*************/
    /* View Menu */
    QMenu *viewMenu = menuBar()->addMenu(tr("&View"));

    collectorViewAct = new QAction(tr("&Collector"));
    viewMenu->addAction(collectorViewAct);
    connect(collectorViewAct, &QAction::triggered, this, &MainWindow::toggleCollectorView);

    //TODO: active works but dissapear when click again
    //viewMenu->setActiveAction(collectorViewAct);
    //collectorViewAct->setChecked(true);
    menuItemRegularFont = viewMenu->font();
    menuItemBoldFont = menuItemRegularFont;
    menuItemBoldFont.setBold(true);

    collectorViewAct->setFont(menuItemBoldFont);

    slipboxViewAct = new QAction(tr("&SlipBox"));
    viewMenu->addAction(slipboxViewAct);
    connect(slipboxViewAct, &QAction::triggered, this, &MainWindow::toggleSlipboxView);

    viewMenu->addSeparator();

    QMenu *layoutSubMenu = viewMenu->addMenu("Layout");
    standardLayoutAct = new QAction(tr("&Standard"));
    standardLayoutAct->setFont(menuItemBoldFont);
    layoutSubMenu->addAction(standardLayoutAct);
    connect(standardLayoutAct, &QAction::triggered, this, &MainWindow::toggleStandardView);
    stackedLayoutAct = new QAction(tr("&Stacked"));
    layoutSubMenu->addAction(stackedLayoutAct);
    connect(stackedLayoutAct, &QAction::triggered, this, &MainWindow::toggleStackedView);

    QMenu *fontsizeSubMenu = viewMenu->addMenu("Font Size");
    QAction *biggerFontAct = new QAction(tr("&Bigger"));
    fontsizeSubMenu->addAction(biggerFontAct);
    connect(biggerFontAct, &QAction::triggered, this, &MainWindow::addArticle);
    QAction *defaultFontAct = new QAction(tr("&Default"));
    fontsizeSubMenu->addAction(defaultFontAct);
    connect(defaultFontAct, &QAction::triggered, this, &MainWindow::addArticle);
    QAction *smallerFontAct = new QAction(tr("&Smaller"));
    fontsizeSubMenu->addAction(smallerFontAct);
    connect(smallerFontAct, &QAction::triggered, this, &MainWindow::addArticle);

    /*************/
    /* Help Menu */
    QMenu *helpMenu = menuBar()->addMenu(tr("&Help"));

    QAction *documentationAct = new QAction(tr("&Documentation"));
    helpMenu->addAction(documentationAct);
    connect(documentationAct, &QAction::triggered, this, &MainWindow::openDocumentationWebpage);

    QAction *checkUpdatesAct = new QAction(tr("&Check for updates"));
    helpMenu->addAction(checkUpdatesAct);
    connect(checkUpdatesAct, &QAction::triggered, this, &MainWindow::checkForUpdates);

    QAction *aboutAct = new QAction(tr("&About Refio"));
    helpMenu->addAction(aboutAct);
    connect(aboutAct, &QAction::triggered, this, &MainWindow::showAboutRefio);
}

void MainWindow::addNewCollection()
{

}

void MainWindow::openFile()
{
    QString fileName = QFileDialog::getOpenFileName(this);
    if (!fileName.isEmpty()){
        //referenceWidget->addressWidget->readFromFile(fileName);
    }
}

void MainWindow::saveFile()
{
    QString fileName = QFileDialog::getSaveFileName(this);
    if (!fileName.isEmpty()){
        //referenceWidget->addressWidget->writeToFile(fileName);
    }
}

void MainWindow::exportLibrary()
{

}

void MainWindow::undo()
{

}

void MainWindow::redo()
{}

void MainWindow::cut()
{}

void MainWindow::copy()
{}

void MainWindow::paste()
{}

void MainWindow::deleteItem()
{}

void MainWindow::findItem()
{}

void MainWindow::advancedSearch()
{}

QString MainWindow::getNextICode()
{
    QSqlDatabase db = QSqlDatabase::database(DATABASE_NAME);
    QSqlQuery query(db);
    bool success = query.exec("SELECT MAX(id) from ref_entries");
    if(!success){
        qDebug() << query.lastError();
        return QString("");
    }
    int last_entry_id = 0;
    if(query.first()){
        // TODO: is this OK? re-use code if over limit?
        last_entry_id = query.value(0).toInt() % 32768;
    }else{
        qDebug() << "no record in ref_entries table";
    }
    qDebug() << "last_entry_id=" << last_entry_id;
    qDebug() << "iCodeSeq size=" << iCodeSeq.count();
    //qDebug() << "hex=" << iCodeSeq.toHex();
    QByteArray part = iCodeSeq.mid(last_entry_id * 2, 2);
    qDebug() << "part=" << part.toHex();
    unsigned char idx_1 = (unsigned char)iCodeSeq.at(last_entry_id * 2);
    unsigned char idx_2 = (unsigned char)iCodeSeq.at(last_entry_id * 2 + 1);
    unsigned int idx_iCodeChar = idx_1 * 256 + idx_2;
    qDebug() << "idx_1, idx_2, idx_iCodeChar=" << idx_1 << idx_2 << idx_iCodeChar;
    QString iCode = QString(QChar::fromLatin1(iCodeChar[idx_iCodeChar >> 10]));
    iCode.append(QChar::fromLatin1(iCodeChar[(idx_iCodeChar >> 5) & 0b11111]));
    iCode.append(QChar::fromLatin1(iCodeChar[idx_iCodeChar & 0b11111]));
    return iCode;
}

void MainWindow::addWebpage()
{

}

void MainWindow::openDocumentationWebpage()
{
    QDesktopServices::openUrl(QUrl("http://refio.org/document.html", QUrl::StrictMode));
}

void MainWindow::addDocument()
{}

void MainWindow::addBook()
{}

void MainWindow::addArticle()
{}

void MainWindow::toggleCollectorView()
{
    if(isSlipbox){
        collectorViewAct->setFont(menuItemBoldFont);
        slipboxViewAct->setFont(menuItemRegularFont);
        isSlipbox = false;
        slipboxWidget->hide();
        referenceWidget->show();
    }
}

void MainWindow::toggleSlipboxView()
{
    if(!isSlipbox){
        collectorViewAct->setFont(menuItemRegularFont);
        slipboxViewAct->setFont(menuItemBoldFont);
        isSlipbox = true;
        slipboxWidget->show();
        referenceWidget->hide();
    }

}

void MainWindow::toggleStandardView()
{
    if(isStacked){
        standardLayoutAct->setFont(menuItemBoldFont);
        stackedLayoutAct->setFont(menuItemRegularFont);
        referenceWidget->toggleRightView();
        isStacked = false;
    }
}

void MainWindow::toggleStackedView()
{
    if(!isStacked){
        standardLayoutAct->setFont(menuItemRegularFont);
        stackedLayoutAct->setFont(menuItemBoldFont);
        referenceWidget->toggleRightView();
        isStacked = true;
    }

}

void MainWindow::showAboutRefio()
{
     AboutRefioDialog aDialog;
     aDialog.exec();
}

void MainWindow::showSettings()
{
     SettingsDialog aDialog;
     aDialog.exec();
}


void MainWindow::updateActions(const QItemSelection &selection)
{
    QModelIndexList indexes = selection.indexes();

    if (!indexes.isEmpty()) {
        removeAct->setEnabled(true);
        editAct->setEnabled(true);
    } else {
        removeAct->setEnabled(false);
        editAct->setEnabled(false);
    }
}

void MainWindow::setup_db()
{
    QSqlDatabase db;
    QString appDataDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir dir(appDataDir);
    if(!dir.exists()){
        dir.mkpath(".");
    }
    qDebug() << "appDataDir - " << appDataDir;
    QString path(appDataDir);
    path.append(QDir::separator()).append("data.db");
    bool createTables = false;
    QFile dbFile;
    dbFile.setFileName(path);
    if(!dbFile.exists()){
        createTables = true;
    }

    db = QSqlDatabase::addDatabase("QSQLITE", DATABASE_NAME);
    db.setDatabaseName(path);

    if (!db.open()) {
        qDebug() << "Can't connect to DB!";
    }else{
        // increase page_size so we read large blob(ref_libraries.icodeseq) faster
        db.exec("PRAGMA page_size = 8192");
        db.exec("PRAGMA foreign_keys = ON");
        // TODO: should we increase cache?
        //db.exec("PRAGMA cache_size = 8192");
        QSqlQuery query(db);
        if(createTables){
            bool success;
            QFile schemaFile(":/schema.sql");
            if(!schemaFile.open(QFile::ReadOnly)){
                qDebug() << "Could not read schema file";
                return;
            }
            QStringList sqls = QString(schemaFile.readAll()).split("-- #");
            //qDebug() << sqls.at(1);
            schemaFile.close();
            for(const QString sql: sqls){
                success = query.exec(sql);
                if(!success){
                    qDebug() << query.lastError().text();
                    qDebug() << "sql=" << sql;
                    return;
                }
            }

            query.prepare( "INSERT INTO ref_libraries (id, icodeseq, created) VALUES (1, :icodeseq, :created)");
            iCodeSeq = genICodeSeq();
            //qDebug() << iCodeSeq.toHex();
            query.bindValue( ":icodeseq", iCodeSeq);
            query.bindValue( ":created", QDateTime::currentDateTime().toString());
            if( !query.exec() ){
                qDebug() << "Error inserting first library into table:\n" << query.lastError();
            }
            query.prepare( "INSERT INTO ref_collections (lib_id, name, created)"
                           " VALUES (1, \"Default Collection\", :created)");
            query.bindValue( ":created", QDateTime::currentDateTime().toString());
            if( !query.exec() ){
                qDebug() << "Error inserting first collection into table:\n" << query.lastError();
            }
        }else{
            // TODO: use last library number
            query.prepare("SELECT id, name, icodeseq FROM ref_libraries LIMIT 1");
            if( !query.exec() ){
                qDebug() << "Error inserting first library into table:\n" << query.lastError();
            }else{
                query.first();
                lib_id = query.value(0).toInt();
                qDebug() << "mainwindow lib_id=" << lib_id;
                iCodeSeq = query.value(2).toByteArray();
            }
        }
    }
}

QByteArray MainWindow::genICodeSeq()
{
    std::vector<quint16> idx(32768);
    std::iota(std::begin(idx), std::end(idx), 0);
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    shuffle(idx.begin(), idx.end(), std::default_random_engine(seed));
    QByteArray byteArray;
    for(const auto& v: idx){
        if(v<256){
            byteArray.append('\0');
        }else{
            byteArray.append(v>>8);
        }
        byteArray.append(v & 0xff);
    }
    return byteArray;
}

void MainWindow::checkForUpdates()
{
    m_updater->setModuleVersion(UPDATE_DEFS_URL, APPLICATION_VERSION); //installed version
    m_updater->setNotifyOnFinish(UPDATE_DEFS_URL, false);
    m_updater->setNotifyOnUpdate(UPDATE_DEFS_URL, true);
    m_updater->setUseCustomAppcast(UPDATE_DEFS_URL, false);
    m_updater->setDownloaderEnabled(UPDATE_DEFS_URL, true);
    m_updater->setMandatoryUpdate(UPDATE_DEFS_URL, false);

    /* Check for updates */
    m_updater->checkForUpdates(UPDATE_DEFS_URL);
}

