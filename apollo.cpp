#include "apollo.h"

#include <qmath.h>
#include <QDebug>
#include <QMessageBox>


/*
**********************************************************************************************************
                                           Notes
**********************************************************************************************************
The purpose of the Apollo Class is to be a catch-all for functions analyzing chemical data.  There are a
variety of analyses and calulations to be done, including partial charge, entropy, and anything else we
need.  These calculations involve physical properties, statistics, and other hardcore calculations
that don't have to do with formatting or directly contributing to a graphics computation.
***********************************************************************************************************
*/


Apollo::Apollo(QObject *parent) :
  QObject(parent)
{
    setupAtomicData();

}

Apollo::~Apollo()
{

}


void Apollo::setupAtomicData()
{
    // Here we setup data needed to perform QEq calculations.

    // electronegatvitiy stores the electronegativity for a given atom.  This value is equal to
    // 0.5 * (Ionization Potential + Electron Affinity), both of which have been determined experimentally
    // for each element**.  These values are stored in units of eV.

    // selfCoulombPotential stores the columb repulsion between electrons in an atom's same orbital, a
    // value unique to each element.  This value is equal to the Ionization Potential - Electron Affinity,
    // both of which have been determined experimentally for each element**.  These values are stored in
    // units of eV.


    // **Because Electron Affinity values are not available for noble gasses and group II elements, QEq is not
    // effective in calculating their partial charges.

    // averageAtomSize is equivalent to 14.4/selfColumbPotential.  14.4 is a conversion factor allowing
    // these values to be stored in units of angstroms.

    // valenceOrbitalExponent is the exponent in the equation describing a given atom's orbital as a Slater
    // oribital.  The value of this exponent is a function of the atom's size.  (2*(principle quantum number) + 1) /
    // (2*averageAtomSize).

    // The QEq method makes some assumptions about a given atom's electronic structure.  Because hydrogen's single
    // electron is easily deformed during bonding, these assumptions have a limited effectiveness, so special calulations
    // must be done for every hydrogen in the model.  The values below apply only if H is neutral.

    electronegativity.resize(119);
    selfCoulombPotential.resize(119);
    averageAtomSize.resize(119);
    valenceOrbitalExponent.resize(119);
    applicableElements.resize(20);


    electronegativity[1] = 4.5280; // This value is only valid when H is neutral.
    electronegativity[3] = 3.006;
    electronegativity[6] = 5.343;
    electronegativity[7] = 6.899;
    electronegativity[8] = 8.741;
    electronegativity[9] = 10.874;

    selfCoulombPotential[1] = 4.5280;  // This value is only valid when H is neutral.
    selfCoulombPotential[3] = 4.772;
    selfCoulombPotential[6] = 10.126;
    selfCoulombPotential[7] = 11.760;
    selfCoulombPotential[8] = 13.364;
    selfCoulombPotential[9] = 14.948;

    averageAtomSize[1] = 0.371; // This value is only valid when H is neutral.
    averageAtomSize[3] = 1.557;
    averageAtomSize[6] = 0.759;
    averageAtomSize[7] = 0.715;
    averageAtomSize[8] = 0.669;
    averageAtomSize[9] = 0.706;

    valenceOrbitalExponent[1] = 1.0698; // This value is only valid when H is neutral.
    valenceOrbitalExponent[3] = 0.4174;
    valenceOrbitalExponent[6] = 0.8563;
    valenceOrbitalExponent[7] = 0.9089;
    valenceOrbitalExponent[8] = 0.9745;
    valenceOrbitalExponent[9] = 0.9206;

    applicableElements[0] = 1;
    applicableElements[1] = 3;
    applicableElements[2] = 6;
    applicableElements[3] = 7;
    applicableElements[4] = 8;
    applicableElements[5] = 9;
}

void Apollo::setNetCharge(double userInput)
{
    netCharge = userInput;
}

