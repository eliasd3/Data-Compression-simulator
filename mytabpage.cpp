#include "mytabpage.h"
#include "ui_mytabpage.h"
#include <qobject.h>
#include "iostream"
#include "QFileDialog"
#include "fstream"
#include <QObject>
#include <qmessagebox.h>
#include "mythread.h"
#include <mainwindow.h>
#include "plotpertab.h"

using namespace std;
MainWindow* TabPage::pointerToMainWindow = nullptr;//needed
int TabPage::nextTabNumber = 0;

// QVBoxLayout* MyTabPage::layout = new QVBoxLayout();

// Function to handle signal-slot connections
void TabPage::setupConnections()
{
    // Connect signals and slots
    connect(ui->pushButton, &QPushButton::clicked, this, &TabPage::onPushButtonClicked);
    connect(ui->columnChoice, &QComboBox::currentIndexChanged, this, &TabPage::onColumnChoiceCurrentIndexChanged);
    connect(ui->runButton, &QPushButton::clicked, this, &TabPage::onRunButtonClicked);
    connect(ui->instantButton, &QPushButton::clicked, this, &TabPage::onInstantButtonClicked);
    connect(ui->pauseButton, &QPushButton::clicked, this, &TabPage::onPauseButtonClicked);
    connect(ui->columnChoice_2, &QComboBox::currentTextChanged, this, &TabPage::onColumnChoice2CurrentTextChanged);

    widget = new plotPerTab();
    pointerToMainWindow->layout->addWidget(widget);

    // x-axis range changed
    connect(getWidget()->getDataplot()->xAxis, SIGNAL(rangeChanged(QCPRange)), this,SLOT(updateXAxisRange(QCPRange)));
    connect(getWidget()->getCompressedPlot()->xAxis, SIGNAL(rangeChanged(QCPRange)), this,SLOT(updateXAxisRange(QCPRange)));
    connect(getWidget()->getDecompressedPlot()->xAxis, SIGNAL(rangeChanged(QCPRange)), this,SLOT(updateXAxisRange(QCPRange)));

    // y-axis range changed
    connect(getWidget()->getDataplot()->yAxis,  SIGNAL(rangeChanged(QCPRange)), this,SLOT(updateYAxisRange(QCPRange)));
    connect(getWidget()->getDecompressedPlot()->yAxis,  SIGNAL(rangeChanged(QCPRange)), this,SLOT(updateYAxisRange(QCPRange)));


}


void TabPage::updateXAxisRange(const QCPRange &newRange) {

    // Disconnect range change signals temporarily
    disconnect(getWidget()->getDataplot()->xAxis, SIGNAL(rangeChanged(QCPRange)), this, SLOT(updateXAxisRange(QCPRange)));
    disconnect(getWidget()->getCompressedPlot()->xAxis, SIGNAL(rangeChanged(QCPRange)), this, SLOT(updateXAxisRange(QCPRange)));
    disconnect(getWidget()->getDecompressedPlot()->xAxis, SIGNAL(rangeChanged(QCPRange)), this, SLOT(updateXAxisRange(QCPRange)));

    // Set x-axis range for all plots
    getWidget()->getDataplot()->xAxis->setRange(newRange);
    getWidget()->getCompressedPlot()->xAxis->setRange(newRange);
    getWidget()->getDecompressedPlot()->xAxis->setRange(newRange);

    // Reconnect range change signals
    connect(getWidget()->getDataplot()->xAxis, SIGNAL(rangeChanged(QCPRange)), this, SLOT(updateXAxisRange(QCPRange)));
    connect(getWidget()->getCompressedPlot()->xAxis, SIGNAL(rangeChanged(QCPRange)), this, SLOT(updateXAxisRange(QCPRange)));
    connect(getWidget()->getDecompressedPlot()->xAxis, SIGNAL(rangeChanged(QCPRange)), this, SLOT(updateXAxisRange(QCPRange)));

    // Replot all plots
    getWidget()->getDataplot()->replot();
    getWidget()->getCompressedPlot()->replot();
    getWidget()->getDecompressedPlot()->replot();
}

void TabPage::updateYAxisRange(const QCPRange &newRange) {
    // Disconnect range change signals temporarily
    disconnect(getWidget()->getDataplot()->yAxis, SIGNAL(rangeChanged(QCPRange)), this, SLOT(updateYAxisRange(QCPRange)));
    disconnect(getWidget()->getDecompressedPlot()->yAxis, SIGNAL(rangeChanged(QCPRange)), this, SLOT(updateYAxisRange(QCPRange)));

    // Set y-axis range for all plots
    getWidget()->getDataplot()->yAxis->setRange(newRange);
    getWidget()->getDecompressedPlot()->yAxis->setRange(newRange);

    // Reconnect range change signals
    connect(getWidget()->getDataplot()->yAxis, SIGNAL(rangeChanged(QCPRange)), this, SLOT(updateYAxisRange(QCPRange)));
    connect(getWidget()->getDecompressedPlot()->yAxis, SIGNAL(rangeChanged(QCPRange)), this, SLOT(updateYAxisRange(QCPRange)));

    // Replot all plots
    getWidget()->getDataplot()->replot();
    getWidget()->getDecompressedPlot()->replot();
}


