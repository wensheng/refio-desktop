/****************************************************************************
**
**
****************************************************************************/

#include "reference_widget.h"

#include <QtWidgets>

ReferenceWidget::ReferenceWidget(int lib_id, QWidget *parent)
    : QWidget(parent)
{
    collectionsWidget = new CollectionsWidget(lib_id),
    rightWidget = new QWidget,
    entriesWidget = new EntriesWidget,
    entryDetailsWidget = new EntryDetailsWidget;

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
    qDebug() << metaObject()->className();
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
