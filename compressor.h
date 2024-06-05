#ifndef COMPRESSOR_H
#define COMPRESSOR_H

#include <QMainWindow>
#include <QObject>
#include <QWidget>
#include <chrono>
// #include "mytabpage.h"

using namespace std;

class compressor
{
private:
    int nextIndex = 0;
    int nextIndex2 = 0;
    vector<double> deltaEncoded;//  TODO:I should delete this and use the deltaencoded in mytabpage
    vector<string> huffmanEncoded;
    std::chrono::nanoseconds durationCompression = std::chrono::nanoseconds::zero();

public:
    compressor();
    vector<double> deltaEncode(const vector<double>& input);
    vector<double> deltaEncodeNext(const vector<double>& input);
    vector<double> deltaDecode(const vector<double>& encodedValues);
    vector<string> HuffmanEncodeNext(const vector<double>& input, int digitsAfterComma);
    vector<double> HuffmanDecode(const vector<string>& encodedValues, int digitsAfterComma);
    vector<double> decimate(const vector<double>& input, int amount);


    int getNextIndex() const;
    void setNextIndex(int newNextIndex);
    int getNextIndex2() const;
    void setNextIndex2(int newNextIndex2);
    std::chrono::nanoseconds getDurationCompression() const;
};

#endif // COMPRESSOR_H