void Apollo::performQEq()
{
    /*
    // Charge Equilibration (QEq) is a powerful method that efficiently assigns partial charge values
    // to atoms within a molecule (Rappe and Goddard, 1991).  Its accuracy rivals quantum methods, but uses
    // only experimental atomic data and classical MD geometry to arrive at its answers.  QEq is limited to main group
    // elements for which electron affinity data is available (excludes Group II and Noble Gasses).

    // Using values for electron affinity, ionization potential, and orbital overlap, we setup a system of
    // equations and solve simultaenously for the atomic partial charges.  We make use of Gaussian elimination, which is vulnerable when it comes
    // to numerical stability.  We use double precision to minimize round off errors.  (This algorithm will likely need to be improved with
    // partial pivoting later... it's on the To-Do List).

    // The system is setup as A*q = c.

    *************************************************
     *  Matrix A
     ***********************************************
    // A is an N by N matrix holding coefficients belonging to variables in q.  Each row in A represents an equation relating the partial charges.
    // The first is q_1 + ... + q_N = Q_Total, so the first row of A is filled with 1 in each column.

    // After that, the equations are pulled a description of chemical potential (elaborated on in Rappe and Goddard, 1991).  The basic pattern is for
    // a given row, i, and a given column, j, the matrix element is:
    //   J_ij - J_1j

    // J is an overlap integral.  In this implementation, it is an overlap integral of Slater type s-orbitals, which we calculate below using average atom size
    // and the orbital expoonent stored for each element.

    // J_ij = J_ji, so we can reduce the number of times we calculate the intgeral, and any time i = j (for the first term) or j = 1 (for the second), J becomes
    // a self-overlap integral between electrons within the same orbital.
    // These values are stored and based on experimental data (Ionization Potential - Electron Affinity)


    *************************************************
     *  Matrix q
     ***********************************************
    // q is a column vector containing variables holding the partial charges we seek (q1, ... , qN).


    *************************************************
     *  Matrix c
     ***********************************************
    // c is a column vector containing constants from the aformentioned equations.
    // For the first row, the value is total charge, Q_total, which is set by the user.

    // After that, the constants come from the equation for chemical potential.  For a given row, i, the matrix element is:
    //   x_i - x_1

    // x is electronegativity.  Here it is defined based on experiemental data (0.5 * (Ionization Potential + Electron Affinity)), and it has been stored for each element.

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/

    QVector<QVector<double> > coeffMatrix; // This is the coefficient matrix A.
    QVector<double> partialCharges; // This is the constant matrix c, but at the end of the calculation, it will hold the solution to the system of equations.
    QVector<double> oldPartialCharges; // This holds onto charges from a previous iteration... only used for hydrogens (see below).

    // First we allocate memory for this problem.  This is complicated by the fact that not all elements can be included in the QEq calculation.
    // Only atoms with available electron affinity data can be used. If the atoms cannot be used, it is just skipped, and its partial charge remains 0.0.

    // Pull element data for all atoms
    QVector<int> allAtoms = prometheusModel.getAtomAtomicNumber();
    QVector<float> allPosX = prometheusModel.getAtomPositionX();
    QVector<float> allPosY = prometheusModel.getAtomPositionY();
    QVector<float> allPosZ = prometheusModel.getAtomPositionZ();

    // Setup arrays to hold onto atoms that QEq can use.
    QVector<int> Atoms;
    QVector<float> PosX, PosY, PosZ;
    bool hydrogenDetected = false;
    bool successfulIteration = true;

    for (int i = 0; i < prometheusModel.getNumberOfAtoms(); i++)
    {
        if (applicableElements.contains(allAtoms[i]))
        {
            Atoms.push_back(allAtoms[i]);
            PosX.push_back(allPosX[i]);
            PosY.push_back(allPosY[i]);
            PosZ.push_back(allPosZ[i]);

            if (allAtoms[i] == 1)
            {
                hydrogenDetected = true;
            }
        }

    }

    // Allocate memory for this problem.
    coeffMatrix.resize(Atoms.size());
    for (int i = 0; i < Atoms.size(); i++)
    {
        coeffMatrix[i].resize(Atoms.size());
    }

    partialCharges.resize(Atoms.size());
    oldPartialCharges.resize(Atoms.size());



    // Now, we formally setup our problem...  we must construct the system of equations we aim to solve for partial charges.
    setupSystem(Atoms, PosX, PosY, PosZ, coeffMatrix, partialCharges, netCharge, oldPartialCharges);

    // Now the problem is setup, and we're finally ready to begin solving it.
    solveSystem(coeffMatrix, partialCharges);

    // But wait, we're not done!  As it turns out, the values we use in coeffMatrix for any hydrogen atom are dependent on partial charge.
    // Since the intial values depend on the final answer, we cannot solve directly.  We must iterate...  altering the values of coeffMatrix
    // when we get an update to the partial charges and iterating until we achieve convergance.  So, if there are hydrogens in our system, we need
    // to use the following function.

    if (hydrogenDetected)
    {
        successfulIteration = iterateHydrogens(Atoms, PosX, PosY, PosZ, coeffMatrix, partialCharges, netCharge, oldPartialCharges);
    }

    if (successfulIteration)
    {
        // Now, we have the final partial charges... store them in the Atlas Framework.

        int count = 0;
        double finalNetCharge = 0;

        for (int i = 0; i < prometheusModel.getNumberOfAtoms(); i++)
        {
            if (applicableElements.contains(allAtoms[i]))
            {
                prometheusModel.setAtomPartialCharge(i + 1, partialCharges[count]);
                finalNetCharge += partialCharges[count];
                count++;
            }
            else
            {
                prometheusModel.setAtomPartialCharge(i + 1, 0);
            }
        }

        QString temp;
        QString message1 = "Partial Charge Assignment Complete:                                                     ";
        QString message2 = QString("Partial charges have been assigned to all applicable atoms according to the Charge Equilibration (QEq) Method.\n\nThe current net charge of this model is %1").arg(finalNetCharge, 5, 'f');

        emit calculationFinished(message1, message2);
    }
}


double Apollo::findFudgeyCoulombIntegral(QVector<float> pos1, QVector<float> pos2, int atomicNumber_i, int atomicNumber_j, float correction1, float correction2)
{
    /**************************************************************************************************************************************************************************
    // This function takes two atoms and estimates the total Coulomb repulsion integral between all their electrons.  Rappe and Goddard modeled a given atom's ns
    // np or nd valence electrons with a single ns Slater orbital per atom.  The size of this orbital is based on the average size of the atom.

    // What should be done (to do list)...  Using the single s Slater orbital on each atom passed this function, we calculate the Coulomb repulsion integral and pass back out
    // the energy between atoms in eV.

    // What REALLY happens in this function...  Since even s Slater orbitals pose "non-trivial" problems when it comes to determining the Coulomb integral, we employ some
    // tricks here to get reasonable answers until the proper integral can be placed in another function.  Julius Su's thesis contains an analytical solution to the Gaussian
    // two-electron Coulomb integral.  Its use of the Error Function is tied to the Gaussian form, but the overall shape is what we want.

    // (14.4 / x) * Erf[ 0.585 * x / (Sqrt[A^2 + B^2]) ]

    // Where x is seperation distance between the two atoms.
    //       A is the average size of atom1 in angstrom.
    //       B is the average size of atom2 in angstrom.

    // 14.4 is conversion factor that outputs the final answer in eV.    the 1/x behavior dominates a large x.
    // At short distance, the Error Function dominates.  Inside the error function, we sorta take an average of the atoms' sizes and use that to divide the
    // seperation distance...   0.585 is a fudge factor I pulled out of my ass that makes this work...
    // it makes the C-C Coulomb interaction come out to the one in the paper.

    // All other mixing parameters or even worrying about an atom's quantum number is taken care of with the average of the atom's average size...
    // those sizes are taken from the Rappe / Goddard paper and are based on Slater orbital sizes.
    **************************************************************************************************************************************************************************/

    float distance, x, y, z, size1, size2, self1, self2;
    double energy;

    // First thing's first....  calculate the distance between these atoms.
    x = pos1[0] - pos2[0];
    y = pos1[1] - pos2[1];
    z = pos1[2] - pos2[2];

    distance = qSqrt(x*x + y*y + z*z);

    if (distance > 0.00001)
    {
        // Now, we look up the average size of each atom.  These values have been stored for each element.  In the case of hydrogen, we must adjust this size based on the hydrogen's
        // charge.
        // valenceOrbitalExponent_new = valenceOrbitalExponent_neutral + Charge
        // averageAtomSize[A] = 0.5*(2*n + 1) / (2*valenceOrbitalExponent[au])  * 0.529[A/au]  => 0.39675 contains all the constants.

        if (atomicNumber_i == 1)
        {
            size1 = 0.39675 / (valenceOrbitalExponent[1] + correction1);
        }
        else
        {
            size1 = averageAtomSize[atomicNumber_i];
        }

        if (atomicNumber_j == 1)
        {
            size2 = 0.39675 / (valenceOrbitalExponent[1] + correction2);
        }
        else
        {
            size2 = averageAtomSize[atomicNumber_j];
        }

        // Now we calculate the Coulomb integral!
        //energy = (14.4 / distance) * erf(0.585 * distance / qSqrt(size1*size1 + size2*size2));
        energy = (14.4 / distance) * erf(0.666667 * distance / qSqrt(size1*size1 + size2*size2));
    }
    else
    {
        // This IF Statement prevents this function from blowing up if the atoms happen to be on top of one another.
        if (atomicNumber_i == 1)
        {
            // The self Coulomb Interaction needs to be modified when hydrogen carries a charge.
            self1 = selfCoulombPotential[1] * (1 + (correction1 / valenceOrbitalExponent[1]));
        }
        else
        {
            self1 = selfCoulombPotential[atomicNumber_i];
        }

        if (atomicNumber_j == 1)
        {
            // The self Coulomb Interaction needs to be modified when hydrogen carries a charge.
            self2 = selfCoulombPotential[1] * (1 + (correction2 / valenceOrbitalExponent[1]));
        }
        else
        {
            self2 = selfCoulombPotential[atomicNumber_j];
        }

        energy = qSqrt(self1 * self2);
    }

    // We're done!
    return energy;
}

