#include "pandora.h"
#include "atlas.h"


#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <QString>
#include <QStringList>
#include <QMessageBox>
#include <qmath.h>
#include <QDateTime>
#include <QDebug>
/*
**********************************************************************************************************
                                           Notes
**********************************************************************************************************
The purpose of the Pandora Class is to be a polyglot of chemical data. There are numerous formats for data files
that include coordinates, charge, velocity of atoms, unit cell dimensions, etc...  and that is just for Molecular Dynamics.
Prometheus includes an interface of advanced eFF and Quantum methods, which requires
specialized treatments for electrons and density functionals.

Pandora runs the gamut of file types by housing the processing wrappers within its functions, and then delivering
those inputs (outputs) to (from) the Atlas framework.
Each wrapper function is independent, so this class is easily extended and maintained.
***********************************************************************************************************
*/

extern Atlas prometheusModel;

Pandora::Pandora(QObject *parent) :
    QObject(parent)
{
    setupLookupTables();
    activeModel = 0;
}

// Empty Destructor

Pandora::~Pandora() {}

void Pandora::setupLookupTables()
{
    // Setup the atomic number lookup table for data manipulation.
    elementLookup.resize(119);
    elementLookup[0] = "Ez"; // Element Zero...  It's a space holder for this array and an opportunity to honor Mass Effect.
    elementLookup[1] = "H";
    elementLookup[2] = "He";
    elementLookup[3] = "Li";
    elementLookup[4] = "Be";
    elementLookup[5] = "B";
    elementLookup[6] = "C";
    elementLookup[7] = "N";
    elementLookup[8] = "O";
    elementLookup[9] = "F";
    elementLookup[10] = "Ne";
    elementLookup[11] = "Na";
    elementLookup[12] = "Mg";
    elementLookup[13] = "Al";
    elementLookup[14] = "Si";
    elementLookup[15] = "P";
    elementLookup[16] = "S";
    elementLookup[17] = "Cl";
    elementLookup[18] = "Ar";
    elementLookup[19] = "K";
    elementLookup[20] = "Ca";
    elementLookup[21] = "Ti";
    elementLookup[22] = "Sc";
    elementLookup[23] = "V";
    elementLookup[24] = "Cr";
    elementLookup[25] = "Mn";
    elementLookup[26] = "Fe";
    elementLookup[27] = "Co";
    elementLookup[28] = "Ni";
    elementLookup[29] = "Cu";
    elementLookup[30] = "Zn";
    elementLookup[31] = "Ga";
    elementLookup[32] = "Ge";
    elementLookup[33] = "As";
    elementLookup[34] = "Se";
    elementLookup[35] = "Br";
    elementLookup[36] = "Kr";
    elementLookup[37] = "Rb";
    elementLookup[38] = "Sr";
    elementLookup[39] = "Y";
    elementLookup[40] = "Zr";
    elementLookup[41] = "Nb";
    elementLookup[42] = "Mo";
    elementLookup[43] = "Tc";
    elementLookup[44] = "Ru";
    elementLookup[45] = "Rh";
    elementLookup[46] = "Pd";
    elementLookup[47] = "Ag";
    elementLookup[48] = "Cd";
    elementLookup[49] = "In";
    elementLookup[50] = "Sn";
    elementLookup[51] = "Sb";
    elementLookup[52] = "Te";
    elementLookup[53] = "I";
    elementLookup[54] = "Xe";
    elementLookup[55] = "Cs";
    elementLookup[56] = "Ba";
    elementLookup[57] = "La";
    elementLookup[58] = "Ce";
    elementLookup[59] = "Pr";
    elementLookup[60] = "Nd";
    elementLookup[61] = "Pm";
    elementLookup[62] = "Sm";
    elementLookup[63] = "Eu";
    elementLookup[64] = "Gd";
    elementLookup[65] = "Tb";
    elementLookup[66] = "Dy";
    elementLookup[67] = "Ho";
    elementLookup[68] = "Er";
    elementLookup[69] = "Tm";
    elementLookup[70] = "Yb";
    elementLookup[71] = "Lu";
    elementLookup[72] = "Hf";
    elementLookup[73] = "Ta";
    elementLookup[74] = "W";
    elementLookup[75] = "Re";
    elementLookup[76] = "Os";
    elementLookup[77] = "Ir";
    elementLookup[78] = "Pt";
    elementLookup[79] = "Au";
    elementLookup[80] = "Hg";
    elementLookup[81] = "Tl";
    elementLookup[82] = "Pb";
    elementLookup[83] = "Bi";
    elementLookup[84] = "Po";
    elementLookup[85] = "At";
    elementLookup[86] = "Rn";
    elementLookup[87] = "Fr";
    elementLookup[88] = "Ra";
    elementLookup[89] = "Ac";
    elementLookup[90] = "Th";
    elementLookup[91] = "Pa";
    elementLookup[92] = "U";
    elementLookup[93] = "Np";
    elementLookup[94] = "Pu";
    elementLookup[95] = "Am";
    elementLookup[96] = "Cm";
    elementLookup[97] = "Bk";
    elementLookup[98] = "Cf";
    elementLookup[99] = "Es";
    elementLookup[100] = "Fm";
    elementLookup[101] = "Md";
    elementLookup[102] = "No";
    elementLookup[103] = "Lr";
    elementLookup[104] = "Rf";
    elementLookup[105] = "Db";
    elementLookup[106] = "Sg";
    elementLookup[107] = "Bh";
    elementLookup[108] = "Hs";
    elementLookup[109] = "Mt";
    elementLookup[110] = "Ds";
    elementLookup[111] = "Rg";
    elementLookup[112] = "Cn";
    elementLookup[113] = "Uut";
    elementLookup[114] = "Fl";
    elementLookup[115] = "Uup";
    elementLookup[116] = "Lv";
    elementLookup[117] = "Uus";
    elementLookup[118] = "Uuo";


    // Setup the van der Waals radius lookup table for data manipulation.  Atomic Radii are given in Angstroms.
    vanderWaalsLookup.resize(119);
    vanderWaalsLookup[0] = 0.5;
    vanderWaalsLookup[1] = 1.1;
    vanderWaalsLookup[2] = 1.4;
    vanderWaalsLookup[3] = 1.81;
    vanderWaalsLookup[4] = 1.53;
    vanderWaalsLookup[5] = 1.92;
    vanderWaalsLookup[6] = 1.70;
    vanderWaalsLookup[7] = 1.55;
    vanderWaalsLookup[8] = 1.52;
    vanderWaalsLookup[9] = 1.47;
    vanderWaalsLookup[10] = 1.54;
    vanderWaalsLookup[11] = 2.27;
    vanderWaalsLookup[12] = 1.73;
    vanderWaalsLookup[13] = 1.84;
    vanderWaalsLookup[14] = 2.10;
    vanderWaalsLookup[15] = 1.80;
    vanderWaalsLookup[16] = 1.80;
    vanderWaalsLookup[17] = 1.75;
    vanderWaalsLookup[18] = 1.88;
    vanderWaalsLookup[19] = 2.75;
    vanderWaalsLookup[20] = 2.31;
    vanderWaalsLookup[21] = 2.16;
    vanderWaalsLookup[22] = 1.87;
    vanderWaalsLookup[23] = 1.79;
    vanderWaalsLookup[24] = 1.89;
    vanderWaalsLookup[25] = 1.97;
    vanderWaalsLookup[26] = 1.94;
    vanderWaalsLookup[27] = 1.92;
    vanderWaalsLookup[28] = 1.84;
    vanderWaalsLookup[29] = 1.86;
    vanderWaalsLookup[30] = 2.10;
    vanderWaalsLookup[31] = 1.87;
    vanderWaalsLookup[32] = 2.11;
    vanderWaalsLookup[33] = 1.85;
    vanderWaalsLookup[34] = 1.90;
    vanderWaalsLookup[35] = 1.83;
    vanderWaalsLookup[36] = 2.02;
    vanderWaalsLookup[37] = 3.03;
    vanderWaalsLookup[38] = 2.49;
    vanderWaalsLookup[39] = 2.19;
    vanderWaalsLookup[40] = 1.86;
    vanderWaalsLookup[41] = 2.07;
    vanderWaalsLookup[42] = 2.09;
    vanderWaalsLookup[43] = 2.09;
    vanderWaalsLookup[44] = 2.07;
    vanderWaalsLookup[45] = 1.95;
    vanderWaalsLookup[46] = 2.02;
    vanderWaalsLookup[47] = 2.03;
    vanderWaalsLookup[48] = 2.30;
    vanderWaalsLookup[49] = 1.93;
    vanderWaalsLookup[50] = 2.17;
    vanderWaalsLookup[51] = 2.06;
    vanderWaalsLookup[52] = 2.06;
    vanderWaalsLookup[53] = 1.98;
    vanderWaalsLookup[54] = 2.16;
    vanderWaalsLookup[55] = 3.43;
    vanderWaalsLookup[56] = 2.68;
    vanderWaalsLookup[57] = 2.40;
    vanderWaalsLookup[58] = 2.35;
    vanderWaalsLookup[59] = 2.39;
    vanderWaalsLookup[60] = 2.29;
    vanderWaalsLookup[61] = 2.36;
    vanderWaalsLookup[62] = 2.29;
    vanderWaalsLookup[63] = 2.33;
    vanderWaalsLookup[64] = 2.37;
    vanderWaalsLookup[65] = 2.21;
    vanderWaalsLookup[66] = 2.29;
    vanderWaalsLookup[67] = 2.16;
    vanderWaalsLookup[68] = 2.35;
    vanderWaalsLookup[69] = 2.27;
    vanderWaalsLookup[70] = 2.42;
    vanderWaalsLookup[71] = 2.21;
    vanderWaalsLookup[72] = 2.12;
    vanderWaalsLookup[73] = 2.17;
    vanderWaalsLookup[74] = 2.10;
    vanderWaalsLookup[75] = 2.17;
    vanderWaalsLookup[76] = 2.16;
    vanderWaalsLookup[77] = 2.02;
    vanderWaalsLookup[78] = 2.09;
    vanderWaalsLookup[79] = 2.17;
    vanderWaalsLookup[80] = 2.09;
    vanderWaalsLookup[81] = 1.96;
    vanderWaalsLookup[82] = 2.02;
    vanderWaalsLookup[83] = 2.07;
    vanderWaalsLookup[84] = 1.97;
    vanderWaalsLookup[85] = 2.02;
    vanderWaalsLookup[86] = 2.20;
    vanderWaalsLookup[87] = 3.48;
    vanderWaalsLookup[88] = 2.83;
    vanderWaalsLookup[89] = 2.60;
    vanderWaalsLookup[90] = 2.37;
    vanderWaalsLookup[91] = 2.43;
    vanderWaalsLookup[92] = 2.40;
    vanderWaalsLookup[93] = 2.21;
    vanderWaalsLookup[94] = 2.43;
    vanderWaalsLookup[95] = 2.44;
    vanderWaalsLookup[96] = 2.45;
    vanderWaalsLookup[97] = 2.44;
    vanderWaalsLookup[98] = 2.45;
    vanderWaalsLookup[99] = 2.45;
    vanderWaalsLookup[100] = 2.45;
    vanderWaalsLookup[101] = 2.46;
    vanderWaalsLookup[102] = 2.46;
    vanderWaalsLookup[103] = 2.46;

    // Data for elements beyond this are not really available...   the value of 2.45 A is a somewhat reasonable placeholder.
    vanderWaalsLookup[104] = 2.45;
    vanderWaalsLookup[105] = 2.45;
    vanderWaalsLookup[106] = 2.45;
    vanderWaalsLookup[107] = 2.45;
    vanderWaalsLookup[108] = 2.45;
    vanderWaalsLookup[109] = 2.45;
    vanderWaalsLookup[110] = 2.45;
    vanderWaalsLookup[111] = 2.45;
    vanderWaalsLookup[112] = 2.45;
    vanderWaalsLookup[113] = 2.45;
    vanderWaalsLookup[114] = 2.45;
    vanderWaalsLookup[115] = 2.45;
    vanderWaalsLookup[116] = 2.45;
    vanderWaalsLookup[117] = 2.45;
    vanderWaalsLookup[118] = 2.45;

    // Setup the color lookup table for data manipulation. (CPK Color Scheme).  Colors given here correspond to a RGB color vector with each element having a
    // range of [0,1].
    colorLookup.resize(119);
    for (int i = 0; i < 119; i++)
    {
        colorLookup[i].resize(3);
    }

    // CPK Coloring...  Colors beyond the original come from Jmol.


    colorLookup[0][0] = 77;     colorLookup[0][1] = 234;    colorLookup[0][2] = 255;

    colorLookup[1][0] = 255;    colorLookup[1][1] = 255;    colorLookup[1][2] = 255;
    colorLookup[2][0] = 217;    colorLookup[2][1] = 255;    colorLookup[2][2] = 255;
    colorLookup[3][0] = 204;    colorLookup[3][1] = 128;    colorLookup[3][2] = 255;
    colorLookup[4][0] = 194;    colorLookup[4][1] = 255;    colorLookup[4][2] = 0;
    colorLookup[5][0] = 255;    colorLookup[5][1] = 181;    colorLookup[5][2] = 181;
    colorLookup[6][0] = 135;    colorLookup[6][1] = 135;    colorLookup[6][2] = 135;
    colorLookup[7][0] = 48;     colorLookup[7][1] = 80;     colorLookup[7][2] = 248;
    colorLookup[8][0] = 255;    colorLookup[8][1] = 13;     colorLookup[8][2] = 13;
    colorLookup[9][0] = 144;    colorLookup[9][1] = 224;    colorLookup[9][2] = 80;
    colorLookup[10][0] = 179;   colorLookup[10][1] = 227;   colorLookup[10][2] = 245;

    colorLookup[11][0] = 171;   colorLookup[11][1] = 92;    colorLookup[11][2] = 242;
    colorLookup[12][0] = 138;   colorLookup[12][1] = 255;   colorLookup[12][2] = 0;
    colorLookup[13][0] = 191;   colorLookup[13][1] = 166;   colorLookup[13][2] = 166;
    colorLookup[14][0] = 240;   colorLookup[14][1] = 200;   colorLookup[14][2] = 160;
    colorLookup[15][0] = 255;   colorLookup[15][1] = 128;   colorLookup[15][2] = 0;
    colorLookup[16][0] = 255;   colorLookup[16][1] = 255;   colorLookup[16][2] = 48;
    colorLookup[17][0] = 31;    colorLookup[17][1] = 240;   colorLookup[17][2] = 31;
    colorLookup[18][0] = 128;   colorLookup[18][1] = 209;   colorLookup[18][2] = 227;
    colorLookup[19][0] = 143;   colorLookup[19][1] = 64;    colorLookup[19][2] = 212;
    colorLookup[20][0] = 61;    colorLookup[20][1] = 255;   colorLookup[20][2] = 0;

    colorLookup[21][0] = 230;   colorLookup[21][1] = 230;   colorLookup[21][2] = 230;
    colorLookup[22][0] = 191;   colorLookup[22][1] = 194;   colorLookup[22][2] = 199;
    colorLookup[23][0] = 166;   colorLookup[23][1] = 166;   colorLookup[23][2] = 171;
    colorLookup[24][0] = 138;   colorLookup[24][1] = 153;   colorLookup[24][2] = 199;
    colorLookup[25][0] = 156;   colorLookup[25][1] = 122;   colorLookup[25][2] = 199;
    colorLookup[26][0] = 224;   colorLookup[26][1] = 102;   colorLookup[26][2] = 51;
    colorLookup[27][0] = 240;   colorLookup[27][1] = 144;   colorLookup[27][2] = 160;
    colorLookup[28][0] = 80;    colorLookup[28][1] = 208;   colorLookup[28][2] = 80;
    colorLookup[29][0] = 200;   colorLookup[29][1] = 128;   colorLookup[29][2] = 51;
    colorLookup[30][0] = 125;   colorLookup[30][1] = 128;   colorLookup[30][2] = 176;

    colorLookup[31][0] = 194;   colorLookup[31][1] = 143;   colorLookup[31][2] = 143;
    colorLookup[32][0] = 102;   colorLookup[32][1] = 143;   colorLookup[32][2] = 143;
    colorLookup[33][0] = 189;   colorLookup[33][1] = 128;   colorLookup[33][2] = 227;
    colorLookup[34][0] = 255;   colorLookup[34][1] = 161;   colorLookup[34][2] = 0;
    colorLookup[35][0] = 166;   colorLookup[35][1] = 41;    colorLookup[35][2] = 41;
    colorLookup[36][0] = 92;    colorLookup[36][1] = 184;   colorLookup[36][2] = 209;
    colorLookup[37][0] = 112;   colorLookup[37][1] = 46;    colorLookup[37][2] = 176;
    colorLookup[38][0] = 0;     colorLookup[38][1] = 255;   colorLookup[38][2] = 0;
    colorLookup[39][0] = 148;   colorLookup[39][1] = 255;   colorLookup[39][2] = 255;
    colorLookup[40][0] = 148;   colorLookup[40][1] = 224;   colorLookup[40][2] = 224;

    colorLookup[41][0] = 115;   colorLookup[41][1] = 194;   colorLookup[41][2] = 201;
    colorLookup[42][0] = 84;    colorLookup[42][1] = 181;   colorLookup[42][2] = 181;
    colorLookup[43][0] = 59;    colorLookup[43][1] = 158;   colorLookup[43][2] = 158;
    colorLookup[44][0] = 36;    colorLookup[44][1] = 143;   colorLookup[44][2] = 143;
    colorLookup[45][0] = 10;    colorLookup[45][1] = 125;   colorLookup[45][2] = 140;
    colorLookup[46][0] = 0;     colorLookup[46][1] = 105;   colorLookup[46][2] = 133;
    colorLookup[47][0] = 192;   colorLookup[47][1] = 192;   colorLookup[47][2] = 192;
    colorLookup[48][0] = 255;   colorLookup[48][1] = 217;   colorLookup[48][2] = 143;
    colorLookup[49][0] = 166;   colorLookup[49][1] = 117;   colorLookup[49][2] = 115;
    colorLookup[50][0] = 102;   colorLookup[50][1] = 128;   colorLookup[50][2] = 128;

    colorLookup[51][0] = 158;   colorLookup[51][1] = 99;    colorLookup[51][2] = 181;
    colorLookup[52][0] = 212;   colorLookup[52][1] = 122;   colorLookup[52][2] = 0;
    colorLookup[53][0] = 148;   colorLookup[53][1] = 0;     colorLookup[53][2] = 148;
    colorLookup[54][0] = 66;    colorLookup[54][1] = 158;   colorLookup[54][2] = 176;
    colorLookup[55][0] = 87;    colorLookup[55][1] = 23;    colorLookup[55][2] = 143;
    colorLookup[56][0] = 0;     colorLookup[56][1] = 201;   colorLookup[56][2] = 0;
    colorLookup[57][0] = 112;   colorLookup[57][1] = 212;   colorLookup[57][2] = 255;
    colorLookup[58][0] = 255;   colorLookup[58][1] = 255;   colorLookup[58][2] = 199;
    colorLookup[59][0] = 217;   colorLookup[59][1] = 255;   colorLookup[59][2] = 199;
    colorLookup[60][0] = 199;   colorLookup[60][1] = 255;   colorLookup[60][2] = 199;

    colorLookup[61][0] = 163;   colorLookup[61][1] = 255;   colorLookup[61][2] = 199;
    colorLookup[62][0] = 143;   colorLookup[62][1] = 255;   colorLookup[62][2] = 199;
    colorLookup[63][0] = 97;    colorLookup[63][1] = 255;   colorLookup[63][2] = 199;
    colorLookup[64][0] = 69;    colorLookup[64][1] = 255;   colorLookup[64][2] = 199;
    colorLookup[65][0] = 48;    colorLookup[65][1] = 255;   colorLookup[65][2] = 199;
    colorLookup[66][0] = 31;    colorLookup[66][1] = 255;   colorLookup[66][2] = 199;
    colorLookup[67][0] = 0;     colorLookup[67][1] = 255;   colorLookup[67][2] = 156;
    colorLookup[68][0] = 0;     colorLookup[68][1] = 230;   colorLookup[68][2] = 117;
    colorLookup[69][0] = 0;     colorLookup[69][1] = 212;   colorLookup[69][2] = 82;
    colorLookup[70][0] = 0;     colorLookup[70][1] = 191;   colorLookup[70][2] = 56;

    colorLookup[71][0] = 0;     colorLookup[71][1] = 171;   colorLookup[71][2] = 36;
    colorLookup[72][0] = 77;    colorLookup[72][1] = 194;   colorLookup[72][2] = 255;
    colorLookup[73][0] = 77;    colorLookup[73][1] = 166;   colorLookup[73][2] = 255;
    colorLookup[74][0] = 33;    colorLookup[74][1] = 148;   colorLookup[74][2] = 214;
    colorLookup[75][0] = 38;    colorLookup[75][1] = 125;   colorLookup[75][2] = 171;
    colorLookup[76][0] = 38;    colorLookup[76][1] = 102;   colorLookup[76][2] = 150;
    colorLookup[77][0] = 23;    colorLookup[77][1] = 84;    colorLookup[77][2] = 135;
    colorLookup[78][0] = 208;   colorLookup[78][1] = 208;   colorLookup[78][2] = 224;
    colorLookup[79][0] = 255;   colorLookup[79][1] = 209;   colorLookup[79][2] = 35;
    colorLookup[80][0] = 184;   colorLookup[80][1] = 184;   colorLookup[80][2] = 208;

    colorLookup[81][0] = 166;   colorLookup[81][1] = 84;    colorLookup[81][2] = 77;
    colorLookup[82][0] = 87;    colorLookup[82][1] = 89;    colorLookup[82][2] = 97;
    colorLookup[83][0] = 158;   colorLookup[83][1] = 79;    colorLookup[83][2] = 181;
    colorLookup[84][0] = 171;   colorLookup[84][1] = 92;    colorLookup[84][2] = 0;
    colorLookup[85][0] = 117;   colorLookup[85][1] = 79;    colorLookup[85][2] = 69;
    colorLookup[86][0] = 66;    colorLookup[86][1] = 130;   colorLookup[86][2] = 150;
    colorLookup[87][0] = 66;    colorLookup[87][1] = 0;     colorLookup[87][2] = 102;
    colorLookup[88][0] = 0;     colorLookup[88][1] = 125;   colorLookup[88][2] = 0;
    colorLookup[89][0] = 112;   colorLookup[89][1] = 171;   colorLookup[89][2] = 250;
    colorLookup[90][0] = 0;     colorLookup[90][1] = 186;   colorLookup[90][2] = 255;

    colorLookup[91][0] = 0;     colorLookup[91][1] = 161;   colorLookup[91][2] = 255;
    colorLookup[92][0] = 0;     colorLookup[92][1] = 143;   colorLookup[92][2] = 255;
    colorLookup[93][0] = 0;     colorLookup[93][1] = 128;   colorLookup[93][2] = 255;
    colorLookup[94][0] = 0;     colorLookup[94][1] = 107;   colorLookup[94][2] = 255;
    colorLookup[95][0] = 84;    colorLookup[95][1] = 92;    colorLookup[95][2] = 242;
    colorLookup[96][0] = 120;   colorLookup[96][1] = 92;    colorLookup[96][2] = 227;
    colorLookup[97][0] = 138;   colorLookup[97][1] = 79;    colorLookup[97][2] = 227;
    colorLookup[98][0] = 161;   colorLookup[98][1] = 54;    colorLookup[98][2] = 212;
    colorLookup[99][0] = 179;   colorLookup[99][1] = 31;    colorLookup[99][2] = 212;
    colorLookup[100][0] = 179;  colorLookup[100][1] = 31;   colorLookup[100][2] = 186;

    colorLookup[101][0] = 179;  colorLookup[101][1] = 13;   colorLookup[101][2] = 166;
    colorLookup[102][0] = 189;  colorLookup[102][1] = 13;   colorLookup[102][2] = 135;
    colorLookup[103][0] = 199;  colorLookup[103][1] = 0;    colorLookup[103][2] = 102;
    colorLookup[104][0] = 204;  colorLookup[104][1] = 0;    colorLookup[104][2] = 89;
    colorLookup[105][0] = 209;  colorLookup[105][1] = 0;    colorLookup[105][2] = 79;
    colorLookup[106][0] = 217;  colorLookup[106][1] = 0;    colorLookup[106][2] = 69;
    colorLookup[107][0] = 224;  colorLookup[107][1] = 0;    colorLookup[107][2] = 56;
    colorLookup[108][0] = 230;  colorLookup[108][1] = 0;    colorLookup[108][2] = 46;
    colorLookup[109][0] = 235;  colorLookup[109][1] = 0;    colorLookup[109][2] = 38;
    colorLookup[110][0] = 255;  colorLookup[110][1] = 0;    colorLookup[110][2] = 0;

    colorLookup[111][0] = 255;  colorLookup[111][1] = 0;    colorLookup[111][2] = 0;
    colorLookup[112][0] = 255;  colorLookup[112][1] = 0;    colorLookup[112][2] = 0;
    colorLookup[113][0] = 255;  colorLookup[113][1] = 0;    colorLookup[113][2] = 0;
    colorLookup[114][0] = 255;  colorLookup[114][1] = 0;    colorLookup[114][2] = 0;
    colorLookup[115][0] = 255;  colorLookup[115][1] = 0;    colorLookup[115][2] = 0;
    colorLookup[116][0] = 255;  colorLookup[116][1] = 0;    colorLookup[116][2] = 0;
    colorLookup[117][0] = 255;  colorLookup[117][1] = 0;    colorLookup[117][2] = 0;
    colorLookup[118][0] = 255;  colorLookup[118][1] = 0;    colorLookup[118][2] = 0;
}


