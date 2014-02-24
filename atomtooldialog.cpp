#include "atomtooldialog.h"
#include "ui_atomtooldialog.h"

AtomToolDialog::AtomToolDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AtomToolDialog)
{
    ui->setupUi(this);

    setupStyleSheets();
}

AtomToolDialog::~AtomToolDialog()
{
    delete ui;
}



void AtomToolDialog::setupStyleSheets()
{
    // This function sets up style sheets for the buttons representing elements in the perodic table.
    // Non-metals = Green
    // Alkali Metals = Red
    // Alkaline Earth Metals = Orange
    // Transition Metals = Pink
    // Lanthanides = Fushia
    // Actinides = Plum
    // Post-Transition Metals = Grey
    // Metalloids = Chartuse
    // Hallogens = Yellow
    // Noble Gases = Sky Blue

    // Come back to this when there is more time.
    // ui->H_button->setStyleSheet("background-color: green");
}




void AtomToolDialog::on_H_button_clicked()
{
    emit requestBuildElementChange(1);
}

void AtomToolDialog::on_He_button_clicked()
{
    emit requestBuildElementChange(2);
}

void AtomToolDialog::on_Li_button_clicked()
{
    emit requestBuildElementChange(3);
}

void AtomToolDialog::on_Be_button_clicked()
{
    emit requestBuildElementChange(4);
}

void AtomToolDialog::on_B_button_clicked()
{
    emit requestBuildElementChange(5);
}

void AtomToolDialog::on_C_button_clicked()
{
    emit requestBuildElementChange(6);
}

void AtomToolDialog::on_N_button_clicked()
{
    emit requestBuildElementChange(7);
}

void AtomToolDialog::on_O_button_clicked()
{
    emit requestBuildElementChange(8);
}

void AtomToolDialog::on_F_button_clicked()
{
    emit requestBuildElementChange(9);
}

void AtomToolDialog::on_Ne_button_clicked()
{
    emit requestBuildElementChange(10);
}

void AtomToolDialog::on_Na_button_clicked()
{
    emit requestBuildElementChange(11);
}

void AtomToolDialog::on_Mg_button_clicked()
{
    emit requestBuildElementChange(12);
}

void AtomToolDialog::on_Al_button_clicked()
{
    emit requestBuildElementChange(13);
}

void AtomToolDialog::on_Si_button_clicked()
{
    emit requestBuildElementChange(14);
}

void AtomToolDialog::on_P_button_clicked()
{
    emit requestBuildElementChange(15);
}

void AtomToolDialog::on_S_button_clicked()
{
    emit requestBuildElementChange(16);
}

void AtomToolDialog::on_Cl_button_clicked()
{
    emit requestBuildElementChange(17);
}

void AtomToolDialog::on_Ar_button_clicked()
{
    emit requestBuildElementChange(18);
}

void AtomToolDialog::on_K_button_clicked()
{
    emit requestBuildElementChange(19);
}

void AtomToolDialog::on_Ca_button_clicked()
{
    emit requestBuildElementChange(20);
}

void AtomToolDialog::on_Sc_button_clicked()
{
    emit requestBuildElementChange(21);
}

void AtomToolDialog::on_Ti_button_clicked()
{
    emit requestBuildElementChange(22);
}

void AtomToolDialog::on_V_button_clicked()
{
    emit requestBuildElementChange(23);
}

void AtomToolDialog::on_Cr_button_clicked()
{
    emit requestBuildElementChange(24);
}

void AtomToolDialog::on_Mn_button_clicked()
{
    emit requestBuildElementChange(25);
}

void AtomToolDialog::on_Fe_button_clicked()
{
    emit requestBuildElementChange(26);
}

void AtomToolDialog::on_Co_button_clicked()
{
    emit requestBuildElementChange(27);
}

void AtomToolDialog::on_Ni_button_clicked()
{
    emit requestBuildElementChange(28);
}

void AtomToolDialog::on_Cu_button_clicked()
{
    emit requestBuildElementChange(29);
}

void AtomToolDialog::on_Zn_button_clicked()
{
    emit requestBuildElementChange(30);
}

void AtomToolDialog::on_Ga_button_clicked()
{
    emit requestBuildElementChange(31);
}

void AtomToolDialog::on_Ge_button_clicked()
{
    emit requestBuildElementChange(32);
}

