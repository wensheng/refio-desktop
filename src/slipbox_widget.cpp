/****************************************************************************
**
**
****************************************************************************/

#include "slipbox_widget.h"

#include <QtWidgets>

SlipboxWidget::SlipboxWidget(QWidget *parent)
    : QWidget(parent),
      addressWidget(new AddressWidget(this))
{
    auto tb = new QToolBar();

    const QIcon newIcon = QIcon::fromTheme("document-new", QIcon(":/images/new.png"));
    QAction *newAct = new QAction(newIcon, tr("&New"), this);
    newAct->setShortcuts(QKeySequence::New);
    newAct->setStatusTip(tr("Create a new file"));
    connect(newAct, &QAction::triggered, this, &SlipboxWidget::newFile);
    tb->addAction(newAct);

    const QIcon openIcon = QIcon::fromTheme("document-open", QIcon(":/images/open.png"));
    QAction *openAct = new QAction(openIcon, tr("&Open..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open an existing file"));
    connect(openAct, &QAction::triggered, this, &SlipboxWidget::openFile);
    tb->addAction(openAct);

    const QIcon saveIcon = QIcon::fromTheme("document-save", QIcon(":/images/save.png"));
    QAction *saveAct = new QAction(saveIcon, tr("&Save"), this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip(tr("Save the document to disk"));
    connect(saveAct, &QAction::triggered, this, &SlipboxWidget::closeFile);
    tb->addAction(saveAct);

    auto layout = new QVBoxLayout();
    layout->setMenuBar(tb);
    layout->addWidget(addressWidget);
    layout->setContentsMargins(0,0,0,0);
    setLayout(layout);
}

void SlipboxWidget::newFile()
{

}

void SlipboxWidget::openFile()
{

}

void SlipboxWidget::closeFile()
{

}