void Pandora::parseDataFile(QString fileType, QString fileName)
{
    // This function takes the file extension and assigns a function capable
    // of parsing the text file. If it finds none, it sends a signal back to "MainWindow"
    // so it can display the appropriate warning.

    if (fileType == "All File Types (*.*)")
    {
        QFileInfo file(fileName);
        fileType = file.suffix();
    }

    if (fileType == "bgf" | fileType == "BIOGRF File (*.bgf)")
    {
        parseBGF(fileName);
    }
//    else if (fileType == "cif")
//    {

//    }
//    else if (fileType == "cml")
//    {

//    }
    else if (fileType == "lammps")
    {
        parseLammpsInput(fileName);
    }
//    else if (fileType == "lmpstrj")
//    {
//        parseLammpsTracjectory(fileName);
//    }
//    else if (fileType == "mol")
//    {

//    }
//    else if (fileType == "mol2")
//    {

//    }
//    else if (fileType == "msi")
//    {

//    }
//    else if (fileType == "pdb")
//    {

//    }
//    else if (fileType == "xyz")
//    {

//    }
    else
    {
        // The file is either unknown or is currently unsupported.  Warn the user.
        QString message = "Apologies. The selected data file format *." + fileType + " is currently unsupported.";
        pandoraError(message);
    }
}


void Pandora::writeDataFile(QString fileType, QString fileName)
{
    // This function takes the file extension and assigns a function capable
    // of writing the text file containing the chemical data. If it finds none, it sends a signal back to "MainWindow"
    // so it can display the appropriate warning.

    //Pandora input;  // create an instance of Pandora so we can call the specific function we need.

    if (fileType == "BIOGRF File (*.bgf)")
    {
        writeBGF(fileName);
    }
//    else if (fileType == "cif")
//    {

//    }
//    else if (fileType == "cml")
//    {

//    }
    else if (fileType == "LAMMPS Input File (*.lammps)")
    {
        writeLammpsInput(fileName);
    }
//    else if (fileType == "mol")
//    {

//    }
//    else if (fileType == "mol2")
//    {

//    }
//    else if (fileType == "msi")
//    {

//    }
//    else if (fileType == "pdb")
//    {

//    }
//    else if (fileType == "xyz")
//    {

//    }
    else
    {
        // The filetype is either unknown or is currently unsupported.  Warn the user.
        QString message = "Apologies. The selected data file format " + fileType + " is currently unsupported.";
        pandoraError(message);
    }
}

void Pandora::pandoraError(QString message)
{
    QMessageBox error;
    error.setIcon(QMessageBox::Critical);
    error.setText(message);
    error.exec();
}

void Pandora::pandoraInfo(QString message)
{
    QMessageBox info;
    info.setIcon(QMessageBox::Information);
    info.setText(message);
    info.exec();
}




/******************************************************************************************************************
* File Parsing Functions
******************************************************************************************************************/


