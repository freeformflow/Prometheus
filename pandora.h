#ifndef PANDORA_H
#define PANDORA_H

#include <QObject>
#include <QString>

#include "atlas.h"

extern Atlas prometheusModel;


class Pandora : public QObject
{
    Q_OBJECT
public:
    explicit Pandora(QObject *parent = 0);
    ~Pandora();

    void setupLookupTables();

    void pandoraError (QString message);
    void pandoraInfo(QString message);

    /*******************************************************************************************
    * These functions hold instructions specific to parsing data in the given format and
    * delivering that data to the viewer for display.
    *******************************************************************************************/
    void parseDataFile (QString fileType, QString fileName);
    void parseBGF(QString fileName);
    void parseCIF();
    void parseCML();
    void parseLammpsInput(QString fileName);
    void parseLammpsTrajectory();
    void parseMOL();
    void parseMOL2();
    void parseMSI();
    void parsePBD();
    void parseXYZ();
    // void parseMMCIF();
    // void parsePBDML();

    /********************************************************************************************
     * These functions hold instructins specific to writing data files of given format and
     * saving them to a location specified by the User.
     *******************************************************************************************/
    void writeDataFile (QString fileType, QString fileName);
    void writeBGF(QString fileName);
    void writeCIF();
    void writeCML();
    void writeLammpsInput(QString fileName);
    void writeMOL();
    void writeMOL2();
    void writeMSI();
    void writePBD();
    void writeXYZ();
    // void writeMMCIF();
    // void writePBDML();


    /********************************************************************************************
     * These functions hold instructins specific to typing atoms according to a given
     * force field specified by the User.
     *******************************************************************************************/
    void assignFFTypes(int ForceField);
    void typeAIREBO();
    void typeAMBER();
    void typeCHARMM();
    void typeDREIDING();
    void typeGAFF();
    void typeReaxFF();
    void typeREBO();
    void typeUFF();


    // Book-keeping functions that will be helpful in reading and displaying chemical data.
    QString findElementalSymbol(int atomicNumber);
    int findAtomicNumber(QString forceFieldType);
    QVector<float> findRadii(QVector<int> &ListOfAtomicNumbers);
    QVector<QVector<float> > findColor(QVector<int> &ListOfAtomicNumbers);
    void findBondData(QVector<QVector<int> > ListOfBonds, int activeModel, QVector<float> &BondLengths, QVector<QVector<float> > &BondCenters, QVector<QVector<float> > &BondNormals);
    void nameAllAtoms();

    void checkForAromaticity(QVector<int> &Atoms, QVector<QString> &FFType, QVector<QVector<int> > &BondList, QVector<int> &AromaticAtoms);

signals:
    
public slots:

private:
    int activeModel;

    /***************************************************************************************************************************
     * These arrays will hold lookup data that make data manipulation easier.
    ****************************************************************************************************************************/
    QVector<QString> elementLookup;
    QVector<float> vanderWaalsLookup;
    QVector<QVector<float> > colorLookup;

    
};

#endif // PANDORA_H
