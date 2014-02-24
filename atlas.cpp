#include "atlas.h"
#include "pandora.h"

#include <QDebug>
#include <qmath.h>


/*
**********************************************************************************************************
                                           Notes
**********************************************************************************************************
The purpose of the Atlas Class is to contain functions at the core of the Prometheus framework.  When an instance
 of this class is created, it allocates the parts needed to build a model for display to the user.  Each
 instance is independent, so multiple models can be displayed in a menu simulataneously.  The CPSviewer takes its
 cues from this framework when displaying coordinate and other data.  Data from input files are loaded into this
structure, and data is pulled from here for export.

Because the vectors that store the data in Atlas are private, the functions of this class are all about accessing and manipulating
those vectors.  Any sort of calculations involving them are done outside in other classes and modules.  Atlas is all
about providing a framework to support the rest of the science.
***********************************************************************************************************
*/


Atlas::Atlas(QObject *parent) :
    QObject(parent)
{
}


// Empty Destructor
Atlas::~Atlas() {}


/*
***********************************************************************************
                            Set Functions
***********************************************************************************
    These functions will deal with loading data into the Atlas Framework. The function
    titles are relatively self-explainatory.
*/

void Atlas::createModel(int atomCount, bool periodicity)
{
    // This function takes the number of atoms (as retrieved from an input file) and allocates memory for all the
    // arrays storing data in the model.  These arrays are filled/manipulated in other functions.

    //isPeriodic = periodicity;
    isPeriodic = false;

    atomChainName.resize(atomCount);
    atomAtomicNumber.resize(atomCount);
    atomForceFieldType.resize(atomCount);
    atomMaxCovalentBonds.resize(atomCount);
    atomNumberOfLonePairs.resize(atomCount);
    atomName.resize(atomCount);
    atomPartialCharge.resize(atomCount);
    atomPositionX.resize(atomCount);
    atomPositionY.resize(atomCount);
    atomPositionZ.resize(atomCount);
    atomResidueName.resize(atomCount);
    atomResidueNumber.resize(atomCount);
    atomVelocity.resize(atomCount);

    bondList.clear();
    bondOrder.clear();

    drawingStyle = 0;
    numberOfAtoms = atomCount;
    numberOfBonds = 0;

    selectedAtoms.clear();
    selectedBonds.clear();

    return;

}

void Atlas::setModelPeriodicity(bool periodicity)
{
    isPeriodic = periodicity;
}

void Atlas::setAtomChainName (int atomIndex, QString input)
{
    atomChainName[atomIndex - 1] = input;
}

void Atlas::setAtomAtomicNumber(int atomIndex, int atomicNumber)
{
    atomAtomicNumber[atomIndex - 1] = atomicNumber;
}

void Atlas::setAtomForceFieldType (int atomIndex, QString input)
{
    atomForceFieldType[atomIndex - 1] = input;
}

void Atlas::setAtomMaxCovalentBonds (int atomIndex, int input)
{
    atomMaxCovalentBonds[atomIndex - 1] = input;
}

void Atlas::setAtomNumberOfLonePairs (int atomIndex, int input)
{
    atomNumberOfLonePairs[atomIndex - 1] = input;
}

void Atlas::setAtomName (int atomIndex, QString input)
{
    atomName[atomIndex - 1] = input;
}

void Atlas::setAtomPartialCharge (int atomIndex, float input)
{
    atomPartialCharge[atomIndex - 1] = input;
}

void Atlas::setAtomPositionX (int atomIndex, float input)
{
    atomPositionX[atomIndex - 1] = input;
}

void Atlas::setAtomPositionY (int atomIndex, float input)
{
    atomPositionY[atomIndex - 1] = input;
}

void Atlas::setAtomPositionZ (int atomIndex, float input)
{
    atomPositionZ[atomIndex - 1] = input;
}

void Atlas::setAtomResidueName (int atomIndex, QString input)
{
    atomResidueName[atomIndex - 1] = input;
}

void Atlas::setAtomResidueNumber (int atomIndex, int input)
{
    atomResidueNumber[atomIndex - 1] = input;
}

void Atlas::setAtomVelocity (int atomIndex, float input)
{
    atomVelocity[atomIndex - 1] = input;
}


void Atlas::setBondList(QVector<int> input)
{
    bondList.push_back(input);
}