void Pandora::parseBGF(QString fileName)
{
    //**************************************************************************************
    // Purpose: This function is able to parse chemical data files adhering to BIOGRAF (.bgf)
    // formatting.   The format is detailed, storing atom position, atom type, force field type,
    // partial charge, chain and residue identity, and bond connectivity.  The format is also
    // self-referencing, with headers that describe the function of each line, as well as the
    // fixed-width of each column.
    //**************************************************************************************

    QFile file (fileName);
    QTextStream inputFile (&file);
    QStringList lineArray;
    QString line, line2, widthCatcher, fieldCatcher, fieldCatcher2;
    QMessageBox cout;
    bool periodicity;

    QVector<int> atomColumnWidths, bondColumnWidths, bondLoader;
    QString::Iterator iter;

    QVector<QVector<int> > bondDuplicateTally;

    int atomCount, atomicNumber, atomID, bondOrder, bookmark, corrector, primaryAtom, secondaryAtom;
    bool whiteSpaceFound, widthStart, valueFound;


    atomColumnWidths.resize(21);
    bondColumnWidths.resize(3);

    // Throughout the file, the first item on each line indicates the data contained on that line.
    // So, we need only grab that first keyword, and we can assign that data into the Prometheus framework.

    // But first, we must count the number of atoms within the file and allocate memory appropriately.
    atomCount = 0;
    file.open(QIODevice::ReadOnly|QIODevice::Text);
    while(!inputFile.atEnd())
    {
        line = inputFile.readLine(); // String that holds the whole line
        lineArray = line.split(" ");  // StringList breaks the string along white spaces...  this lets us look at the keyword.

        // The label associated with atomic data is either "HETATM" or "ATOM".  Count these.

        if (lineArray[0] == "HETATM" || lineArray[0] == "ATOM")
        {
            ++atomCount;
        }
    }

    //close the file and allocate memory for the model.
    file.close();
    periodicity = false; // take this out when we improve file reading.
    prometheusModel.createModel(atomCount, periodicity);
    bondDuplicateTally.resize(atomCount);
    for (int i = 0; i < atomCount; ++i)
    {
        bondDuplicateTally[i].clear();
    }

    // Re-open the file for a proper read that parses the data and loads it into the model.
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    while (!inputFile.atEnd())
    {
        line = inputFile.readLine(); // String that holds the whole line.
        lineArray = line.split(" ",QString::SkipEmptyParts); // StringList breaks the string along white spaces...  this lets us look at the keyword.


        if (lineArray[0] == "FORMAT")
        {
            // This line indicates that it contains the legend for reading the fixed-column format for this file.

            // The following code is vulnerable...  it breaks if there is even a single white-space in the format definition section.  First, we heal
            // any whitespaces, by placing the pieces into the same element.
            if (lineArray.size() > 3)
            {
                line.clear();
                for(int i = 2; i < lineArray.size(); i++)
                {
                     line.append(lineArray[i]);
                }

                lineArray[2] = line;
            }

            if (lineArray[1] == "ATOM" | lineArray[1] == "HETATM")
            {
                // This line contains the formatting for the data under the HETATM or ATOM headers (position, charge, etc)

                // The formatting information is (ironically) formatted somewhat obtusely to save space.  Isolate the string that contains
                // relevant data and split it using commas as delimiters.
                line = lineArray[2];
                lineArray = line.split(",");


                // The general format is:
                //             [Multiplier - (optional) states the number of times the following is about to occur]
                // followed by [Identifier - states what this field's type is:  a = string, i = integer, f = float (see below), x = whitespace]
                // followed by [Field Width - states the number of characters this field occupies]
                // followed by [Decimal Place - (optional) for float representation, the number of places after the decimal is given after a period]

                // Example:  to show a representation of three position components, represented as float numbers, and with no spaces between columns
                // that are 10 characters wide each, and with a precision of 5 digits after the decimal ---->  use 3f10.5
                //  Very concise, but kinda annoying.

                // The multiplier only applies to coordinate data.  Only coordinate and partial charge data require float type values, so only they
                // will use the decimal precision flag.

                // Iterate over the fields...  21 in total.
                // 0) BGF Keyword ID [string]    2) Atom Number[int]  4) Atom Label[string]    6) Residue Name[string]
                // 8) Chain Designation [int] 10) Residue Number[int] 12) Atom Position Coordinates in Angstrom[float]
                // 14) Atom Type from Force Field[string] 16) Max Number of Bonds [int] 18) Number of Lone Electron Pairs [int]
                // 20) Partial Charge [float]

                // The missing fields designate the width of whitespace between the above fields.  They are optional, but must be accounted for.
                // If we come accross a missing whitespace flag, we need to not get ahead of ourselves reading the
                // line, so "corrector" will keep track and allow us to read the correct field if we've discovered a missing
                // whitespace flag field.

                corrector = 0;
                for (int i = 0; i < 21; i = i + 2)
                {
                    widthCatcher.clear();

                    if (i != 12 && i != 20)
                    {
                        // Most fields will use this section.

                        // Iterate over the characters in this field.
                        for (iter = lineArray[i - corrector].begin(); iter != lineArray[i - corrector].end(); ++iter)
                        {
                            // Ignore parentheses and letters to grab the any numeral characters...
                            if (*iter != '(' && *iter != ')' && *iter != 'a' && *iter != 'f' && *iter != 'i' && *iter != 'x')
                            {
                                // pile these characters together into a string for conversion.
                                widthCatcher.append(*iter);
                            }
                        }

                        // Store the field width as an integer.
                        atomColumnWidths[i] = widthCatcher.toInt();


                    }
                    else
                    {
                        // We must take more care in the fields that give formatting for position and partial charge data.

                        widthStart = false;

                        // Iterate over the characters in this field.
                        for (iter = lineArray[i - corrector].begin(); iter != lineArray[i - corrector].end(); ++iter)
                        {

                            if (widthStart && *iter != '.')
                            {
                                // pile the characters between the 'f' and the '.' into a string for conversion.
                                widthCatcher.append(*iter);
                            }

                            // These functions will control the switches that pile the characters into their proper strings for conversion.
                            if (*iter == 'f')
                            {
                                widthStart = true;
                            }

                            if (*iter == '.')
                            {
                                widthStart = false;
                            }
                        }

                        // Store the field width as an integer.
                        atomColumnWidths[i] = widthCatcher.toInt();

                    }

                    // Move on the the next field.  This should contain the width of the whitespace between fields.
                    // However, its existance is optional.  To make matters worse, it doesn't start with an identifier, but
                    // rather the width of the whitespace.  The identifer 'x' comes after the number.  Generally, the width should be 1.

                    // Also, skip this section if we've made it to the partial charge field (i = 20)

                    widthCatcher.clear();

                    if (i != 20)
                    {

                        // Iterate over the characters in this field.
                        for (iter = lineArray[i+1 - corrector].begin(); iter != lineArray[i+1 - corrector].end(); ++iter)
                        {
                            // Ignore parentheses and letters for now to grab the any numeral characters...
                            if (*iter != ')' && *iter != 'a' && *iter != 'f' && *iter != 'i' && *iter != 'x')
                            {
                                // pile these characters together into a string for conversion.
                                widthCatcher.append(*iter);
                            }

                            else if (*iter == 'a' || *iter == 'f' || *iter == 'i')
                            {
                                // This field does not describe a whitespace.
                                whiteSpaceFound = false;
                            }
                            else if (*iter == 'x')
                            {
                                // This field describes a whitespace.
                                whiteSpaceFound = true;
                            }
                        }

                        // Store the field width as an integer.
                        if (whiteSpaceFound)
                        {
                            // Store the width that we found (generally 1)
                            atomColumnWidths[i+1] = widthCatcher.toInt();
                        }
                        else
                        {
                            // There is no whitespace, and we need to know to not include any when we are counting
                            // characters below.  Also, we were searching for whitespace field information, so we
                            // must make sure we re-read this field and store its width in the appropriate location.
                            // the corrector will adjust the iterator so we do not read out of range of the "lineArray".

                            atomColumnWidths[i+1] = 0;
                            ++corrector;
                        }
                    }
                }

            }
            else if (lineArray[1] == "CONECT")
            {
                // This line contains the formmating for the data under the "CONECT" header (atom connectivity)
                // Once more, the line is formatted in a way that is difficult to parse. However, there is much less to deal with, so this is pretty easy.
                // Isolate the string that contains relevant data and split it using commas as delimiters.
                line = lineArray[2];
                lineArray = line.split(",");

                // The first field denotes a text field that contains the label "CONECT".  The second specifies which atom is being described, as identified by
                // its atom number (integer).  All other fields identify atoms that are connected to the first one.  In general, there are no spaces between the
                // fields, and the width of each field (besides the label CONECT) can be specfied with the multiplier 12.

                // iterate over the characters in the first field.
                widthCatcher.clear();
                for (iter = lineArray[0].begin(); iter != lineArray[0].end(); ++iter)
                {
                    // Grab only numeral characters.
                    if (*iter != '(' && *iter != 'a' && *iter != ' ')
                    {
                        // Pile these together into a string for conversion.
                        widthCatcher.append(*iter);
                    }
                }

                //  Store the field width as an integer.
                bondColumnWidths[0] = widthCatcher.toInt();

                // Now move onto the second field.  We'll need to grab the multiplier first (usually 12).
                // Iterate over the characters of the second field.

                widthCatcher.clear();
                for (iter = lineArray[1].begin(); iter != lineArray[1].end(); ++iter)
                {
                    if (*iter == 'i') break;  // exit once we hit the integer identifer. We have captured the multiplier fully.

                    widthCatcher.append(*iter);
                }

                // Store the multiplier as an integer.
                bondColumnWidths[1] = widthCatcher.toInt();

                // Now get the width of the fields that hold these atom numbers.
                widthCatcher.clear();
                widthStart = false;
                for (iter = lineArray[1].begin(); iter != lineArray[1].end(); ++iter)
                {
                    // Grab only the numeral character(s).
                    if (widthStart && *iter != ')')
                    {
                        // Pile the characters together in a string for conversion.
                        widthCatcher.append(*iter);
                    }

                    // watch for the identifier that will denote the width information.
                    if (*iter == 'i') widthStart = true;
                }

                // Store the width as an integer.
                bondColumnWidths[2] = widthCatcher.toInt();
            }
        }
        else if (lineArray[0] == "HETATM" || lineArray[0] == "ATOM")
        {
            // These lines contain the actual data related to the atoms themselves.  We established the column widths earlier, so we
            // can just chop these lines up accordingly and grab what we find.  We use an integer variable to keep a running count
            // of where we are reading in the string...  remember we cannot rely on whitespaces to give us each token.

            bookmark = 0;   // our placeholder in this string.

            // BGF Label field...  basically just skip over this and the optional whitespace that follows it.
            bookmark += atomColumnWidths[0] + atomColumnWidths[1];

            // Atom ID number...   assumes that the atomID numbers are presented in order.  We'll need this for the set functions of the Atlas Class.
            fieldCatcher.clear();
            for (int i = bookmark; i < bookmark + atomColumnWidths[2]; ++i)
            {
                fieldCatcher.append(line[i]);
            }

            // This value is used to order all other atom properties.
            atomID = fieldCatcher.toInt();
            bookmark += atomColumnWidths[2] + atomColumnWidths[3];

            // Atom Name...  This is a string, usually an atomic symbol followed by number.
            fieldCatcher.clear();
            for (int i = bookmark; i < bookmark + atomColumnWidths[4]; ++i)
            {
                fieldCatcher.append(line[i]);
            }
            prometheusModel.setAtomName(atomID, fieldCatcher);
            bookmark += atomColumnWidths[4] + atomColumnWidths[5];

            // Residue Name...  This is a string, generally 3 letters that identifies a structural element of a model (monomer, amino acid, etc)
            fieldCatcher.clear();
            for (int i = bookmark; i < bookmark + atomColumnWidths[6]; ++i)
            {
                fieldCatcher.append(line[i]);
            }
            prometheusModel.setAtomResidueName(atomID, fieldCatcher);
            bookmark += atomColumnWidths[6] + atomColumnWidths[7];

            // Chain Identifier...   This is a string, all captial letters, that identifies a chain to which the atom belongs.
            fieldCatcher.clear();
            for (int i = bookmark; i < bookmark + atomColumnWidths[8]; ++i)
            {
                fieldCatcher.append(line[i]);
            }
            prometheusModel.setAtomChainName(atomID, fieldCatcher);
            bookmark += atomColumnWidths[8] + atomColumnWidths[9];

            // Residue Number...  This is an integer. Beyond identifying struture types, we also give them a number to ID that specific subunit.
            fieldCatcher.clear();
            for (int i = bookmark; i < bookmark + atomColumnWidths[10]; ++i)
            {
                fieldCatcher.append(line[i]);
            }
            prometheusModel.setAtomResidueNumber(atomID, fieldCatcher.toInt());
            bookmark += atomColumnWidths[10] + atomColumnWidths[11];

            // Atomic Position in Cartesian coordinates.  Here, we cover fields for X, Y, and Z components given in float format.
            // We take the width of the field multiplied by three to capture each component.  We assume there are no whitespaces here.
            fieldCatcher.clear();
            for (int i = bookmark; i < bookmark + atomColumnWidths[12]; ++i)
            {
                fieldCatcher.append(line[i]);
            }
            prometheusModel.setAtomPositionX(atomID, fieldCatcher.toFloat());
            bookmark += atomColumnWidths[12];

            fieldCatcher.clear();
            for (int i = bookmark; i < bookmark + atomColumnWidths[12]; ++i)
            {
                fieldCatcher.append(line[i]);
            }
            prometheusModel.setAtomPositionY(atomID, fieldCatcher.toFloat());
            bookmark += atomColumnWidths[12];

            fieldCatcher.clear();
            for (int i = bookmark; i < bookmark + atomColumnWidths[12]; ++i)
            {
                fieldCatcher.append(line[i]);
            }
            prometheusModel.setAtomPositionZ(atomID, fieldCatcher.toFloat());
            bookmark += atomColumnWidths[12] + atomColumnWidths[13];

            // Force Field Type...  This is a string.  This indicates the type of atom as labeled by a particular force field.
            // Therefore, this assigns the parameters used in later calcuations.
            fieldCatcher.clear();
            for (int i = bookmark; i < bookmark + atomColumnWidths[14]; ++i)
            {
                fieldCatcher.append(line[i]);
            }
            prometheusModel.setAtomForceFieldType(atomID, fieldCatcher);
            bookmark += atomColumnWidths[14] + atomColumnWidths[15];

            // Determine the element this atom represents from the force field type provided.
            atomicNumber = findAtomicNumber(fieldCatcher);
            prometheusModel.setAtomAtomicNumber(atomID, atomicNumber);

            // Maximum Number of Covalent Bonds...  This is an integer.  It is possible this number could be useful adjusting hydrogens in the model.
            fieldCatcher.clear();
            for (int i = bookmark; i < bookmark + atomColumnWidths[16]; ++i)
            {
                fieldCatcher.append(line[i]);
            }
            prometheusModel.setAtomMaxCovalentBonds(atomID, fieldCatcher.toInt());
            bookmark += atomColumnWidths[16] + atomColumnWidths[17];

            // Number of Electron Lone Pairs...  This is an integer.
            fieldCatcher.clear();
            for (int i = bookmark; i < bookmark + atomColumnWidths[18]; ++i)
            {
                fieldCatcher.append(line[i]);
            }
            prometheusModel.setAtomNumberOfLonePairs(atomID, fieldCatcher.toInt());
            bookmark += atomColumnWidths[18] + atomColumnWidths[19];

            // Partial Charge...   This is a float type number.
            fieldCatcher.clear();
            for (int i = bookmark; i < bookmark + atomColumnWidths[20]; ++i)
            {
                fieldCatcher.append(line[i]);
            }
            prometheusModel.setAtomPartialCharge(atomID, fieldCatcher.toFloat());
        }
        else if (lineArray[0] == "CONECT")
        {
            // Lines under this header contain atoms that are bonded.  The first field indicates the primary atom of interest.
            // The following fields indicate atoms bonded to the primary atom.  This representation results in redundancies, specifically,
            // each bond is noted in two places.

            // Also, each line is paired with a second line just after this one, containing bond order information.  Under each bond entry, there
            // is a number 1, 2, or 3, signifying a single, double, or triple bond, respectively.  Though the header is ORDER, the format is identical to
            // CONECT...  so when we read in the bond information, but we must check each entry for duplication against a tally we maintain, and when we find a
            // unique entry, we also grab its corresponding bond order.

            // grab the ORDER line.
            line2 = inputFile.readLine(); // String that holds the whole line...  the ORDER line.

            bookmark = bondColumnWidths[0];   // our placeholder in this string. Ignore the CONECT/ORDER header since we are already aware of it.

            // Determine the primary atom ID number.
            fieldCatcher.clear();

            for (int i = bookmark; i < bookmark + bondColumnWidths[2]; ++i)
            {
                fieldCatcher.append(line[i]);
            }
            // Store the primary atom ID number.
            primaryAtom = fieldCatcher.toInt();
            bookmark += bondColumnWidths[2];

            // Now, iterate through the remaining fields on this line.
            // Recall that bondColumnWidths[1] stores the multiplier indicating the number of fields.
            for (int i = 1; i < bondColumnWidths[1]; ++i)
            {
                // Determine the secondary atom ID number and corresponding bond order.
                fieldCatcher.clear();
                fieldCatcher2.clear();
                for (int j = bookmark; j < bookmark + bondColumnWidths[2]; ++j)
                {
                    fieldCatcher.append(line[j]);
                    fieldCatcher2.append((line2[j]));
                }

                // Store the secondary atom ID number.

                // It is unlikely all the fields will be occupied.  If we encounter a blank field, we must stop.  We can detect the blank field
                // by using the string-to-integer conversion function in Qt.  We can give it a bool variable (valueFound), and if the conversion fails, the
                // variable comes back false.  If the valueFound comes back false, we need to break out of this loop and move on to the next line.
                // Otherwise, we should stay in this loop and analyze the bond pair.
                secondaryAtom = fieldCatcher.toInt(&valueFound, 10);
                bondOrder = fieldCatcher2.toInt();
                bookmark += bondColumnWidths[2];
                if (!valueFound) break;

                if (!bondDuplicateTally[primaryAtom - 1].contains(secondaryAtom))
                {
                    // We did not find this bond pairing in our running tally.  Now we need to store this unique entry.
                    // We adhere to the convention of storing the atom with the lowest ID number first.  Because the order the
                    // bond information is presented in BGFs, unique bonds should already always have the primary atom's ID number be lower.
                    bondLoader.clear();
                    bondLoader.resize(2);
                    bondLoader[0] = primaryAtom;
                    bondLoader[1] = secondaryAtom;

                    // Add this data to the Atlas framework.
                    prometheusModel.setBondList(bondLoader);
                    prometheusModel.setBondOrder(bondOrder);

                    // Now we need to add this unique bond to the running tally, so we do not duplicate the bond.
                    // Remember, we'll need to add to the tally in two places.
                    bondDuplicateTally[primaryAtom - 1].push_back(secondaryAtom);
                    bondDuplicateTally[secondaryAtom - 1].push_back(primaryAtom);

                }
            }

        }
        else if (lineArray[0] == "END")
        {
            // We've made it to the end of the BGF file.
            prometheusModel.setNumberOfBonds();

            file.close();
        }
    }
}


