/****************************************************************************
**
**
****************************************************************************/

#include "entry_note_tab.h"

#include <QtWidgets>

EntryNoteTab::EntryNoteTab(QWidget *parent)
    : QWidget(parent)
{

    noteLabel = new QLabel(tr("There are currently no contacts in your address book. "
                                          "\nClick Add to add new contacts."));

    auto addButton = new QPushButton(tr("Add"));

    connect(addButton, &QAbstractButton::clicked, this, &EntryNoteTab::addEntry);

    auto mainLayout = new QVBoxLayout;
    mainLayout->addWidget(noteLabel);
    mainLayout->addWidget(addButton, 0, Qt::AlignCenter);

    setLayout(mainLayout);
}

void EntryNoteTab::addEntry()
{

}

void EntryNoteTab::updateLabel(const QString &qs){
    noteLabel->setText(qs);
}