void Atlas::setBondOrder(int input)
{
    bondOrder.push_back(input);
}



void Atlas::setDrawingStyle(int input)
{
    drawingStyle = input;
}

void Atlas::setNumberOfAtoms()
{
    numberOfAtoms = atomPositionX.size();
}

void Atlas::setNumberOfBonds()
{
    numberOfBonds = bondList.size();
}

void Atlas::setSelectedAtom(int atomID)
{
    selectedAtoms.push_back(atomID);
}

void Atlas::setSelectedBond(int bondID)
{
    selectedBonds.push_back(bondID);
}


/*
***********************************************************************************
                            Get Functions
***********************************************************************************
    These functions will deal with retrieving data from the Atlas Framework.  The function
    titles are relatively self-explainatory.
*/


bool& Atlas::getModelPeriodicity()
{
    return isPeriodic;
}

QVector<int>& Atlas::getAtomAtomicNumber()
{
    return atomAtomicNumber;
}

QVector<QString>& Atlas::getAtomName()
{
    return atomName;
}

QVector<QString>& Atlas::getAtomForceFieldType()
{
    return atomForceFieldType;
}

QVector<float>& Atlas::getAtomPartialCharge()
{
    return atomPartialCharge;
}

QVector<float>& Atlas::getAtomPositionX()
{
    return atomPositionX;
}

QVector<float>& Atlas::getAtomPositionY()
{
    return atomPositionY;
}

QVector<float>& Atlas::getAtomPositionZ()
{
    return atomPositionZ;
}



QVector< QVector<int> >& Atlas::getBondList()
{
    return bondList;
}

QVector<int>& Atlas::getBondOrder()
{
    return bondOrder;
}


int& Atlas::getDrawingStyle()
{
    return drawingStyle;
}

int& Atlas::getNumberOfAtoms()
{
    return numberOfAtoms;
}

int& Atlas::getNumberOfBonds()
{
    return numberOfBonds;
}

QVector<int>& Atlas::getSelectedAtom()
{
    return selectedAtoms;
}

QVector<int>& Atlas::getSelectedBond()
{
    return selectedBonds;
}


/*
***********************************************************************************
                       Framwork Manipulation Functions
***********************************************************************************
    These functions will deal with manipulating data within the Atlas Framework
    according to the User's requests.
*/


void Atlas::clearSelection()
{
    /*****************************************************************************************************************************
     * This function is meant to un-highlight or un-select every atom and bond on-screen.  The atoms, themselves, are allowed to
     * remain within the Atlas Framework.
    ******************************************************************************************************************************/

    selectedAtoms.clear();
    selectedBonds.clear();
}

void Atlas::unSelectAtom(int atomID)
{
    /*****************************************************************************************************************************
     * This function is meant to un-highlight or un-select a single atom and only that atom. The atom, iteself is allowed to
     * remain within the Atlas Framework.
    ******************************************************************************************************************************/

    for (int i = 0; i < selectedAtoms.size(); ++i)
    {
        if (selectedAtoms[i] == atomID)
        {
            selectedAtoms.remove(i);
            break;
        }
    }
}

void Atlas::unSelectBond(int bondID)
{
    /*****************************************************************************************************************************
     * This function is meant to un-highlight or un-select a single bond and only that bond. The bond, iteself is allowed to
     * remain within the Atlas Framework.
    ******************************************************************************************************************************/

    for (int i = 0; i < selectedBonds.size(); ++i)
    {
        if (selectedBonds[i] == bondID)
        {
            selectedBonds.remove(i);
            break;
        }
    }
}

