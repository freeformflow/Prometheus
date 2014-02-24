#ifndef ATLAS_H
#define ATLAS_H

#include <QObject>
#include <QVector>
#include <QString>
#include <QQuaternion>
#include <QPoint>

class Atlas : public QObject
{
    Q_OBJECT
public:
    explicit Atlas(QObject *parent = 0);
    ~Atlas();

    /***********************************
    //    The SET Functions
    ***********************************/
    void createModel(int atomCount, bool periodicity);

    void setModelPeriodicity(bool periodicity);

    void setAtomChainName (int atomIndex, QString atomChainName);
    void setAtomAtomicNumber (int atomIndex, int atomicNumber);
    void setAtomForceFieldType (int atomIndex, QString atomForceFieldType);
    void setAtomMaxCovalentBonds (int atomIndex, int atomMaxCovalentBonds);
    void setAtomNumberOfLonePairs (int atomIndex, int atomNumberOfLonePairs);
    void setAtomName (int atomIndex, QString atomName);
    void setAtomPartialCharge (int atomIndex, float atomPartialCharge);
    void setAtomPositionX (int atomIndex, float atomPositionX);
    void setAtomPositionY (int atomIndex, float atomPositionY);
    void setAtomPositionZ (int atomIndex, float atomPositionZ);
    void setAtomResidueName (int atomIndex, QString atomResidueName);
    void setAtomResidueNumber (int atomIndex, int atomResidueNumber);
    void setAtomVelocity (int atomIndex, float atomVelocity);

    void setBondList (QVector<int> bondLoader);
    void setBondOrder (int bondOrder);

    void setDrawingStyle(int style);
    void setNumberOfAtoms();
    void setNumberOfBonds();

    void setSelectedAtom(int atomID);
    void setSelectedBond(int bondID);



    /***********************************
    //    The GET Functions
    ***********************************/
    bool& getModelPeriodicity();

    QVector<int>& getAtomAtomicNumber();
    QVector<QString>& getAtomName();
    QVector<QString>& getAtomForceFieldType();
    QVector<float>& getAtomPartialCharge();
    QVector<float>& getAtomPositionX();
    QVector<float>& getAtomPositionY();
    QVector<float>& getAtomPositionZ();


    QVector<QVector<int> >& getBondList();
    QVector<int>& getBondOrder();

    int& getDrawingStyle();
    int& getNumberOfAtoms();
    int& getNumberOfBonds();

    QVector<int>& getSelectedAtom();
    QVector<int>& getSelectedBond();


    /********************************************
     * Other Functions that Manipulate the Model
     ********************************************/

    void clearSelection();
    void unSelectAtom(int atomID);
    void unSelectBond(int bondID);

    void deleteHighlightedObjects();

    void singleSaveAtom(int atomID);
    void singleRecoverAtom();
    void editAtom(int atomID, int newAtomicNumber, bool atomBuilderMoved);
    void addAtom(int newAtomicNumber, QVector<float> pos);
    void placeNewAtom(QPoint cursorCoordinates, int windowWidth, int windowHeight, float xTrans, float yTrans, float zTrans, QQuaternion rotation, int atomicNumber, bool atomBuilderMoved);
    void purgeLastRecord(int atomID);

    void incrementBond();
    void decrementBond();

    
private:
    /***************************************************************************************************************************
     * These arrays will be needed to store the atom (or electron, core, etc) properties.  Atlas (brother of Prometheus) is the
     * universal framework that supports Prometheus.  The framework drives the format conversion abilities of Prometheus since
     * data can be pulled or ported to files as needed.  Instances of this class can be called to hold a number of models while
     * the program is active.
     ****************************************************************************************************************************/


    QVector<QString> atomChainName;
    QVector<int> atomAtomicNumber;
    QVector<QString> atomForceFieldType;
    QVector<int> atomMaxCovalentBonds;
    QVector<int> atomNumberOfLonePairs;
    QVector<QString> atomName;
    QVector<float> atomPartialCharge;
    QVector<float> atomPositionX;
    QVector<float> atomPositionY;
    QVector<float> atomPositionZ;
    QVector<QString> atomResidueName;
    QVector<int> atomResidueNumber;
    QVector<float> atomVelocity;

    QVector<QVector<int> > bondList;
    QVector<int> bondOrder;

    bool isPeriodic;
    int drawingStyle;
    int numberOfAtoms;
    int numberOfBonds;

    QVector<int> selectedAtoms;
    QVector<int> selectedBonds;

    int atomBuilderOrigin;
    int savedBondListSize;
    int savedAtomListSize;

    // These variables hold onto individual atoms that may need to be recovered during the use of the Atom Building Tool.
    int singleSave_atomID;
    QString singleSave_atomChainName;
    int singleSave_atomAtomicNumber;
    QString singleSave_atomForceFieldType;
    int singleSave_atomMaxCovalentBonds;
    int singleSave_atomNumberOfLonePairs;
    QString singleSave_atomName;
    float singleSave_atomPartialCharge;
    float singleSave_atomPositionX;
    float singleSave_atomPositionY;
    float singleSave_atomPositionZ;
    QString singleSave_atomResidueName;
    int singleSave_atomResidueNumber;
    float singleSave_atomVelocity;

signals:
    
public slots:
    
};

#endif // ATLAS_H
