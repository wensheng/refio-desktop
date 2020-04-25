/****************************************************************************
**
**
****************************************************************************/

#include "constants.h"
#include "entry_info_tab.h"

#include <QtWidgets>

EntryInfoTab::EntryInfoTab(QWidget *parent)
    : QWidget(parent)
{

    infoLabel = new QLabel(tr("There are currently no contacts in your address book. "
                                          "\nClick Add to add new contacts."));

    auto addButton = new QPushButton(tr("Add"));

    connect(addButton, &QAbstractButton::clicked, this, &EntryInfoTab::addEntry);

    auto mainLayout = new QVBoxLayout;
    mainLayout->addWidget(infoLabel);
    mainLayout->addWidget(addButton, 0, Qt::AlignCenter);

    setLayout(mainLayout);
    setObjectName(REF_ENTRY_DETAILS_INFO_TAB_NAME);
}

void EntryInfoTab::addEntry()
{

}

void EntryInfoTab::updateLabel(const QString &qs){
    infoLabel->setText(qs);
}
