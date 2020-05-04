/****************************************************************************
**
**
****************************************************************************/

#include "constants.h"
#include "entry_note_tab.h"
#include "markdownhighlighter.h"

#include <QtWidgets>

EntryNoteTab::EntryNoteTab(QWidget *parent)
    : QWidget(parent),
      layout(new QVBoxLayout)
{
    setLayout(layout);
    setObjectName(REF_ENTRY_DETAILS_NOTE_TAB_NAME);
    qDebug() << metaObject()->className();
}

void EntryNoteTab::addEntry()
{

}

void EntryNoteTab::setup(QPlainTextEdit *edit, QWebEngineView *view)
{
    layout->addWidget(edit);
    layout->addWidget(view);
    edit->show();
    view->hide();
}

void EntryNoteTab::updateLabel(const QString &qs)
{
    noteLabel->setText(qs);
}

void EntryNoteTab::updateTab(const QList<QVector<QVariant> > &notes)
{

}