void AtomToolDialog::on_As_button_clicked()
{
    emit requestBuildElementChange(33);
}

void AtomToolDialog::on_Se_button_clicked()
{
    emit requestBuildElementChange(34);
}

void AtomToolDialog::on_Br_button_clicked()
{
    emit requestBuildElementChange(35);
}

void AtomToolDialog::on_Kr_button_clicked()
{
    emit requestBuildElementChange(36);
}

void AtomToolDialog::on_Rb_button_clicked()
{
    emit requestBuildElementChange(37);
}

void AtomToolDialog::on_Sr_button_clicked()
{
    emit requestBuildElementChange(38);
}

void AtomToolDialog::on_Y_button_clicked()
{
    emit requestBuildElementChange(39);
}

void AtomToolDialog::on_Zr_button_clicked()
{
    emit requestBuildElementChange(40);
}

void AtomToolDialog::on_Nb_button_clicked()
{
    emit requestBuildElementChange(41);
}

void AtomToolDialog::on_Mo_button_clicked()
{
    emit requestBuildElementChange(42);
}

void AtomToolDialog::on_Tc_button_clicked()
{
    emit requestBuildElementChange(43);
}

void AtomToolDialog::on_Ru_button_clicked()
{
    emit requestBuildElementChange(44);
}

void AtomToolDialog::on_Rh_button_clicked()
{
    emit requestBuildElementChange(45);
}

void AtomToolDialog::on_Pd_button_clicked()
{
    emit requestBuildElementChange(46);
}

void AtomToolDialog::on_Ag_button_clicked()
{
    emit requestBuildElementChange(47);
}

void AtomToolDialog::on_Cd_button_clicked()
{
    emit requestBuildElementChange(48);
}

void AtomToolDialog::on_In_button_clicked()
{
    emit requestBuildElementChange(49);
}

void AtomToolDialog::on_Sn_button_clicked()
{
    emit requestBuildElementChange(50);
}

void AtomToolDialog::on_Sb_button_clicked()
{
    emit requestBuildElementChange(51);
}

void AtomToolDialog::on_Te_button_clicked()
{
    emit requestBuildElementChange(52);
}

void AtomToolDialog::on_I_button_clicked()
{
    emit requestBuildElementChange(53);
}

void AtomToolDialog::on_Xe_button_clicked()
{
    emit requestBuildElementChange(54);
}

void AtomToolDialog::on_Cs_button_clicked()
{
    emit requestBuildElementChange(55);
}

void AtomToolDialog::on_Ba_button_clicked()
{
    emit requestBuildElementChange(56);
}

void AtomToolDialog::on_La_button_clicked()
{
    emit requestBuildElementChange(57);
}

void AtomToolDialog::on_Ce_button_clicked()
{
    emit requestBuildElementChange(58);
}

void AtomToolDialog::on_Pr_button_clicked()
{
    emit requestBuildElementChange(59);
}

void AtomToolDialog::on_Nd_button_clicked()
{
    emit requestBuildElementChange(60);
}

void AtomToolDialog::on_Pm_button_clicked()
{
    emit requestBuildElementChange(61);
}

void AtomToolDialog::on_Sm_button_clicked()
{
    emit requestBuildElementChange(62);
}

void AtomToolDialog::on_Eu_button_clicked()
{
    emit requestBuildElementChange(63);
}

void AtomToolDialog::on_Gd_button_clicked()
{
    emit requestBuildElementChange(64);
}

void AtomToolDialog::on_Tb_button_clicked()
{
    emit requestBuildElementChange(65);
}

void AtomToolDialog::on_Dy_button_clicked()
{
    emit requestBuildElementChange(66);
}

void AtomToolDialog::on_Ho_button_clicked()
{
    emit requestBuildElementChange(67);
}

void AtomToolDialog::on_Er_button_clicked()
{
    emit requestBuildElementChange(68);
}

void AtomToolDialog::on_Tm_button_clicked()
{
    emit requestBuildElementChange(69);
}

void AtomToolDialog::on_Yb_button_clicked()
{
    emit requestBuildElementChange(70);
}

void AtomToolDialog::on_Lu_button_clicked()
{
    emit requestBuildElementChange(71);
}

void AtomToolDialog::on_Hf_button_clicked()
{
    emit requestBuildElementChange(72);
}