plotPerTab *TabPage::getWidget() const
{
    return widget;
}

vector<QString> TabPage::getComments() const
{
    return comments;
}

QString TabPage::getCurrentColumn() const
{
    return currentColumn;
}

double TabPage::getCompressionRatio() const
{
    return compressionRatio;
}

void TabPage::setCompressionRatio(double newCompressionRatio)
{
    compressionRatio = newCompressionRatio;
}

int TabPage::getDigitsAfterDecimal() const
{
    return digitsAfterDecimal;
}

void TabPage::setRunning(bool newRunning)
{
    running = newRunning;
    ui->runButton->setChecked(running);
}

void TabPage::setLabelTime(QString q)
{
    ui->labelms->setText(q);
}

int TabPage::getTabNumber() const
{
    return tabNumber;
}

QString TabPage::getAlgorithmName() const
{
    return algorithmName;
}

void TabPage::setAlgorithmName(const QString &newAlgorithmName)
{
    algorithmName = newAlgorithmName;
}

bool TabPage::getPaused() const
{
    return paused;
}

void TabPage::setPaused(bool newPaused)
{
    paused = newPaused;
}

int TabPage::getTimeBetweenValues() const
{
    return timeBetweenValues;
}

void TabPage::setTimeBetweenValues(int newTimeBetweenValues)
{
    timeBetweenValues = newTimeBetweenValues;
}

vector<double>& TabPage::getDecompressedData()
{
    return decompressedData;
}

void TabPage::setDecompressedData(const vector<double> &newDecompressedData)
{
    decompressedData = newDecompressedData;
}

vector<double>& TabPage::getCompressedData()
{
    return compressedData;
}

void TabPage::setCompressedData(const vector<double> &newCompressedData)
{
    compressedData = newCompressedData;
}

vector<double>& TabPage::getDeltaEncodedData()
{
    return deltaEncodedData;
}

void TabPage::setDeltaEncodedData(const vector<double> &newDeltaEncodedData)
{
    deltaEncodedData = newDeltaEncodedData;
}

vector<double>& TabPage::getOriginalData()
{
    return originalData;
}

void TabPage::setOriginalData(const vector<double> &newOriginalData)
{
    originalData = newOriginalData;
}

TabPage::TabPage(MainWindow *pointerToMainWindow, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TabPage)
{
    ui->setupUi(this);
    tabNumber = nextTabNumber++;
    // qDebug() << tabNumber;

    TabPage::pointerToMainWindow = pointerToMainWindow;

    setupConnections();
}

TabPage::TabPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TabPage)
{
    ui->setupUi(this);
    tabNumber = nextTabNumber++;
    // qDebug() << tabNumber;

    setupConnections();
}

void TabPage::onPushButtonClicked()
{
    QString filename = "";
    filename=QFileDialog::getOpenFileName (
        this, tr("Open File"),
        "C://", "All files (*.*);; Text File (*.txt)"
    );
    file.close();
    file.open(filename.toStdString());

    if (!file.is_open()) {
        cout << "Failed to open the file." << endl;
    }
    ui->textFile->setText(filename.section("/", -1));

    string line = "";
    vector<double> temps;

    // qDebug() << "_";
    //skipping all commented lines:
    while (getline(file, line)) {
        // qDebug() << ".";
        // Extract the first 6 characters of the line to search for the word Cruise which exists
        // in every (the 3 I checked) document.

        if (line.find("//") == 0) {
            // If it's a comment line, store it in the comments vector
            comments.push_back(QString::fromStdString(line));
            continue; // Skip to the next line
        }

        string firstSixChars = line.substr(0, 6);
        if (firstSixChars == "Cruise") {
            // qDebug() << line << "\n"; //this is the line with all column names
            // columnPosition = line.find(columnName.toStdString());
            istringstream iss(line);

            currentPosition = file.tellg();


            // Parse the line and extract column names
            columnNames = vector<string>();
            string columnName = "";
            int columnIndex = 0;
            while (getline(iss, columnName, '\t')) {
                columnIndex++;
                // Skip the first 9 columns //metadata columns
                if (columnIndex <= 9) {
                    columnNames.push_back(" ");
                    continue;
                }
                // Skip columns with names "QV:SEADATANET" or "QF:SEADATANET"
                if (columnName == "QV:SEADATANET\r" || columnName == "QV:SEADATANET" || columnName == "QF:SEADATANET"|| columnName == "QF:SEADATANET\r") {
                    columnNames.push_back(" ");
                    continue;
                }
                qDebug() << columnName;
                columnNames.push_back(columnName);
            }
            break;
        }
    }

    disconnect(ui->columnChoice, &QComboBox::currentIndexChanged, this, &TabPage::onColumnChoiceCurrentIndexChanged);

    // Populate the QComboBox with column names
    ui->columnChoice->clear(); // Clear existing items
    for (const string& columnName : columnNames) {
        QString qColumnName = QString::fromStdString(columnName);
        if(qColumnName!=" "){//for leaving away unnecessary collumns
            ui->columnChoice->addItem(qColumnName);
        }
    }

    connect(ui->columnChoice, &QComboBox::currentIndexChanged, this, &TabPage::onColumnChoiceCurrentIndexChanged);

}

