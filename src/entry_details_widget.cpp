/****************************************************************************
**
**
****************************************************************************/

#include "constants.h"
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
    setObjectName(REF_ENTRY_DETAILS_WIDGET_NAME);
}

void EntryDetailsWidget::setupTabs()
{
    addTab(entryNoteTab, tr("Note"));

}

void EntryDetailsWidget::updateDetail(const MTreeItem *data)
{
    //"Title", "id", "cid", "icode", "parent", "Note"
    QString icode = data->data(3).toString();
    QString info = data->data(4).toString();
    QString note = data->data(5).toString();
    entryInfoTab->updateLabel(info);
    entryNoteTab->updateLabel(note);
    //

}
