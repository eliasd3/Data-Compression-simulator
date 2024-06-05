#include "mainwindow.h"
#include "mytabpage.h"
// #include "./ui_mytabpage.h"
#include "./ui_mainwindow.h"
#include "qcustomplot.h"
// #include "iostream"
#include "QFileDialog"
// #include "fstream"
// #include "compressor.h"
// #include "mythread.h"
#include <QObject>
#include "exportwindow.h"
#include "plotpertab.h"

using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    QPushButton *button = new QPushButton();
    button->setText("+");
    button->setFixedSize(25, 25);
    button->setStyleSheet("QPushButton {""    background-color: #f0f0f0;""    border: 1px solid #c0c0c0;""    border-radius: 5px;""}""QPushButton:hover {""    background-color: #e0e0e0;" "}");
    QHBoxLayout *cornerLayout = new QHBoxLayout();
    cornerLayout->setContentsMargins(0, 0, 0, 0);
    cornerLayout->addWidget(button);
    QWidget *cornerWidget = new QWidget();
    cornerWidget->setLayout(cornerLayout);
    ui->myTabWidget->setCornerWidget(cornerWidget, Qt::TopRightCorner);

    connect(button, &QPushButton::clicked, this, &MainWindow::onButtonClicked);

    QWidget *scrollAreaWidgetContents = new QWidget();
    layout = new QVBoxLayout(scrollAreaWidgetContents);
    ui->scrollAreaPlots->setWidget(scrollAreaWidgetContents);

    TabPage *myNewTab = new TabPage(this); //add pointer to mainwindow to make connections
    TabPage *myNewTab2 = new TabPage;
    TabPage *myNewTab3 = new TabPage;

    tabPages.push_back(myNewTab);
    tabPages.push_back(myNewTab2);
    tabPages.push_back(myNewTab3);

    addTab(0);
    addTab(1);
    addTab(2);


    // myNewTab->functionDeclaredInMyTabPage ();
    // for (MyTabPage* element : tabPages) {

    // }


    connect(ui->fileAddAction, &QAction::triggered, this, &MainWindow::onFileAddActionTriggered);

    this->setAttribute(Qt::WA_DeleteOnClose); //this is to make sure destructor is called, for closing new export window
}

void MainWindow::addTab(int tabNumber) {
    const char* prefix = "t";
    QString tabName = QString("%1%2").arg(prefix).arg(tabNumber);
    ui->myTabWidget->addTab(tabPages.at(tabNumber), tr(tabName.toStdString().c_str()));
}

void MainWindow::onButtonClicked() {
    tabPages.push_back(new TabPage());
    addTab(tabPages.size()-1);
}

void MainWindow::plotgraph(const vector<double> &originalStream, const vector<double> &deltaEncoded,const vector<double> decodedValues, int tabNumber)
{
    // Determine the size of the data to use for x-axis labels
    int dataSize = std::min(originalStream.size(), deltaEncoded.size());

    QVector<double> xData(dataSize);
    xData.fill(0.0);//ensures all data is zero
    for (int i = 0; i < dataSize; ++i) {
        xData[i] = i;
    }

    // Convert original and compressed data to QVector<double>
    QVector<double> yDataOriginal;
    QVector<double> yDataCompressed;
    QVector<double> yDataDecoded;

    yDataDecoded.reserve(dataSize);
    yDataOriginal.reserve(dataSize);
    yDataCompressed.reserve(dataSize);
    for (int i = 0; i < dataSize; ++i) { //from int = 1 to skip first one
        if(i==0){ //only to skip first element to make scaling of graph better.
            yDataOriginal.append(originalStream[i]);
            yDataCompressed.append(0.0);
            if(!decodedValues.empty()){
                yDataDecoded.append(decodedValues[i]);
            }
        } else {
            yDataOriginal.append(originalStream[i]);
            yDataCompressed.append(deltaEncoded[i]);
            if(!decodedValues.empty()){
                yDataDecoded.append(decodedValues[i]);
            }
        }
    }
    QCustomPlot* dataplott = tabPages.at(tabNumber)->getWidget()->getDataplot();
    QCustomPlot* compressedplott = tabPages.at(tabNumber)->getWidget()->getCompressedPlot();
    QCustomPlot* decompressedplott = tabPages.at(tabNumber)->getWidget()->getDecompressedPlot();

    // Clear previous data
    dataplott->clearGraphs();
    compressedplott->clearGraphs();
    decompressedplott->clearGraphs();

    // Set up graph properties
    dataplott->addGraph();
    compressedplott->addGraph();
    decompressedplott->addGraph();

    dataplott->graph(0)->setScatterStyle(QCPScatterStyle::ssDisc);
    dataplott->graph(0)->setLineStyle(QCPGraph::lsNone);
    compressedplott->graph(0)->setScatterStyle(QCPScatterStyle::ssDisc);
    compressedplott->graph(0)->setLineStyle(QCPGraph::lsNone);
    decompressedplott->graph(0)->setScatterStyle(QCPScatterStyle::ssDisc);
    decompressedplott->graph(0)->setLineStyle(QCPGraph::lsNone);


    // and graph->setValueAxis()

    dataplott->xAxis->setLabel("Measurement");
    dataplott->yAxis->setLabel("value");
    compressedplott->xAxis->setLabel("Measurement");
    compressedplott->yAxis->setLabel("delta");
    decompressedplott->xAxis->setLabel("Measurement");
    decompressedplott->yAxis->setLabel("recovered value");

    dataplott->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    compressedplott->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    decompressedplott->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);


    // Update original data
    dataplott->graph(0)->setData(xData, yDataOriginal);

    // Update compressed data
    compressedplott->graph(0)->setData(xData, yDataCompressed);

    // Update decompressed data
    if(!yDataDecoded.isEmpty()){
        decompressedplott->graph()->setData(xData, yDataDecoded);
    }

    dataplott->xAxis->setRange(0,originalStream.size());
    compressedplott->xAxis->setRange(0,originalStream.size());
    decompressedplott->xAxis->setRange(0,originalStream.size());

    dataplott->yAxis->rescale();
    compressedplott->yAxis->rescale();
    decompressedplott->yAxis->rescale();

    // Redraw plots
    dataplott->replot();
    compressedplott->replot();
    decompressedplott->replot();
}




