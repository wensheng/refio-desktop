/****************************************************************************
**
**
****************************************************************************/

#include "constants.h"
#include "entry_details_widget.h"
#include "adddialog.h"
#include "markdownhighlighter.h"
#include "note_preview_page.h"

#include <QtWidgets>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QWebChannel>

EntryDetailsWidget::EntryDetailsWidget(QWidget *parent)
    : QTabWidget(parent),
      entryInfoTab(new EntryInfoTab(this)),
      standaloneEditor(new StandaloneEditor)
{
    //auto tb = new QToolBar();
    //tb->addAction("hello");
    //tb->addAction("world");
    //connect(entryInfoTab, &EntryInfoTab::sendDetails, this, &EntryDetailsWidget::addEntry);
    isPreviewing = false;

    addTab(entryInfoTab, tr("Info"));
    QTabWidget *notesTab = new QTabWidget(this);

    auto tb = new QToolBar();
    const QIcon newIcon = QIcon::fromTheme("document-new", QIcon(":/images/new.png"));
    QAction *newAct = new QAction(newIcon, tr("&Add Note"), this);
    connect(newAct, &QAction::triggered, this, &EntryDetailsWidget::addNote);
    tb->addAction(newAct);

    const QIcon openIcon = QIcon::fromTheme("document-open", QIcon(":/images/open.png"));
    QAction *openAct = new QAction(openIcon, tr("&Preview"), this);
    connect(openAct, &QAction::triggered, this, &EntryDetailsWidget::previewNote);
    tb->addAction(openAct);

    const QIcon standaloneIcon = QIcon::fromTheme("document-save", QIcon(":/images/save.png"));
    QAction *openStandalone = new QAction(standaloneIcon, tr("&Standalone"), this);
    connect(openStandalone, &QAction::triggered, this, &EntryDetailsWidget::openStandaloneEditor);
    tb->addAction(openStandalone);

    //QToolButton *btn = new QToolButton(notesTab);
    //btn->setText("Add Note");
    //btn->setCursor(Qt::ArrowCursor);
    //btn->setAutoRaise(true);
    notesTab->setCornerWidget(tb, Qt::TopRightCorner);

    noteEdit = new QPlainTextEdit(this);
    auto doc = noteEdit->document();
    auto *highlighter = new MarkdownHighlighter(doc);
    notePreview = new QWebEngineView(this);
    notePreview->setContextMenuPolicy(Qt::NoContextMenu);
    NotePreviewPage *page = new NotePreviewPage(this);
    notePreview->setPage(page);
    connect(noteEdit, &QPlainTextEdit::textChanged, [this]() { m_content.setText(noteEdit->toPlainText()); });

    QWebChannel *channel = new QWebChannel(this);
    channel->registerObject(QStringLiteral("content"), &m_content);
    page->setWebChannel(channel);
    notePreview->setUrl(QUrl("qrc:/index.html"));

    QHBoxLayout *standaloneEditorLayout = new QHBoxLayout(standaloneEditor);
    standaloneEditorSplitter = new QSplitter(Qt::Horizontal, standaloneEditor);
    standaloneEditorLayout->addWidget(standaloneEditorSplitter);
    standaloneEditor->setLayout(standaloneEditorLayout);
    standaloneEditor->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    standaloneEditor->setMaximumSize(1280, 800);
    standaloneEditor->resize(1280, 800);
    standaloneEditor->hide();
    connect(standaloneEditor, &StandaloneEditor::beClosed, this, &EntryDetailsWidget::standaloneEditorClosed);

    noteTab = new EntryNoteTab(noteEdit, notePreview);
    notesTab->addTab(noteTab, tr("Note"));

    addTab(notesTab, tr("Notes"));

    //setupTabs();
    setObjectName(REF_ENTRY_DETAILS_WIDGET_NAME);
    qDebug() << metaObject()->className();
}

void EntryDetailsWidget::setupTabs()
{
    addTab(entryNoteTab, tr("Notes"));

}

void EntryDetailsWidget::addNote()
{

}

void EntryDetailsWidget::standaloneEditorClosed()
{
    //noteEdit->document()->setPlainText(noteEdit2->toPlainText());
    noteEdit->setParent(noteTab);
    notePreview->setParent(noteTab);
    if(isPreviewing){
        noteEdit->setVisible(false);
        notePreview->setVisible(true);
        notePreview->show();
    }else{
        noteEdit->setVisible(true);
        notePreview->setVisible(false);
        noteEdit->show();
    }
}


void EntryDetailsWidget::previewNote()
{
    if(noteEdit->isVisible()){
        noteEdit->setVisible(false);
        notePreview->setVisible(true);
        isPreviewing = true;
    }else{
        noteEdit->setVisible(true);
        notePreview->setVisible(false);
        isPreviewing = false;
    }
}

void EntryDetailsWidget::openStandaloneEditor()
{
    //noteEdit2->document()->setPlainText(noteEdit->toPlainText());
    noteEdit->setParent(standaloneEditorSplitter);
    noteEdit->show();
    notePreview->setParent(standaloneEditorSplitter);
    notePreview->show();
    standaloneEditorSplitter->setSizes({640, 640});
    standaloneEditor->show();
}

void EntryDetailsWidget::updateDetail(const MTreeItem *entry)
{
    //"Title", "id", "cid", "icode", "parent", "Note"
    int entry_id = entry->itemId();

    QString info("iCode: ");
    info = info.append(entry->data(3).toString()).append("\n");
    info = info.append("Title: ").append(entry->data(0).toString()).append("\n");
    info = info.append("Created: ").append(entry->data(6).toString());
    entryInfoTab->updateLabel(info);

    QSqlDatabase db = QSqlDatabase::database(DATABASE_NAME);
    QList<QVector<QVariant>> notes;
    QSqlQuery query(db);
    query.prepare("select id, entry_id, title, body, created from ref_notes where entry_id=:eid");
    query.bindValue(":eid", entry_id);
    if(!query.exec()){
        qDebug() << query.lastError().text();
    }else{
        while (query.next()) {
            QVector<QVariant> result;
            result.push_back(query.value(0).toInt()); // id
            result.push_back(query.value(1).toInt()); // eid
            result.push_back(query.value(2).toString()); // title
            result.push_back(query.value(3).toString()); // body
            result.push_back(query.value(4).toString()); // created
            notes.push_back(result);
        }
    }
    entryNoteTab->updateTab(notes);
}
