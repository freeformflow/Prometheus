/********************************************************************************
** Form generated from reading UI file 'savedialog.ui'
**
** Created: Thu Feb 28 15:35:24 2013
**      by: Qt User Interface Compiler version 5.0.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SAVEDIALOG_H
#define UI_SAVEDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SaveDialog
{
public:
    QComboBox *fileTypeComboBox;
    QListView *listView;
    QWidget *widget;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLineEdit *saveNameEdit;

    void setupUi(QDialog *SaveDialog)
    {
        if (SaveDialog->objectName().isEmpty())
            SaveDialog->setObjectName(QStringLiteral("SaveDialog"));
        SaveDialog->resize(479, 417);
        fileTypeComboBox = new QComboBox(SaveDialog);
        fileTypeComboBox->setObjectName(QStringLiteral("fileTypeComboBox"));
        fileTypeComboBox->setGeometry(QRect(280, 160, 111, 26));
        listView = new QListView(SaveDialog);
        listView->setObjectName(QStringLiteral("listView"));
        listView->setGeometry(QRect(20, 110, 231, 201));
        widget = new QWidget(SaveDialog);
        widget->setObjectName(QStringLiteral("widget"));
        widget->setGeometry(QRect(30, 50, 204, 23));
        horizontalLayout = new QHBoxLayout(widget);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(widget);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout->addWidget(label);

        saveNameEdit = new QLineEdit(widget);
        saveNameEdit->setObjectName(QStringLiteral("saveNameEdit"));

        horizontalLayout->addWidget(saveNameEdit);


        retranslateUi(SaveDialog);

        QMetaObject::connectSlotsByName(SaveDialog);
    } // setupUi

    void retranslateUi(QDialog *SaveDialog)
    {
        SaveDialog->setWindowTitle(QApplication::translate("SaveDialog", "Dialog", 0));
        label->setText(QApplication::translate("SaveDialog", "Save As:", 0));
    } // retranslateUi

};

namespace Ui {
    class SaveDialog: public Ui_SaveDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SAVEDIALOG_H
