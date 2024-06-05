#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <qcustomplot.h>


class TabPage;
class exportWindow;

using namespace std;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void plotgraph(const vector<double> &originalStream,const vector<double> &deltaEncoded, const vector<double> decodedvalues, int tabNumber);
    vector<TabPage*> tabPages;
    void addTab(int tabNumber);
    QVBoxLayout *layout;

public slots:
    void handleDeltaCompressedData(vector<double> dataStream, vector<double> compressedData, vector<double> decompressedData, int tabNumber);
    void handleHuffmanData(vector<double> dataStream, vector<string> compressedData, vector<double> encodedValues, vector<double> decodedValues, int tabNumber);
    // void handleFIREData(vector<double> encodedValues, int tabNumber);
    void handleFireUpdate(vector<string> compressedData, int tabNumber);
    void handleRatiosUpdates(double ratio1, int tabNumber);
    void onButtonClicked();

private slots:
    void onFileAddActionTriggered();
    void on_linemsall_returnPressed();
    void on_runButtonMain_clicked();
    void on_pauseButtonMain_clicked();

    void on_instantButtonMain_clicked();
    // void updateXAxisRange(const QCPRange &newRange);
    // void updateXAxisRange_2(const QCPRange &newRange);
    // void updateXAxisRange_3(const QCPRange &newRange);
    // void updateYAxisRange(const QCPRange &newRange);
    // void updateYAxisRange_2(const QCPRange &newRange);
    // void updateYAxisRange_3(const QCPRange &newRange);

    // void updateXAxisRange(const QCPRange &newRange);
    // void updateYAxisRange(const QCPRange &newRange);

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    int threadCounter = 0; // Counter to keep track of thread numbers
    bool updateInProgress = false;
    bool updateInProgressy = false;
    exportWindow *newWindow;
};
#endif // MAINWINDOW_H