void Pandora::parseCIF()
{

}
void Pandora::parseCML()
{

}
void Pandora::parseLammpsInput(QString fileName)
{

}
void Pandora::parseLammpsTrajectory()
{

}
void Pandora::parseMOL()
{

}
void Pandora::parseMOL2()
{

}
void Pandora::parseMSI()
{

}
void Pandora::parsePBD()
{

}
void Pandora::parseXYZ()
{

}


/******************************************************************************************************************
* File Writing Functions
******************************************************************************************************************/
void Pandora::writeBGF(QString fileName)
{
    //**************************************************************************************
    // Purpose: This function is able to write chemical data files adhering to BIOGRAF (.bgf)
    // formatting.   The format is detailed, storing atom position, atom type, force field type,
    // partial charge, chain and residue identity, and bond connectivity.  The format is also
    // self-referencing, with headers that describe the function of each line, as well as the
    // fixed-width of each column.
    //**************************************************************************************

    QFile file (fileName);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream outputFile (&file);

    // First thing's first...  load in all available data on this model.

    // Refresh and then retrieve atom names.
    nameAllAtoms();
    QVector<QString> AtomName = prometheusModel.getAtomName();

    // Residue Name
    QString ResidueName = "RES";

    // Chain Name
    QString ChainName = "A";

    // Residue Number
    QString ResidueNumber = "444";

    // Position
    QVector<float> PosX = prometheusModel.getAtomPositionX();
    QVector<float> PosY = prometheusModel.getAtomPositionY();
    QVector<float> PosZ = prometheusModel.getAtomPositionZ();

    // Force Field Type
    QVector<QString> FFType = prometheusModel.getAtomForceFieldType();

    // Maximum Number of Covalent Bonds
    int MaxNumOfBonds = 4;

    // Number of Lone Pairs
    int NumOfLonePairs = 0;

    // Partial Charges
    QVector<float> Charges = prometheusModel.getAtomPartialCharge();

    // Connectivity Data... List of Bond Pairs
    QVector<QVector<int> > BondList = prometheusModel.getBondList();
    QVector<int> BondOrder = prometheusModel.getBondOrder();

    // We need to reorganize this data so that it can be displayed in a BGF file.
    QVector<QVector<int> > PrintedBondList;
    QVector<QVector<int> > PrintedBondOrder;
    PrintedBondList.resize(prometheusModel.getNumberOfAtoms());
    PrintedBondOrder.resize(prometheusModel.getNumberOfAtoms());
    for (int i = 0; i < BondList.size(); i++)
    {
        PrintedBondList[BondList[i][0] - 1].push_back(BondList[i][1]);
        PrintedBondOrder[BondList[i][0] - 1].push_back(BondOrder[i]);

        PrintedBondList[BondList[i][1] - 1].push_back(BondList[i][0]);
        PrintedBondOrder[BondList[i][1] - 1].push_back(BondOrder[i]);
    }

    /*
    // Now sort each bond record to conform to BGF standards.
    for (int i = 0; i < PrintedBondList.size(); i++)
    {
        qSort(PrintedBondList[i]);
    }
    */


    // While certain parts of the header are always present... there are parts that are only printed when the model is periodic.

    // Identifies the format
    if (prometheusModel.getModelPeriodicity())
    {
        outputFile << "XTLGRF 200\n";
    }
    else
    {
        outputFile << "BIOGRF 200\n";
    }

    // Short description...  we just state the filename.
    QFileInfo info (fileName);
    QString description = info.baseName();
    outputFile << "DESCRP " << description << endl;

    // A short descriptive remark of our choosing...  or maybe a little shameless self promotion?  :)
    outputFile << "REMARK BGF file created by Prometheus - The Open Chemical Physics Suite on " << QDateTime::currentDateTime().toString() << endl;

    // Identifies the force field used...  for now, we'll always say DREIDING, but we may need to come back to this.
    outputFile << "FORCEFIELD DREIDING\n";

    // The rest of the header only applies to periodic models.
    if (prometheusModel.getModelPeriodicity())
    {
        // States periodicity in x, y, and z.  0 = non-periodic, 1 = periodic    ('PERIOD',1X,3I1)
        outputFile << "PERIOD 111\n";

        // Axis order  Ignored on reading, set to 'zyx' when writing crystals.  ('AXES',3X,A)
        outputFile << "AXES ZYX\n";

        // Space Group Name...  useful in crystallography, but all the coordinates are below, so only useful to humans
        // reading the file, not Prometheus.   'SGNAME', space group name, Schönflies symbol for the space group, space group number,
        // case number. A case number is only used for space groups where there is a choice of unique axis, cell origin, or axis
        // geometry.  Note: Ignored on reading because all atoms (including symmetry copies) are contained in atom records.
        // ('SGNAME',1X,A8,1X,A8,2I5)
        outputFile << "SGNAME Ignore\n";

        // Periodic Cell Parameters...  much more useful.  a, b, and c side lengths in Angstroms, followed by
        // angles alpha, beta, and gamma in degrees.  ('CRYSTX',1X,6F11.5)
        outputFile << "CRYSTX a b c alpha beta gamma\n";

        // Cell extension limits - 'CELLS', Min a, Max a, Min b, Max b, Min c, Max c (the number of rows of cells to be added in
        // negative and positive directions along the a, b, and c axes). Ignored on reading. On writing, min set to -1, max to +1.
        // This ensures that model is extended in POLYGRAF, ensuring any crystal bonding is not lost.
        outputFile << "CELLS 0    0    0    0    0    0\n";
    }

    // From here on, the BGF file looks the same whether it's periodic or not.

    // Format of the atomic data section in all its funky glory.
    outputFile << "FORMAT ATOM   (a6,1x,i5,1x,a5,1x,a3,1x,a1,1x,a5,3f10.5,1x,a5,i3,i2,1x,f8.5)\n";

    // Now we adhere to what we just printed as we output the atomic data.
    for (int i = 0; i < prometheusModel.getNumberOfAtoms(); i++)
    {

        outputFile << QString("HETATM %1 %2 %3 %4 %5%6%7%8 %9%10%11 %12").arg(i+1, 5).arg(AtomName[i], -5).arg(ResidueName, 3).arg(ChainName, 1).arg(ResidueNumber, 5).arg(PosX[i], 10, 'g', 5).arg(PosY[i], 10, 'g', 5).arg(PosZ[i], 10, 'g', 5).arg(FFType[i], -5).arg(MaxNumOfBonds, 3).arg(NumOfLonePairs, 2).arg(Charges[i], 8, 'f', 5) << endl;

    }


    // Format of the connectivity data section in all its funky glory.
    outputFile << "FORMAT CONECT (a6,12i6)\n";

    // Now we adhere to what we just printed as we output the conectivity data.
    for (int i = 0; i < prometheusModel.getNumberOfAtoms(); i++)
    {
        if (PrintedBondList[i].size() == 0)
        {
            outputFile << QString("CONECT%1").arg(i + 1, 6) << endl;
            outputFile << QString("ORDER %1").arg(i + 1, 6) << endl;
        }
        else if (PrintedBondList[i].size() == 1)
        {
            outputFile << QString("CONECT%1%2").arg(i + 1, 6).arg(PrintedBondList[i][0], 6) << endl;
            outputFile << QString("ORDER %1%2").arg(i + 1, 6).arg(PrintedBondOrder[i][0], 6) << endl;
        }
        else if (PrintedBondList[i].size() == 2)
        {
            outputFile << QString("CONECT%1%2%3").arg(i + 1, 6).arg(PrintedBondList[i][0], 6).arg(PrintedBondList[i][1], 6) << endl;
            outputFile << QString("ORDER %1%2%3").arg(i + 1, 6).arg(PrintedBondOrder[i][0], 6).arg(PrintedBondOrder[i][1], 6) << endl;
        }
        else if (PrintedBondList[i].size() == 3)
        {
            outputFile << QString("CONECT%1%2%3%4").arg(i + 1, 6).arg(PrintedBondList[i][0], 6).arg(PrintedBondList[i][1], 6).arg(PrintedBondList[i][2], 6) << endl;
            outputFile << QString("ORDER %1%2%3%4").arg(i + 1, 6).arg(PrintedBondOrder[i][0], 6).arg(PrintedBondOrder[i][1], 6).arg(PrintedBondOrder[i][2], 6) << endl;
        }
        else if (PrintedBondList[i].size() == 4)
        {
            outputFile << QString("CONECT%1%2%3%4%5").arg(i + 1, 6).arg(PrintedBondList[i][0], 6).arg(PrintedBondList[i][1], 6).arg(PrintedBondList[i][2], 6).arg(PrintedBondList[i][3], 6) << endl;
            outputFile << QString("ORDER %1%2%3%4%5").arg(i + 1, 6).arg(PrintedBondOrder[i][0], 6).arg(PrintedBondOrder[i][1], 6).arg(PrintedBondOrder[i][2], 6).arg(PrintedBondOrder[i][3], 6) << endl;
        }
        else if (PrintedBondList[i].size() == 5)
        {
            outputFile << QString("CONECT%1%2%3%4%5%6").arg(i + 1, 6).arg(PrintedBondList[i][0], 6).arg(PrintedBondList[i][1], 6).arg(PrintedBondList[i][2], 6).arg(PrintedBondList[i][3], 6).arg(PrintedBondList[i][4], 6) << endl;
            outputFile << QString("ORDER %1%2%3%4%5%6").arg(i + 1, 6).arg(PrintedBondOrder[i][0], 6).arg(PrintedBondOrder[i][1], 6).arg(PrintedBondOrder[i][2], 6).arg(PrintedBondOrder[i][3], 6).arg(PrintedBondOrder[i][4], 6) << endl;
        }
        else if (PrintedBondList[i].size() == 6)
        {
            outputFile << QString("CONECT%1%2%3%4%5%6%7").arg(i + 1, 6).arg(PrintedBondList[i][0], 6).arg(PrintedBondList[i][1], 6).arg(PrintedBondList[i][2], 6).arg(PrintedBondList[i][3], 6).arg(PrintedBondList[i][4], 6).arg(PrintedBondList[i][5], 6) << endl;
            outputFile << QString("ORDER %1%2%3%4%5%6%7").arg(i + 1, 6).arg(PrintedBondOrder[i][0], 6).arg(PrintedBondOrder[i][1], 6).arg(PrintedBondOrder[i][2], 6).arg(PrintedBondOrder[i][3], 6).arg(PrintedBondOrder[i][4], 6).arg(PrintedBondOrder[i][5], 6) << endl;
        }
        else if (PrintedBondList[i].size() == 7)
        {
            outputFile << QString("CONECT%1%2%3%4%5%6%7%8").arg(i + 1, 6).arg(PrintedBondList[i][0], 6).arg(PrintedBondList[i][1], 6).arg(PrintedBondList[i][2], 6).arg(PrintedBondList[i][3], 6).arg(PrintedBondList[i][4], 6).arg(PrintedBondList[i][5], 6).arg(PrintedBondList[i][6], 6) << endl;
            outputFile << QString("ORDER %1%2%3%4%5%6%7%8").arg(i + 1, 6).arg(PrintedBondOrder[i][0], 6).arg(PrintedBondOrder[i][1], 6).arg(PrintedBondOrder[i][2], 6).arg(PrintedBondOrder[i][3], 6).arg(PrintedBondOrder[i][4], 6).arg(PrintedBondOrder[i][5], 6).arg(PrintedBondOrder[i][6], 6) << endl;
        }
        else if (PrintedBondList[i].size() == 8)
        {
            outputFile << QString("CONECT%1%2%3%4%5%6%7%8%9").arg(i + 1, 6).arg(PrintedBondList[i][0], 6).arg(PrintedBondList[i][1], 6).arg(PrintedBondList[i][2], 6).arg(PrintedBondList[i][3], 6).arg(PrintedBondList[i][4], 6).arg(PrintedBondList[i][5], 6).arg(PrintedBondList[i][6], 6).arg(PrintedBondList[i][7], 6) << endl;
            outputFile << QString("ORDER %1%2%3%4%5%6%7%8%9").arg(i + 1, 6).arg(PrintedBondOrder[i][0], 6).arg(PrintedBondOrder[i][1], 6).arg(PrintedBondOrder[i][2], 6).arg(PrintedBondOrder[i][3], 6).arg(PrintedBondOrder[i][4], 6).arg(PrintedBondOrder[i][5], 6).arg(PrintedBondOrder[i][6], 6).arg(PrintedBondOrder[i][7], 6) << endl;
        }
        else if (PrintedBondList[i].size() == 9)
        {
            outputFile << QString("CONECT%1%2%3%4%5%6%7%8%9%10").arg(i + 1, 6).arg(PrintedBondList[i][0], 6).arg(PrintedBondList[i][1], 6).arg(PrintedBondList[i][2], 6).arg(PrintedBondList[i][3], 6).arg(PrintedBondList[i][4], 6).arg(PrintedBondList[i][5], 6).arg(PrintedBondList[i][6], 6).arg(PrintedBondList[i][7], 6).arg(PrintedBondList[i][8], 6) << endl;
            outputFile << QString("ORDER %1%2%3%4%5%6%7%8%9%10").arg(i + 1, 6).arg(PrintedBondOrder[i][0], 6).arg(PrintedBondOrder[i][1], 6).arg(PrintedBondOrder[i][2], 6).arg(PrintedBondOrder[i][3], 6).arg(PrintedBondOrder[i][4], 6).arg(PrintedBondOrder[i][5], 6).arg(PrintedBondOrder[i][6], 6).arg(PrintedBondOrder[i][7], 6).arg(PrintedBondOrder[i][8], 6) << endl;
        }
        else if (PrintedBondList[i].size() == 10)
        {
            outputFile << QString("CONECT%1%2%3%4%5%6%7%8%9%10%11").arg(i + 1, 6).arg(PrintedBondList[i][0], 6).arg(PrintedBondList[i][1], 6).arg(PrintedBondList[i][2], 6).arg(PrintedBondList[i][3], 6).arg(PrintedBondList[i][4], 6).arg(PrintedBondList[i][5], 6).arg(PrintedBondList[i][6], 6).arg(PrintedBondList[i][7], 6).arg(PrintedBondList[i][8], 6).arg(PrintedBondList[i][9], 6) << endl;
            outputFile << QString("ORDER %1%2%3%4%5%6%7%8%9%10%11").arg(i + 1, 6).arg(PrintedBondOrder[i][0], 6).arg(PrintedBondOrder[i][1], 6).arg(PrintedBondOrder[i][2], 6).arg(PrintedBondOrder[i][3], 6).arg(PrintedBondOrder[i][4], 6).arg(PrintedBondOrder[i][5], 6).arg(PrintedBondOrder[i][6], 6).arg(PrintedBondOrder[i][7], 6).arg(PrintedBondOrder[i][8], 6).arg(PrintedBondOrder[i][9], 6) << endl;
        }
        else if (PrintedBondList[i].size() == 11)
        {
            outputFile << QString("CONECT%1%2%3%4%5%6%7%8%9%10%11%12").arg(i + 1, 6).arg(PrintedBondList[i][0], 6).arg(PrintedBondList[i][1], 6).arg(PrintedBondList[i][2], 6).arg(PrintedBondList[i][3], 6).arg(PrintedBondList[i][4], 6).arg(PrintedBondList[i][5], 6).arg(PrintedBondList[i][6], 6).arg(PrintedBondList[i][7], 6).arg(PrintedBondList[i][8], 6).arg(PrintedBondList[i][9], 6).arg(PrintedBondList[i][10], 6) << endl;
            outputFile << QString("ORDER %1%2%3%4%5%6%7%8%9%10%11%12").arg(i + 1, 6).arg(PrintedBondOrder[i][0], 6).arg(PrintedBondOrder[i][1], 6).arg(PrintedBondOrder[i][2], 6).arg(PrintedBondOrder[i][3], 6).arg(PrintedBondOrder[i][4], 6).arg(PrintedBondOrder[i][5], 6).arg(PrintedBondOrder[i][6], 6).arg(PrintedBondOrder[i][7], 6).arg(PrintedBondOrder[i][8], 6).arg(PrintedBondOrder[i][9], 6).arg(PrintedBondOrder[i][10], 6) << endl;
        }
    }


    // And finally we place "END" as the last line.
    outputFile << "END\n";

    // And we're all done!
    file.close();
}

