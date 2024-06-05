#ifndef MYTABPAGE_H
#define MYTABPAGE_H

#include <QWidget>
#include <fstream>
#include <mainwindow.h>
#include "plotpertab.h"

class Thread;

using namespace std;

namespace Ui {
class TabPage;
}

class TabPage : public QWidget
{
    Q_OBJECT

public:
    explicit TabPage(QWidget *parent = nullptr);
    explicit TabPage(MainWindow *pointerToMainWindow,QWidget *parent = nullptr);
    ~TabPage();

    static MainWindow *pointerToMainWindow;

    vector<double>& getOriginalData();
    void setOriginalData(const vector<double> &newOriginalData);

    vector<double>& getDeltaEncodedData();
    void setDeltaEncodedData(const vector<double> &newDeltaEncodedData);

    vector<double>& getCompressedData();
    void setCompressedData(const vector<double> &newCompressedData);

    vector<double>& getDecompressedData();
    void setDecompressedData(const vector<double> &newDecompressedData);

    int getTimeBetweenValues() const;
    void setTimeBetweenValues(int newTimeBetweenValues);

    bool getPaused() const;
    void setPaused(bool newPaused);

    QString getAlgorithmName() const;
    void setAlgorithmName(const QString &newAlgorithmName);


    int getTabNumber() const;

    void setRunning(bool newRunning);

    void setLabelTime(QString q);

    int getDigitsAfterDecimal() const;

    double getCompressionRatio() const;
    void setCompressionRatio(double newCompressionRatio);

    QString getCurrentColumn() const;

    vector<QString> getComments() const;

    plotPerTab *getWidget() const;

private slots:
    void onPushButtonClicked();
    void onColumnChoiceCurrentIndexChanged(int index);
    void onColumnChoice2CurrentTextChanged(const QString &arg1);
    void on_lineEdit_returnPressed();
    void on_spinBox_valueChanged(int arg1);
    void updateXAxisRange(const QCPRange &newRange);
    void updateYAxisRange(const QCPRange &newRange);

public slots:
    void onRunButtonClicked();
    void onInstantButtonClicked();
    void onPauseButtonClicked();

private:
    Thread *thread = NULL;
    void setupConnections();
    Ui::TabPage *ui;
    int index = 0;
    ifstream file;
    vector<double> originalData = vector<double>();
    vector<double> deltaEncodedData = vector<double>();
    vector<double> compressedData = vector<double>();
    vector<double> decompressedData = vector<double>();
    int timeBetweenValues = 50;
    bool paused = false;
    QString algorithmName ="";
    QString filename = "";
    streampos currentPosition;
    static int nextTabNumber;
    int tabNumber; //object id
    bool running = false;
    int digitsAfterDecimal = 1;
    double compressionRatio = 0.0;
    vector<QString> comments;
    QString currentColumn = "";
    plotPerTab *widget = NULL;
    double takenTime = 0.0;
    vector<string> columnNames = vector<string>();
};

#endif // MYTABPAGE_H
