#include "plotpertab.h"
#include "ui_plotpertab.h"

plotPerTab::plotPerTab(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::plotPerTab)
{
    ui->setupUi(this);
    dataplot = ui->dataplot;
    compressedPlot = ui->compressedplot;
    decompressedPlot = ui->decompressedplot;
    compressedDataView = ui->compressedDataView;
    result = ui->labelResults;
    compressTime = ui->compressedTime;
    decompressTime = ui->DecompressedTime;
    connect(ui->dataplot, &QCustomPlot::mouseDoubleClick, this, &plotPerTab::onDataplotDoubleClick);
    connect(ui->compressedplot, &QCustomPlot::mouseDoubleClick, this, &plotPerTab::onCompressedplotDoubleclick);
    connect(ui->decompressedplot, &QCustomPlot::mouseDoubleClick, this, &plotPerTab::onDecompressedplotDoubleclick);
}

void plotPerTab::onDataplotDoubleClick()
{
    ui->dataplot->rescaleAxes();
    ui->dataplot->replot();
    ui->dataplot->update();
}

QLabel *plotPerTab::getDecompressTime() const
{
    return decompressTime;
}

QLabel *plotPerTab::getCompressTime() const
{
    return compressTime;
}


void plotPerTab::onCompressedplotDoubleclick()
{
    ui->compressedplot->rescaleAxes();
    ui->compressedplot->replot();
    ui->compressedplot->update();
}

void plotPerTab::onDecompressedplotDoubleclick()
{
    ui->decompressedplot->rescaleAxes();
    ui->decompressedplot->replot();
    ui->decompressedplot->update();
}

plotPerTab::~plotPerTab()
{
    delete ui;
}

QCustomPlot *plotPerTab::getDataplot() const
{
    return dataplot;
}

QCustomPlot *plotPerTab::getCompressedPlot() const
{
    return compressedPlot;
}

QCustomPlot *plotPerTab::getDecompressedPlot() const
{
    return decompressedPlot;
}

QLabel *plotPerTab::getCompressedDataView() const
{
    return compressedDataView;
}

QLabel *plotPerTab::getResult() const
{
    return result;
}
