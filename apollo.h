#ifndef APOLLO_H
#define APOLLO_H

#include <QObject>

#include "atlas.h"

extern Atlas prometheusModel;

class Apollo : public QObject
{
    Q_OBJECT
public:
    Apollo(QObject *parent = 0);
    ~Apollo();

    void setupAtomicData();

    // Charge Equilibration (QEq) Method
    void setNetCharge(double userInput);
    void setupSystem (QVector<int> &Atoms, QVector<float> &PosX, QVector<float> &PosY, QVector<float> &PosZ, QVector<QVector<double> > &coeffMatrix, QVector<double> &partialCharges, double netCharge, QVector<double> &oldPartialCharges);
    void solveSystem (QVector<QVector<double> > &coeffMatrix, QVector<double> &partialCharges);
    bool iterateHydrogens(QVector<int> &Atoms, QVector<float> &PosX, QVector<float> &PosY, QVector<float> &PosZ, QVector<QVector<double> > &coeffMatrix, QVector<double> &partialCharges, double netCharge, QVector<double> &oldPartialCharges);
    double findFudgeyCoulombIntegral(QVector<float> pos1, QVector<float> pos2, int atomicNumber_i, int atomicNumber_j, float correction1, float correction2);

public slots:
    void performQEq();

signals:
    void calculationFinished(QString message, QString informativeMessage);

private:
    double netCharge;

    QVector<float> electronegativity;
    QVector<float> selfCoulombPotential;
    QVector<float> averageAtomSize;
    QVector<float> valenceOrbitalExponent;

    QVector<int> applicableElements;

signals:
    
public slots:
    
};

#endif // APOLLO_H
