/****************************************************************************
**
**
****************************************************************************/

#include "entry_details_widget.h"
#include "adddialog.h"

#include <QtWidgets>

EntryDetailsWidget::EntryDetailsWidget(QWidget *parent)
    : QTabWidget(parent),
      entryInfoTab(new EntryInfoTab(this))
{
    //auto tb = new QToolBar();
    //tb->addAction("hello");
    //tb->addAction("world");
    //connect(entryInfoTab, &EntryInfoTab::sendDetails, this, &EntryDetailsWidget::addEntry);

    addTab(entryInfoTab, tr("Address Book"));

    setupTabs();
}

void EntryDetailsWidget::setupTabs()
{

}