void Atlas::deleteHighlightedObjects()
{
    /*****************************************************************************************************************************
     * This function is meant to delete all selected atoms and/or bonds. The atoms and bond are removed from the Atlas Framework
     * and deleted from the molecular model.  This function works with multiple selected bonds and atoms or it can delete items
     * one at a time along with the User's clicks.
    ******************************************************************************************************************************/

    int countDown;
    QVector<int>::Iterator it, it2;
    QVector<int> deleteCheckList; // These arrays will tally values we need to manipulate without messing up our for loops.
    QVector< QVector<int> > bondListCopy;

    // Create a copy of the bond list...  some of the bonds will be deleted because their constituent atoms have been deleted...  however, the User
    // may want some bonds deleted directly.  We must make sure we don't lose track of our bond index so we can comply.
    bondListCopy.resize(bondList.size());
    for (int i = 0; i < bondList.size(); i++)
    {
        bondListCopy[i].resize(2);
    }

    bondListCopy = bondList;



    // Start with atom properties...
    qSort(selectedAtoms);
    deleteCheckList.clear();

    countDown = selectedAtoms.size();
    for (it = selectedAtoms.begin(); it != selectedAtoms.end(); ++it)
    {
        // Cycle through the atoms highlighted on screen and delete data assocated with them. During the loading of the model file, Pandora was able to guarantee
        // atom's data is placed in the appropriate array, indexed to match up to the atom's ID number...  still, we have to be careful
        // because we are deleting data using an index that becomes out-dated every time we delete a record.  We use the countDown
        // counter to adjust our target.

        atomChainName.remove(*it - 1 - selectedAtoms.size() + countDown);
        atomAtomicNumber.remove(*it - 1 - selectedAtoms.size() + countDown);
        atomForceFieldType.remove(*it - 1 - selectedAtoms.size() + countDown);
        atomMaxCovalentBonds.remove(*it - 1 - selectedAtoms.size() + countDown);
        atomNumberOfLonePairs.remove(*it - 1 - selectedAtoms.size() + countDown);
        atomName.remove(*it - 1 - selectedAtoms.size() + countDown);
        atomPartialCharge.remove(*it - 1 - selectedAtoms.size() + countDown);
        atomPositionX.remove(*it - 1 - selectedAtoms.size() + countDown);
        atomPositionY.remove(*it - 1 - selectedAtoms.size() + countDown);
        atomPositionZ.remove(*it - 1 - selectedAtoms.size() + countDown);
        atomResidueName.remove(*it - 1 - selectedAtoms.size() + countDown);
        atomResidueNumber.remove(*it - 1 - selectedAtoms.size() + countDown);
        atomVelocity.remove(*it - 1 - selectedAtoms.size() + countDown);

        countDown--;

        // We must delete any bond that contains the atom being deleted...  No atom means no bond.  But we must be careful.  Deleting items from a list we are
        // iterating through will throw us off... so we'll make a list first (deleteCheckList)...  and then delete the right bonds from this checklist.

        for (int i = 0; i < bondList.size(); ++i)
        {
            if (bondList[i].contains(*it))
            {
                if (!deleteCheckList.contains(i))
                {
                    // Add this record to the checklist of bonds we need to delete.
                    deleteCheckList.push_back(i);
                }
            }
        }
    }


    // Now we'll check for bonds that have been directly requested for deletion.
    if (!selectedBonds.isEmpty())
    {
        // We determine what bond index matches what the User has requested in selectedBonds and see if that unique pairing of atoms
        // is already slated for deletion or not. If it isn't, we delete it... if it's already marked, we just move on.

        for (it = selectedBonds.begin(); it != selectedBonds.end(); ++it)
        {
            int atomA = bondListCopy[*it][0];
            int atomB = bondListCopy[*it][1];

            for (int i = 0; i < bondList.size(); ++i)
            {
                if (atomA == bondList[i][0] & atomB == bondList[i][1])
                {
                    // Check to see if we already have it.
                    if (!deleteCheckList.contains(i))
                    {
                        // We have a match, and this bond must be deleted.
                        deleteCheckList.push_back(i);
                    }
                }
            }
        }
    }



    // Now we delete the bonds, if any.
    if (!deleteCheckList.isEmpty())
    {
        qSort(deleteCheckList);
        countDown = deleteCheckList.size();
        for (it = deleteCheckList.begin(); it != deleteCheckList.end() ; ++it)
        {
            // Remove all data under this bond index...  but we must be careful and correct the index as we delete bonds.
            bondList.remove(*it - deleteCheckList.size() + countDown);
            bondOrder.remove(*it - deleteCheckList.size() + countDown);

            // Decrement countDown...  the lists get shorter, so as countDown shrinks, it keeps us from going out of range.
            countDown--;
        }
    }

    // Now we move on to bonds affected by the deleted atoms indirectly.  The bond drawing coordinates are referenced to the location of the atoms
    // that make up the bond.  So when we adjusted the index of the atoms above, it forces us to now adjust the values in the bond arrays.
    for (int i = 0; i < bondList.size(); ++i)
    {
        for (int j = 0; j < 2; j++)
        {
            int indexAdjust = selectedAtoms.size();
            for (int k = selectedAtoms.size() - 1; k > -1; k--)
            {
                // Starting with the largest atomID, we work our way down the index, correcting atoms we find.  By starting with the biggest, we only
                // need to adjust the index once.

                if (bondList[i][j] > selectedAtoms[k])
                {
                    bondList[i][j] -= indexAdjust;
                    break;
                }

                indexAdjust--;
            }
        }
    }


    // Now that we've cycled the selected atoms and bonds, do some clean up and exit this function.

    // Clear our lists of both and exit this function.
    selectedAtoms.clear();
    selectedBonds.clear();

    // Ask Prometheus to re-count the number of atoms and bonds in its model.
    setNumberOfAtoms();
    setNumberOfBonds();
}


