/********************************************************************************
** Form generated from reading UI file 'kestrel.ui'
**
** Created by: Qt User Interface Compiler version 6.0.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_KESTREL_H
#define UI_KESTREL_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Kestrel
{
public:
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    QPushButton *startButton;
    QPushButton *addTxButton;
    QPushButton *debugButton;
    QLabel *logoLabel;
    QTextBrowser *textBrowser;
    QPushButton *mineButton;
    QLabel *label;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *Kestrel)
    {
        if (Kestrel->objectName().isEmpty())
            Kestrel->setObjectName(QString::fromUtf8("Kestrel"));
        Kestrel->setEnabled(true);
        Kestrel->resize(1280, 800);
        centralwidget = new QWidget(Kestrel);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        gridLayout = new QGridLayout(centralwidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        startButton = new QPushButton(centralwidget);
        startButton->setObjectName(QString::fromUtf8("startButton"));
        startButton->setEnabled(true);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(startButton->sizePolicy().hasHeightForWidth());
        startButton->setSizePolicy(sizePolicy);

        gridLayout->addWidget(startButton, 6, 0, 1, 1);

        addTxButton = new QPushButton(centralwidget);
        addTxButton->setObjectName(QString::fromUtf8("addTxButton"));
        addTxButton->setEnabled(false);

        gridLayout->addWidget(addTxButton, 6, 1, 1, 1);

        debugButton = new QPushButton(centralwidget);
        debugButton->setObjectName(QString::fromUtf8("debugButton"));
        debugButton->setEnabled(false);

        gridLayout->addWidget(debugButton, 5, 0, 1, 1);

        logoLabel = new QLabel(centralwidget);
        logoLabel->setObjectName(QString::fromUtf8("logoLabel"));
        logoLabel->setEnabled(true);
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(logoLabel->sizePolicy().hasHeightForWidth());
        logoLabel->setSizePolicy(sizePolicy1);
        logoLabel->setMinimumSize(QSize(0, 0));
        logoLabel->setMaximumSize(QSize(300, 300));
        logoLabel->setBaseSize(QSize(0, 0));
        logoLabel->setPixmap(QPixmap(QString::fromUtf8(":/files/images/k-1k1k.png")));
        logoLabel->setScaledContents(true);
        logoLabel->setAlignment(Qt::AlignHCenter|Qt::AlignTop);

        gridLayout->addWidget(logoLabel, 2, 0, 1, 1);

        textBrowser = new QTextBrowser(centralwidget);
        textBrowser->setObjectName(QString::fromUtf8("textBrowser"));

        gridLayout->addWidget(textBrowser, 2, 1, 4, 2);

        mineButton = new QPushButton(centralwidget);
        mineButton->setObjectName(QString::fromUtf8("mineButton"));
        mineButton->setEnabled(false);
        mineButton->setCheckable(true);

        gridLayout->addWidget(mineButton, 6, 2, 1, 1);

        label = new QLabel(centralwidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label, 3, 0, 2, 1);

        Kestrel->setCentralWidget(centralwidget);
        menubar = new QMenuBar(Kestrel);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 1280, 24));
        Kestrel->setMenuBar(menubar);
        statusbar = new QStatusBar(Kestrel);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        Kestrel->setStatusBar(statusbar);

        retranslateUi(Kestrel);

        QMetaObject::connectSlotsByName(Kestrel);
    } // setupUi

    void retranslateUi(QMainWindow *Kestrel)
    {
        Kestrel->setWindowTitle(QCoreApplication::translate("Kestrel", "Kestrel", nullptr));
        startButton->setText(QCoreApplication::translate("Kestrel", "Start", nullptr));
        addTxButton->setText(QCoreApplication::translate("Kestrel", "Execute Transaction", nullptr));
        debugButton->setText(QCoreApplication::translate("Kestrel", "Debug", nullptr));
        logoLabel->setText(QString());
        mineButton->setText(QCoreApplication::translate("Kestrel", "Start Mining", nullptr));
        label->setText(QCoreApplication::translate("Kestrel", "<html><head/><body><p><span style=\" font-size:18pt; font-weight:600;\">Kestrel Alpha 0.1</span></p><p><span style=\" font-size:18pt;\">Blockchain Simulator</span></p><p><span style=\" font-size:12pt;\">Click </span><span style=\" font-size:12pt; font-weight:600;\">Start</span><span style=\" font-size:12pt;\"> to begin.</span></p><p><span style=\" font-size:12pt;\">As transactions are executed,</span></p><p><span style=\" font-size:12pt;\">they will be added to blocks for mining.</span></p><p><span style=\" font-size:12pt;\">Please wait for the mining process</span></p><p><span style=\" font-size:12pt;\">to finish before exiting.</span></p><p><br/></p><p><span style=\" font-size:12pt;\">Databases are created at</span></p><p><span style=\" font-size:12pt; font-weight:600;\">/tmp</span></p><p><span style=\" font-size:12pt;\">and are </span><span style=\" font-size:12pt; font-weight:600;\">not</span><span style=\" font-size:12pt;\"> deleted when</span></p><p><span style=\" font-size:12pt;\">Kestrel is closed.</sp"
                        "an></p></body></html>", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Kestrel: public Ui_Kestrel {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_KESTREL_H
