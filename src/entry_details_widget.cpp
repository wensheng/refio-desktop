/****************************************************************************
**
**
****************************************************************************/

#include "entry_details_widget.h"
#include "adddialog.h"

#include <QtWidgets>

EntryDetailsWidget::EntryDetailsWidget(QWidget *parent)
    : QTabWidget(parent),
      entryInfoTab(new EntryInfoTab(this)),
      entryNoteTab(new EntryNoteTab(this))
{
    //auto tb = new QToolBar();
    //tb->addAction("hello");
    //tb->addAction("world");
    //connect(entryInfoTab, &EntryInfoTab::sendDetails, this, &EntryDetailsWidget::addEntry);

    addTab(entryInfoTab, tr("Info"));

    setupTabs();
}

void EntryDetailsWidget::setupTabs()
{
    addTab(entryNoteTab, tr("Note"));

}

void EntryDetailsWidget::updateDetail(const TreeItem *data)
{
    QString title = data->data(0).toString();
    QString note = data->data(1).toString();
    entryInfoTab->updateLabel(title);
    entryNoteTab->updateLabel(note);
    //

}
