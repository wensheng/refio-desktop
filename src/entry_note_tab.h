/****************************************************************************
**
**
****************************************************************************/

#ifndef ENTRY_NOTE_TAB_H
#define ENTRY_NOTE_TAB_H

#include <QWidget>

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
    void updateLabel(const QString &);

public slots:
    void addEntry();

signals:
    void sendDetails(const QString &name, const QString &address);
private:
    QLabel *noteLabel;
};

#endif