void Apollo::setupSystem (QVector<int> &Atoms, QVector<float> &PosX, QVector<float> &PosY, QVector<float> &PosZ, QVector<QVector<double> > &coeffMatrix, QVector<double> &partialCharges, double netCharge, QVector<double> &oldPartialCharges)
{
    // This function contains all the efforts needed to setup the system of equations outlined the the Rappe / Goddard paper.  The relevant matricies have been
    // passed to this function by reference, so there is nothing to return when we are done.

    float correction1, correction2;
    double energy;

    QVector<float> pos1;
    pos1.resize(3);
    QVector<float> pos2;
    pos2.resize(3);


    // Fill in values...  we can start with the easy values, the top row of coeffMatrix should be filled with 1.
    for (int i = 0; i < coeffMatrix.size(); i++)
    {
        coeffMatrix[0][i] = 1;
    }

    // From here we perform the Coulomb integrals...  since J_ij = J_ji (see above), we can utilize one calculation two places within coeffMatrix.
    // We skip the first atom to focus on it later.

    for  (int i = 1; i < coeffMatrix.size(); i++)
    {
        pos1[0] = PosX[i];
        pos1[1] = PosY[i];
        pos1[2] = PosZ[i];

        // If this atom is a hydrogen, we will need to adjust its average atomic radius according to its partial charge.
        if (Atoms[i] == 1)
        {
            correction1 = oldPartialCharges[i];
        }
        else
        {
            correction1 = 0;
        }

        for (int j = 1; j <= i; j++)
        {
            if (i != j)
            {
                pos2[0] = PosX[j];
                pos2[1] = PosY[j];
                pos2[2] = PosZ[j];

                // If this atom is a hydrogen, we will need to adjust its average atomic radius according to its partial charge.
                if (Atoms[j] == 1)
                {
                    correction2 = oldPartialCharges[j];
                }
                else
                {
                    correction2 = 0;
                }

                energy = findFudgeyCoulombIntegral(pos1, pos2, Atoms[i], Atoms[j], correction1, correction2);

                // Place the value in two spots within the coeffMatrix.
                coeffMatrix[i][j] = energy;
                coeffMatrix[j][i] = energy;
            }
            else
            {
                // In the case that i = j, the Coulomb interaction energy can be pulled from the stored self-Coulomb Potential.
                // Also, this value will only be placed in one location within the coeffMatrix.

                if (Atoms[i] == 1)
                {
                    coeffMatrix[i][j] = selfCoulombPotential[1] * (1 + (oldPartialCharges[i] / valenceOrbitalExponent[1]));
                }
                else
                {
                    coeffMatrix[i][j] = selfCoulombPotential[Atoms[i]];
                }
            }
        }
    }

    // Now, we come back to the first atom.  Its interactions are included in every term within the coeffMatrix.  The energy value that gets returned for an interaction between
    // the first atom and atom i is subtracted from every entry in column i.

    pos1[0] = PosX[0];
    pos1[1] = PosY[0];
    pos1[2] = PosZ[0];

    if (Atoms[0] == 1)
    {
        // This atom is a hydrogen, and its charge must be considered.
        correction1 = oldPartialCharges[0];
    }
    else
    {
        correction1 = 0;
    }

    for (int i = 0; i < coeffMatrix.size(); i++)
    {
        if (i != 0)
        {
            // We get the energy that we will apply to all values in this column.
            pos2[0] = PosX[i];
            pos2[1] = PosY[i];
            pos2[2] = PosZ[i];

            if (Atoms[i] == 1)
            {
                // This atom is a hydrogen, and its charge must be considered.
                correction2 = oldPartialCharges[i];
            }
            else
            {
                correction2 = 0;
            }

            energy = findFudgeyCoulombIntegral(pos1, pos2, Atoms[0], Atoms[i], correction1, correction2);
        }

        for (int j = 1; j < coeffMatrix.size(); j++)
        {
            if (i == 0)
            {
                // This interaction is pulled directly from the stored self-Coulomb Potential.
                if (Atoms[0] == 1)
                {
                    // This atom is a hydrogen, so its self-Coulomb Potential must be modified according to its charge.
                    coeffMatrix[i][j] -= selfCoulombPotential[1] * (1 + (oldPartialCharges[i] / valenceOrbitalExponent[1]));
                }
                else
                {
                    coeffMatrix[j][i] -= selfCoulombPotential[Atoms[i]];
                }
            }
            else
            {
                // This interaction is the value calculated using our Coulomb Integral funtion.
                coeffMatrix[j][i] -= energy;
            }
        }
    }

    // Now we are done filling in the coeffMatrix (matrix A).  We move on to fill in the constant value matrix.  Even though the values we fill in here are the constants from the
    // system of equations, by the end of the solution, this matrix will contain the desired partial charges.  Therefore, it is named here as partialCharges.

    // The first equation states that the sum of partial charges equals the net charge.  So the first value in this matrix is the desired net charge.
    partialCharges[0] = netCharge;

    // From here, we fill in values according to X_1 - X_i  (Rappe/Goddard says X_i - X_1, but the sign turns out wrong for me).
    // where X is electronegativity for a given element

    for (int i = 1; i < coeffMatrix.size(); i++)
    {
        partialCharges[i] = electronegativity[Atoms[0]] - electronegativity[Atoms[i]];
    }

    // And now our system is complete!
}

