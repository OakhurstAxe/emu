/********************************************************************************
** Form generated from reading UI file 'vcsmainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VCSMAINWINDOW_H
#define UI_VCSMAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_EmuMainWindow
{
public:
    QVBoxLayout *verticalLayout;

    void setupUi(QDialog *EmuMainWindow)
    {
        if (EmuMainWindow->objectName().isEmpty())
            EmuMainWindow->setObjectName(QString::fromUtf8("EmuMainWindow"));
        EmuMainWindow->resize(640, 768);
        verticalLayout = new QVBoxLayout(EmuMainWindow);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));

        retranslateUi(EmuMainWindow);

        QMetaObject::connectSlotsByName(EmuMainWindow);
    } // setupUi

    void retranslateUi(QDialog *EmuMainWindow)
    {
        EmuMainWindow->setWindowTitle(QApplication::translate("EmuMainWindow", "emumainwindow", nullptr));
    } // retranslateUi

};

namespace Ui {
    class EmuMainWindow: public Ui_EmuMainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VCSMAINWINDOW_H