void TabPage::onColumnChoiceCurrentIndexChanged(int index)
{
    currentColumn = ui->columnChoice->currentText();
    // currentColumn = ui->columnChoice->currentText().section(" ", 0, 0);

    //for getting the right index out of the columnnames list (this also contains the unnecessary collumns)
        //  *****
    std::string currentColumnStd = currentColumn.toStdString();

    // Initialize index to -1 to indicate "not found" by default
    this->index = -1;

    // Iterate over each element in columnNames and compare
    for (int i = 0; i < (int) columnNames.size(); ++i) {
        if (columnNames[i] == currentColumnStd) {
            this->index = i;
            qDebug() << "this index is " << i;
            break;
        }
    }
        //  *****

    originalData = vector<double>();

    //Read the data:
    string line;

    // qDebug() << file.is_open();


    while (getline(file, line)) {
        istringstream iss(line); //puts the line in a istringstream
        string dataValue;

        // Skipping columns until reaching right column (position in 'index' variable)
        for (int i = 0; i < this->index; ++i) {
            getline(iss, dataValue, '\t');
        }

        // Reading of the chosen (by index) columb
        if (getline(iss, dataValue, '\t')) {

            // Check if dataValue is not empty and contains a valid float value
            if (!dataValue.empty()) {
                try {
                    originalData.push_back(stod(dataValue));
                } catch (const std::invalid_argument& e) {
                    qDebug() << "invalid value";
                }
            } else {
                // empty value is skipped
            }
        }
    }

    file.clear();// some kind of workaround for the getline
    file.seekg(currentPosition);//sets pointer to beginning of the columns
}


void TabPage::onRunButtonClicked()
{

    if(this->index==0){
        QMessageBox msgBox;
        QString message = QString("Select a column and file first for tab %2").arg(this->tabNumber);
        msgBox.setText(message);
        msgBox.exec();
        ui->runButton->setChecked(false);
    } else if (ui->columnChoice_2->currentText().isEmpty()){
        QMessageBox msgBox;
        QString message = QString("Select an algorithm first for tab %2").arg(this->tabNumber);
        msgBox.setText(message);
        msgBox.exec();
        ui->runButton->setChecked(false);
    } else if (!running){
        running = true;
        qRegisterMetaType<std::vector<double>>("std::vector<double>");
        
        thread = new Thread(this);
        
        connect(thread, &Thread::deltaCompressedDataUpdate, pointerToMainWindow, &MainWindow::handleDeltaCompressedData);
        connect(thread, &Thread::HuffmanDataUpdate, pointerToMainWindow, &MainWindow::handleHuffmanData);
        // connect(thread, &MyThread::finished, thread, &QObject::deleteLater); //this insures the object is deleted.
        thread->start();
        
        connect(thread, &Thread::finished, thread, &QObject::deleteLater); //for deleting the thread when it finishes
    } else {
        ui->runButton->setChecked(true);
    }
}

void TabPage::onInstantButtonClicked()
{
    if(timeBetweenValues==3){
        timeBetweenValues = 100;
        ui->labelms->setText(QString::number(100));
        ui->instantButton->setChecked(false);
    } else {
        timeBetweenValues = 3;
        ui->labelms->setText(QString::number(3));
        ui->instantButton->setChecked(true);
    }
}

void TabPage::onPauseButtonClicked()
{
    paused = !paused;
    ui->pauseButton->setChecked(paused);
}

TabPage::~TabPage()
{
    delete thread;
    file.close();
}

void TabPage::onColumnChoice2CurrentTextChanged(const QString &arg1)
{
    algorithmName = arg1;
}


void TabPage::on_lineEdit_returnPressed()
{
    timeBetweenValues = ui->lineEdit->text().toInt();
    ui->labelms->setText(QString::number(timeBetweenValues));
}


void TabPage::on_spinBox_valueChanged(int arg1)
{
    digitsAfterDecimal = arg1;
}

