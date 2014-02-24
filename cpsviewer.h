#ifndef CPSVIEWER_H
#define CPSVIEWER_H


#include <QGLWidget>
#include <QObject>
#include <OpenGl/glu.h>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QKeyEvent>
#include <QQuaternion>
#include <QVector3D>
#include <QDropEvent>


#include <QGLShader>
#include <QGLShaderProgram>
#include <QGLBuffer>

#include "atlas.h"

extern Atlas prometheusModel;


class CPSviewer : public QGLWidget
{
    Q_OBJECT
public:
    explicit CPSviewer(QWidget *parent = 0);
    ~CPSviewer();

    QSize minimumSizeHint() const;  // Sets minimium size of view window
    QSize sizeHint() const;         // Sets beginning size of view window

    GLuint vaoHandle;

    // These functions set variables that control camera location.
    void setRotation(int X, int Y, int previousX, int previousY);
    void setXTranslation(float distance);
    void setYTranslation(float distance);
    void setZoom(float distance);

    /*****************************************************************************************************************
     * These functions control the color and size of atoms...  doesn't actually impact the data, but is more about
     * how it is displayed...  the results from these functions are stored within the CPSviewer class...  this class
     * stores the actively viewed part of the Atlas Framework.
     ******************************************************************************************************************/
    void deepRefresh(); // Ask CPSviewer to process some data before having OpenGL redraw.
    void editAtomColor();
    void editAtomRadius();
    //void editBondColor();
    //void editBondRadius();

    // These functions handle drawing routines for the various model representations.
    void drawModel();
    void drawBallandStickRepresentation();
    void drawDreidingRepresentation();
    void drawSpaceFillingRepresentation();
    void drawVectorRepresentation();
    void drawEnhancedVectorRepresentation();

    // These copy-cat functions handle drawing routines for the highlighted atoms.
    void drawSelectedModel();
    void drawSelectedBallandStickRepresentation();
    void drawSelectedDreidingRepresentation();
    void drawSelectedSpaceFillingRepresentation();
    void drawSelectedVectorRepresentation();
    void drawSelectedEnhancedVectorRepresentation();

    // This function handles the ability of the viewer to support item selection within the view.
    void selectObjects(int centerX, int centerY, int boxWidth, int boxHeight);
    void processHits(GLint hitCount, GLuint *selectionBuffer);
    void drawSelectionBox (QPoint start, QPoint current);
    void useSelectionBox(int finalX, int finalY, int initialX, int initialY);
    void selectMolecule();

    // This function draws labels on the atoms
    void drawLabels();
    void locateOnScreen();

    // bitmaps
    void drawBitmapGrid();
    void pickBitmapGrid(int x, int y);
    void CharacterLookup();

protected:
    //  These next three are the bare bone basic functions needed to execute an OpenGL widget.
    void initializeGL();                   // starts up the widget
    void resizeGL(int width, int height);  // gets called whenever you resize the window
    void paintGL();                        // indicates what to draw

    // These handle mouse inputs.
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

    // These functions handle Drag and Drop functionality of the viewer.  Any file dragged to the view has
    // its filename extracted and then Prometheus tries to load the model.
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);

    // These functions handle keyboard inputs.
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

private:
    // This identifies the User's desired representation method for the displayed model.
    int activeModel;

    int OpenGLMethod;
    int representationMethod;
    int projectionMethod;

    // *************************************************************************************************
    // These variables control camera movement (translation, rotation, zoom) for the Prometheus Viewer.
    // *************************************************************************************************
    // hold's the camera's orentation by telling OpenGL where it needs to rotate after the axis reset each time a frame is drawn.
    // Does not include translation information.
    QQuaternion accumulatedRotation;
    QQuaternion inputRotation;
    bool isStartStored;

    // hold's camera's (translation) position by telling OpenGL where to shift the whole model...  does not include rotation information.
    float xTrans;
    float yTrans;
    float zTrans;

    QPoint lastPos; // will be used to keep track of the mouse cursor's position on screen.
    QPoint initialPos; // will be used to store the position of mouse on mouse click.
    QPoint finalPos; // will be used to store the position of mouse upon mouse button release.
    float WheelDelta;  // will be used to keep track of the mouse wheel's orentation.


    // **************************************************************
    // These variables control switching between Tools in Prometheus
    // **************************************************************
    int selectionBufferSize;

    bool viewTool;
    bool selectTool;
    bool atomTool;
    bool deleteTool;
    bool measureTool;
    bool incrementBOTool;
    bool decrementBOTool;

    bool singleSelect;
    bool selectRenderMode;
    bool controlIsPressed;
    bool doubleClick;
    bool controlAll;

    bool atomBuilderMoved;
    bool mousePressed;
    int builderAtom;

    bool bitmapMode;
    int bitmapWidth;
    int bitmapHeight;
    QVector<int> pickedBits;

    /**************************************************************************************
    * These arrays hold model data, but only for what is being displayed...  the Atlas
    * Framework holds everything, and this structure is a copy of what's actively being
    * viewed by the User.
    ****************************************************************************************/
    QVector<float> PosX;
    QVector<float> PosY;
    QVector<float> PosZ;
    QVector<QString> AtomNames;
    QVector<int> AtomicNumbers;
    QVector<QString> FFTypes;
    QVector<float> PartialCharges;
    QVector<float> Radii;
    QVector< QVector<float> > Colors;


    QVector<QVector<int> > BondList;
    QVector<int> BondOrders;
    QVector<float> BondLengths;
    QVector<QVector<float> > BondCenters;
    QVector<QVector<float> > BondNormals;


    QVector<int> SelectedAtoms;
    QVector<int> SelectedBonds;


    // These variables hold onto data neccessary to draw labels next to every atom.
    int requestedLabelStyle;
    QVector<QVector<int> > onScreenPos;

public slots:
    void requestShallowRefresh();
    void requestDeepRefresh();
    void updateProjectionMethod(int index);


    // These slots take signals from the tool buttons in the MainWindow gui.  These slots change the member variables viewTool, selectTool, etc
    // which will then inform OpenGL about what it needs to do to provide that tool's functionality.
    void useViewTool();
    void useSelectTool();
    void useAtomTool();
    void useDeleteTool();
    void useMeasureTool();
    void useIncrementBOTool();
    void useDecrementBOTool();

    // These slots take signals from various parts of the GUI and are used set options.
    void requestBuildElementChange(int atomicNumber);
    void requestLabelStyle(int label);

    void setBitmapMode(bool isChecked);
    void setBitGrid(int width, int height);
    void printBitmap();

signals:
    void dragAndDropFilename(QString filename);


};

#endif // CPSVIEWER_H
