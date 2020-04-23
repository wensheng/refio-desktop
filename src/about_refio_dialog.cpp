/****************************************************************************
**
**
****************************************************************************/

#include "about_refio_dialog.h"

#include <QtWidgets>

AboutRefioDialog::AboutRefioDialog(QWidget *parent)
    : QDialog(parent)
{
    Qt::WindowFlags flags(Qt::Dialog | Qt::WindowStaysOnTopHint | Qt::WindowTitleHint);
    setWindowFlags(flags);
    auto nameLabel = new QLabel(tr("Refio"));
    auto versionLabel = new QLabel(tr("0.1"));
    auto addressLabel = new QLabel(tr("Address"));
    auto okButton = new QPushButton(tr("Close"));

    auto gLayout = new QGridLayout;
    gLayout->setColumnStretch(1, 2);
    gLayout->addWidget(nameLabel, 0, 0);
    gLayout->addWidget(versionLabel, 0, 1);

    gLayout->addWidget(addressLabel, 1, 0, Qt::AlignLeft|Qt::AlignTop);
    //gLayout->addWidget(addressText, 1, 1, Qt::AlignLeft);

    auto buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(okButton);

    gLayout->addLayout(buttonLayout, 2, 1, Qt::AlignRight);

    auto mainLayout = new QVBoxLayout;
    mainLayout->addLayout(gLayout);
    setLayout(mainLayout);

    connect(okButton, &QAbstractButton::clicked, this, &QDialog::accept);

    setWindowTitle(tr("About Refio"));
}