void AtomToolDialog::on_Ta_button_clicked()
{
    emit requestBuildElementChange(73);
}

void AtomToolDialog::on_W_button_clicked()
{
    emit requestBuildElementChange(74);
}

void AtomToolDialog::on_Re_button_clicked()
{
    emit requestBuildElementChange(75);
}

void AtomToolDialog::on_Os_button_clicked()
{
    emit requestBuildElementChange(76);
}

void AtomToolDialog::on_Ir_button_clicked()
{
    emit requestBuildElementChange(77);
}

void AtomToolDialog::on_Pt_button_clicked()
{
    emit requestBuildElementChange(78);
}

void AtomToolDialog::on_Au_button_clicked()
{
    emit requestBuildElementChange(79);
}

void AtomToolDialog::on_Hg_button_clicked()
{
    emit requestBuildElementChange(80);
}

void AtomToolDialog::on_Tl_button_clicked()
{
    emit requestBuildElementChange(81);
}

void AtomToolDialog::on_Pb_button_clicked()
{
    emit requestBuildElementChange(82);
}

void AtomToolDialog::on_Bi_button_clicked()
{
    emit requestBuildElementChange(83);
}

void AtomToolDialog::on_Po_button_clicked()
{
    emit requestBuildElementChange(84);
}

void AtomToolDialog::on_At_button_clicked()
{
    emit requestBuildElementChange(85);
}

void AtomToolDialog::on_Rn_button_clicked()
{
    emit requestBuildElementChange(86);
}

void AtomToolDialog::on_Fr_button_clicked()
{
    emit requestBuildElementChange(87);
}

void AtomToolDialog::on_Ra_button_clicked()
{
    emit requestBuildElementChange(88);
}

void AtomToolDialog::on_Ac_button_clicked()
{
    emit requestBuildElementChange(89);
}

void AtomToolDialog::on_Th_button_clicked()
{
    emit requestBuildElementChange(90);
}

void AtomToolDialog::on_Pa_button_clicked()
{
    emit requestBuildElementChange(91);
}

void AtomToolDialog::on_U_button_clicked()
{
    emit requestBuildElementChange(92);
}

void AtomToolDialog::on_Np_button_clicked()
{
    emit requestBuildElementChange(93);
}

void AtomToolDialog::on_Pu_button_clicked()
{
    emit requestBuildElementChange(94);
}

void AtomToolDialog::on_Am_button_clicked()
{
    emit requestBuildElementChange(95);
}

void AtomToolDialog::on_Cm_button_clicked()
{
    emit requestBuildElementChange(96);
}

void AtomToolDialog::on_Bk_button_clicked()
{
    emit requestBuildElementChange(97);
}

void AtomToolDialog::on_Cf_button_clicked()
{
    emit requestBuildElementChange(98);
}

void AtomToolDialog::on_Es_button_clicked()
{
    emit requestBuildElementChange(99);
}

void AtomToolDialog::on_Fm_button_clicked()
{
    emit requestBuildElementChange(100);
}

void AtomToolDialog::on_Md_button_clicked()
{
    emit requestBuildElementChange(101);
}

void AtomToolDialog::on_No_button_clicked()
{
    emit requestBuildElementChange(102);
}

void AtomToolDialog::on_Lr_button_clicked()
{
    emit requestBuildElementChange(103);
}

void AtomToolDialog::on_Rf_button_clicked()
{
    emit requestBuildElementChange(104);
}

void AtomToolDialog::on_Db_button_clicked()
{
    emit requestBuildElementChange(105);
}

void AtomToolDialog::on_Sg_button_clicked()
{
    emit requestBuildElementChange(106);
}

void AtomToolDialog::on_Bh_button_clicked()
{
    emit requestBuildElementChange(107);
}

void AtomToolDialog::on_Hs_button_clicked()
{
    emit requestBuildElementChange(108);
}

void AtomToolDialog::on_Mt_button_clicked()
{
    emit requestBuildElementChange(109);
}

void AtomToolDialog::on_Ds_button_clicked()
{
    emit requestBuildElementChange(110);
}

void AtomToolDialog::on_Rg_button_clicked()
{
    emit requestBuildElementChange(111);
}

void AtomToolDialog::on_Cn_button_clicked()
{
    emit requestBuildElementChange(112);
}

void AtomToolDialog::on_Uut_button_clicked()
{
    emit requestBuildElementChange(113);
}

