#ifndef EXPORTWINDOW_H
#define EXPORTWINDOW_H

#include <QWidget>
#include "mainwindow.h"

namespace Ui {
class exportWindow;
}

class exportWindow : public QWidget
{
    Q_OBJECT

public:
    exportWindow(QWidget *parent = nullptr);
    ~exportWindow();
    MainWindow* pointerToMainWindow; //mainwindow contains all the tabs, exportWindow also needs access to them


private slots:
    void on_plainTextEdit_blockCountChanged(int newBlockCount);
    void on_pushButton_clicked();

public slots:
    void mainWindowClosed();

private:
    Ui::exportWindow *ui;
    int previousBlockCount = 0;
    QString cruise;
    QString station;
    QString type;
    QString date;
    QString longitude;
    QString latitude;
    QString LOCAL;
    QString EDMO;
    QString depth;
    QString directory;
};

#endif // EXPORTWINDOW_H


    // Cruise
    // Station
    // Type
    // yyyy-mm-ddThh:mm:ss.sss
    // Longitude [degrees\_east]
    // Latitude [degrees\_north]
    // LOCAL\_CDI\_ID
    // EDMO\_code
    // Bot. Depth [m]
