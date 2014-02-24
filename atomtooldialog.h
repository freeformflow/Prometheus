#ifndef ATOMTOOLDIALOG_H
#define ATOMTOOLDIALOG_H

#include <QDialog>

namespace Ui {
class AtomToolDialog;
}

class AtomToolDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit AtomToolDialog(QWidget *parent = 0);
    ~AtomToolDialog();
    
private:
    Ui::AtomToolDialog *ui;

signals:
    void requestBuildElementChange(int atomicNumber);
private slots:

    // Sets up colors for the periodic table
    void setupStyleSheets();



    // These functions all emit a singal to the viewer when one of the element buttons are selected.
    void on_H_button_clicked();
    void on_He_button_clicked();

    void on_Li_button_clicked();
    void on_Be_button_clicked();
    void on_B_button_clicked();
    void on_C_button_clicked();
    void on_N_button_clicked();
    void on_O_button_clicked();
    void on_F_button_clicked();
    void on_Ne_button_clicked();

    void on_Na_button_clicked();
    void on_Mg_button_clicked();
    void on_Al_button_clicked();
    void on_Si_button_clicked();
    void on_P_button_clicked();
    void on_S_button_clicked();
    void on_Cl_button_clicked();
    void on_Ar_button_clicked();

    void on_K_button_clicked();
    void on_Ca_button_clicked();
    void on_Sc_button_clicked();
    void on_Ti_button_clicked();
    void on_V_button_clicked();
    void on_Cr_button_clicked();
    void on_Mn_button_clicked();
    void on_Fe_button_clicked();
    void on_Co_button_clicked();
    void on_Ni_button_clicked();
    void on_Cu_button_clicked();
    void on_Zn_button_clicked();
    void on_Ga_button_clicked();
    void on_Ge_button_clicked();
    void on_As_button_clicked();
    void on_Se_button_clicked();
    void on_Br_button_clicked();
    void on_Kr_button_clicked();

    void on_Rb_button_clicked();
    void on_Sr_button_clicked();
    void on_Y_button_clicked();
    void on_Zr_button_clicked();
    void on_Nb_button_clicked();
    void on_Mo_button_clicked();
    void on_Tc_button_clicked();
    void on_Ru_button_clicked();
    void on_Rh_button_clicked();
    void on_Pd_button_clicked();
    void on_Ag_button_clicked();
    void on_Cd_button_clicked();
    void on_In_button_clicked();
    void on_Sn_button_clicked();
    void on_Sb_button_clicked();
    void on_Te_button_clicked();
    void on_I_button_clicked();
    void on_Xe_button_clicked();
    void on_Cs_button_clicked();
    void on_Ba_button_clicked();
    void on_La_button_clicked();
    void on_Ce_button_clicked();
    void on_Pr_button_clicked();
    void on_Nd_button_clicked();
    void on_Pm_button_clicked();
    void on_Sm_button_clicked();
    void on_Eu_button_clicked();
    void on_Gd_button_clicked();
    void on_Tb_button_clicked();
    void on_Dy_button_clicked();
    void on_Ho_button_clicked();
    void on_Er_button_clicked();
    void on_Tm_button_clicked();
    void on_Yb_button_clicked();
    void on_Lu_button_clicked();
    void on_Hf_button_clicked();
    void on_Ta_button_clicked();
    void on_W_button_clicked();
    void on_Re_button_clicked();
    void on_Os_button_clicked();
    void on_Ir_button_clicked();
    void on_Pt_button_clicked();
    void on_Au_button_clicked();
    void on_Hg_button_clicked();
    void on_Tl_button_clicked();
    void on_Pb_button_clicked();
    void on_Bi_button_clicked();
    void on_Po_button_clicked();
    void on_At_button_clicked();
    void on_Rn_button_clicked();

    void on_Fr_button_clicked();
    void on_Ra_button_clicked();
    void on_Ac_button_clicked();
    void on_Th_button_clicked();
    void on_Pa_button_clicked();
    void on_U_button_clicked();
    void on_Np_button_clicked();
    void on_Pu_button_clicked();
    void on_Am_button_clicked();
    void on_Cm_button_clicked();
    void on_Bk_button_clicked();
    void on_Cf_button_clicked();
    void on_Es_button_clicked();
    void on_Fm_button_clicked();
    void on_Md_button_clicked();
    void on_No_button_clicked();
    void on_Lr_button_clicked();
    void on_Rf_button_clicked();
    void on_Db_button_clicked();
    void on_Sg_button_clicked();
    void on_Bh_button_clicked();
    void on_Hs_button_clicked();
    void on_Mt_button_clicked();
    void on_Ds_button_clicked();
    void on_Rg_button_clicked();
    void on_Cn_button_clicked();
    void on_Uut_button_clicked();
    void on_Fl_button_clicked();
    void on_Uup_button_clicked();
    void on_Lv_button_clicked();
    void on_Uus_button_clicked();
    void on_Uuo_button_clicked();
    void on_CloseButton_clicked();
    void on_comboBox_currentIndexChanged(int index);
};

#endif // ATOMTOOLDIALOG_H