void Atlas::singleSaveAtom(int atomID)
{
    // This function is meant to store all data about an atom that is being edited with the Build Atom Tool.  If the User
    // moves off the atom, a new atom is to be created, bonded to the old.  On the first click, the atom is replaced, so this
    // function allows Prometheus to remember the atom and put it back.

    singleSave_atomID = atomID;

    singleSave_atomChainName = atomChainName[atomID - 1];
    singleSave_atomAtomicNumber = atomAtomicNumber[atomID - 1];
    singleSave_atomForceFieldType = atomForceFieldType[atomID - 1];
    singleSave_atomMaxCovalentBonds = atomMaxCovalentBonds[atomID - 1];
    singleSave_atomNumberOfLonePairs = atomMaxCovalentBonds[atomID - 1];
    singleSave_atomName = atomName[atomID - 1];
    singleSave_atomPartialCharge = atomPartialCharge[atomID - 1];
    singleSave_atomPositionX = atomPositionX[atomID - 1];
    singleSave_atomPositionY = atomPositionY[atomID - 1];
    singleSave_atomPositionZ = atomPositionZ[atomID - 1];
    singleSave_atomResidueName = atomResidueName[atomID - 1];
    singleSave_atomResidueNumber = atomResidueNumber[atomID - 1];
    singleSave_atomVelocity = atomVelocity[atomID - 1];
}

void Atlas::singleRecoverAtom()
{
    // This function is meant to restore all data about an atom that is being edited with the Build Atom Tool.  If the User
    // moves off the atom, a new atom is to be created, bonded to the old.  On the first click, the atom is replaced, so this
    // function allows Prometheus to remember the atom and put it back.

    int atomID = singleSave_atomID;

    atomChainName[atomID - 1] = singleSave_atomChainName;
    atomAtomicNumber[atomID - 1] = singleSave_atomAtomicNumber;
    atomForceFieldType[atomID - 1] = singleSave_atomForceFieldType;
    atomMaxCovalentBonds[atomID - 1] = singleSave_atomMaxCovalentBonds;
    atomMaxCovalentBonds[atomID - 1] = singleSave_atomNumberOfLonePairs;
    atomName[atomID - 1] = singleSave_atomName;
    atomPartialCharge[atomID - 1] = singleSave_atomPartialCharge;
    atomPositionX[atomID - 1] = singleSave_atomPositionX;
    atomPositionY[atomID - 1] = singleSave_atomPositionY;
    atomPositionZ[atomID - 1] = singleSave_atomPositionZ;
    atomResidueName[atomID - 1] = singleSave_atomResidueName;
    atomResidueNumber[atomID - 1] = singleSave_atomResidueNumber;
    atomVelocity[atomID - 1] = singleSave_atomVelocity;
}

void Atlas::addAtom(int newAtomicNumber, QVector<float> pos)
{
    // This function creates an atom at the given coordinates.  Atomic data is merely added onto existing
    // arrays via the push_back function...  need to come back to this and add the passage of other atomic data.

    atomAtomicNumber.push_back(newAtomicNumber);
    atomChainName.push_back("A");
    atomForceFieldType.push_back("?");
    atomMaxCovalentBonds.push_back(4);
    atomNumberOfLonePairs.push_back(0);
    atomName.push_back("C47");
    atomPartialCharge.push_back(0);
    atomPositionX.push_back(pos[0]);
    atomPositionY.push_back(pos[1]);
    atomPositionZ.push_back(pos[2]);
    atomResidueName.push_back("RES");
    atomResidueNumber.push_back(1);
    atomVelocity.push_back(0);
}

