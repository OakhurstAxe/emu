/********************************************************************************
** Form generated from reading UI file 'oaemumainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_OAEMUMAINWINDOW_H
#define UI_OAEMUMAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QListView *lstGameView;
    QGroupBox *gbViewVcs;
    QLabel *lblName;
    QLabel *lblController;
    QLabel *lblCompany;
    QLabel *lblScreen;
    QLabel *lblRom;
    QLabel *lblNameValue;
    QLabel *lblControllerValue;
    QLabel *lblCompanyValue;
    QLabel *lblScreenValue;
    QLabel *lblRomValue;
    QLineEdit *leName;
    QLineEdit *leController;
    QLineEdit *leCompany;
    QLineEdit *leScreen;
    QLineEdit *leRomFile;
    QPushButton *btnRun;
    QPushButton *btnEdit;
    QPushButton *btnAdd;
    QPushButton *btnSave;

    void setupUi(QWidget *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(800, 600);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        centralWidget->setGeometry(QRect(0, 10, 471, 584));
        lstGameView = new QListView(centralWidget);
        lstGameView->setObjectName(QString::fromUtf8("lstGameView"));
        lstGameView->setGeometry(QRect(10, 20, 256, 192));
        gbViewVcs = new QGroupBox(centralWidget);
        gbViewVcs->setObjectName(QString::fromUtf8("gbViewVcs"));
        gbViewVcs->setGeometry(QRect(10, 240, 451, 261));
        lblName = new QLabel(gbViewVcs);
        lblName->setObjectName(QString::fromUtf8("lblName"));
        lblName->setGeometry(QRect(40, 50, 58, 18));
        lblController = new QLabel(gbViewVcs);
        lblController->setObjectName(QString::fromUtf8("lblController"));
        lblController->setGeometry(QRect(30, 80, 58, 18));
        lblCompany = new QLabel(gbViewVcs);
        lblCompany->setObjectName(QString::fromUtf8("lblCompany"));
        lblCompany->setGeometry(QRect(30, 110, 58, 18));
        lblScreen = new QLabel(gbViewVcs);
        lblScreen->setObjectName(QString::fromUtf8("lblScreen"));
        lblScreen->setGeometry(QRect(30, 140, 58, 18));
        lblRom = new QLabel(gbViewVcs);
        lblRom->setObjectName(QString::fromUtf8("lblRom"));
        lblRom->setGeometry(QRect(30, 180, 58, 18));
        lblNameValue = new QLabel(gbViewVcs);
        lblNameValue->setObjectName(QString::fromUtf8("lblNameValue"));
        lblNameValue->setGeometry(QRect(140, 50, 261, 18));
        lblControllerValue = new QLabel(gbViewVcs);
        lblControllerValue->setObjectName(QString::fromUtf8("lblControllerValue"));
        lblControllerValue->setGeometry(QRect(140, 80, 251, 18));
        lblCompanyValue = new QLabel(gbViewVcs);
        lblCompanyValue->setObjectName(QString::fromUtf8("lblCompanyValue"));
        lblCompanyValue->setGeometry(QRect(140, 110, 271, 18));
        lblScreenValue = new QLabel(gbViewVcs);
        lblScreenValue->setObjectName(QString::fromUtf8("lblScreenValue"));
        lblScreenValue->setGeometry(QRect(140, 150, 271, 18));
        lblRomValue = new QLabel(gbViewVcs);
        lblRomValue->setObjectName(QString::fromUtf8("lblRomValue"));
        lblRomValue->setGeometry(QRect(140, 180, 281, 18));
        leName = new QLineEdit(gbViewVcs);
        leName->setObjectName(QString::fromUtf8("leName"));
        leName->setGeometry(QRect(120, 40, 291, 32));
        leController = new QLineEdit(gbViewVcs);
        leController->setObjectName(QString::fromUtf8("leController"));
        leController->setGeometry(QRect(120, 80, 291, 32));
        leCompany = new QLineEdit(gbViewVcs);
        leCompany->setObjectName(QString::fromUtf8("leCompany"));
        leCompany->setGeometry(QRect(120, 110, 291, 32));
        leScreen = new QLineEdit(gbViewVcs);
        leScreen->setObjectName(QString::fromUtf8("leScreen"));
        leScreen->setGeometry(QRect(120, 150, 291, 32));
        leRomFile = new QLineEdit(gbViewVcs);
        leRomFile->setObjectName(QString::fromUtf8("leRomFile"));
        leRomFile->setGeometry(QRect(120, 180, 291, 32));
        lblName->raise();
        lblController->raise();
        lblCompany->raise();
        lblScreen->raise();
        lblRom->raise();
        lblControllerValue->raise();
        lblCompanyValue->raise();
        lblScreenValue->raise();
        lblRomValue->raise();
        leName->raise();
        leController->raise();
        leCompany->raise();
        leScreen->raise();
        leRomFile->raise();
        lblNameValue->raise();
        btnRun = new QPushButton(centralWidget);
        btnRun->setObjectName(QString::fromUtf8("btnRun"));
        btnRun->setGeometry(QRect(100, 530, 88, 34));
        btnEdit = new QPushButton(centralWidget);
        btnEdit->setObjectName(QString::fromUtf8("btnEdit"));
        btnEdit->setGeometry(QRect(250, 530, 88, 34));
        btnAdd = new QPushButton(centralWidget);
        btnAdd->setObjectName(QString::fromUtf8("btnAdd"));
        btnAdd->setGeometry(QRect(300, 30, 88, 34));
        btnSave = new QPushButton(centralWidget);
        btnSave->setObjectName(QString::fromUtf8("btnSave"));
        btnSave->setGeometry(QRect(300, 110, 88, 34));

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QWidget *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "oafrontend", nullptr));
        gbViewVcs->setTitle(QApplication::translate("MainWindow", "Game Info", nullptr));
        lblName->setText(QApplication::translate("MainWindow", "Name:", nullptr));
        lblController->setText(QApplication::translate("MainWindow", "Controller:", nullptr));
        lblCompany->setText(QApplication::translate("MainWindow", "Company:", nullptr));
        lblScreen->setText(QApplication::translate("MainWindow", "Screen:", nullptr));
        lblRom->setText(QApplication::translate("MainWindow", "Rom File:", nullptr));
        lblNameValue->setText(QApplication::translate("MainWindow", "TextLabel", nullptr));
        lblControllerValue->setText(QApplication::translate("MainWindow", "TextLabel", nullptr));
        lblCompanyValue->setText(QApplication::translate("MainWindow", "TextLabel", nullptr));
        lblScreenValue->setText(QApplication::translate("MainWindow", "TextLabel", nullptr));
        lblRomValue->setText(QApplication::translate("MainWindow", "TextLabel", nullptr));
        btnRun->setText(QApplication::translate("MainWindow", "Run", nullptr));
        btnEdit->setText(QApplication::translate("MainWindow", "Edit", nullptr));
        btnAdd->setText(QApplication::translate("MainWindow", "Add", nullptr));
        btnSave->setText(QApplication::translate("MainWindow", "Save", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_OAEMUMAINWINDOW_H