void Pandora::writeCIF()
{

}

void Pandora::writeCML()
{

}

void Pandora::writeLammpsInput(QString fileName)
{
  // This function is about 80% done.  But it won't complie in its current state.

//    //**************************************************************************************************
//    // Purpose: This function is able to write chemical data files adhering to LAMMPS input (.lammps)
//    // formatting.   The format is stores all data needed to start a simulation with the LAMMPS software
//    // package.  After a header section counting the number of items in this file, data is grouped
//    // according to section headers and then given as records.
//    //************************************************************************************************

//    QFile file (fileName);
//    file.open(QIODevice::WriteOnly | QIODevice::Text);
//    QTextStream outputFile (&file);

//    // First thing's first...  load in all available data on this model.

//    // Residue Number
//    QString ResidueNumber = "444";

//    // Position
//    QVector<float> PosX = prometheusModel.getAtomPositionX();
//    QVector<float> PosY = prometheusModel.getAtomPositionY();
//    QVector<float> PosZ = prometheusModel.getAtomPositionZ();

//    // Atomic Number and Force Field Type
//    QVector<int> Atoms = prometheusModel.getAtomAtomicNumber();
//    QVector<QString> FFType = prometheusModel.getAtomForceFieldType();

//    // Partial Charges
//    QVector<float> Charges = prometheusModel.getAtomPartialCharge();

//    // Connectivity Data... List of Bond Pairs, which is all that LAMMPS needs with regards to bond representation in the file
//    QVector<QVector<int> > BondList = prometheusModel.getBondList();
//    QVector<int> BondOrder = prometheusModel.getBondOrder();

//    // From here, we must perform additional calculations.  The LAMMPS input file does not merely hold structure...  it holds all neccessary parameters to assign energies
//    // to a given bond length, bond angle, dihedral, and inversion.  These values are not normally stored within the Atlas Framework because they aren't needed for
//    // display.  Therefore, we need to generate them here.

//    // Angles...  for every two bonds that share a central atom, we must create an angle entry for LAMMPS to track during the simulation.  AngleList is a vector of
//    // vectors.  Each vector holds three atom IDs, with the second atom being the central atom that belongs to two bonds.
//    QVector<QVector<int> > AngleList;
//    QVector<int> angle;
//    angle.resize(3);

//    for (int i = 0; i < prometheusModel.getNumberOfBonds(); i++)
//    {
//        // Cycle through all bond entries.  Compare this bond to all other bonds and search for bonds that share common atoms.

//        for (int j = 0; j < i; j++)
//        {
//            if (BondList[i][0] == BondList[j][0])
//            {
//                // The first atom in the bond entry, BondList[i][0], is the central atom in this angle.
//                angle[0] = BondList[i][1];
//                angle[1] = BondList[i][0];
//                angle[2] = BondList[j][1];

//                AngleList.push_back(angle);
//            }
//            else if (BondList[i][0] == BondList[j][1])
//            {
//                // The first atom in the bond entry, BondList[i][0], is the central atom in this angle.
//                angle[0] = BondList[i][1];
//                angle[1] = BondList[i][0];
//                angle[2] = BondList[j][0];

//                AngleList.push_back(angle);
//            }
//            else if (BondList[i][1] == BondList[j][0])
//            {
//                // The second atom in the bond entry, BondList[i][1], is the central atom in this angle.
//                angle[0] = BondList[i][0];
//                angle[1] = BondList[i][1];
//                angle[2] = BondList[j][1];

//                AngleList.push_back(angle);
//            }
//            else if (BondList[i][1] == BondList[j][1])
//            {
//                // The second atom in the bond entry, BondList[i][1], is the central atom in this angle.
//                angle[0] = BondList[i][0];
//                angle[1] = BondList[i][1];
//                angle[2] = BondList[j][0];

//                AngleList.push_back(angle);
//            }
//        }
//    }


//    // Dihedrals...  a given bond, we need to create a dihedral entry for every unique pair of atoms attached to either side of the bond.  LAMMPS will track their interactions
//    // during the simulation.  DihedralList is a vector of vectors.  Each vector holds four atom IDs, with the second and third atoms being those belonging to the given bond.
//    // DihedralCentralBondOrder holds the bond order of the center bond of the dihedral.... it is needed to identify the dihedral type.
//    QVector<QVector<int> > DihedralList;
//    QVector<int> DihedralCentralBondOrder;
//    QVector<int> dihedral;
//    dihedral.resize(4);

//    // The first step of this calcuation is to rearrange the bond data into a form that makes comparisons more efficient, even though it'll take up more memory.  BondsPerAtom
//    // is a vector of vectors...  each vector holds that atoms bonded to the atom at that index.
//    QVector<QVector<int> > BondsPerAtom;
//    BondsPerAtom.resize(prometheusModel.getNumberOfAtoms());
//    int atom2, atom3;

//    for (int i = 0; i < prometheusModel.getNumberOfBonds(); i++)
//    {
//        BondsPerAtom[BondList[i][0] - 1].push_back(BondList[i][1]);
//        BondsPerAtom[BondList[i][1] - 1].push_back(BondList[i][0]);
//    }


//    // Now we create dihedral entries.
//    for (int i = 0; i < prometheusModel.getNumberOfBonds(); i++)
//    {
//        // Cycle through all bonds.  Take each atom and cycle through all atoms connected to it...  compare to the results of the process to the second atom from the bond entry.
//        // We are looking for unique pairs of atoms for 1,4 interactions.

//        atom2 = BondList[i][0];
//        atom3 = BondList[i][1];
//        for (int j = 0; j < BondsPerAtom[atom2 - 1].size(); j++)
//        {
//            if (BondsPerAtom[atom2 - 1][j] != atom3)
//            {
//                for (int k = 0; k < BondsPerAtom[atom3 - 1].size(); k++)
//                {
//                    if (BondsPerAtom[atom3 - 1][k] != atom2)
//                    {
//                        // We have located a unique set of four atoms.
//                        dihedral[0] = BondsPerAtom[atom2 - 1][j];
//                        dihedral[1] = atom2;
//                        dihedral[2] = atom3;
//                        dihedral[3] = BondsPerAtom[atom3 - 1][k];

//                        DihedralList.push_back(dihedral);
//                        DihedralCentralBondOrder.push_back(BondOrder[i]);
//                    }
//                }
//            }
//        }
//    }


//    // Inversions (Impropers)...  We look for atoms that are bonded to exactly 3 other atoms.  LAMMPS tracks these atoms and applies an extra term to account for the
//    // energy barrier that must be overcome to simultaneously force all three bonds into or out of the same plane.
//    QVector<QVector<int> > InversionList;
//    QVector<int> inversion;
//    inversion.resize(4);

//    for (int i = 0; i < prometheusModel.getNumberOfAtoms(); i++)
//    {
//        // For DREIDING, the inversion terms don't need to be applied to atoms in the Nitrogen Group (N, P, As, Sb)
//        if (BondsPerAtom[i].size() == 3 && Atoms[i] != 7 && Atoms[i] != 15 && Atoms[i] != 33 && Atoms[i] != 51)
//        {
//            inversion[0] = i + 1;
//            inversion[1] = BondsPerAtom[i][0];
//            inversion[2] = BondsPerAtom[i][1];
//            inversion[3] = BondsPerAtom[i][2];

//            InversionList.push_back(inversion);
//        }
//    }


//    /*********************************************************************
//     *  All Bonds, Angles, Dihedrals, and Inversions have been identified
//     ********************************************************************/

//    // Now we need to loop through each category and count the number of types there are.  Each type will get a parameter loaded into the LAMMPS input file.

//    // Atoms
//    QVector<QString> TypesOfAtoms;
//    QVector<int> LammpsAtomTypeID;
//    LammpsAtomTypeID.resize(prometheusModel.getNumberOfAtoms());

//    for (int i = 0; i < prometheusModel.getNumberOfAtoms(); i++)
//    {
//        if (!TypesOfAtoms.contains(FFType[i]))
//        {
//            // If this is a unique atom type, then add it to the list.
//            TypesOfAtoms.push_back(FFType[i]);
//        }

//        // Assign the LAMMPS Type ID to the atom.
//        for (int j = 0; j < TypesOfAtoms.size(); j++)
//        {
//            if (TypesOfAtoms[j] == FFType[i])
//            {
//                LammpsAtomTypeID[i] = j + 1;
//            }
//        }
//    }


//    // Bonds...
//    QVector<QVector<QString> > TypesOfBonds;
//    QVector<int> LammpsBondTypeID;
//    LammpsBondTypeID.resize(prometheusModel.getNumberOfBonds());
//    QVector<QString> bondinput1, bondinput2, bondtest;
//    bondinput1.resize(2);
//    bondinput2.resize(2);
//    bondtest.resize(2);

//    for (int i = 0; i < prometheusModel.getNumberOfBonds(); i++)
//    {
//        //  Since we're not sure which order the atoms will be presented, we need to make sure we check both orders for a unique pairing of atom types.
//        bondinput1[0] = FFType[BondList[i][0] - 1];
//        bondinput1[1] = FFType[BondList[i][1] - 1];

//        bondinput2[0] = FFType[BondList[i][1] - 1];
//        bondinput2[1] = FFType[BondList[i][0] - 1];

//        if (!TypesOfBonds.contains(bondinput1) && !TypesOfBonds.contains(bondinput2))
//        {
//            // If this is a unique bond type, then add it to the list.
//            TypesOfBonds.push_back(bondinput1);
//        }

//        // Assign the LAMMPS Type ID to the bond.
//        for (int j = 0; j < TypesOfBonds.size(); j++)
//        {
//            bondtest[0] = TypesOfBonds[j][0];
//            bondtest[1] = TypesOfBonds[j][1];

//            if (bondtest == bondinput1 | bondtest == bondinput2)
//            {
//                LammpsBondTypeID[i] = j + 1;
//            }
//        }

//    }


//    // Angles...  only depend on the center atom of the angle.
//    QVector<QString> TypesOfAngles;
//    QVector<int> LammpsAngleTypeID;
//    LammpsAngleTypeID.resize(AngleList.size());

//    for (int i = 0; i < AngleList.size(); i++)
//    {
//        if (!TypesOfAngles.contains(FFType[AngleList[i][1]]))
//        {
//            // If this is a unique angle type, then add it to the list.
//            TypesOfAngles.push_back(FFType[AngleList[i][1]]);
//        }

//        // Assign the LAMMPS Type ID to the angle.
//        for (int j = 0; j < TypesOfAngles.size(); j++)
//        {
//            if (TypesOfAngles[j] == FFType[AngleList[i][1]])
//            {
//                LammpsAngleTypeID[i] = j + 1;
//            }
//        }
//    }


//    // Dihedrals...  For atoms IJKL involved in a dihedral, the paramters that describe the dihedral depend only on the central atoms, J and K.
//    // We are only concerned with the hybridization of these atoms, and not their element.
//    QVector<QString> TypesOfDihedrals, symmetryTest;
//    QVector<int> LammpsDihedralTypeID;
//    LammpsDihedralTypeID.resize(DihedralList.size());
//    QString atomType, dihedralTest;
//    int type1, type2, bondedAtom1, bondedAtom2, bondedAtom3;

//    // We will also need to know which atoms are participating in aromatic systems in this section.  We perform that search now.  The results are stored within AromaticAtoms.
//    QVector<QVector<int> > AromaticAtoms;
//    checkForAromaticity(Atoms, FFType, BondList, AromaticAtoms);

//    for (int i = 0; i < DihedralList.size(); i++)
//    {
//        // For a group of atoms involved in a dihedral, IJKL, we are only concerned with the central atoms, J and K.
//        // We start with J.

//        atomType = FFType[DihedralList[i][1] - 1];

//        if (atomType.size() == 2)
//        {
//            type1 = 1;
//        }
//        else if (atomType[2] == '1')
//        {
//            type1 = 1;
//        }
//        else if (atomType[2] == '2')
//        {
//            type1 = 2;
//        }
//        else if (atomType[2] == '3')
//        {
//            type1 = 3;
//        }
//        else if (atomType[2] == 'R')
//        {
//            type1 = 4;
//        }
//        else
//        {
//            type1 = 1;
//        }

//        // Now, we move onto K.

//        atomType = FFType[DihedralList[i][2] - 1];

//        if (atomType.size() == 2)
//        {
//            type2 = 1;
//        }
//        else if (atomType[2] == '1')
//        {
//            type2 = 1;
//        }
//        else if (atomType[2] == '2')
//        {
//            type2 = 2;
//        }
//        else if (atomType[2] == '3')
//        {
//            type2 = 3;
//        }
//        else if (atomType[2] == 'R')
//        {
//            type2 = 4;
//        }
//        else
//        {
//            type2 = 1;
//        }


//        /* Now that we have identified the hybrization of atoms J and K, we go through all the possible types of dihedrals possible in DREIDING.  These are defined
//        * in the Mayo/Olafson/Goddard paper.  What is described below is the hybridization of atom J bonded to the hybridization of atom K.  When we compile the list
//        * of types in TypesOfDihedrals. This vector will contain characters from the ID list below.
//        *
//        * a = sp3 single bonded to sp3
//        * b = sp2 single bonded to sp3 with 6-fold symmetry...  the sp2 atom is bonded to two more atoms with identical hybridization (both must be *_R).
//        * c = sp2 double bonded to sp2
//        * d = [ Resonance Atom (*_R) ] resonance bonded (bond order 1.5) to [ Resonance Atom (*_R) ]
//        * e = sp2 or [Resonance Atom (*_R) ] single bonded to sp2 or [ Resonance Atom (*_R) ]...  example, middle bond of butadiene.
//        * f = special case of (e)...  (*_R) single bonded to (*_R) where the bond is exocyclic and both atoms are aromatic.
//        * g = sp1 or metals bonded to anything.  The barrier is 0.0 kcal/mol, so really, we won't include dihedrals including these atoms.
//        *
//        * Now, we also need to look out for elements from oxygen's group.  The electron configuration is s2p4, so two p electrons are used in bonding,
//        * and the other two are free to engage in pi interactions that affect the torsional barrier.
//        *
//        * h = sp3 single bonded to sp3, when BOTH atoms belong to oxygen's group.
//        * i = sp3 of oxygen's group bonded to sp2 or [Resonance Atom (*_R)] from another group.
//        *
//        * And finally, we want to be careful when an sp2 (or *_R) atom is bonded to an sp3 atom.  The dihedral can have either 6-fold or 3-fold symmetry, depending on what the
//        * sp2 atom is bonded to.  If the sp2 atom is bonded to another sp2 (or resonance [*_R] atom), the dihedral should have 6-fold symmetry and fall under the ID (b) [Example: Acetate].
//        * If the sp2 atom is *NOT* bonded to another sp2 (or *_R), the dihedral has 3-fold symmetry and falls under this ID (j) [Example: Propene].
//        *
//        * j = sp3 single bonded to sp2 (*_R) with 3-fold symmetry....   the sp2 atom is bonded to two more atoms with different hybridization.
//        */

//        if (type1 == 3 && type2 == 3)
//        {
//            // Two sp3 atoms are bonded...  there are two possiblities.  Either both atoms belong to Oxygen's group [type (h) dihedral] or this is regular single bond [type (a) dihedral].
//            if ((Atoms[DihedralList[i][1] - 1] == 8 || Atoms[DihedralList[i][1] - 1] == 16 || Atoms[DihedralList[i][1] - 1] == 34 || Atoms[DihedralList[i][1] - 1] == 52) && (Atoms[DihedralList[i][2] - 1] == 8 || Atoms[DihedralList[i][2] - 1] == 16 || Atoms[DihedralList[i][2] - 1] == 34 || Atoms[DihedralList[i][2] - 1] == 52))
//            {
//                // This is a type (h) dihedral.
//                if (!TypesOfDihedrals.contains("h"))
//                {
//                    TypesOfDihedrals.push_back("h");
//                }

//                dihedralTest = "h";
//            }
//            else
//            {
//                // This is a type (a) dihedral.
//                if (!TypesOfDihedrals.contains("a"))
//                {
//                    TypesOfDihedrals.push_back("a");
//                }

//                dihedralTest = "a";
//            }
//        }
//        else if ((type1 == 2 && type2 == 2) && (DihedralCentralBondOrder[i] == 2))
//        {
//            // This is a true double bond.  Type (c) dihedral.
//            if (!TypesOfDihedrals.contains("c"))
//            {
//                TypesOfDihedrals.push_back("c");
//            }

//            dihedralTest = "c";
//        }
//        else if (type1 == 1 || type2 == 1)
//        {
//            // If either atom is sp1 hybridized or is a metal, then this is a type (g) dihedral... which has no torsion barrier.
//            if (!TypesOfDihedrals.contains("g"))
//            {
//                TypesOfDihedrals.push_back("g");
//            }

//            dihedralTest = "g";
//        }
//        else if (type1 == 3 || type2 == 3)
//        {
//            // After eliminating several possibilities, it gets harder from here. It isn't easy to tell what kind of dihedral is in play
//            // just by looking at the center atoms.  We'll need to expose them to several tests to determine what dihedral type applies.

//            // This section handles what happens when a sp3 atom is bonded to an sp2 or resonance atom.  We can use the OR operator above because we have eliminated
//            // other posiblities in the preceding if statements.

//            // There are three possibilities.
//            // 6-fold symmetry, type (b) dihedral
//            // 3-fold symmetry, type (j) dihedral
//            // special case... oxygen group sp3 atom bonded to an sp2 or *_R atom, type (i) dihedral.


//            if (type1 == 3)
//            {
//                // This if statement is executed if the sp3 atom is atom J of atoms I, J, K, and L in a dihedral.

//                // We check for atoms from the oxygen group first.
//                if (Atoms[DihedralList[i][1] - 1] == 8 || Atoms[DihedralList[i][1] - 1] == 16 || Atoms[DihedralList[i][1] - 1] == 34 || Atoms[DihedralList[i][1] - 1] == 52)
//                {
//                    // The sp3 atom belongs to an atom from oxygen's group.  This is a type (i) dihedral.
//                    if (!TypesOfDihedrals.contains("i"))
//                    {
//                        TypesOfDihedrals.push_back("i");
//                    }

//                    dihedralTest = "i";
//                }
//                else
//                {
//                    // The sp3 atom does not belong to oxygen's group, so look for either 3 or 6 fold symmetry.

//                    // We'll check out the atoms attached to atom K of this dihedral.
//                    bondedAtom1 = BondsPerAtom[DihedralList[i][2] - 1][0];
//                    bondedAtom2 = BondsPerAtom[DihedralList[i][2] - 1][1];
//                    bondedAtom3 = BondsPerAtom[DihedralList[i][2] - 1][2];

//                    symmetryTest.clear();

//                    if (bondedAtom1 != DihedralList[i][1])
//                    {
//                        atomType = FFType[bondedAtom1 - 1];
//                        if (atomType.size() > 2)
//                        {
//                            symmetryTest.push_back(atomType[2]);
//                        }
//                        else
//                        {
//                            symmetryTest.push_back(' ');
//                        }
//                    }

//                    if (bondedAtom2 != DihedralList[i][1])
//                    {
//                        atomType = FFType[bondedAtom2 - 1];
//                        if (atomType.size() > 2)
//                        {
//                            symmetryTest.push_back(atomType[2]);
//                        }
//                        else
//                        {
//                            symmetryTest.push_back(' ');
//                        }
//                    }

//                    if (bondedAtom3 != DihedralList[i][1])
//                    {
//                        atomType = FFType[bondedAtom3 - 1];
//                        if (atomType.size() > 2)
//                        {
//                            symmetryTest.push_back(atomType[2]);
//                        }
//                        else
//                        {
//                            symmetryTest.push_back(' ');
//                        }
//                    }


//                    // Now that we've examined each of the atoms bonded to the sp2 atom, we can compare them and come to a decision about this dihedral.
//                    if ((symmetryTest[0] == "R" || symmetryTest[0] == "2") && (symmetryTest[1] == "R" || symmetryTest[1] == "2"))
//                    {
//                        // This dihedral meets the criteria for 6-fold symmetry.  This is a type (b) dihedral.
//                        if (!TypesOfDihedrals.contains("b"))
//                        {
//                            TypesOfDihedrals.push_back("b");
//                        }

//                        dihedralTest = "b";
//                    }
//                    else
//                    {
//                        // This dihedral meets the criteria for 3-fold symmetry.  This is a type (j) dihedral.
//                        if (!TypesOfDihedrals.contains("j"))
//                        {
//                            TypesOfDihedrals.push_back("j");
//                        }

//                        dihedralTest = "j";
//                    }
//                }
//            }
//            else
//            {
//                // This if statement is executed if the sp3 atom is atom K of atoms I, J, K, and L in a dihedral.

//                // We check for atoms from the oxygen group first.
//                if (Atoms[DihedralList[i][2] - 1] == 8 || Atoms[DihedralList[i][2] - 1] == 16 || Atoms[DihedralList[i][2] - 1] == 34 || Atoms[DihedralList[i][2] - 1] == 52)
//                {
//                    // The sp3 atom belongs to an atom from oxygen's group.  This is a type (i) dihedral.
//                    if (!TypesOfDihedrals.contains("i"))
//                    {
//                        TypesOfDihedrals.push_back("i");
//                    }

//                    dihedralTest = "i";
//                }
//                else
//                {
//                    // The sp3 atom does not belong to oxygen's group, so look for either 3 or 6 fold symmetry.

//                    // We'll check out the atoms attached to atom J of this dihedral.
//                    bondedAtom1 = BondsPerAtom[DihedralList[i][1] - 1][0];
//                    bondedAtom2 = BondsPerAtom[DihedralList[i][1] - 1][1];
//                    bondedAtom3 = BondsPerAtom[DihedralList[i][1] - 1][2];

//                    symmetryTest.clear();

//                    if (bondedAtom1 != DihedralList[i][2])
//                    {
//                        atomType = FFType[bondedAtom1 - 1];
//                        if (atomType.size() > 2)
//                        {
//                            symmetryTest.push_back(atomType[2]);
//                        }
//                        else
//                        {
//                            symmetryTest.push_back(' ');
//                        }
//                    }

//                    if (bondedAtom2 != DihedralList[i][2])
//                    {
//                        atomType = FFType[bondedAtom2 - 1];
//                        if (atomType.size() > 2)
//                        {
//                            symmetryTest.push_back(atomType[2]);
//                        }
//                        else
//                        {
//                            symmetryTest.push_back(' ');
//                        }
//                    }

//                    if (bondedAtom3 != DihedralList[i][2])
//                    {
//                        atomType = FFType[bondedAtom3 - 1];
//                        if (atomType.size() > 2)
//                        {
//                            symmetryTest.push_back(atomType[2]);
//                        }
//                        else
//                        {
//                            symmetryTest.push_back(' ');
//                        }
//                    }


//                    // Now that we've examined each of the atoms bonded to the sp2 atom, we can compare them and come to a decision about this dihedral.
//                    if ((symmetryTest[0] == "R" || symmetryTest[0] == "2") && (symmetryTest[1] == "R" || symmetryTest[1] == "2"))
//                    {
//                        // This dihedral meets the criteria for 6-fold symmetry.  This is a type (b) dihedral.
//                        if (!TypesOfDihedrals.contains("b"))
//                        {
//                            TypesOfDihedrals.push_back("b");
//                        }

//                        dihedralTest = "b";
//                    }
//                    else
//                    {
//                        // This dihedral meets the criteria for 3-fold symmetry.  This is a type (j) dihedral.
//                        if (!TypesOfDihedrals.contains("j"))
//                        {
//                            TypesOfDihedrals.push_back("j");
//                        }

//                        dihedralTest = "j";
//                    }
//                }
//            }

//        }
//        else
//        {
//            // And now we're left with everything else.  In this case two resonance (*_R) atoms are bonded, but there are a couple of possiblities as to how the
//            // dihedral turns out.

//            // (1) This is a true 1.5 bond order dihedral.  This occurs when two resonance atoms are bonded by a double bond formally (Example: the double bond dihedrals of
//            //         butadiene).  This also occurs to bonds within the main ring of an aromatic system (Example: carbons within a benzene ring would have this dihedral type along the
//            //         CC bond).  This is a type (d) dihedral.
//            // (2) This kind of dihedral is less stiff.  It occurs when two resonance atoms are bonded by a mere single bond and one is not part of an aromatic system
//            //         (Example:  The center CC bond of butadiene).  The formal bond order of 1 is taken to mean the pi electrons have less interaction across this bond.
//            //         This is a type (e) dihedral.
//            // (3) This is a special case of a type (e) dihedral.  It occurs when two resonance atoms are bonded by a mere single bond, and this bond is *EXTRACYCLIC* (not part
//            //         part of an aromatic ring) and yet *BOTH ATOMS ARE AROMATIC* (Example: The center bond connecting two benzene rings in biphenyl).  This is a type (f) dihedral.


//            // Figuring out if a bond belongs to type (d) or (e) is relatively easy.  The problem is aromaticity.  We have to correct any type (e) dihedrals that are part of
//            // aromatic systems so they become type (d).  We also have to use aromaticity checks to look for type (f) dihedrals.

//            // That means we first have to check for aromaticity.  Because we don't want to perform a scan of the whole system for each dihedral, we've already performed this search
//            // and stored the results in AromaticAtoms.

//        }



//        // Now that we've figured out which kind of dihedral this is according to the Mayo/Olafson/Goddard paper, we need to match up this type to its LAMMPS ID
//        // in this file.  Cycle through the current list of identified dihedral types until we get the LAMMPS ID number for this one.
//        for (int j = 0; j < TypesOfDihedrals.size(); j++)
//        {
//            if (TypesOfDihedrals[j] == dihedralTest)
//            {
//                LammpsDihedralTypeID[i] = j + 1;
//                break;
//            }
//        }


//    }


//    // And now all Atoms, Bond, Angles, and Dihedrals have been typed.

//    /**********************************
//    * Now we begin writing the file.
//    **********************************/

//    // A short descriptive remark of our choosing...  or maybe a little shameless self promotion?  :)
//    outputFile << "LAMMPS Input created by Prometheus - The Open Chemical Physics Suite on " << QDateTime::currentDateTime().toString() << endl << endl;

//    // The first section must always be a header giving counts of what is contained within.
//    outputFile << QString("%1 atoms\n").arg(prometheusModel.getNumberOfAtoms(), 15);
//    outputFile << QString("%1 bonds\n").arg(prometheusModel.getNumberOfBonds(), 15);
//    outputFile << QString("%1 angles\n").arg(AngleList.size(), 15);
//    outputFile << QString("%1 dihedrals\n").arg(DihedralList.size(), 15);
//    outputFile << QString("%1 impropers\n\n").arg(InversionList.size(), 15);

//    // This is followed by a count of the number of types of each category.
//
}