void Atlas::editAtom(int atomID, int newAtomicNumber, bool atomBuilderMoved)
{
    /*****************************************************************************************************************************
     * This function is meant to edit an already existing atom. It only works on a single atom at a time in conjunction with the
     * Atom Tool.  It is passed the atomID of a clicked atom and the desired element by way of an atomic number.  This function
     * then modifies that atom's record within the Atlas Framework to comply with the User's wishes.
    ******************************************************************************************************************************/


    if (!atomBuilderMoved)   // Only do this if this is the first action of the Atom Build Tool...
    {
        // Determine if the User is changing the atom.
        if (atomAtomicNumber[atomID - 1] == newAtomicNumber)
        {
            // Nothing needs to be done.
            atomBuilderOrigin = atomID;
            savedBondListSize = bondList.size();
            return;
        }
    }

    if (atomBuilderMoved)
    {
        // If this function has been activated because the User is dragging the Atom Build Tool, we must put the previously changed atom back.
        singleRecoverAtom();
    }

    // This may not just be a single atom edit.  If the User later drags off this atom, a new atom is placed - bonded to the old atom.
    // We will need to store the old atom just in case we have to replace it.
    singleSaveAtom(atomID);

    // First thing we need to do is set the new element the User is requesting.
    atomAtomicNumber[atomID - 1] = newAtomicNumber;

    // And now we adjust the Force Field Type of this Atom
    // atomForceFieldType[atomID - 1] = some lookup function...  still have to setup force fields.

    // Re-initialize this atom's partial charge to be 0.
    atomPartialCharge[atomID - 1] = 0.0f;

    if (!atomBuilderMoved)  // Only do this if this is the first action of the Atom Build Tool...
    {
        // Place this atom's ID in memory as the starting point for any bonds drawn, if the User activates the special functions of the Atom Build Tool.
        atomBuilderOrigin = atomID;
        savedAtomListSize = atomPositionX.size();
        savedBondListSize = bondList.size();
    }
    else
    {
        // This is the second, special action of the Atom Build Tool...  the User is dragging an atom bonded to an existing one.
        // They have now dragged that new atom over a second exisiting atom...  so our task is to create a bond record between the two existing atoms,
        // and to not place a new atom.

        QVector<int> bond;
        bond.resize(2);

        if (atomID > atomBuilderOrigin)
        {
            bond[0] = atomBuilderOrigin;
            bond[1] = atomID;
        }
        else if (atomBuilderOrigin > atomID)
        {
            bond[0] = atomID;
            bond[1] = atomBuilderOrigin;
        }
        else if (atomBuilderOrigin == atomID)
        {
            // Then we're wasting our time...  There's nothing to do.
            return;
        }

        // Check the current bond list to see if the bond already exists.
        for (int i = 0; i < bondList.size(); i++)
        {
            if (bondList[i].contains(bond[0]) & bondList[i].contains(bond[1]))
            {
                // This bond already exists... there's nothing to do.
                return;
            }
        }

        // The bond wasn't found, so we add it here.
        bondList.push_back(bond);
        bondOrder.push_back(1);
    }

}