MainWindow::~MainWindow()
{
    newWindow->close(); //werkt nie
    for (TabPage* tab : tabPages) {
        delete tab;
    }
    delete ui;
    delete newWindow; //export window
}



void MainWindow::handleDeltaCompressedData(vector<double> dataStream, vector<double> compressedData, vector<double> decodedValues, int tabNumber)
{
    this->plotgraph(dataStream,compressedData,decodedValues,tabNumber);
}

void MainWindow::handleHuffmanData(vector<double> dataStream,
                                   vector<string> compressedData,
                                   vector<double> encodedValues,
                                   vector<double> decodedValues,
                                   int tabNumber)
{

    QString compressedDataString;
    for(const auto& str : compressedData) {
        compressedDataString.append(QString::fromStdString(str));
        compressedDataString.append(" * "); // Add newline for better readability
    }

    QLabel* textBox = tabPages.at(tabNumber)->getWidget()->getCompressedDataView();
    // qDebug() << tabNumber;

    this->plotgraph(dataStream,encodedValues,decodedValues,tabNumber);
    textBox->setText(compressedDataString);
}

void MainWindow::handleFireUpdate(vector<string> compressedData, int tabNumber)
{
    QString compressedDataString;
    for(const auto& str : compressedData) {
        compressedDataString.append("(int_16b) ");
        compressedDataString.append(QString::fromStdString(str));
        compressedDataString.append("  *  "); // Add newline for better readability
    }

    QLabel* textBox = tabPages.at(tabNumber)->getWidget()->getCompressedDataView();


    textBox->setText(compressedDataString);

}

void MainWindow::handleRatiosUpdates(double ratio1, int tabNumber)
{
    tabPages.at(tabNumber)->getWidget()->getResult()->setText(QString::number(ratio1));
}

void MainWindow::onFileAddActionTriggered()
{

}

void MainWindow::on_linemsall_returnPressed()
{

    for (TabPage* element : tabPages) {
        element->setTimeBetweenValues(ui->linemsall->text().toInt());
        element->setLabelTime(ui->linemsall->text());
    }

    ui->labelmsAll->setText(ui->linemsall->text());
}

void MainWindow::on_runButtonMain_clicked()
{

    for (TabPage* element : tabPages) {
        element->onRunButtonClicked();
    }
}


void MainWindow::on_pauseButtonMain_clicked()
{
    for (TabPage* element : tabPages) {
        element->onPauseButtonClicked();
    }
}


void MainWindow::on_instantButtonMain_clicked()
{
    for (TabPage* element : tabPages) {
        element->onInstantButtonClicked();
    }
}


void MainWindow::on_pushButton_clicked()
{
    newWindow = new exportWindow();
    newWindow->show();
    newWindow->pointerToMainWindow = this;
    connect(this, &QWidget::destroyed, newWindow, &exportWindow::mainWindowClosed);
}


void MainWindow::on_pushButton_2_clicked()
{
    //vector<TabPage*> tabPages; //variable exists inside object/class

}

