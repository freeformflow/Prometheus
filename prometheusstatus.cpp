#include "prometheusstatus.h"
#include "ui_prometheusstatus.h"

PrometheusStatus::PrometheusStatus(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PrometheusStatus)
{
    ui->setupUi(this);
}

PrometheusStatus::~PrometheusStatus()
{
    delete ui;
}


void PrometheusStatus::setupText(QString message, QString informativeMessage)
{
    ui->mainText->setText(message);
    ui->informativeText->setText(informativeMessage);

    ui->progressBar->setMinimum(0);
    ui->progressBar->setMaximum(0);
}