void Pandora::writeMOL()
{

}

void Pandora::writeMOL2()
{

}

void Pandora::writeMSI()
{

}

void Pandora::writePBD()
{

}

void Pandora::writeXYZ()
{

}



/******************************************************************************************************************
* Atom Typing Functions
******************************************************************************************************************/
void Pandora::assignFFTypes(int ForceField)
{
    if (ForceField == 3)
    {
        typeDREIDING();
    }
    else
    {
        QString message = "Apologies.  The selected force field is not yet supported.";
        pandoraError(message);
    }
}

void Pandora::typeAIREBO()
{

}

void Pandora::typeAMBER()
{

}

void Pandora::typeCHARMM()
{

}

void Pandora::typeDREIDING()
{
    /*******************************************************************************************************************
     * DREIDING is a non-reactive force field developed by Mayo, Olafson, and Goddard circa 1990.  While not useful for
     * Transition Metals, DREIDING has proven effective as a generic force field for elements considered "interesting".
     * DREIDING can model organic, biological, and inorganic Main Group molecules by using simple rules to represent
     * bond stretching, angle bending, diehdral torsions, and inversions.
     *******************************************************************************************************************/

    // Get model data...  start with a list element types.
    QVector<int> Atoms = prometheusModel.getAtomAtomicNumber();
    QVector<QString> FFTypes = prometheusModel.getAtomForceFieldType();

    // We also need connectivity data...  but it is currently stored as a list of bond pairs...  we need to be able to quickly determine how
    // many bonds a given atom has and what they are.  Grab the data and reorganize it for our current needs.
    QVector<QVector<int> > BondPairs = prometheusModel.getBondList();
    QVector<int> BondPairOrders = prometheusModel.getBondOrder();

    QVector<QVector<int> > FullBondList;
    FullBondList.resize(prometheusModel.getNumberOfAtoms());
    QVector<QVector<int> > FullBondOrderList;
    FullBondOrderList.resize(prometheusModel.getNumberOfAtoms());

    // The full list makes it easier to count bonded neighbors, but we must store data twice in our array.
    for (int i = 0; i < prometheusModel.getNumberOfBonds(); i++)
    {
        FullBondList[BondPairs[i][0] - 1].push_back(BondPairs[i][1]);
        FullBondOrderList[BondPairs[i][0] - 1].push_back(BondPairOrders[i]);

        FullBondList[BondPairs[i][1] - 1].push_back(BondPairs[i][0]);
        FullBondOrderList[BondPairs[i][1] - 1].push_back(BondPairOrders[i]);
    }

    // And finally, since some typing can only be determined by knowing the neighboring types, we'll need to keep a tally of which atoms we've
    // processed.  In this array, false is un-typed, true is typed.
    QVector<bool> atomTyped;
    atomTyped.resize(prometheusModel.getNumberOfAtoms());
    for (int i = 0; i < prometheusModel.getNumberOfAtoms(); i++)
    {
        atomTyped[i] = false;
    }

    // Now we are ready to type all atoms.  Let's start with the easy ones...  The atoms that have only one force field type
    // can be identified simply by their element.
    for (int i = 0; i < prometheusModel.getNumberOfAtoms(); i++)
    {
        if (Atoms[i] == 1 | Atoms[i] == 6 | Atoms[i] == 7 | Atoms[i] == 8)
        {
            // Since atoms are generally likely to be H, C, or O, we can look for them and skip the following statments.
        }
        else if (Atoms[i] == 9)
        {
            prometheusModel.setAtomForceFieldType(i+1, "F_");
            atomTyped[i] = true;
        }
        else if (Atoms[i] == 13)
        {
            prometheusModel.setAtomForceFieldType(i+1, "Al3");
            atomTyped[i] = true;
        }
        else if (Atoms[i] == 14)
        {
            prometheusModel.setAtomForceFieldType(i+1, "Si3");
            atomTyped[i] = true;
        }
        else if (Atoms[i] == 15)
        {
            prometheusModel.setAtomForceFieldType(i+1, "P_3");
            atomTyped[i] = true;
        }
        else if (Atoms[i] == 16)
        {
            prometheusModel.setAtomForceFieldType(i+1, "S_3");
            atomTyped[i] = true;
        }
        else if (Atoms[i] == 17)
        {
            prometheusModel.setAtomForceFieldType(i+1, "Cl");
            atomTyped[i] = true;
        }
        else if (Atoms[i] == 31)
        {
            prometheusModel.setAtomForceFieldType(i+1, "Ga3");
            atomTyped[i] = true;
        }
        else if (Atoms[i] == 32)
        {
            prometheusModel.setAtomForceFieldType(i+1, "Ge3");
            atomTyped[i] = true;
        }
        else if (Atoms[i] == 33)
        {
            prometheusModel.setAtomForceFieldType(i+1, "As3");
            atomTyped[i] = true;
        }
        else if (Atoms[i] == 34)
        {
            prometheusModel.setAtomForceFieldType(i+1, "Se3");
            atomTyped[i] = true;
        }
        else if (Atoms[i] == 35)
        {
            prometheusModel.setAtomForceFieldType(i+1, "Br");
            atomTyped[i] = true;
        }
        else if (Atoms[i] == 49)
        {
            prometheusModel.setAtomForceFieldType(i+1, "In3");
            atomTyped[i] = true;
        }
        else if (Atoms[i] == 50)
        {
            prometheusModel.setAtomForceFieldType(i+1, "Sn3");
            atomTyped[i] = true;
        }
        else if (Atoms[i] == 51)
        {
            prometheusModel.setAtomForceFieldType(i+1, "Sb3");
            atomTyped[i] = true;
        }
        else if (Atoms[i] == 52)
        {
            prometheusModel.setAtomForceFieldType(i+1, "Te3");
            atomTyped[i] = true;
        }
        else if (Atoms[i] == 53)
        {
            prometheusModel.setAtomForceFieldType(i+1, "I_");
            atomTyped[i] = true;
        }
        else if (Atoms[i] == 11)
        {
            prometheusModel.setAtomForceFieldType(i+1, "Na");
            atomTyped[i] = true;
        }
        else if (Atoms[i] == 20)
        {
            prometheusModel.setAtomForceFieldType(i+1, "Ca");
            atomTyped[i] = true;
        }
        else if (Atoms[i] == 26)
        {
            prometheusModel.setAtomForceFieldType(i+1, "Fe");
            atomTyped[i] = true;
        }
        else if (Atoms[i] == 30)
        {
            prometheusModel.setAtomForceFieldType(i+1, "Zn");
            atomTyped[i] = true;
        }
    }

    // Now we move onto atoms that are more difficult.  They are identified by their element, but they also possess a modifier
    // indicating valence hybridization.  It can be determined from connectivity and bond order.  First, we'll go through every atom
    // left and type it.

    int hybridOrbitals;

    for (int i = 0; i < prometheusModel.getNumberOfAtoms(); i++)
    {
        if (!atomTyped[i])
        {
            if (Atoms[i] == 1)
            {
                // Hydrogen...  It's special because it can be either a regular hydrogen bonded
                // to one atom, a bridging hydrogen bonded to two atoms, or a hydrogen that is capable of
                // participating in a hydrogen bond (bonded to nitrogen, oxygen, or fluorine).

                // Check number of bonds
                qDebug() << BondPairs;
                qDebug() << FullBondList;
                qDebug() << i << FullBondList[i];
                if (FullBondList[i].size() == 1)
                {
                    // One bond, check to see if this is a hydrogen bonding hydrogen
                    if (Atoms[FullBondList[i][0] - 1] == 7 | Atoms[FullBondList[i][0] - 1] == 8 | Atoms[FullBondList[i][0] - 1] == 9)
                    {
                        // This is a hydrogen bonding hydrogen.
                        prometheusModel.setAtomForceFieldType(i+1, "H_HB");
                        atomTyped[i] = true;
                    }
                    else
                    {
                        // This is a typical hydrogen
                        prometheusModel.setAtomForceFieldType(i+1, "H_");
                        atomTyped[i] = true;
                    }
                }
                else
                {
                    // Two bonds, this is a bridging hydrogen.
                    prometheusModel.setAtomForceFieldType(i+1, "H_b");
                    atomTyped[i] = true;
                }
            }
            else if (Atoms[i] == 5)
            {
                // Boron...  Either sp3 or sp2 hybridized
                if (FullBondList[i].size() == 3)
                {
                    prometheusModel.setAtomForceFieldType(i+1, "B_2");
                    atomTyped[i] = true;
                }
                else
                {
                    prometheusModel.setAtomForceFieldType(i+1, "B_3");
                    atomTyped[i] = true;
                }
            }
            else if (Atoms[i] == 6)
            {
                // Carbon...  we ignore resonance for now and only type according to bond order.  This means that even after this round
                // of typing, we do not count these atoms as finished (atomTyped[bool])...  we'll come back to them in a final round.

                hybridOrbitals = 3;
                for (int j = 0; j < FullBondOrderList[i].size(); j++)
                {
                    if (FullBondOrderList[i][j] == 3)
                    {
                        // Triple bonds force two orbitals to adopt pi-behavior and keep them from hybridizing.
                        hybridOrbitals -= 2;
                    }
                    else if (FullBondOrderList[i][j] == 2)
                    {
                        // Double bonds force one oribital to adopt pi-behavior keep it from hybridizing.
                        hybridOrbitals -= 1;
                    }
                }


                // Using the number of orbitals available for hybridization, we can assign labels.
                if (hybridOrbitals == 3)
                {
                    prometheusModel.setAtomForceFieldType(i+1, "C_3");
                    atomTyped[i] = true;
                }
                else if (hybridOrbitals == 2)
                {
                    // We need to check this atom for participation in resonance structures.
                    prometheusModel.setAtomForceFieldType(i+1, "C_2");
                }
                else
                {
                    prometheusModel.setAtomForceFieldType(i+1, "C_1");
                    atomTyped[i] = true;
                }
            }
            else if (Atoms[i] == 7)
            {
                // Nitrogen...  we ignore resonance for now and only type according to bond order.  This means that even after this round
                // of typing, we do not count these atoms as finished (atomTyped[bool])...  we'll come back to them in a final round.

                hybridOrbitals = 3;
                for (int j = 0; j < FullBondOrderList[i].size(); j++)
                {
                    if (FullBondOrderList[i][j] == 3)
                    {
                        // Triple bonds force two orbitals to adopt pi-behavior and keep them from hybridizing.
                        hybridOrbitals -= 2;
                    }
                    else if (FullBondOrderList[i][j] == 2)
                    {
                        // Double bonds force one oribital to adopt pi-behavior keep it from hybridizing.
                        hybridOrbitals -= 1;
                    }
                }


                // Using the number of orbitals available for hybridization, we can assign labels.
                if (hybridOrbitals == 3)
                {
                    prometheusModel.setAtomForceFieldType(i+1, "N_3");
                }
                else if (hybridOrbitals == 2)
                {
                    prometheusModel.setAtomForceFieldType(i+1, "N_2");
                }
                else
                {
                    prometheusModel.setAtomForceFieldType(i+1, "N_1");
                    atomTyped[i] = true;  // Triple bonded nitrogen cannot donate into the pi system in a way designated by the *_R modifier.
                }
            }
            else if (Atoms[i] == 8)
            {
                // Oxygen...  we ignore resonance for now and only type according to bond order.  This means that even after this round
                // of typing, we do not count these atoms as finished (atomTyped[bool])...  we'll come back to them in a final round.

                hybridOrbitals = 3;
                for (int j = 0; j < FullBondOrderList[i].size(); j++)
                {
                    if (FullBondOrderList[i][j] == 3)
                    {
                        // Triple bonds force two orbitals to adopt pi-behavior and keep them from hybridizing.
                        hybridOrbitals -= 2;
                    }
                    else if (FullBondOrderList[i][j] == 2)
                    {
                        // Double bonds force one oribital to adopt pi-behavior keep it from hybridizing.
                        hybridOrbitals -= 1;
                    }
                }


                // Using the number of orbitals available for hybridization, we can assign labels.
                if (hybridOrbitals == 3)
                {
                    prometheusModel.setAtomForceFieldType(i+1, "O_3");
                }
                else if (hybridOrbitals == 2)
                {
                    prometheusModel.setAtomForceFieldType(i+1, "O_2");
                }
                else
                {
                    prometheusModel.setAtomForceFieldType(i+1, "O_1");
                    atomTyped[i] = true;  // Triple bonded oxygen is no able to contribute to the pi system in the way the *_R modifier designates.
                }
            }
        }
    }

    // Now that the first-glance force field types have been assigned for these atoms, we need to go back and look for resonance structures
    // and assign the *_R modifier (resonance structures involving sp3 and sp2 hybridization) to properly use those parameters laid out in the
    // DREIDING force field.  Only elements C, N, and O display this behavior.

    QVector <int> Frag, newFrag, newNeighbor;
    int saveFragSize = 0;
    QVector<int>::Iterator it;
    int test;

    for (int i = 0; i < prometheusModel.getNumberOfAtoms(); i++)
    {

        // What we need to do is build chains of atoms that are capable of forming resonance structures.  All atoms have been typed, save those whose
        // final type is in question.  We search through these for neighboring atoms who can possibly share their electrons in a delocalized pi system.
        // These chains can also take the form of cyclic structures.
        //
        // Carbon atoms can do this if they are sp2 hybridized since one orbital is locked in a pi-bond. (We ignore possible non-coplanar orientations of the pi-electrons
        // since we can't really check without optimized geometry, which requires a force field...  we leave these tricky situations to QM methods).
        // Oxygen and nitrogen atoms can participate because they have lone pairs to donate into the pi-system, if they have 0 formal charge or are sp3 or sp2 hybridized.

        if (!atomTyped[i])
        {
            // This atom will serve as a starting point for our fragment.
            Frag.push_back(i + 1);
            newFrag.push_back(i + 1);

            // We enter a recursive loop seraching neighbors for eligible atoms until we don't find any more.
            while (saveFragSize != Frag.size())
            {
                saveFragSize = Frag.size();

                // Search any atoms added from the last cycle for eligible neighbors (newFrag)...  we will need to keep track of these
                // new neighbors, because they will become the elements of newFrag in the next cycle.
                newNeighbor.clear();

                for (it = newFrag.begin(); it != newFrag.end(); it++)
                {
                    for (int j = 0; j < FullBondList[*it - 1].size(); j++)
                    {
                        // Check to make sure it's eligible and that we don't already have it (cyclic structures can be a bitch).
                        if (!atomTyped[FullBondList[*it - 1][j] - 1] && !Frag.contains(FullBondList[*it - 1][j]))
                        {
                            Frag.push_back(FullBondList[*it - 1][j]);
                            newNeighbor.push_back(FullBondList[*it - 1][j]);
                        }
                    }

                }

                // We move onto the next cycle in this recursive function.  Whatever new neighbors we captured become the new part of the
                // fragment (newFrag) to search.  We couldn't modify newFrag before because we were iterating through it, but now, we
                // can alter its values.
                newFrag = newNeighbor;
            }

            // Now that we're done with the search, we look at our results.  Resonance only applies to groups of three or more atoms.
            if (Frag.size() >= 3)
            {
                // We've got a winner.  Apply the *_R modifier to all these atoms.
                for (int j = 0; j < Frag.size(); j++)
                {
                    if (Atoms[Frag[j] - 1] == 6)
                    {
                        // Carbon
                        prometheusModel.setAtomForceFieldType(Frag[j], "C_R");
                    }
                    if (Atoms[Frag[j] - 1] == 7)
                    {
                        // Nitrogen
                        prometheusModel.setAtomForceFieldType(Frag[j], "N_R");
                    }
                    if (Atoms[Frag[j] - 1] == 8)
                    {
                        // Oxygen
                        prometheusModel.setAtomForceFieldType(Frag[j], "O_R");
                    }

                    atomTyped[Frag[j] - 1] = true;
                }
            }
            else
            {
                // The atoms were eligible, but there were not enough to participate in a resonance structure.
                for (int j = 0; j < Frag.size(); j++)
                {
                    atomTyped[Frag[j] - 1] = true;
                }
            }
        }
    }

    QString message = "Force field typing is complete.  All atoms conform to the DREIDING atom types.";
    pandoraInfo(message);
}

