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
      notesTabWidget(new QTabWidget(this)),
      standaloneEditor(new StandaloneEditor)
{
    entryId = 0;
    currentNoteId = 0;
    isPreviewing = false;

    addTab(entryInfoTab, tr("Info"));

    auto tb = new QToolBar();

    const QIcon standaloneIcon = QIcon::fromTheme("document-save", QIcon(":/images/save.png"));
    QAction *openStandalone = new QAction(standaloneIcon, tr("&Standalone"), this);
    connect(openStandalone, &QAction::triggered, this, &EntryDetailsWidget::openStandaloneEditor);
    tb->addAction(openStandalone);

    const QIcon newIcon = QIcon::fromTheme("document-new", QIcon(":/images/new.png"));
    QAction *newAct = new QAction(newIcon, tr("&Add Note"), this);
    connect(newAct, &QAction::triggered, this, &EntryDetailsWidget::addNote);
    tb->addAction(newAct);

    const QIcon openIcon = QIcon::fromTheme("document-open", QIcon(":/images/open.png"));
    QAction *openAct = new QAction(openIcon, tr("&Preview"), this);
    connect(openAct, &QAction::triggered, this, &EntryDetailsWidget::previewNote);
    tb->addAction(openAct);

    const QIcon saveNoteIcon = QIcon::fromTheme("document-save", QIcon(":/images/save.png"));
    QAction *saveNoteAct = new QAction(saveNoteIcon, tr("&Save Note"), this);
    connect(saveNoteAct, &QAction::triggered, this, &EntryDetailsWidget::saveNote);
    tb->addAction(saveNoteAct);

    //QToolButton *btn = new QToolButton(notesTab);
    //btn->setText("Add Note");
    //btn->setCursor(Qt::ArrowCursor);
    //btn->setAutoRaise(true);
    notesTabWidget->setCornerWidget(tb, Qt::TopRightCorner);

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
    standaloneEditor->setWindowTitle(tr("Markdown Editor"));
    standaloneEditor->setWindowIcon(QIcon(":/images/save.png"));
    connect(standaloneEditor, &StandaloneEditor::beClosed, this, &EntryDetailsWidget::standaloneEditorClosed);

    firstNoteTab = new EntryNoteTab;
    firstNoteTab->setup(noteEdit, notePreview);
    notesTabWidget->addTab(firstNoteTab, tr("Note 1"));
    connect(notesTabWidget, &QTabWidget::currentChanged, this, &EntryDetailsWidget::currentTabChanged);

    addTab(notesTabWidget, tr("Notes"));

    setObjectName(REF_ENTRY_DETAILS_WIDGET_NAME);
    qDebug() << metaObject()->className();
}

void EntryDetailsWidget::addNote()
{
    if(notes.count() < notesTabWidget->count()){
        QMessageBox::warning(this,
                             tr("Warning"),
                             tr("Please edit/save current note before creating new note") );
        return;
    }
    currentNoteId = 0;
    int oldCount = noteIds.count();
    QString tabTitle("Note ");
    tabTitle = tabTitle.append(QString::number(oldCount + 1));
    notesTabWidget->addTab(new EntryNoteTab, tabTitle);
    notesTabWidget->setCurrentIndex(oldCount);
    //noteEdit->setParent(tab);
    //notePreview->setParent(tab);
    //noteEdit->setPlainText("");
    //noteEdit->show();
    //notePreview->hide();
    //currentTabChanged will be called
}

void EntryDetailsWidget::currentTabChanged(int index)
{
    EntryNoteTab *tab = qobject_cast<EntryNoteTab*>(notesTabWidget->currentWidget());
    noteEdit->setParent(tab);
    if(noteIds.count() <= index){
        // newly added tab
        noteEdit->setPlainText("");
    }else{
        noteEdit->setPlainText(notes[noteIds[index]]);
        currentNoteId = noteIds[index];
    }
    noteEdit->show();
    notePreview->setParent(tab);
    notePreview->hide();
    qDebug() << "current index:" << index << ", currentNoteId:" << currentNoteId;
}

void EntryDetailsWidget::standaloneEditorClosed()
{
    //noteEdit->document()->setPlainText(noteEdit2->toPlainText());
    // TODO:
    //noteEdit->setParent(noteTab);
    //notePreview->setParent(noteTab);
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
    entryId = entry->itemId();
    entryInfoTab->updateLabel(entry);

    notes.clear();
    noteIds.clear();
    // remove all nota tabs except first one
    for(int i=0; i< noteWidgets.size(); i++){
        delete noteWidgets.at(i);
    }
    noteWidgets.clear();

    int count = notesTabWidget->count();
    for(int i=1; i < count; i++){
        notesTabWidget->removeTab(i);
    }

    QSqlDatabase db = QSqlDatabase::database(DATABASE_NAME);
    QSqlQuery query(db);
    query.prepare("select id, body from ref_notes where entry_id=:eid");
    query.bindValue(":eid", entryId);
    if(!query.exec()){
        qDebug() << query.lastError().text();
    }else{
        while (query.next()) {
            noteIds.append(query.value(0).toInt());
            notes.insert(query.value(0).toInt(),  query.value(1).toString());
        }
    }
    firstNoteTab->setup(noteEdit, notePreview);
    if(!notes.isEmpty()){
        noteEdit->setPlainText(notes[noteIds.at(0)]);
        if(notes.count()>1){
            for(int i=1;i<notes.size();i++){
                EntryNoteTab *noteTab = new EntryNoteTab;
                noteWidgets.append(noteTab);
                notesTabWidget->addTab(noteTab, tr("Note"));
            }
        }
        //EntryNoteTab *first = noteWidgets.last();
        //first->setup(noteEdit, notePreview);
    }
    //entryNoteTab->updateTab(notes);
}

void EntryDetailsWidget::saveNote()
{
    QSqlDatabase db = QSqlDatabase::database(DATABASE_NAME);
    QSqlQuery query(db);
    if(currentNoteId == 0){
        query.prepare("insert into ref_notes (entry_id, title, body, created)"
                      "values (:entry_id, :title, :body, :created)");
        query.bindValue(":entry_id", entryId);
        QString body = noteEdit->toPlainText();
        query.bindValue(":body", body);
        QStringList ss = body.split("\n");
        query.bindValue(":title", ss.at(0));
        query.bindValue(":created", QDateTime::currentDateTime().toString(Qt::ISODate));
        if(!query.exec()){
            qDebug() << query.lastError().text();
        }else{
            int iid = query.lastInsertId().toInt();
            qDebug() << "lastInsertId()=" << iid;
            noteIds.append(iid);
            notes.insert(iid, body);
            currentNoteId = iid;
        }
    }else{
        query.prepare("update ref_notes set "
                      " title = :title, "
                      " body = :body where id = :note_id");
        query.bindValue(":note_id", currentNoteId);
        QString body = noteEdit->toPlainText();
        query.bindValue(":body", body);
        QStringList ss = body.split("\n");
        query.bindValue(":title", ss.at(0));
        if(!query.exec()){
            qDebug() << query.lastError().text();
        }
    }
}
