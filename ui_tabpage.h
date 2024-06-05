/********************************************************************************
** Form generated from reading UI file 'mytabpage.ui'
**
** Created by: Qt User Interface Compiler version 5.15.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TABPAGE_H
#define UI_TABPAGE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TabPage
{
public:
    QVBoxLayout *verticalLayout_2;
    QLabel *label_4;
    QComboBox *columnChoice_2;
    QPushButton *pushButton;
    QLabel *label;
    QTextBrowser *textFile;
    QLabel *label_3;
    QComboBox *columnChoice;
    QLabel *label_2;
    QHBoxLayout *horizontalLayout;
    QLineEdit *lineEdit;
    QSpacerItem *horizontalSpacer;
    QLabel *labelms;
    QLabel *label_5;
    QLabel *label_6;
    QSpinBox *spinBox;
    QSpacerItem *verticalSpacer;
    QVBoxLayout *verticalLayout;
    QPushButton *runButton;
    QPushButton *pauseButton;
    QPushButton *instantButton;

    void setupUi(QWidget *TabPage)
    {
        if (TabPage->objectName().isEmpty())
            TabPage->setObjectName(QString::fromUtf8("TabPage"));
        TabPage->resize(301, 592);
        verticalLayout_2 = new QVBoxLayout(TabPage);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        label_4 = new QLabel(TabPage);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        verticalLayout_2->addWidget(label_4);

        columnChoice_2 = new QComboBox(TabPage);
        columnChoice_2->addItem(QString());
        columnChoice_2->addItem(QString());
        columnChoice_2->addItem(QString());
        columnChoice_2->addItem(QString());
        columnChoice_2->addItem(QString());
        columnChoice_2->addItem(QString());
        columnChoice_2->addItem(QString());
        columnChoice_2->setObjectName(QString::fromUtf8("columnChoice_2"));

        verticalLayout_2->addWidget(columnChoice_2);

        pushButton = new QPushButton(TabPage);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));

        verticalLayout_2->addWidget(pushButton);

        label = new QLabel(TabPage);
        label->setObjectName(QString::fromUtf8("label"));

        verticalLayout_2->addWidget(label);

        textFile = new QTextBrowser(TabPage);
        textFile->setObjectName(QString::fromUtf8("textFile"));
        textFile->setMaximumSize(QSize(16777215, 70));

        verticalLayout_2->addWidget(textFile);

        label_3 = new QLabel(TabPage);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        verticalLayout_2->addWidget(label_3);

        columnChoice = new QComboBox(TabPage);
        columnChoice->setObjectName(QString::fromUtf8("columnChoice"));

        verticalLayout_2->addWidget(columnChoice);

        label_2 = new QLabel(TabPage);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        verticalLayout_2->addWidget(label_2);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        lineEdit = new QLineEdit(TabPage);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));

        horizontalLayout->addWidget(lineEdit);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        labelms = new QLabel(TabPage);
        labelms->setObjectName(QString::fromUtf8("labelms"));
        labelms->setStyleSheet(QString::fromUtf8("color: rgb(94, 92, 100);"));

        horizontalLayout->addWidget(labelms);

        label_5 = new QLabel(TabPage);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setStyleSheet(QString::fromUtf8("color: rgb(94, 92, 100);"));

        horizontalLayout->addWidget(label_5);


        verticalLayout_2->addLayout(horizontalLayout);

        label_6 = new QLabel(TabPage);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        verticalLayout_2->addWidget(label_6);

        spinBox = new QSpinBox(TabPage);
        spinBox->setObjectName(QString::fromUtf8("spinBox"));
        spinBox->setMinimum(0);
        spinBox->setValue(1);

        verticalLayout_2->addWidget(spinBox);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        runButton = new QPushButton(TabPage);
        runButton->setObjectName(QString::fromUtf8("runButton"));
        runButton->setStyleSheet(QString::fromUtf8("background-color: rgb(0,111,60);\n"
"color: black; \n"
""));
        runButton->setCheckable(true);

        verticalLayout->addWidget(runButton);

        pauseButton = new QPushButton(TabPage);
        pauseButton->setObjectName(QString::fromUtf8("pauseButton"));
        pauseButton->setStyleSheet(QString::fromUtf8("background-color: rgb(0,111,60);\n"
"color: black; \n"
""));
        pauseButton->setCheckable(true);

        verticalLayout->addWidget(pauseButton);

        instantButton = new QPushButton(TabPage);
        instantButton->setObjectName(QString::fromUtf8("instantButton"));
        instantButton->setStyleSheet(QString::fromUtf8("background-color: rgb(0,111,60);\n"
"color: black; \n"
""));
        instantButton->setCheckable(true);

        verticalLayout->addWidget(instantButton);


        verticalLayout_2->addLayout(verticalLayout);


        retranslateUi(TabPage);

        QMetaObject::connectSlotsByName(TabPage);
    } // setupUi

    void retranslateUi(QWidget *TabPage)
    {
        TabPage->setWindowTitle(QCoreApplication::translate("TabPage", "Form", nullptr));
        label_4->setText(QCoreApplication::translate("TabPage", "Choose Algorithm:", nullptr));
        columnChoice_2->setItemText(0, QString());
        columnChoice_2->setItemText(1, QCoreApplication::translate("TabPage", "Delta", nullptr));
        columnChoice_2->setItemText(2, QCoreApplication::translate("TabPage", "Delta + Huffman", nullptr));
        columnChoice_2->setItemText(3, QCoreApplication::translate("TabPage", "Delta - Sprintz", nullptr));
        columnChoice_2->setItemText(4, QCoreApplication::translate("TabPage", "FIRE - Sprintz", nullptr));
        columnChoice_2->setItemText(5, QCoreApplication::translate("TabPage", "ZLib", nullptr));
        columnChoice_2->setItemText(6, QCoreApplication::translate("TabPage", "ZStandard", nullptr));

        pushButton->setText(QCoreApplication::translate("TabPage", "Choose File", nullptr));
        label->setText(QCoreApplication::translate("TabPage", "Filename:", nullptr));
        textFile->setHtml(QCoreApplication::translate("TabPage", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"hr { height: 1px; border-width: 0; }\n"
"li.unchecked::marker { content: \"\\2610\"; }\n"
"li.checked::marker { content: \"\\2612\"; }\n"
"</style></head><body style=\" font-family:'Ubuntu'; font-size:11pt; font-weight:400; font-style:normal;\">\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><br /></p></body></html>", nullptr));
        label_3->setText(QCoreApplication::translate("TabPage", "Choose Datastream:", nullptr));
        label_2->setText(QCoreApplication::translate("TabPage", "MilliSeconds Between Measurements:", nullptr));
        lineEdit->setText(QCoreApplication::translate("TabPage", "60", nullptr));
        labelms->setText(QCoreApplication::translate("TabPage", "60", nullptr));
        label_5->setText(QCoreApplication::translate("TabPage", "ms", nullptr));
        label_6->setText(QCoreApplication::translate("TabPage", "Precision (Digits After Decimal):", nullptr));
        runButton->setText(QCoreApplication::translate("TabPage", "Run", nullptr));
        pauseButton->setText(QCoreApplication::translate("TabPage", "Pause/Resume", nullptr));
        instantButton->setText(QCoreApplication::translate("TabPage", "Speed", nullptr));
    } // retranslateUi

};

namespace Ui {
    class TabPage: public Ui_TabPage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TABPAGE_H
