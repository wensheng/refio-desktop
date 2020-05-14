#ifndef COMMON_H
#define COMMON_H

#include <QMainWindow>
#include <QApplication>
#include <QByteArray>
#include <vector>
#include <numeric>
#include <algorithm>
#include <random>
#include <chrono>

QByteArray genICodeSeq()
{
    std::vector<quint16> idx(32768);
    std::iota(std::begin(idx), std::end(idx), 0);
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    shuffle(idx.begin(), idx.end(), std::default_random_engine(seed));
    QByteArray byteArray;
    for(const auto& v: idx){
        if(v<256){
            byteArray.append('\0');
        }else{
            byteArray.append(v>>8);
        }
        byteArray.append(v & 0xff);
    }
    return byteArray;
}

/*
 * this is not used
 * we use static MainWindow::instance() instead
 */
QMainWindow* getMainWindow()
{
    for(QWidget *w: QApplication::topLevelWidgets()){
        if(QMainWindow *mainWin = qobject_cast<QMainWindow*>(w)){
            return mainWin;
        }
    }
    return nullptr;
}



#endif // COMMON_H