void Atlas::placeNewAtom(QPoint finalPos, int windowWidth, int windowHeight, float xTrans, float yTrans, float zTrans, QQuaternion rotation, int atomicNumber, bool atomBuilderMoved)
{
    /*****************************************************************************************************************************
     * This function is meant to create a new atom located at coordinates provided by the cursor. It only works on a single atom at
     * a time in conjunction with the Atom Tool.  This function takes the cursor position, and the viewer's current zoom (zTrans)
     * and does a little math to place an atom in model space, on a plane perpendicular to the screen center - model center vector.
     * Assumes a 45 degree field of view.
    ******************************************************************************************************************************/

    float XYheight, XYwidth, screenX, screenY, dot, theta;
    QVector<float> pos, axis, cross, Rv;
    QVector<QVector<float> > R;
    pos.resize(3);
    axis.resize(3);
    cross.resize(3);
    Rv.resize(3);

    R.resize(3);
    R[0].resize(3);
    R[1].resize(3);
    R[2].resize(3);


    // The User selects a point on-screen...  we take that point and map it on the far side of the viewing fulstrum... we want to project that
    // point outward and always place it on what the camera views as the XY plane.  Additional rotation and translation complicates our efforts,
    // but these can be corrected at the end.

    /******************************************************************************************************************************
    * Determine the height and width of the XY plane at the origin within the view frusrum.
    *   We know from Pythagorean's Theorem that c^2 = sqrt(a^2 + b^2)
    *   when a = vector from viewpoint to origin...          from (0, 0, zTrans) to (0, 0, 0)
    *           b = vector from origin to top of XY plane...    from (0, 0, 0)      to (0, XYheight/2, 0)
    *          c = vector from viewpoint to top of XY plane... from (0, 0, zTrans) to (0, XYheight/2, 0)
    *
    * So,   a = zTrans
    *       b = XYheight / 2;
    * and   c = sqrt(a^2 + b^2)
    *
    * We also know from the generalized law of cosines that f^2 = d^2 + e^2 - 2*d*e*Cos(theta)
    * when d = the vector from the viewpoint to the top of the XY plane,
    *     therefore, d = e = c (from above).
    *
    *       f = total height of the XY plane ->  f = XYheight
    *    and theta is the field of view (we assume 45 degrees).
    *
    *   So,     f^2 = 2*c^2 * (1 - cos(theta)) = 2*c^2 * (1 - 0.707) = 0.5858*c^2
    *
    *   that means f = 0.7654 * c
    *
    *   or XYheight = 0.7654 * sqrt(zTrans^2 + XYheight^2 / 4)
    *
    *   XYheight^2 = 0.5858 * zTrans^2 + 0.1464 * XYheight^2
    *
    *   0.8536 * XYheight^2 = 0.5858 * zTrans^2
    *
    *   XYheight = 0.8285 * zTrans
    ********************************************************************************************************/

    XYheight = -0.8285 * zTrans;

    // The width can be found from the aspect ratio of the window.
    XYwidth = XYheight * (windowWidth / windowHeight);

    // Now that we have the dimensions of the XY plane, we need to find where on the screen was clicked, and then map those coordinates to the XYplane.
    screenX = float(finalPos.x()) / windowWidth;
    screenY = float(finalPos.y()) / windowHeight;

    // Find the model coordinates in X.
    // screenX runs from [0, 1] while the model coordinate in plane run from [-XYwidth / 2, XYwidth / 2]
    pos[0] = -XYwidth/2 + (screenX * XYwidth);

    // Same for Y.
    pos[1] = -XYheight/2 + (screenY * XYheight);

    // Since this is placed on the XY Plane, the z component of this point's location is zero.
    pos[2] = 0;


    // Okay, so now we have the point placed on the XY Plane, but if the User has shifted the view via translation or rotation, the Build Atom Tool's on-screen
    // clicks will not correspond to the proper model coordinates.  We fix that here.

    // Start with translations.
    pos[0] -= xTrans;
    pos[1] -= yTrans;


    /***********************************************************************************************************
    * And now implement rotation...  we make use of Rodrigues' Formula:
    *
    * Vrot = V*cos(theta) + (V cross W)*sin(theta) + W*(W dot V)*(1 - cos(theta))
    *
    * Where Vrot = final 3D coordinates of our atom
    *       V = starting 3D coordinates of our atom (after translation)
    *       W = unit vector representing the axis of rotation.
    *       theta = angle of rotation about the axis, W.
    *
    *   In our case both W and theta are given via a quaternion that keeps track of the User's rotations for the main
    *   view.  The axis of rotation is given by the quaternion's first three elements, and the angle of rotation is
    *   given by the fourth (actually stored as Cos( theta / 2) ).
    **************************************************************************************************************/

    axis[0] = rotation.x();
    axis[1] = rotation.y();
    axis[2] = rotation.z();
    theta = 2 * qAcos(rotation.scalar());

    // Ensure that the axis of rotation is a unit vector.
    float mag = qSqrt(axis[0] * axis[0] + axis[1] * axis[1] + axis[2] * axis[2]);

    if (mag > 0)
    {
        axis[0] /= mag;
        axis[1] /= mag;
        axis[2] /= mag;

        // Dot Product of Position Vector and Axis of Rotation Vector
        dot = pos[0] * axis[0] + pos[1] * axis[1] + pos[2] * axis[2];

        // Cross Product of Position Vector and Axis of Rotation Vector
        cross[0] = pos[1] * axis[2] - pos[2] * axis[1];
        cross[1] = pos[2] * axis[0] - pos[0] * axis[2];
        cross[2] = pos[0] * axis[1] - pos[1] * axis[0];

        // Now we use Rodrigues' Formula
        pos[0] = pos[0] * qCos(theta) + cross[0] * qSin(theta) + axis[0] * dot * (1 - qCos(theta));
        pos[1] = pos[1] * qCos(theta) + cross[1] * qSin(theta) + axis[1] * dot * (1 - qCos(theta));
        pos[2] = pos[2] * qCos(theta) + cross[2] * qSin(theta) + axis[2] * dot * (1 - qCos(theta));
    }


    // Now we load this final data into the prometheus model.
    addAtom(atomicNumber, pos);

    // And ask Prometheus to re-count the number of atoms and bonds it has.
    setNumberOfAtoms();

    if (!atomBuilderMoved)  // Only do this if this is the first action of the Atom Build Tool.
    {
        // Place this atom's ID in memory as the starting point for any bonds drawn, if the User activates the special functions of the Atom Build Tool.
        atomBuilderOrigin = atomPositionX.size();
        savedAtomListSize = atomPositionX.size();
        savedBondListSize = bondList.size();
        singleSaveAtom(atomBuilderOrigin);
    }
    else
    {
        // This is the second, special action of the Atom Build Tool...  the User is dragging an atom bonded to an existing one.
        // Not only do we need to place a new atom in empty space, we must create a bond between the new atom and the existing atom it's attached to.

        singleRecoverAtom();

        int atomID = atomPositionX.size();

        QVector<int> bond;
        bond.resize(2);

        bond[0] = atomBuilderOrigin;
        bond[1] = atomID;

        // Add the bond to the Atlas Framework.
        bondList.push_back(bond);
        bondOrder.push_back(1);
    }
}




