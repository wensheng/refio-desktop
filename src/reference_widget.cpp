/****************************************************************************
**
**
****************************************************************************/

#include "reference_widget.h"

#include <QtWidgets>

ReferenceWidget::ReferenceWidget(QWidget *parent)
    : QWidget(parent),
      collectionsWidget(new CollectionsWidget),
      rightWidget(new QWidget),
      entriesWidget(new EntriesWidget),
      entryDetailsWidget(new EntryDetailsWidget)
{
    /*
    auto tb = new QToolBar();

    const QIcon newIcon = QIcon::fromTheme("document-new", QIcon(":/images/new.png"));
    QAction *newAct = new QAction(newIcon, tr("&New"), this);
    newAct->setShortcuts(QKeySequence::New);
    newAct->setStatusTip(tr("Create a new file"));
    connect(newAct, &QAction::triggered, this, &ReferenceWidget::newFile);
    tb->addAction(newAct);

    const QIcon openIcon = QIcon::fromTheme("document-open", QIcon(":/images/open.png"));
    QAction *openAct = new QAction(openIcon, tr("&Open..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open an existing file"));
    connect(openAct, &QAction::triggered, this, &ReferenceWidget::openFile);
    tb->addAction(openAct);

    const QIcon saveIcon = QIcon::fromTheme("document-save", QIcon(":/images/save.png"));
    QAction *saveAct = new QAction(saveIcon, tr("&Save"), this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip(tr("Save the document to disk"));
    connect(saveAct, &QAction::triggered, this, &ReferenceWidget::closeFile);
    tb->addAction(saveAct);
    */

    //setSizePolicy(QSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding));
    QHBoxLayout *hBoxLayout = new QHBoxLayout;
    innerSplitter = new QSplitter(Qt::Horizontal, rightWidget);
    innerSplitter->addWidget(entriesWidget);
    innerSplitter->addWidget(entryDetailsWidget);
    innerSplitter->setHandleWidth(2);
    hBoxLayout->addWidget(innerSplitter);
    hBoxLayout->setContentsMargins(0,0,0,0);
    rightWidget->setLayout(hBoxLayout);

    auto layout = new QHBoxLayout;
    QSplitter *splitter = new QSplitter(Qt::Horizontal, this);
    //QSizePolicy leftSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    //leftSizePolicy.setHorizontalStretch(1);
    //collectionsWidget->setSizePolicy(leftSizePolicy);
    //layout->addWidget(collectionsWidget);
    splitter->addWidget(collectionsWidget);

    //QSizePolicy rightSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    //rightSizePolicy.setHorizontalStretch(3);
    //rightWidget->setSizePolicy(rightSizePolicy);
    //layout->addWidget(rightWidget);
    splitter->addWidget(rightWidget);
    //splitter->setStretchFactor(1,1);
    int thisWidth = width();
    int leftWidth = (int) thisWidth * 0.2;
    int rightWidth = thisWidth - leftWidth;
    splitter->setSizes({leftWidth, rightWidth});
    splitter->setHandleWidth(1);
    layout->addWidget(splitter);

    layout->setContentsMargins(0,0,0,0);
    setLayout(layout);
    // uncomment to adjust layout
    //setStyleSheet("border: 1px solid red");
}

void ReferenceWidget::newFile()
{

}

void ReferenceWidget::openFile()
{

}

void ReferenceWidget::closeFile()
{

}

void ReferenceWidget::toggleRightView()
{
    Qt::Orientation orientation = innerSplitter->orientation();
    if(orientation == Qt::Horizontal){
        innerSplitter->setOrientation(Qt::Vertical);
    }else{
        innerSplitter->setOrientation(Qt::Horizontal);
    }
}