void AtomToolDialog::on_Fl_button_clicked()
{
    emit requestBuildElementChange(114);
}

void AtomToolDialog::on_Uup_button_clicked()
{
    emit requestBuildElementChange(115);
}

void AtomToolDialog::on_Lv_button_clicked()
{
    emit requestBuildElementChange(116);
}

void AtomToolDialog::on_Uus_button_clicked()
{
    emit requestBuildElementChange(117);
}

void AtomToolDialog::on_Uuo_button_clicked()
{
    emit requestBuildElementChange(118);
}




void AtomToolDialog::on_CloseButton_clicked()
{
    this->close();
}

void AtomToolDialog::on_comboBox_currentIndexChanged(int index)
{
    if (index == 0)
    {
        ui->H_button->setEnabled(true);
        ui->He_button->setEnabled(true);
        ui->Li_button->setEnabled(true);
        ui->Be_button->setEnabled(true);
        ui->B_button->setEnabled(true);
        ui->C_button->setEnabled(true);
        ui->N_button->setEnabled(true);
        ui->O_button->setEnabled(true);
        ui->F_button->setEnabled(true);
        ui->Ne_button->setEnabled(true);

        ui->Na_button->setEnabled(true);
        ui->Mg_button->setEnabled(true);
        ui->Al_button->setEnabled(true);
        ui->Si_button->setEnabled(true);
        ui->P_button->setEnabled(true);
        ui->S_button->setEnabled(true);
        ui->Cl_button->setEnabled(true);
        ui->Ar_button->setEnabled(true);
        ui->K_button->setEnabled(true);
        ui->Ca_button->setEnabled(true);

        ui->Sc_button->setEnabled(true);
        ui->Ti_button->setEnabled(true);
        ui->V_button->setEnabled(true);
        ui->Cr_button->setEnabled(true);
        ui->Mn_button->setEnabled(true);
        ui->Fe_button->setEnabled(true);
        ui->Co_button->setEnabled(true);
        ui->Ni_button->setEnabled(true);
        ui->Cu_button->setEnabled(true);
        ui->Zn_button->setEnabled(true);

        ui->Ga_button->setEnabled(true);
        ui->Ge_button->setEnabled(true);
        ui->As_button->setEnabled(true);
        ui->Se_button->setEnabled(true);
        ui->Br_button->setEnabled(true);
        ui->Kr_button->setEnabled(true);
        ui->Rb_button->setEnabled(true);
        ui->Sr_button->setEnabled(true);
        ui->Y_button->setEnabled(true);
        ui->Zr_button->setEnabled(true);

        ui->Nb_button->setEnabled(true);
        ui->Mo_button->setEnabled(true);
        ui->Tc_button->setEnabled(true);
        ui->Ru_button->setEnabled(true);
        ui->Rh_button->setEnabled(true);
        ui->Pd_button->setEnabled(true);
        ui->Ag_button->setEnabled(true);
        ui->Cd_button->setEnabled(true);
        ui->In_button->setEnabled(true);
        ui->Sn_button->setEnabled(true);

        ui->Sb_button->setEnabled(true);
        ui->Te_button->setEnabled(true);
        ui->I_button->setEnabled(true);
        ui->Xe_button->setEnabled(true);
        ui->Cs_button->setEnabled(true);
        ui->Ba_button->setEnabled(true);
        ui->La_button->setEnabled(true);
        ui->Ce_button->setEnabled(true);
        ui->Pr_button->setEnabled(true);
        ui->Nd_button->setEnabled(true);

        ui->Pm_button->setEnabled(true);
        ui->Sm_button->setEnabled(true);
        ui->Eu_button->setEnabled(true);
        ui->Gd_button->setEnabled(true);
        ui->Tb_button->setEnabled(true);
        ui->Dy_button->setEnabled(true);
        ui->Ho_button->setEnabled(true);
        ui->Er_button->setEnabled(true);
        ui->Tm_button->setEnabled(true);
        ui->Yb_button->setEnabled(true);

        ui->Lu_button->setEnabled(true);
        ui->Hf_button->setEnabled(true);
        ui->Ta_button->setEnabled(true);
        ui->W_button->setEnabled(true);
        ui->Re_button->setEnabled(true);
        ui->Os_button->setEnabled(true);
        ui->Ir_button->setEnabled(true);
        ui->Pt_button->setEnabled(true);
        ui->Au_button->setEnabled(true);
        ui->Hg_button->setEnabled(true);

        ui->Tl_button->setEnabled(true);
        ui->Pb_button->setEnabled(true);
        ui->Bi_button->setEnabled(true);
        ui->Po_button->setEnabled(true);
        ui->At_button->setEnabled(true);
        ui->Rn_button->setEnabled(true);
        ui->Fr_button->setEnabled(true);
        ui->Ra_button->setEnabled(true);
        ui->Ac_button->setEnabled(true);
        ui->Th_button->setEnabled(true);

        ui->Pa_button->setEnabled(true);
        ui->U_button->setEnabled(true);
        ui->Np_button->setEnabled(true);
        ui->Pu_button->setEnabled(true);
        ui->Am_button->setEnabled(true);
        ui->Cm_button->setEnabled(true);
        ui->Bk_button->setEnabled(true);
        ui->Cf_button->setEnabled(true);
        ui->Es_button->setEnabled(true);
        ui->Fm_button->setEnabled(true);

        ui->Md_button->setEnabled(true);
        ui->No_button->setEnabled(true);
        ui->Lr_button->setEnabled(true);
        ui->Rf_button->setEnabled(true);
        ui->Db_button->setEnabled(true);
        ui->Sg_button->setEnabled(true);
        ui->Bh_button->setEnabled(true);
        ui->Hs_button->setEnabled(true);
        ui->Mt_button->setEnabled(true);
        ui->Ds_button->setEnabled(true);

        ui->Rg_button->setEnabled(true);
        ui->Cn_button->setEnabled(true);
        ui->Uut_button->setEnabled(true);
        ui->Fl_button->setEnabled(true);
        ui->Uup_button->setEnabled(true);
        ui->Lv_button->setEnabled(true);
        ui->Uus_button->setEnabled(true);
        ui->Uuo_button->setEnabled(true);
    }
    else if (index == 4)
    {
        ui->H_button->setEnabled(true);
        ui->He_button->setDisabled(true);
        ui->Li_button->setDisabled(true);
        ui->Be_button->setDisabled(true);
        ui->B_button->setEnabled(true);
        ui->C_button->setEnabled(true);
        ui->N_button->setEnabled(true);
        ui->O_button->setEnabled(true);
        ui->F_button->setEnabled(true);
        ui->Ne_button->setDisabled(true);

        ui->Na_button->setEnabled(true);
        ui->Mg_button->setDisabled(true);
        ui->Al_button->setEnabled(true);
        ui->Si_button->setEnabled(true);
        ui->P_button->setEnabled(true);
        ui->S_button->setEnabled(true);
        ui->Cl_button->setEnabled(true);
        ui->Ar_button->setDisabled(true);
        ui->K_button->setDisabled(true);
        ui->Ca_button->setEnabled(true);

        ui->Sc_button->setDisabled(true);
        ui->Ti_button->setDisabled(true);
        ui->V_button->setDisabled(true);
        ui->Cr_button->setDisabled(true);
        ui->Mn_button->setDisabled(true);
        ui->Fe_button->setEnabled(true);
        ui->Co_button->setDisabled(true);
        ui->Ni_button->setDisabled(true);
        ui->Cu_button->setDisabled(true);
        ui->Zn_button->setEnabled(true);

        ui->Ga_button->setEnabled(true);
        ui->Ge_button->setEnabled(true);
        ui->As_button->setEnabled(true);
        ui->Se_button->setEnabled(true);
        ui->Br_button->setEnabled(true);
        ui->Kr_button->setDisabled(true);
        ui->Rb_button->setDisabled(true);
        ui->Sr_button->setDisabled(true);
        ui->Y_button->setDisabled(true);
        ui->Zr_button->setDisabled(true);

        ui->Nb_button->setDisabled(true);
        ui->Mo_button->setDisabled(true);
        ui->Tc_button->setDisabled(true);
        ui->Ru_button->setDisabled(true);
        ui->Rh_button->setDisabled(true);
        ui->Pd_button->setDisabled(true);
        ui->Ag_button->setDisabled(true);
        ui->Cd_button->setDisabled(true);
        ui->In_button->setEnabled(true);
        ui->Sn_button->setEnabled(true);

        ui->Sb_button->setEnabled(true);
        ui->Te_button->setEnabled(true);
        ui->I_button->setEnabled(true);
        ui->Xe_button->setDisabled(true);
        ui->Cs_button->setDisabled(true);
        ui->Ba_button->setDisabled(true);
        ui->La_button->setDisabled(true);
        ui->Ce_button->setDisabled(true);
        ui->Pr_button->setDisabled(true);
        ui->Nd_button->setDisabled(true);

        ui->Pm_button->setDisabled(true);
        ui->Sm_button->setDisabled(true);
        ui->Eu_button->setDisabled(true);
        ui->Gd_button->setDisabled(true);
        ui->Tb_button->setDisabled(true);
        ui->Dy_button->setDisabled(true);
        ui->Ho_button->setDisabled(true);
        ui->Er_button->setDisabled(true);
        ui->Tm_button->setDisabled(true);
        ui->Yb_button->setDisabled(true);

        ui->Lu_button->setDisabled(true);
        ui->Hf_button->setDisabled(true);
        ui->Ta_button->setDisabled(true);
        ui->W_button->setDisabled(true);
        ui->Re_button->setDisabled(true);
        ui->Os_button->setDisabled(true);
        ui->Ir_button->setDisabled(true);
        ui->Pt_button->setDisabled(true);
        ui->Au_button->setDisabled(true);
        ui->Hg_button->setDisabled(true);

        ui->Tl_button->setDisabled(true);
        ui->Pb_button->setDisabled(true);
        ui->Bi_button->setDisabled(true);
        ui->Po_button->setDisabled(true);
        ui->At_button->setDisabled(true);
        ui->Rn_button->setDisabled(true);
        ui->Fr_button->setDisabled(true);
        ui->Ra_button->setDisabled(true);
        ui->Ac_button->setDisabled(true);
        ui->Th_button->setDisabled(true);

        ui->Pa_button->setDisabled(true);
        ui->U_button->setDisabled(true);
        ui->Np_button->setDisabled(true);
        ui->Pu_button->setDisabled(true);
        ui->Am_button->setDisabled(true);
        ui->Cm_button->setDisabled(true);
        ui->Bk_button->setDisabled(true);
        ui->Cf_button->setDisabled(true);
        ui->Es_button->setDisabled(true);
        ui->Fm_button->setDisabled(true);

        ui->Md_button->setDisabled(true);
        ui->No_button->setDisabled(true);
        ui->Lr_button->setDisabled(true);
        ui->Rf_button->setDisabled(true);
        ui->Db_button->setDisabled(true);
        ui->Sg_button->setDisabled(true);
        ui->Bh_button->setDisabled(true);
        ui->Hs_button->setDisabled(true);
        ui->Mt_button->setDisabled(true);
        ui->Ds_button->setDisabled(true);

        ui->Rg_button->setDisabled(true);
        ui->Cn_button->setDisabled(true);
        ui->Uut_button->setDisabled(true);
        ui->Fl_button->setDisabled(true);
        ui->Uup_button->setDisabled(true);
        ui->Lv_button->setDisabled(true);
        ui->Uus_button->setDisabled(true);
        ui->Uuo_button->setDisabled(true);
    }
    else
    {
        ui->H_button->setDisabled(true);
        ui->He_button->setDisabled(true);
        ui->Li_button->setDisabled(true);
        ui->Be_button->setDisabled(true);
        ui->B_button->setDisabled(true);
        ui->C_button->setDisabled(true);
        ui->N_button->setDisabled(true);
        ui->O_button->setDisabled(true);
        ui->F_button->setDisabled(true);
        ui->Ne_button->setDisabled(true);

        ui->Na_button->setDisabled(true);
        ui->Mg_button->setDisabled(true);
        ui->Al_button->setDisabled(true);
        ui->Si_button->setDisabled(true);
        ui->P_button->setDisabled(true);
        ui->S_button->setDisabled(true);
        ui->Cl_button->setDisabled(true);
        ui->Ar_button->setDisabled(true);
        ui->K_button->setDisabled(true);
        ui->Ca_button->setDisabled(true);

        ui->Sc_button->setDisabled(true);
        ui->Ti_button->setDisabled(true);
        ui->V_button->setDisabled(true);
        ui->Cr_button->setDisabled(true);
        ui->Mn_button->setDisabled(true);
        ui->Fe_button->setDisabled(true);
        ui->Co_button->setDisabled(true);
        ui->Ni_button->setDisabled(true);
        ui->Cu_button->setDisabled(true);
        ui->Zn_button->setDisabled(true);

        ui->Ga_button->setDisabled(true);
        ui->Ge_button->setDisabled(true);
        ui->As_button->setDisabled(true);
        ui->Se_button->setDisabled(true);
        ui->Br_button->setDisabled(true);
        ui->Kr_button->setDisabled(true);
        ui->Rb_button->setDisabled(true);
        ui->Sr_button->setDisabled(true);
        ui->Y_button->setDisabled(true);
        ui->Zr_button->setDisabled(true);

        ui->Nb_button->setDisabled(true);
        ui->Mo_button->setDisabled(true);
        ui->Tc_button->setDisabled(true);
        ui->Ru_button->setDisabled(true);
        ui->Rh_button->setDisabled(true);
        ui->Pd_button->setDisabled(true);
        ui->Ag_button->setDisabled(true);
        ui->Cd_button->setDisabled(true);
        ui->In_button->setDisabled(true);
        ui->Sn_button->setDisabled(true);

        ui->Sb_button->setDisabled(true);
        ui->Te_button->setDisabled(true);
        ui->I_button->setDisabled(true);
        ui->Xe_button->setDisabled(true);
        ui->Cs_button->setDisabled(true);
        ui->Ba_button->setDisabled(true);
        ui->La_button->setDisabled(true);
        ui->Ce_button->setDisabled(true);
        ui->Pr_button->setDisabled(true);
        ui->Nd_button->setDisabled(true);

        ui->Pm_button->setDisabled(true);
        ui->Sm_button->setDisabled(true);
        ui->Eu_button->setDisabled(true);
        ui->Gd_button->setDisabled(true);
        ui->Tb_button->setDisabled(true);
        ui->Dy_button->setDisabled(true);
        ui->Ho_button->setDisabled(true);
        ui->Er_button->setDisabled(true);
        ui->Tm_button->setDisabled(true);
        ui->Yb_button->setDisabled(true);

        ui->Lu_button->setDisabled(true);
        ui->Hf_button->setDisabled(true);
        ui->Ta_button->setDisabled(true);
        ui->W_button->setDisabled(true);
        ui->Re_button->setDisabled(true);
        ui->Os_button->setDisabled(true);
        ui->Ir_button->setDisabled(true);
        ui->Pt_button->setDisabled(true);
        ui->Au_button->setDisabled(true);
        ui->Hg_button->setDisabled(true);

        ui->Tl_button->setDisabled(true);
        ui->Pb_button->setDisabled(true);
        ui->Bi_button->setDisabled(true);
        ui->Po_button->setDisabled(true);
        ui->At_button->setDisabled(true);
        ui->Rn_button->setDisabled(true);
        ui->Fr_button->setDisabled(true);
        ui->Ra_button->setDisabled(true);
        ui->Ac_button->setDisabled(true);
        ui->Th_button->setDisabled(true);

        ui->Pa_button->setDisabled(true);
        ui->U_button->setDisabled(true);
        ui->Np_button->setDisabled(true);
        ui->Pu_button->setDisabled(true);
        ui->Am_button->setDisabled(true);
        ui->Cm_button->setDisabled(true);
        ui->Bk_button->setDisabled(true);
        ui->Cf_button->setDisabled(true);
        ui->Es_button->setDisabled(true);
        ui->Fm_button->setDisabled(true);

        ui->Md_button->setDisabled(true);
        ui->No_button->setDisabled(true);
        ui->Lr_button->setDisabled(true);
        ui->Rf_button->setDisabled(true);
        ui->Db_button->setDisabled(true);
        ui->Sg_button->setDisabled(true);
        ui->Bh_button->setDisabled(true);
        ui->Hs_button->setDisabled(true);
        ui->Mt_button->setDisabled(true);
        ui->Ds_button->setDisabled(true);

        ui->Rg_button->setDisabled(true);
        ui->Cn_button->setDisabled(true);
        ui->Uut_button->setDisabled(true);
        ui->Fl_button->setDisabled(true);
        ui->Uup_button->setDisabled(true);
        ui->Lv_button->setDisabled(true);
        ui->Uus_button->setDisabled(true);
        ui->Uuo_button->setDisabled(true);
    }
}
