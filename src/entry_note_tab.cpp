/****************************************************************************
**
**
****************************************************************************/

#include "constants.h"
#include "entry_note_tab.h"
#include "markdownhighlighter.h"

#include <QtWidgets>

EntryNoteTab::EntryNoteTab(QPlainTextEdit *noteEdit, QWebEngineView *notePreview, QWidget *parent)
    : QWidget(parent),
      layout(new QVBoxLayout),
      edit(noteEdit),
      preview(notePreview)
{
    layout->addWidget(edit);
    layout->addWidget(preview);
    preview->setVisible(false);

    setLayout(layout);
    setObjectName(REF_ENTRY_DETAILS_NOTE_TAB_NAME);
    qDebug() << metaObject()->className();
}

void EntryNoteTab::addEntry()
{

}

void EntryNoteTab::updateLabel(const QString &qs){
    noteLabel->setText(qs);
}

void EntryNoteTab::updateTab(const QList<QVector<QVariant> > &notes)
{

}
