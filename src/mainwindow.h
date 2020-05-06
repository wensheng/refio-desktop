/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "reference_widget.h"
#include "slipbox_widget.h"
#include "QSimpleUpdater.h"

#include <QMainWindow>
#include <QDesktopServices>
#include <QSqlDatabase>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();
    ~MainWindow();
    QString getNextICode();

public slots:
    void checkForUpdates();
    //void updateChangelog(const QString& url);
    //void displayAppcast (const QString& url, const QByteArray& reply);

private slots:
    void updateActions(const QItemSelection &selection);
    void addNewCollection();
    void openFile();
    void saveFile();
    void exportLibrary();
    void undo();
    void redo();
    void copy();
    void cut();
    void paste();
    void deleteItem();
    void findItem();
    void advancedSearch();
    void addWebpage();
    void openDocumentationWebpage();
    void addArticle();
    void addBook();
    void addDocument();
    void showAboutRefio();
    void showSettings();
    void toggleCollectorView();
    void toggleSlipboxView();
    void toggleStandardView();
    void toggleStackedView();
private:
    void createMenus();
    void setup_db();
    QByteArray genICodeSeq();

    ReferenceWidget *referenceWidget;
    SlipboxWidget *slipboxWidget;
    QSimpleUpdater *m_updater;
    QFont menuItemBoldFont, menuItemRegularFont;
    QAction *collectorViewAct;
    QAction *slipboxViewAct;
    QAction *standardLayoutAct;
    QAction *stackedLayoutAct;
    QAction *editAct;
    QAction *removeAct;
    bool isSlipbox;
    bool isStacked;
    int lib_id;
    QByteArray iCodeSeq;
};
//! [0]

#endif // MAINWINDOW_H