void Pandora::typeGAFF()
{

}

void Pandora::typeReaxFF()
{

}

void Pandora::typeREBO()
{

}

void Pandora::typeUFF()
{

}




/******************************************************************************************************************
* Book Keeping Functions
******************************************************************************************************************/

int Pandora::findAtomicNumber(QString forceFieldType)
{
    /******************************************************************************************
     * The purpose of this function is to package all the code that takes:
     * An input string of an element, as defined by one of many possible force-field types,
     * and ouputs:
     * A text string containing an elemental symbol.
     * Example:  C_3 -> C
     *           H__A -> H
     *
     * We'll have to parse a block of raw text, but in the end, we match it up to an element
     * and return that element's atomic number.
     *******************************************************************************************/

    QString fieldCatcher;
    int element;
    QString::Iterator iter;

    for (iter = forceFieldType.begin(); iter != forceFieldType.end(); ++iter)
    {
        if (*iter != ' ')
        {
            fieldCatcher.append(*iter);
        }
    }

   ///////////////////////////
    // DREIDING Force-field
   ///////////////////////////
    if (fieldCatcher == "H_" | fieldCatcher == "H___A")
    {
        element = 1;
    }
    else if (fieldCatcher == "C_3" | fieldCatcher == "C_2" | fieldCatcher == "C_R")
    {
        element = 6;
    }
    else if (fieldCatcher == "O_3" | fieldCatcher == "O_2" | fieldCatcher == "O_R")
    {
        element = 8;
    }

    ///////////////////////////
     // F3C Water Force-field
    ///////////////////////////
    else if (fieldCatcher == "H_F")
    {
        element = 1;
    }
    else if (fieldCatcher == "O_3F")
    {
        element = 8;
    }
    else
    {
        element = 0;
    }

    return element;
}

