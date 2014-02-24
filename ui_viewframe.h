/********************************************************************************
** Form generated from reading UI file 'viewframe.ui'
**
** Created: Tue Feb 26 13:26:28 2013
**      by: Qt User Interface Compiler version 5.0.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VIEWFRAME_H
#define UI_VIEWFRAME_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>

QT_BEGIN_NAMESPACE

class Ui_ViewFrame
{
public:

    void setupUi(QDialog *ViewFrame)
    {
        if (ViewFrame->objectName().isEmpty())
            ViewFrame->setObjectName(QStringLiteral("ViewFrame"));
        ViewFrame->resize(578, 441);

        retranslateUi(ViewFrame);

        QMetaObject::connectSlotsByName(ViewFrame);
    } // setupUi

    void retranslateUi(QDialog *ViewFrame)
    {
        ViewFrame->setWindowTitle(QApplication::translate("ViewFrame", "Dialog", 0));
    } // retranslateUi

};

namespace Ui {
    class ViewFrame: public Ui_ViewFrame {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VIEWFRAME_H
