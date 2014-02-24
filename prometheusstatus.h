#ifndef PROMETHEUSSTATUS_H
#define PROMETHEUSSTATUS_H

#include <QDialog>

namespace Ui {
class PrometheusStatus;
}

class PrometheusStatus : public QDialog
{
    Q_OBJECT
    
public:
    explicit PrometheusStatus(QWidget *parent = 0);
    ~PrometheusStatus();

    void setupText(QString message, QString informativeMessage);
    
private:
    Ui::PrometheusStatus *ui;
};

#endif // PROMETHEUSSTATUS_H
