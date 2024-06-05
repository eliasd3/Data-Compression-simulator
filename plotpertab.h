#ifndef PLOTPERTAB_H
#define PLOTPERTAB_H

#include <QWidget>
#include <qcustomplot.h>

namespace Ui {
class plotPerTab;
}

class plotPerTab : public QWidget
{
    Q_OBJECT

public:
    explicit plotPerTab(QWidget *parent = nullptr);
    ~plotPerTab();

    QCustomPlot *getDataplot() const;
    QCustomPlot *getCompressedPlot() const;
    QCustomPlot *getDecompressedPlot() const;

    QLabel *getCompressedDataView() const;

    QLabel *getResult() const;

    QLabel *getCompressTime() const;

    QLabel *getDecompressTime() const;

private slots:
    void onDecompressedplotDoubleclick();
    void onCompressedplotDoubleclick();
    void onDataplotDoubleClick();

private:
    Ui::plotPerTab  *ui;
    QCustomPlot     *dataplot;
    QCustomPlot     *compressedPlot;
    QCustomPlot     *decompressedPlot;
    QLabel          *compressedDataView;
    QLabel          *result;
    QLabel          *compressTime;
    QLabel          *decompressTime;
};

#endif // PLOTPERTAB_H
