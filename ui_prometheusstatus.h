/********************************************************************************
** Form generated from reading UI file 'prometheusstatus.ui'
**
** Created: Mon Mar 18 18:18:00 2013
**      by: Qt User Interface Compiler version 5.0.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PROMETHEUSSTATUS_H
#define UI_PROMETHEUSSTATUS_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_PrometheusStatus
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *mainText;
    QLabel *informativeText;
    QProgressBar *progressBar;

    void setupUi(QDialog *PrometheusStatus)
    {
        if (PrometheusStatus->objectName().isEmpty())
            PrometheusStatus->setObjectName(QStringLiteral("PrometheusStatus"));
        PrometheusStatus->resize(450, 150);
        PrometheusStatus->setMinimumSize(QSize(450, 150));
        PrometheusStatus->setMaximumSize(QSize(450, 150));
        verticalLayout = new QVBoxLayout(PrometheusStatus);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        mainText = new QLabel(PrometheusStatus);
        mainText->setObjectName(QStringLiteral("mainText"));

        verticalLayout->addWidget(mainText);

        informativeText = new QLabel(PrometheusStatus);
        informativeText->setObjectName(QStringLiteral("informativeText"));

        verticalLayout->addWidget(informativeText);

        progressBar = new QProgressBar(PrometheusStatus);
        progressBar->setObjectName(QStringLiteral("progressBar"));
        progressBar->setValue(24);

        verticalLayout->addWidget(progressBar);


        retranslateUi(PrometheusStatus);

        QMetaObject::connectSlotsByName(PrometheusStatus);
    } // setupUi

    void retranslateUi(QDialog *PrometheusStatus)
    {
        PrometheusStatus->setWindowTitle(QApplication::translate("PrometheusStatus", "Dialog", 0));
        mainText->setText(QApplication::translate("PrometheusStatus", "TextLabel", 0));
        informativeText->setText(QApplication::translate("PrometheusStatus", "TextLabel", 0));
    } // retranslateUi

};

namespace Ui {
    class PrometheusStatus: public Ui_PrometheusStatus {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PROMETHEUSSTATUS_H
