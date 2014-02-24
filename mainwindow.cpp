#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "atomTooldialog.h"

#include "cpsviewer.h"
#include "pandora.h"
#include "atlas.h"
#include "apollo.h"

#include <QDockWidget>
#include <QLabel>
#include <QString>
#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>
#include <QProgressBar>
#include <QGLFormat>
#include <QtGlobal>
#include <QDebug>


// Create global object of the Atlas Class...  Global variables are generally poor form, but
// this is the framework of all models within Prometheus. Most functions will need access to this structure.
// "MainWindow" is the top level of Prometheus, so we can pass this object by reference to whatever part of the
// program we need.


Atlas prometheusModel;





MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    activeModel = 0; // Initializes the starting index for prometheusModel...  a vector holding the Atlas Framework.

    createCpsViewer();  // Sets up the view window for Prometheus

    createElementSelectionDialog(); // Sets up the dialog that allows the User to pick the element they desire.

    createStatusBar();  // Sets up the dialog that lets the User know a calculation is ongoing.

    currentFFIndex = 0;
}

void MainWindow::closeEvent(QCloseEvent *)
{
    exit(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createCpsViewer()
{
    // This function creates the view window for the CPS, sets up its OpenGL properties,
    // and makes the whole thing dockable in the main window.

    // We have an instance of the class CPSviewer, a OpenGL widget of our design, setup in this class's header file.  We will need this widget so that
    // we can tell Qt to place it in the dock widget.
    mainView = new CPSviewer;
    mainView->show();

    // Add the CPSviewer widget into the dock.
    //ui->cps_viewer_dock->setWidget(mainView);

    // Using Qt's Signal and Slot mechanics, we create several connctions:

    // A connection between a signal emitted from the main window to a slot contianed within the OpenGL viewer class.
    //      This connection allows us to nudge the CPS Viewer and force it to update with the most current view.
    this->connect(this, SIGNAL(requestShallowRefresh()),
                  mainView, SLOT(requestShallowRefresh()));

    // A connection between a signal emitted from the main window to a slot contianed within the OpenGL viewer class.
    //      This connection allows us to update the atomic properties within the viewer before we ask the CPS Viewer to update with the most current view.
    this->connect(this, SIGNAL(requestDeepRefresh()),
                  mainView, SLOT(requestDeepRefresh()));

    // A connection between the OpenGL viewer class and the main program.  Whenever a file is drag and dropped onto the viewer, the user desires that file loaded.
    //     The filename is determine, passed to where it can be parsed and Prometheus can attempt to load it into the Altas framework.
    this->connect(mainView, SIGNAL(dragAndDropFilename(QString)),
                  this, SLOT(dragAndDropParsing(QString)));

    // A connection between the View Tool GUI button and the OpenGL viewer class, informing OpenGL of a desired configuration for that tool (see below).
    this->connect(this, SIGNAL(viewToolSelected()),
                  mainView, SLOT(useViewTool()));

    // A connection between the Select Tool GUI button and the OpenGL viewer class, informing OpenGL of a desired configuration for that tool (see below).
    this->connect(this, SIGNAL(selectToolSelected()),
                  mainView, SLOT(useSelectTool()));

    // A connection between the Atom Build Tool GUI button and the OpenGL viewer class, informing OpenGL of a desired configuration for that tool (see below).
    this->connect(this, SIGNAL(atomToolSelected()),
                  mainView, SLOT(useAtomTool()));

    // A connection between the Delete Tool GUI button and the OpenGL viewer class, informing OpenGL of a desired configuration for that tool (see below).
    this->connect(this, SIGNAL(deleteToolSelected()),
                  mainView, SLOT(useDeleteTool()));

    // A connection between the Measure Tool GUI button and the OpenGL viewer class, informing OpenGL of a desired configuration for that tool (see below).
    this->connect(this, SIGNAL(measureToolSelected()),
                  mainView, SLOT(useMeasureTool()));

    // A connection between the Increment Bond Order Tool GUI button and the OpenGL viewer class, informing OpenGL of a desired configuration for that tool (see below).
    this->connect(this, SIGNAL(incrementBOToolSelected()),
                  mainView, SLOT(useIncrementBOTool()));

    // A connection between the Increment Bond Order Tool GUI button and the OpenGL viewer class, informing OpenGL of a desired configuration for that tool (see below).
    this->connect(this, SIGNAL(decrementBOToolSelected()),
                  mainView, SLOT(useDecrementBOTool()));


    // A connection between the Projection Method combo box and the OpenGL viewer class, informing OpenGL of the desired projection method (perspective or orthogonal.
    this->connect(this, SIGNAL(updateProjectionMethod(int)),
                  mainView, SLOT(updateProjectionMethod(int)));

    // A connection between the Label Style combo box and the OpenGL viewer class, informing OpenGL of the desired labels to be drawn.
    this->connect(this, SIGNAL(requestLabelStyle(int)),
                  mainView, SLOT(requestLabelStyle(int)));


    /*************************************
     *   Bitmap Generating Code
     ************************************/
    // A connection between the Bitmap Mode Toggle Button  and the OpenGL viewer class.
    this->connect(this, SIGNAL(setBitmapMode(bool)),
                  mainView, SLOT(setBitmapMode(bool)));

    // A connection between the Bitmap Grid Setup Button  and the OpenGL viewer class.
    this->connect(this, SIGNAL(setBitmapGrid(int,int)),
                  mainView, SLOT(setBitGrid(int,int)));

    // A connection between the Print Bitmap Button and the OpenGL viewer class.
    this->connect(this, SIGNAL(printBitmap()),
                  mainView, SLOT(printBitmap()));
}

void MainWindow::createElementSelectionDialog()
{
    PeriodicTable = new AtomToolDialog(this);
    // A connection between a signal emitted from the Element Selection Dialog to a slot contianed within the OpenGL viewer class.
    //      This connection allows us to pass the User's desired element to the viewer controls.
    this->connect(PeriodicTable, SIGNAL(requestBuildElementChange(int)),
                  mainView, SLOT(requestBuildElementChange(int)));
}

void MainWindow::createStatusBar()
{
    statusBar = new PrometheusStatus(this);
}



int MainWindow::getOperatingSystem()
{
    // This function returns an integer signaling which operating system is being used.
    // 0 = Windows, 1 = Mac, 2 = Linux

    int osFlag;

#ifdef Q_OS_WIN
    osFlag = 0;
#endif

#ifdef Q_OS_MAC
    osFlag = 1;
#endif

#ifdef Q_OS_LINUX
    osFlag = 2;
#endif

    return osFlag;
}

void MainWindow::on_actionOpen_Model_triggered()
{
    //**************************************************************************************
    // Purpose: This function contains what happens when you select "File > Open Model..."
    // from the drop down menu on the GUI.
    //**************************************************************************************

    Pandora input;
    QString homeDir, fileName, filters, selectedFilter;

    // QFileDialog creates a window that allows the user to select a file within their directory.
    // It then returns a string with that file's pathname to the QString "fileName".
    // We specify the home directory as the starting place of the search.

    // Ask Qt to determine the operating system that is being used.
    int osFlag = getOperatingSystem();

    if (osFlag == 0)
    {
        // Windows
        homeDir = "C:/";
    }
    else if (osFlag == 1)
    {
        // Mac
        homeDir = "/Users";
    }
    else if (osFlag == 2)
    {
        // Linux
        homeDir = "/home/";
    }

    // Setup possible filetypes...  each different type is seperated by a semicolon.
    filters = "All File Types (*.*);;BIOGRF File (*.bgf);;Crystallographic Information File (*.cif);;Chemical Markup Language File (*.cml);;MDL Molfile (*.mol);;Tripos Mol2 File (*.mol2) ;;Cerius2 Native File (*.msi);;Protein Data Base File (*.pdb);;XYZ Coordinate File (*.xyz)";

    // And we're ready to use the file dialog.
    fileName = QFileDialog::getOpenFileName(this, tr("Open Model"), homeDir, filters, &selectedFilter);

    if (fileName != "")
    {
        // The User had an opportunity to select the filetype within the dialog.  We pass this information to the Pandora class,
        // which handles the parsing of a wide range of file formats.
        input.parseDataFile(selectedFilter, fileName);
    }
    else
    {
        // let the function exit because the user pressed "Cancel" in the dialog, returning null ("").
        return;
    }

    // Ask Prometheus to update its view.
    emit requestDeepRefresh();
}


void MainWindow::dragAndDropParsing(QString filename)
{
    //**************************************************************************************
    // Purpose: This function contains what happens when you drop a file onto the main viewer.
    //  Here we process the file's name, and if it appears relevant, we send it off to
    // Pandora for loading.
    //**************************************************************************************

    Pandora input;

    // Qt has a built-in function for determining file extensions under the class QFileInfo.
    // We then pass this filetype to the Pandora class, which handles parsing a wide range of file formats.
    QFileInfo info (filename);
    QString fileType = info.suffix();
    input.parseDataFile(fileType, filename);

    // Ask Prometheus to update its view... specifically, the update that links the position arrays to the
    // Atlas Framework.
    emit requestDeepRefresh();
}


void MainWindow::on_actionSave_Model_triggered()
{
    //**************************************************************************************
    // Purpose: This function contains what happens when you select "File > Save Model..."
    // from the drop down menu on the GUI...  We want to let the User save a model as a
    // chemical datafile of a format of the User's choosing.
    //**************************************************************************************

    Pandora input;
    QString homeDir, fileName, filters, selectedFilter;

    // QFileDialog creates a window that allows the User to interact with their directory.
    // The User chooses a filename and path for their model, we take that information,
    // and create the file using a function from the Pandora class.

    // Ask Qt to determine the operating system that is being used.
    int osFlag = getOperatingSystem();

    if (osFlag == 0)
    {
        // Windows
        homeDir = "C:/";
    }
    else if (osFlag == 1)
    {
        // Mac
        homeDir = "/Users";
    }
    else if (osFlag == 2)
    {
        // Linux
        homeDir = "/home/";
    }


    // Setup possible filetypes...  each different type is seperated by a semicolon.
    filters = "BIOGRF File (*.bgf);;Crystallographic Information File (*.cif);;Chemical Markup Language File (*.cml);;LAMMPS Input File (*.lammps);;MDL Molfile (*.mol);;Tripos Mol2 File (*.mol2) ;;Cerius2 Native File (*.msi);;Protein Data Base File (*.pdb);;XYZ Coordinate File (*.xyz)";

    // And we're ready to use the file dialog.
    fileName = QFileDialog::getSaveFileName(this, tr("Save Model"), homeDir, filters, &selectedFilter);

    if (fileName != "")
    {
        // The User had an opportunity to select the filetype within the save dialog.  We pass this information to the Pandora class,
        // which handles the creation of a wide range of file formats.

        input.writeDataFile(selectedFilter, fileName);
    }
    else
    {
        // let the function exit because the user pressed "Cancel" in the dialog, returning null ("").
        return;
    }
}



/**********************************************************************************************
 * These functions are slots that take inputs from the GUI viewer options tab and route those
 * instructions to the CPSviewer class.
 *********************************************************************************************/

void MainWindow::on_comboBox_RepresentationStyle_currentIndexChanged(int index)
{
    // This slot receives a signal from the ComboBox that the user uses to select a representation type for the model.
    // This integer is stored in the Atlas framework and used to choose which OpenGL commands draw the model.
    // Example:  1 -> Ball-and-Stick, 3 -> Space Filling, 2 -> Dreiding.

    // Store the flag in the Atlas framework.
    prometheusModel.setDrawingStyle(index);

    // Ask Prometheus to update its view.
    emit requestShallowRefresh();
}

void MainWindow::on_comboBox_ProjectionMethod_currentIndexChanged(int index)
{
    // This slot recieves a singal from the ComboBox that the User uses to select the projection method for the viewer.
    // This integer is stored in the CPSviewer class to infrom the projection of all models.

    emit updateProjectionMethod(index);
}

//******************************************************************************************************************************************************************************
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// These slots govern the buttons that allow the user to select the mode of Prometheus's viewer.  The buttons can be thought of like tools used in a paint program.
// Whether the user is interested in merely examining a system, or wishes to interact with the model, these tools allow the user to select the effects of mouse clicks within
// the viewer.  It also allows the viewer's OpenGL code to be optimized since it takes more computational effort if the user interacts with the model directly.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//******************************************************************************************************************************************************************************
void MainWindow::on_viewTool_clicked()
{
    // The View Tool encompases simple viewing and rotation.  It is not possible to select or manipulate any atom or bond.
    // Left Mouse Button = Rotate
    // Right Mouse Button = Rotate
    // Middle Button / Wheel Click = Translation
    // Wheel Scroll = Zoom

    emit viewToolSelected();
}

void MainWindow::on_selectTool_clicked()
{
    // The Select Tool encompasses the ability to hightlight atoms and bonds within the viewer.  Single atoms/bonds can be highlighted, or the user can drag out an area
    // to select many atoms and bonds.  The selected atoms/bonds can then be used with copy, paste, and delete functions, or they can be moved/rotated by using those tools.
    // Clicking a selected atom will de-select any other atoms and bonds and leave only the atom selected.  Clicking empty space will de-select all atoms.

    // Left Mouse Button = Highlight Single Atom, or Start Rectangle
    // Right Mouse Button = Rotate
    // Middle Button / Wheel Click = Translation
    // Wheel Scroll = Zoom

    emit selectToolSelected();
}

void MainWindow::on_atomTool_clicked()
{
    // The Build Tool encompasses the ability to place atoms and bonds...  to put these building blocks into the user's hands so they may construct molecules of their choosing.
    // Clicking in a blank area drops an atom of the type selected by the user.  Clicking on an existing atom will replace it with another if the user has a different element selected.
    // Clicking on an existing atom and dragging away will place a new atom bonded to the first.

    // Left Mouse Button = Place Atom over empty space or edit existing atom
    //                      When dragged, place atom a second atom bonded to the mentioned previously.
    // Right Mouse Button = Rotate
    // Middle Button / Wheel Click = Translation
    // Wheel Scroll = Zoom


    emit atomToolSelected();
}

void MainWindow::on_deleteTool_clicked()
{
    // The Delete Tool encompasses the ability to delete atoms and bonds...
    // Clicking on an existing atom or bond will delete it.

    // Left Mouse Button = Delete Atom/Bond
    // Right Mouse Button = Rotate
    // Middle Button / Wheel Click = Translation
    // Wheel Scroll = Zoom

    emit deleteToolSelected();
}

void MainWindow::on_measureTool_clicked()
{
    // The Measure Tool encompasses the ability analyze and fine tune models.
    // The user can measure bond distances, bond angles, and bond dihedrals by selecting 2, 3, and 4 atoms respectively.
    // Clicking on an existing atom will select it.  Clicking on a selected atom will de-select it.
    // The user will also be able to manipulate a given distance or angle and set it to a desired value.

    // Left Mouse Button = Select / De-Select Atom
    // Right Mouse Button = Rotate
    // Middle Button / Wheel Click = Translation
    // Wheel Scroll = Zoom

    emit measureToolSelected();
}


void MainWindow::on_increaseBondOrderTool_clicked()
{
    // This tool allows the User to specify the bond order of any selected bond on-screen.  Clicking on an existing bond will increment its order by 1.

    // Left Mouse Button = Only effective on bonds...  If BO = 1, increment to 2.  If BO = 2, increment to 3.  If BO = 3, no effect.
    // Right Mouse Button = Rotate
    // Middle Button / Wheel Click = Translation
    // Wheel Scroll = Zoom

    emit incrementBOToolSelected();
}

void MainWindow::on_decreaseBondOrderTool_clicked()
{
    // This tool allows the User to specify the bond order of any selected bond on-screen.  Clicking on an existing bond will decrement its order by 1.

    // Left Mouse Button = Only effective on bonds...  If BO = 1, no effect.  If BO = 2, decrement to 1.  If BO = 3, decrement to 2.
    // Right Mouse Button = Rotate
    // Middle Button / Wheel Click = Translation
    // Wheel Scroll = Zoom

    emit decrementBOToolSelected();
}

void MainWindow::on_elements_clicked()
{
    if (PeriodicTable->isVisible())
    {
        // If the table is already on-screen, we need to bring it to the top.
        PeriodicTable->close();
        PeriodicTable->show();
    }
    else
    {
        // This button reveals the Element Selection Dialog that was initialized in the constructor.
        PeriodicTable->show();
    }
}



void MainWindow::on_comboBox_LabelStyle_currentIndexChanged(int index)
{
    // This sends a signal to CPSViewer to inform it what kind of label we want drawn.
    emit requestLabelStyle(index);
}



void MainWindow::on_toggleBitmap_toggled(bool checked)
{
    emit setBitmapMode(checked);
}


void MainWindow::on_createBitmapGrid_clicked()
{
    int width = ui->spinBox_width->value();
    int height = ui->spinBox_height->value();

    emit setBitmapGrid(width, height);
}

void MainWindow::on_pushButton_clicked()
{
    emit printBitmap();
}




void MainWindow::on_comboBox_Typing_currentIndexChanged(int index)
{
    // This slot captures the User's desired Force Field to use in typing.
    currentFFIndex = index;
}

void MainWindow::on_pushButton_AssignAtomTypes_clicked()
{
    // This slot captures the User's click on the pushbutton to initiate Force Field typing.  The actual
    // function is a member of Pandora.

    Pandora input;
    input.assignFFTypes(currentFFIndex);
}

// These functions pertain to calculations performed within the Apollo Class.  These calculations involve physical properties, statistics, and other hardcore calculations
// that don't have to do with formatting or graphics computation.



void MainWindow::apolloMessage(QString message, QString informativeMessage)
{
    // This function passes a text string to a message box to let the User know whatever Apollo has been working on is done.
    statusBar->hide();

    QMessageBox msgbox;

    msgbox.setIcon(QMessageBox::Information);
    msgbox.setText(message);
    msgbox.setInformativeText(informativeMessage);

    emit requestDeepRefresh();

    msgbox.exec();
}

void MainWindow::on_QEQpushButton_clicked()
{
    // This slot captures the User's click on the pushbutton to initiate a QEq calculation.  The actual function is a member of Apollo.


    // The function we call to perform QEq takes the desired net charge as an argument.  That value is pulled from the doubleSpinBox adjacent to
    // the button that initiated this Slot.

    Apollo *calculation = new Apollo;
    QThread *thread = new QThread;

    calculation->setNetCharge(ui->netChargeSpinBox->value());

    calculation->moveToThread(thread);
    connect(thread, SIGNAL(started()),
            calculation, SLOT(performQEq()));
    connect(calculation, SIGNAL(calculationFinished(QString,QString)),
            thread, SLOT(quit()));
    connect(calculation, SIGNAL(calculationFinished(QString,QString)),
            this, SLOT(apolloMessage(QString,QString)));

    QString message1 = "Partial Charge Assignment:";
    QString message2 = "working....";
    statusBar->setupText(message1, message2);
    statusBar->show();

    thread->start();


}