QString Pandora::findElementalSymbol(int atomicNumber)
{
    // This function allows us to match up an atomic number to the elemental symbol if we have to display it for
    // some reason...  generally it's easier to work with integers, so the atoms are reffered to by their atomic
    // numbers within the program.

    return elementLookup[atomicNumber];
}

QVector<float> Pandora::findRadii(QVector<int> &ListOfAtomicNumbers)
{
    // This function takes an array listing the atomic number of every atom contained within a model.
    // This function returns a corresponding radius for every atom based on data contained within the Pandora
    // Framework.  The setup is like this to accomodate future radii defintions, van der Waals, atomic, User defined, etc.

    QVector<float> ListOfRadii;
    ListOfRadii.resize(ListOfAtomicNumbers.size());

    for (int i = 0; i < ListOfAtomicNumbers.size(); i++)
    {
        ListOfRadii[i] = vanderWaalsLookup[ListOfAtomicNumbers[i]];
    }

    return ListOfRadii;
}

QVector<QVector<float> > Pandora::findColor(QVector<int> &ListOfAtomicNumbers)
{
    // The purpose of this function is to accept an array of model data...  the element of every atom is
    // listed...  we use this data to determine the appropriate color for that atom.  The colors are represented
    // by a vector of three float values representing Red, Green, and Blue (RGB) components.  The final output is
    // a vector containg all these vectors.

    QVector<int>::Iterator it;
    QVector<float> color;
    QVector<QVector<float> > ListOfColors;

    color.resize(3);

    // Cycle through all atoms listed in the input vector and add the result to ListOfColors;
    for (it = ListOfAtomicNumbers.begin(); it != ListOfAtomicNumbers.end(); it++)
    {
        color[0] = colorLookup[*it][0] / 255;
        color[1] = colorLookup[*it][1] / 255;
        color[2] = colorLookup[*it][2] / 255;

        ListOfColors.push_back(color);
    }

    return ListOfColors;
}


void Pandora::findBondData(QVector<QVector<int> > ListOfBonds, int activeModel, QVector<float> &BondLengths, QVector<QVector<float> > &BondCenters, QVector<QVector<float> > &BondNormals)
{
    /**************************************************************************************************************************
     * This function takes an array of atom IDs in bond pairs...  There are several calculations to be done here...
     * We need:
     * 1) The length of the bond.
     * 2) The 3D coordinates of the bond center.
     * 3a) The vector normal to the bond.
     * 3b) The angle between the positive z-axis and the bond normal.
     *
     * The calculations are performed on each bond pair, and then stored in arrays together by category.  To "return" multiple
     * arrays back to the function asking for these values, empty arrays that will hold the desired answers are passed by
     * reference and filled with the push_back function here.
     **************************************************************************************************************************/

    float x, y, z, length, angle;
    QVector<float> center, normal;
    center.resize(3);
    normal.resize(3);


    // First thing's first... get the position data.
    QVector<float> posX = prometheusModel.getAtomPositionX();
    QVector<float> posY = prometheusModel.getAtomPositionY();
    QVector<float> posZ = prometheusModel.getAtomPositionZ();

    // Loop through the whole list and gather the needed data.
    for (int i = 0; i < ListOfBonds.size(); i++)
    {
        // We start with the length because it is the easiest to calculate.

        x = posX[ListOfBonds[i][1] - 1] - posX[ListOfBonds[i][0] - 1];
        y = posY[ListOfBonds[i][1] - 1] - posY[ListOfBonds[i][0] - 1];
        z = posZ[ListOfBonds[i][1] - 1] - posZ[ListOfBonds[i][0] - 1];

        length = qSqrt(x*x + y*y + z*z);

        // Place the result into the array passed by reference to this function.
        BondLengths.push_back(length);


        // Next, we move onto bond centers...  we need to determine the 3D coordinates of the midpoint.
        center[0] = (posX[ListOfBonds[i][0] - 1] + posX[ListOfBonds[i][1] - 1]) / 2;
        center[1] = (posY[ListOfBonds[i][0] - 1] + posY[ListOfBonds[i][1] - 1]) / 2;
        center[2] = (posZ[ListOfBonds[i][0] - 1] + posZ[ListOfBonds[i][1] - 1]) / 2;

        // Place the result into the array passed by reference to this function.
        BondCenters.push_back(center);


        // Next, we move onto bond normals...

        /*************************************************************************************************************************************
        * We need to specify an axis of rotation when creating cylinders in OpenGL.
        * Cylinders are created along the positive Z axis by default, but we'd like them to be aligned with the bond vector, please.
        * The axis of rotation is the vector normal to plane formed by the 1) positive z axis and 2) bond vector.
        *
        * We require a cross product, but because one of the vectors is known [0,0,1], the result is simple.
        *
        * normal[0] = -y;
        * normal[1] = x;
        * normal[2] = 0;
        *
        * Also, we need to know the angle between the positive z-axis and the bond vector for rotation.
        * cos(angle) = (dot product) / (product of magnitudes)
        * dot product = x*0 + y*0 + z*1
        * product of magnitudes = (length) * (1)
        *
        * angle = qAcos(z/length) * 180 / 3.14159
        *************************************************************************************************************************************/

        angle = qAcos(z/length) * 57.29583;

        // Because normal[2] is always 0, we skip its inclusion and store the angle we'd like to rotate around the normal vector.
        normal[0] = -y;
        normal[1] = x;
        normal[2] = angle;

        // Place the result into the array passed by reference to this function.
        BondNormals.push_back(normal);
    }

}

void Pandora::nameAllAtoms()
{
    /**************************************************************************************************************************
     * This function is used to determine the proper name of all atoms according to the convention
     * [Element Name][Number of Times That Element Has Occured]...  this function is to be called
     * just before writing a data file or drawing labels to the screen since the atom IDs can change around depending on
     * what actions the User is performing.
     **************************************************************************************************************************/

    QVector<int> counter, AtomicNumbers;
    QString element, finalString;


    AtomicNumbers = prometheusModel.getAtomAtomicNumber();

    for (int i = 0; i < 119; i++)
    {
        counter.push_back(1);
    }

    for (int i = 0; i < AtomicNumbers.size(); i++)
    {
        element = findElementalSymbol(AtomicNumbers[i]);

        finalString = element + QString::number(counter[AtomicNumbers[i]]);

        prometheusModel.setAtomName(i + 1, finalString);

        counter[AtomicNumbers[i]] += 1;
    }

}



//void Pandora::checkForAromaticity(QVector<int> &Atoms, QVector<QString> &FFType, QVector<QVector<int> > &BondList, QVector<int> &AromaticAtoms)
//{
//    /**************************************************************************************************************************
//     * This function is used to find all atoms participating in aromatic rings within this system.  These data are useful in
//     * determining force field assignments and parameters.  A list of aromatic atoms is returned in the vector AromaticAtoms.
//     **************************************************************************************************************************/

//    QString AtomType;
//    QVector <int> candidateAromaticAtoms;


//    // First we will narrow the scope of our search.  Aromaticity can only occur among atoms that are displaying resonance properties, which has already been determined
//    // during atom type assignement.

//    for (int i = 0; i < prometheusModel.getNumberOfAtoms(); i++)
//    {
//        AtomType = FFType[i];
//        if (AtomType[2] == 'R')
//        {
//            // We add this atom to the list of possible aromatic atoms.
//            candidateAromaticAtoms.push_back(i + 1);
//        }
//    }

//    // Now that these have been identified

//}
