/****************************************************************************
**
**
****************************************************************************/

#include "entry_info_tab.h"

#include <QtWidgets>

EntryInfoTab::EntryInfoTab(QWidget *parent)
    : QWidget(parent)
{

    auto descriptionLabel = new QLabel(tr("There are currently no contacts in your address book. "
                                          "\nClick Add to add new contacts."));

    auto addButton = new QPushButton(tr("Add"));

    connect(addButton, &QAbstractButton::clicked, this, &EntryInfoTab::addEntry);

    auto mainLayout = new QVBoxLayout;
    mainLayout->addWidget(descriptionLabel);
    mainLayout->addWidget(addButton, 0, Qt::AlignCenter);

    setLayout(mainLayout);
}

void EntryInfoTab::addEntry()
{

}
