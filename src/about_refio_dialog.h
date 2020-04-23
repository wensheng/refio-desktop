/****************************************************************************
**
**
****************************************************************************/

#ifndef ABOUT_REFIO_H
#define ABOUT_REFIO_H

#include <QDialog>

QT_BEGIN_NAMESPACE
class QLabel;
class QPushButton;
QT_END_NAMESPACE

class AboutRefioDialog : public QDialog
{
    Q_OBJECT

public:
    AboutRefioDialog(QWidget *parent = nullptr);

};

#endif // ABOUT_REFIO_H