void Atlas::purgeLastRecord(int atomID)
{
    // This function is to be used in conjuntion with the Atom Build Tool.  When the User drags a new atom that is bonded to an existing one, we want to
    // provide the User with position feedback, but we do not want to permanently add this atom.  We delete the atom between frames before we add another in
    // the next position.

    if (atomID == -1)
    {
        // In this case, the User has selected to place an atom in empty space, so we must remove an atom record, in addition to a bond record.
        // We removed the one added most recently.


        if (atomPositionX.size() > savedAtomListSize)
        {
            atomAtomicNumber.pop_back();
            atomChainName.pop_back();
            atomForceFieldType.pop_back();
            atomMaxCovalentBonds.pop_back();
            atomNumberOfLonePairs.pop_back();
            atomName.pop_back();
            atomPartialCharge.pop_back();
            atomPositionX.pop_back();
            atomPositionY.pop_back();
            atomPositionZ.pop_back();
            atomResidueName.pop_back();
            atomResidueNumber.pop_back();
            atomVelocity.pop_back();

            setNumberOfAtoms();
        }

        if (bondList.size() > savedBondListSize)
        {
            bondList.pop_back();
            bondOrder.pop_back();

            setNumberOfBonds();
        }
    }
    else
    {
        // In this case, the User is attempting to edit existing atoms, and they may or may not have altered connectivity.
        if (bondList.size() != savedBondListSize)
        {
            bondList.pop_back();
            bondOrder.pop_back();
            setNumberOfBonds();
            singleRecoverAtom();
        }
    }

}


void Atlas::incrementBond()
{
    if (bondOrder[selectedBonds[0]] == 1)
    {
        bondOrder[selectedBonds[0]] = 2;
    }
    else if (bondOrder[selectedBonds[0]] == 2)
    {
        bondOrder[selectedBonds[0]] = 3;
    }
    else if (bondOrder[selectedBonds[0]] == 3)
    {
        bondOrder[selectedBonds[0]] = 1;
    }

    clearSelection();
}

void Atlas::decrementBond()
{
    if (bondOrder[selectedBonds[0]] == 1)
    {
        bondOrder[selectedBonds[0]] = 3;
    }
    else if (bondOrder[selectedBonds[0]] == 2)
    {
        bondOrder[selectedBonds[0]] = 1;
    }
    else if (bondOrder[selectedBonds[0]] == 3)
    {
        bondOrder[selectedBonds[0]] = 2;
    }

    clearSelection();
}