void Apollo::solveSystem(QVector<QVector<double> > &coeffMatrix, QVector<double> &partialCharges)
{
    // This function takes the coefficient matrix and the column vector that will eventually hold the partial charges we seek, and solves the system of equations they represent.
    // We employ Gaussian Elimination, followed up with backwards substition to get our results.  The relevant matricies have been passed by reference to this function,
    // so there is nothing to pass back when we are done.

    double multiplier;

    // We know that the first row of coeffMatrix is composed entirely of 1's, so that row is "solved".  Start with the second row.
    for (int i = 1; i < coeffMatrix.size(); i++)
    {
        // i is the row index we are processing.

        for (int j = 0; j < i; j++)
        {
            // j is the column index we are processing.  We need to make all values to the left of the diagonal (j = i) element 0.
            // To do this, we use the value at the [ith][jth] position, change its sign, and use it as a multiplier on the jth row.  The previously "solved" equations
            // have a 1 in the [jth][jth] position, so (1 * multiplier + the [ith][jth] element) = 0... which zeroes out thh [ith][jth] element, as desired.

            multiplier = -coeffMatrix[i][j];

            // Now we use this multiplier on the jth row and add the jth row to the ith row....  including the ith element of partialCharges.
            for (int k = 0; k < coeffMatrix.size(); k++)
            {
                coeffMatrix[i][k] += coeffMatrix[j][k] * multiplier;
            }

            partialCharges[i] += partialCharges[j] * multiplier;

        }

        // Now that all elements to the left of diagonal are zeroed out, we need to make the diagonal element on this row 1.  To accomplish this, divide row i
        // by the value in the [ith][ith] element...  don't forget to do the same to the ith element of partialCharges.

        multiplier = coeffMatrix[i][i];

        for (int j = i; j < coeffMatrix.size(); j++)
        {
            coeffMatrix[i][j] /= multiplier;
        }

        partialCharges[i] /= multiplier;
    }

    // Now coeffMatrix has been successfully converted to an upper triangluar matrix.  From here, we employ backwards-substitution to get our final answers.
    // Starting from the bottom of the matrix, we solve each equation, and get a final answer for a given partial charge, one at a time.
    for (int i = coeffMatrix.size() - 2; i >= 0; i--)
    {
        for (int j = i + 1; j < coeffMatrix.size(); j++)
        {
            partialCharges[i] -= coeffMatrix[i][j] * partialCharges[j];
        }
    }
}


