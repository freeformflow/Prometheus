#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QListWidget>
#include <QtConcurrent/QtConcurrent>
#include <QFuture>
#include <QDialog>

#include "atlas.h"
#include "cpsviewer.h"
#include "atomtooldialog.h"
#include "prometheusstatus.h"


extern Atlas prometheusModel;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void createCpsViewer();
    void createElementSelectionDialog();
    void createStatusBar();

    int getOperatingSystem();    
protected:
    void closeEvent(QCloseEvent *);

public slots:
    void dragAndDropParsing(QString filename);

private slots:
    void on_actionOpen_Model_triggered();
    void on_actionSave_Model_triggered();

    void on_comboBox_RepresentationStyle_currentIndexChanged(int index);
    void on_comboBox_ProjectionMethod_currentIndexChanged(int index);


    // These slots contain Prometheus's viewer tools as seen from the gui interface.
    void on_viewTool_clicked();
    void on_selectTool_clicked();
    void on_atomTool_clicked();
    void on_deleteTool_clicked();
    void on_measureTool_clicked();
    void on_increaseBondOrderTool_clicked();
    void on_decreaseBondOrderTool_clicked();


    void on_elements_clicked();



    void on_comboBox_LabelStyle_currentIndexChanged(int index);

    void on_toggleBitmap_toggled(bool checked);
    void on_createBitmapGrid_clicked();
    void on_pushButton_clicked();


    void on_comboBox_Typing_currentIndexChanged(int index);

    void on_pushButton_AssignAtomTypes_clicked();

    // These slots deal with the Apollo Class functions.
    void apolloMessage(QString message, QString informativeMessage);
    void on_QEQpushButton_clicked();

signals:
    //  These signals are connected to the CPSviewer class that actually controls the underlying OpenGL of the viewer.
    void viewToolSelected();
    void selectToolSelected();
    void atomToolSelected();
    void deleteToolSelected();
    void measureToolSelected();
    void incrementBOToolSelected();
    void decrementBOToolSelected();

    void requestShallowRefresh();
    void requestDeepRefresh();
    void updateProjectionMethod(int index);

    void requestLabelStyle(int index);

    // Bitmap Singals
    void setBitmapMode(bool isChecked);
    void setBitmapGrid(int width, int height);
    void printBitmap();

private:
    Ui::MainWindow *ui;

    int activeModel;  // The index of the Model currently being viewed.

    CPSviewer *mainView;
    AtomToolDialog *PeriodicTable;
    QListWidget *atlasList;

    PrometheusStatus *statusBar;

    int currentFFIndex; // Integer holding the desired Force Field to be used for typing.


};

#endif // MAINWINDOW_H
