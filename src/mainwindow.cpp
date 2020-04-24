/****************************************************************************
**
**
****************************************************************************/

#include "mainwindow.h"

#include <QAction>
#include <QFileDialog>
#include <QMenuBar>
#include <QDesktopWidget>
#include <QScreen>
#include <QDebug>
#include <QGuiApplication>

#include "about_refio_dialog.h"
#include "settings_dialog.h"

MainWindow::MainWindow()
    : QMainWindow(),
      referenceWidget(new ReferenceWidget),
      slipboxWidget(new SlipboxWidget)
{
    // referenceWidget is instantiated first, database connection is set up there
    isSlipbox = false;
    isStacked = false;
    slipboxWidget->hide();

    setCentralWidget(referenceWidget);
    createMenus();
    QDesktopWidget desktopWidget;
    QScreen *mainScreen = QGuiApplication::primaryScreen();
    QRect mainScreenSize = mainScreen->availableGeometry();
    resize(mainScreenSize.width() * 0.7f, mainScreenSize.height() * 0.7f);
    setWindowTitle("Refio");
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

void MainWindow::addWebpage()
{}

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
    }
}

void MainWindow::toggleSlipboxView()
{
    if(!isSlipbox){
        collectorViewAct->setFont(menuItemRegularFont);
        slipboxViewAct->setFont(menuItemBoldFont);
        isSlipbox = true;
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

