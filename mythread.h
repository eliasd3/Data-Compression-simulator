#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QObject>
#include <QThread>

#include <mytabpage.h>
#include <compressor.h>

using namespace std;


class Thread : public QThread
{
    Q_OBJECT


private:
    compressor *c = NULL;
    TabPage *tab;
    int16_t* fireCompressedData = nullptr;
    vector<double> decimatedStream = vector<double>();
    vector<double> compressedStream = vector<double>();
    vector<string> compressedData;
    int delayedTime = 0;
    chrono::time_point<std::chrono::system_clock> start_time_sprintz = std::chrono::time_point<std::chrono::system_clock>::min();
    chrono::time_point<std::chrono::system_clock> end_time_sprintz = std::chrono::time_point<std::chrono::system_clock>::min();



public:
    Thread(TabPage *tab);
    void run();

    compressor *getC() const;
    void setC(compressor *newC);

    void newValuePredicted(int err);
    void fireCompressedDataUpdate(int i);

    TabPage *getTab() const;

    void setHowManySleepsFire(int newHowManySleepsFire);

signals:
    void deltaCompressedDataUpdate(vector<double> dataStream, vector<double> compressedData, vector<double> decodedValues, int tabNumber);
    void HuffmanDataUpdate(vector<double> dataStream, vector<string> compressedData, vector<double> encodedValues,vector<double> decodedValues, int tabNumber);
    // void FIREupdate(vector<double> encodedValues, int tabNumber);
    void fireDataUpdate(vector<string> compressedData, int tabNumber);
    void ratiosUpdates(double ratio1, int tabNumber);

public slots:


};

#endif // MYTHREAD_H
