/****************************************************************************
**
**
****************************************************************************/

#ifndef ENTRY_DETAILS_WIDGET_H
#define ENTRY_DETAILS_WIDGET_H

#include <QItemSelection>
#include <QTabWidget>
#include <QWebEngineView>
#include "entry_info_tab.h"
#include "entry_note_tab.h"
#include "mtree_item.h"
#include "markdown_document.h"

QT_BEGIN_NAMESPACE
class QItemSelectionModel;
QT_END_NAMESPACE

class EntryDetailsWidget : public QTabWidget
{
    Q_OBJECT

public:
    EntryDetailsWidget(QWidget *parent = nullptr);
    EntryInfoTab *entryInfoTab;
    EntryNoteTab *entryNoteTab;
    void updateDetail(const MTreeItem *data);

//public slots:

//signals:
//    void selectionChanged (const QItemSelection &selected);

private:
    QPlainTextEdit *noteEdit;
    QWebEngineView *notePreview;
    QWidget *standaloneEditor;
    MarkdownDocument m_content;
    void setupTabs();
    void addNote();
    void previewNote();

};

#endif
