/****************************************************************************
**
**
****************************************************************************/

#ifndef ENTRY_INFO_TAB_H
#define ENTRY_INFO_TAB_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QLabel;
class QPushButton;
class QVBoxLayout;
QT_END_NAMESPACE

class EntryInfoTab : public QWidget
{
    Q_OBJECT

public:
    EntryInfoTab(QWidget *parent = nullptr);
    void updateLabel(const QString &);

public slots:
    void addEntry();

signals:
    void sendDetails(const QString &name, const QString &address);

private:
    QLabel *infoLabel;
};

#endif