bool Apollo::iterateHydrogens(QVector<int> &Atoms, QVector<float> &PosX, QVector<float> &PosY, QVector<float> &PosZ, QVector<QVector<double> > &coeffMatrix, QVector<double> &partialCharges, double netCharge, QVector<double> &oldPartialCharges)
{
    // We have a problem...  when performing a QEq calculation, the equations we use are completely dependent on getting reasonably accurate coulomb integral interactions between atoms.
    // The approximation we use is a spherical orbital whose size is based on the average size of the atom.  In the case of atoms with multiple electrons, that assumption is reasonable
    // after bonding, but that is not the case with hydrogen.  Hydrogen's single electron is distorted by bonding, so we need to re-evaluate it's size, and therefore its interaction
    // contribution in coeffMatrix.

    // Now, here is the problem...  Goddard/Rappe show that the Hydrogen atom's size can be expressed as a function of partial charge, which means
    // our initial system is dependent of the final answer.  We cannot solve directly, but the good news is that we can iterate, using the charges we get to alter the Hydrogen's size
    // and get more charges.  We continue to iterate until we achieve convergance.  Goddard/Rappe report consistently getting convergence after 6 to 10 iterations.

    int count = 0;
    double divergance = 1;

    while (divergance > 0.0001)
    {
        if (count > 50)
        {
           // There is something wrong and this system's partial charges will not converge.  Report this answer to the User.
            QString message1 = "ERROR:  Partial Charge Assignment Failure:                                 ";
            QString message2 = "The calculation to determine this model's partial charges failed to converge.";

            emit calculationFinished(message1, message2);
            return false;
        }

        // This function will be activated after the system of equations have been solved once, so we will use that answer to start our loop.
        // Take the partial charges we found in the most recent cycle, and store them to affect the next cycle.
        for (int i = 0; i < Atoms.size(); i++)
        {
            oldPartialCharges[i] = partialCharges[i];
        }

        // Now, re-calculate the system's partial charges.  The hydrogen partial charges we stored will affect the outcome.

        setupSystem(Atoms, PosX, PosY, PosZ, coeffMatrix, partialCharges, netCharge, oldPartialCharges);
        solveSystem(coeffMatrix, partialCharges);

        // Now we measure the difference between the current hydrogen charges and the ones from the last cycle.  If they are above a certain threshold, we should iterate again
        // to see if we can get closer to convergance.

        divergance = 0;

        for (int i = 0; i < Atoms.size(); i++)
        {
            if (Atoms[i] == 1)
            {
                divergance += qAbs(partialCharges[i] - oldPartialCharges[i]);
            }
        }

        count++;
    }

    return true;
}
