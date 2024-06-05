#include "exportwindow.h"
#include "ui_exportwindow.h"
#include "mytabpage.h"

exportWindow::exportWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::exportWindow)
{
    ui->setupUi(this);

}

exportWindow::~exportWindow()
{
    delete ui;
}

void exportWindow::on_plainTextEdit_blockCountChanged(int newBlockCount)
{
    if(newBlockCount>previousBlockCount){
        ui->plainTextEdit->insertPlainText("//");
        qDebug() << ui->CruisEdit->text();
    }
}

void exportWindow::on_pushButton_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Save File"),
                                                    QDir::homePath(), // Start in the user's home directory
                                                    tr("Text Files (*.txt);;All Files (*)"));
    if (!fileName.isEmpty()) {
        // Add .txt extension if not provided by the user
        if (!fileName.endsWith(".txt", Qt::CaseInsensitive)) {
            fileName += ".txt";
        }
    }

    //saving all input data
    cruise = ui->CruisEdit->text();
    station = ui->StationEdit->text();
    type = ui->TypeEdit->text();
    date = ui->DateEdit->text();
    longitude = ui->LongitudeEdit->text();
    latitude = ui->LatitudeEdit->text();
    LOCAL = ui->LocalEdit->text();
    EDMO = ui->EDMOEdit->text();
    depth = ui->BotDepthEdit->text();

    QVector<QString> currentColumnStrings;
    QVector<QString> commentsContainingCurrentColumns;

    for (TabPage* tabPage : pointerToMainWindow->tabPages) {
        QString currentColumnString = tabPage->getCurrentColumn().section(" ", 0, 0);
        currentColumnStrings.append(currentColumnString);

        QString commentContainingCurrentColumn;
        // Iterate through each comment
        for (const QString& comment : tabPage->getComments()) {
            // Check if the comment contains the current column string
            if (comment.contains(currentColumnString)) {
                // Store the comment containing the current column string
                commentContainingCurrentColumn = comment;
                break; // Exit the loop if found
            }
        }
        commentsContainingCurrentColumns.append(commentContainingCurrentColumn);
    }

    qDebug() << "extra comments" << ui->plainTextEdit->toPlainText();

    // Output the comments containing the current column strings
    for (const QString& comment : commentsContainingCurrentColumns) {
        qDebug() << "Comment containing the current column:" << comment;
    }

    // Create a QFile object
    QFile outputFile(fileName);
    if (outputFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        // Create a QTextStream object to write to the file
        QTextStream out(&outputFile);

        // Write "extra comments" followed by the content of ui->plainTextEdit->toPlainText() to the file
        out << ui->plainTextEdit->toPlainText() << "\n";

        out << "//SDN_parameter_mapping" << "\n";
        // Write the comments containing the current column strings to the file
        for (const QString& comment : commentsContainingCurrentColumns) {
            if (!comment.isEmpty()) out << comment << "\n";
        }
        out << "//" << "\n";

        out << "Cruise\tStation\tType\tYYYY-MM-DDThh:mm:ss.sss\tLongitude [degrees_east]\tLatitude [degrees_north]\tLOCAL_CDI_ID\tEDMO_code\tBot. Depth [m]\t";
        for (const QString& currentColumnString : currentColumnStrings) {
            if (!currentColumnString.isEmpty()) out << currentColumnString << "\t" << "QV:SEADATANET" << "\t";
        }

        out << "\n";

        out << ui->CruisEdit->text() << "\t" << ui->StationEdit->text() << "\t" << ui->TypeEdit->text() << "\t" << ui->DateEdit->text() << "\t" << ui->LongitudeEdit->text() << "\t" << ui->LatitudeEdit->text() << "\t" << ui->LocalEdit->text() << "\t" << ui->EDMOEdit->text() << "\t" << ui->BotDepthEdit->text() << "\t";


        // Get the decompressed data
        QVector<vector<double>> decompressedDataList;
        for (TabPage* tabPage : pointerToMainWindow->tabPages) {
            decompressedDataList.append(tabPage->getDecompressedData());
        }

        // Determine the maximum size among the vectors
        int maxSize = 0;
        for (const vector<double>& decompressedData : decompressedDataList) {
            if (decompressedData.size() > maxSize) {
                maxSize = decompressedData.size();
            }
        }

        // Loop through each row
        for (int i = 0; i < maxSize; i++) {
            if(i != 0) {
                out << "\t"<< "\t"<< "\t"<< "\t"<< "\t"<< "\t"<< "\t"<< "\t"<< "\t";
            }

            // Print the data from each column if it exists
            for (int j = 0; j < decompressedDataList.size(); j++) {
                if (i < decompressedDataList[j].size() && !commentsContainingCurrentColumns[j].isEmpty()) {
                    out << decompressedDataList[j][i] << "\t" << "1" << "\t";
                }
            }
            out << "\n";
        }

        outputFile.close();

        QMessageBox msgBox;
        msgBox.setText("File saved!");
        msgBox.exec();


    } else {
        // Handle error opening the file
        qDebug() << "Failed to   open the output file!";
    }

}

void exportWindow::mainWindowClosed() //does not work (yet)
{
    this->destroy();
}

