/****************************************************************************
**
**
****************************************************************************/

#ifndef ENTRY_NOTE_TAB_H
#define ENTRY_NOTE_TAB_H

#include <QWidget>
#include <QPlainTextEdit>
#include <QWebEngineView>

QT_BEGIN_NAMESPACE
class QLabel;
class QPushButton;
class QVBoxLayout;
QT_END_NAMESPACE

class EntryNoteTab : public QWidget
{
    Q_OBJECT

public:
    EntryNoteTab(QWidget *parent = nullptr);
    void setup(QPlainTextEdit *edit, QWebEngineView *view);
    void updateLabel(const QString &);
    void updateTab(const QList<QVector<QVariant>> &);

public slots:
    void addEntry();

signals:
    void sendDetails(const QString &name, const QString &address);
private:
    QLabel *noteLabel;
    QVBoxLayout *layout;
};

#endif
