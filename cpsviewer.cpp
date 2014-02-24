#include "cpsviewer.h"
#include "pandora.h"

#include <OpenGL/glu.h>
#include <QGLFormat>
#include <QtGui>
#include <QtOpenGL>
#include <QGLBuffer>
#include <qmath.h>
#include <QDropEvent>
#include <QKeyEvent>



// Constructor
CPSviewer::CPSviewer(QWidget *parent) :
    QGLWidget(parent)
{
    setAutoFillBackground(false);

    // Set the widget to Double Buffer mode.  This will be needed for animation and efficient realtime rendering.
    // This is technically not a OpenGL command because it depends on the windowing context (QGLWidget), but it has an important effect on all the other OpenGL commands.
    setFormat(QGLFormat(QGL::DoubleBuffer | QGL::DepthBuffer | QGL::Rgba | QGL::SampleBuffers));

    // Sets up the first component of the vector prometheusModel as the active model for display.
    activeModel = 0;
    OpenGLMethod = 0;

    // Set intital values for the camera's position for the viewer.

    // Initialize with the Identity Quaternion to describe the camera's starting rotation.
    accumulatedRotation.setVector(0, 0, 0);
    accumulatedRotation.setScalar(1);
    isStartStored = false;

    xTrans = 0;
    yTrans = 0;
    zTrans = -20.0;

    // Set up the viewer to accept files dropped on it.
    setAcceptDrops(true);

    // Determines the projection method for the viewer...   0 = Perspective and 1 = Orthogonal.
    projectionMethod = 0;

    // Initialize the flags that let the viewer know which tools the user wishes to use in the viewer.
    viewTool = true;
    selectTool = false;
    atomTool = false;
    deleteTool = false;
    measureTool = false;
    incrementBOTool = false;
    decrementBOTool = false;


    // Initialize the size of the Selection Buffer (see below for use with Tools).
    selectionBufferSize = 512;
    //selectRenderMode = false;
    controlIsPressed = false;
    doubleClick = false;
    controlAll = false;

    // These variables control atom placement for the Build Tool.
    builderAtom = 6;  // Initializes atomic number 6, Carbon.

    // Initialize lable settings.
    requestedLabelStyle = 0; // Default value requests no labels be drawn.

    bitmapMode = false;
    bitmapWidth = 1;
    bitmapHeight = 1;
}

// Empty destructor
CPSviewer::~CPSviewer() {}


// Set minimum size of the viewer so that it's displayed reasonably.
QSize CPSviewer::minimumSizeHint() const
{
    return QSize(200, 200);
}

// Set initial size of the viewer.
QSize CPSviewer::sizeHint() const
{
    return QSize(600, 600);
}


// Initialize OpenGL.  Quality settings and fixed-pipline lights are declared here.
void CPSviewer::initializeGL()
{
    glShadeModel(GL_SMOOTH); // Enable smooth shading
    qglClearColor(Qt::black); // Set the clear color to a black background

    glClearDepth(1.0f); // Depth buffer setup
    glEnable(GL_DEPTH_TEST); // Enable depth testing.  This tells the GPU to perform hidden surface calculations for you.
    glDepthFunc(GL_LEQUAL); // Set type of depth test

    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); // Really nice perspective calculations

    // Blending breaks jaggie fragments and smears out the color more smoothly over an area.
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // OpenGL can speed calculations by avoiding drawing surfaces that are hidden from the user's point of view.
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);


    // Calls to OpenGL that set up the lighting.
    glEnable(GL_LIGHTING);
    GLfloat lightPos[] = {5, 5, 0.5, 0.0};  // This sets the light position as shining from the upper right hand side of the screen.
                                            // The last zero in the lightPos vector specifies that it's a directional light.  It is not a real light with a location,
                                            // but a beam coming in from infinity.
    GLfloat ambientLight[] = {0.3, 0.3, 0.3, 1.0};  // Medium ambient light (white in color).
    GLfloat diffuseLight[] = {0.7, 0.7, 0.7, 1.0};  // Farily strong directed light (white in color).
    GLfloat specularLight[] = {1.0, 1.0, 1.0, 1.0};  // Strong, specular light source (white in color).

    // Tell OpenGL about the material we are lighting.
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    GLfloat specularReflectance[] = {1.0, 1.0, 1.0, 1.0};  // High specular reflectance.
    glMaterialfv(GL_FRONT, GL_SPECULAR, specularReflectance);
    glMateriali(GL_FRONT,GL_SHININESS,128);  // Focuses the specular effects to a small area.


    glLightfv(GL_LIGHT0,GL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT0,GL_DIFFUSE, diffuseLight);
    glLightfv(GL_LIGHT0,GL_SPECULAR, specularLight);
    glLightfv(GL_LIGHT0,GL_POSITION,lightPos);
    //glEnable(GL_LIGHT0);


    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    if (OpenGLMethod == 2 | OpenGLMethod == 0)
    {
        // We have access to Vertex and Fragment Shaders...  while the drawing must occur on the CPU, we can still leverage the GPU's abilities
        // to improve quality and efficiency.

        // Set up shaders under the GLSL.
        QGLShader vShader (QGLShader::Vertex);
        vShader.compileSourceFile(":/Shaders/vertexShader.vert");

        QGLShader fShader (QGLShader::Fragment);
        fShader.compileSourceFile(":/Shaders/fragmentShader.frag");

        // Now create a shader program and add the shaders to it.
        QGLShaderProgram cpsProgram(this);
        cpsProgram.addShader(&vShader);
        cpsProgram.addShader(&fShader);

        // Link the various shaders together.
        cpsProgram.link();

        // And finally, bind the shader program to the QGLWidget context.
        cpsProgram.bind();

    }
    else if (OpenGLMethod == 3)
    {
        // We have access to Vertex, Fragment, and, Geometry Shaders... so we may use some of the most advanced methods for drawing.

        // Set up shaders under the GLSL.
        QGLShader vShader (QGLShader::Vertex);
        vShader.compileSourceFile(":/Shaders/vertexShader.vert");

        QGLShader fShader (QGLShader::Fragment);
        fShader.compileSourceFile(":/Shaders/fragmentShader.frag");

        QGLShader gShader (QGLShader::Geometry);
        gShader.compileSourceFile(":/Shaders/geometryShader.geom");

        // Now create a shader program and add the shaders to it.
        QGLShaderProgram cpsProgram(this);
        cpsProgram.addShader(&vShader);
        cpsProgram.addShader(&fShader);
        cpsProgram.addShader(&gShader);

        // Link the various shaders together.
        cpsProgram.link();

        // And finally, bind the shader program to the QGLWidget context.
        cpsProgram.bind();
    }


}

// This is called when the OpenGL window is resized.  It handles redrawing the window,
// so we can resize the window without worry.
void CPSviewer::resizeGL(int width, int height)
{
    // Prevent divide by zero (in the gluPerspective call)
    if (height == 0)
        height = 1;


    glViewport(0,0, width, height);


}

// OpenGL painting code goes here.
void CPSviewer::paintGL()
{

    float angle;

    // First off, we have to decide how the model will be displayed.  This integer will inform this function's drawing method.
    // 1 = Ball-and-Stick, 2 = Dreiding (Licorice), 3 = Space Filling (CPK), 4 = Vector (Wireframe), 5 = Vector with Points
    representationMethod = prometheusModel.getDrawingStyle();

    // Pass this instruction to the shaders via a uniform variable.
    glUniform1i(0, representationMethod);


    glMatrixMode(GL_PROJECTION); // Select projection matrix
    glLoadIdentity(); // Reset projection matrix

    float aspect = this->width() / this->height();

    if (projectionMethod == 0)
    {
        // The User desires a view using a perspective projection.
        //gluPerspective(45.0f, this->width()/this->height(), 0.1f, 100.0f);
        gluPerspective(45, aspect, 0.1, 1000);
    }
    else if (projectionMethod == 1)
    {
        // The User desires a view using an orthogonal projection.
        glOrtho(-zTrans, zTrans, -zTrans*aspect, zTrans*aspect, -100, 100);
    }

    glMatrixMode(GL_MODELVIEW); // Select modelview matrix
    glLoadIdentity(); // Reset modelview matrix



    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear screen and depth buffer
    glLoadIdentity(); // Reset current modelview matrix

    // This line handles where you've dragged / zoomed the model.
    // xTrans and yTrans are assigned from dragging functions.
    // zTrans is assigned from the Zoom function.
    if (projectionMethod == 0)
    {
        // In perspective, all three translations are model transformations.
        glTranslatef(xTrans, yTrans, zTrans);
    }
    else
    {
        // In orthogonal, only x and y translations are model transformations...  "zoom" is achieved by adjusting the viewers clipping planes.
        glTranslatef(-xTrans, -yTrans, 0);
    }

    // Rotation...  We ask OpenGL to rotate the model using the function glRotatef.  The paramters are angle of rotation in degrees, and the x-y-z components of a vector describing
    // the axis of rotation.  These values are stored within the quaternion "accumulatedRotation".  See the function "setRotation()" below for its calculation.
    // Quaternions hold cos(angle / 2).  In order to retrieve the angle in degrees, we multiply the conversion factor by 2.
    // 2 * 180 / 3.14159 = 114.592750
    angle = qAcos(accumulatedRotation.scalar()) * 114.592750;
    glRotatef(angle, accumulatedRotation.x(), accumulatedRotation.y(), accumulatedRotation.z());


    if (!bitmapMode)
    {
        if (OpenGLMethod == 0)
        {
            // This is basic calls using the glBegin() and glEnd() - fixed function paradigm...  very old and very inefficient.  OpenGL before v1.5

            // Now that the view and orientation have been established, the OpenGL commands to draw the model are contained within this function.
            drawModel();

            /***************************************************************************************************************************************************************/
            // Now that drawing is complete, what happens next is up to the User.  The View Tool merely displays the model, so we wouldn't have to do any more in that case.
            // If the User is using some other tool, we'll have more work to do.
            /***************************************************************************************************************************************************************/

            if (!SelectedAtoms.isEmpty() | !SelectedBonds.isEmpty())
            {
                // This informs the fragment shader that we wish for diffuse lighting on the selection "surface", but we do not want specular highlights.
                // -1 is a special flag that shouldn't change, even if other representation methods are added to Prometheus.
                glUniform1i(0, -1);
                drawSelectedModel();
            }

            if (selectTool)
            {
                if (!singleSelect)
                {
                    // If the User has triggered bulk selection, this function draws a rectangle representing the selection area.
                    // -2 is a special flag that shouldn't change even if other representation methods are added to Prometheus.
                    glUniform1i(0, -2);
                    drawSelectionBox(initialPos, finalPos);
                }
            }


            // Draw any requested labels...
            drawLabels();

        }
        else if (OpenGLMethod == 1)
        {
            // This features buffer objects, first introduced in OpenGL v1.5.  All vertices are generated on the CPU, but it happens as infrequently as possible, and the results
            // are stored for frquent drawing.  Rotation and Translation occur on the CPU.  Lighting is processed on the CPU.  Applied for OpenGL v1.5 to v3.0

            glColor3f(1, 0, 0);

            #define BUFFER_OFFSET(bytes)  ((GLubyte*) NULL + (bytes))

            GLuint  buffers[2];
            GLfloat vertices[][3] = {
                { -2.0, -2.0, -2.0 },
                {  2.0, -2.0, -2.0 },
                {  2.0,  2.0, -2.0 },
                { -2.0,  2.0, -2.0 },
                { -2.0, -2.0,  2.0 },
                {  2.0, -2.0,  2.0 },
                {  2.0,  2.0,  2.0 },
                { -2.0,  2.0,  2.0 }
            };

           GLubyte indices[] = {
                0, 1, 2, 3,
                4, 7, 6, 5,
                0, 4, 5, 1,
                3, 2, 6, 7,
                0, 3, 7, 4,
                1, 5, 6, 2
            };

            // Get numbers to ID our various buffers and store them in the array buffers
            glGenBuffers(2, buffers);

            // Start with our first buffer and bind (enable) it so we can give it data.
            glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);

            // Feed the data from the array vertices into the buffer object.
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


            // Because touching surfaces share vertices, we don't want OpenGL to draw them more than once...  we tell OpenGL which vertices are
            // part of the same surface by using indices.  We bind and load the data, just as before.
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[1]);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


            // This buffer object will be rendered with glDrawElements...  but first we have tell OpenGL what this data is and prepare the "client" for drawing.
            glEnableClientState(GL_VERTEX_ARRAY);

            // And how to iterate through it with this pointer.
            glVertexPointer(3, GL_FLOAT, 0, BUFFER_OFFSET(0));

            // And now we can draw...  let OpenGL know what we want to draw, and from where to pull the
            glDrawElements(GL_QUADS, 24, GL_UNSIGNED_BYTE, BUFFER_OFFSET(0));
        }
        else if (OpenGLMethod == 2)
        {
            // This is a hybrid method.  All vertices are still generated on the CPU, but we make use of programmable vertex and fragment shaders.
            // Rotation, translation, and lighting (Phong) all occur on the GPU.   Applied for OpenGL v3.0 to v3.2
        }
        else if (OpenGLMethod == 3)
    {
        // This is the most advanced method and fastest.  The GPU is passed a set of atomic coordinates, and makes use of programmable vertex, fragment, and geometry shaders...
        // The GPU is able to draw the needed shapes, rotate and translate them into position, and apply Phong Lighting.
        // Only for GPUs and implmentations supporting OpenGL beyond v3.2
    }
    }
    else
    {
        //drawBitmapGrid();
    }


}


//********************************************************************************************
// Functions in this section deal with drawing routines for the various model representations.
//*********************************************************************************************

void CPSviewer::drawModel()
{
    // From here on, we need call OpenGL commands based on the representation method we're going after.
    // 1 = Ball-and-Stick
    // 2 = Licorice (Dreiding)
    // 3 = Space Filling (CPK)
    // 4 = Vector (Wireframe)
    // 5 = Vector with Points

    if (representationMethod == 0)
    {
        drawBallandStickRepresentation();
    }
    else if (representationMethod == 1)
    {
        drawDreidingRepresentation();
    }
    else if (representationMethod == 2)
    {
        drawSpaceFillingRepresentation();
    }
    else if (representationMethod == 3)
    {
        drawVectorRepresentation();
    }
    else if (representationMethod == 4)
    {
        drawEnhancedVectorRepresentation();
    }

}

void CPSviewer::drawBallandStickRepresentation()
{
    // Ball-and-Stick Model... A favorite staple of chemistry. We're going to render a lot of spheres and cylinders, so we're going to rely on the handy routines provided by the
    // Graphics Library Utility (glu) to efficiently draw the model.

    ///////////////////////////////////////////////////////////////////////////
    //              SETUP
    ///////////////////////////////////////////////////////////////////////////

    // Enable anti-aliasing techniques to improve image quality.  For the Ball-and-Stick Representation, we need
    // to make sure OpenGL uses techniques designed for 3D surfaces because the intersection of solids looks really
    // bad, even when a lot of polygons are used.  We can turn off anti-aliasing methods for lines and points for this representation.
    if (glIsEnabled(GL_POINT_SMOOTH))
    {
        glDisable(GL_POINT_SMOOTH);
    }

    if (glIsEnabled(GL_LINE_SMOOTH))
    {
        glDisable(GL_LINE_SMOOTH);
    }

    glEnable(GL_MULTISAMPLE);
    glHint(GL_MULTISAMPLE, GL_NICEST);



    GLUquadricObj *sphere, *cylinder; // Quadric Object Pointers
    sphere = gluNewQuadric();  // initialize objects for the glu routine
    cylinder = gluNewQuadric();

    // Tell OpenGL about the normals we'd like to use for vertex shading.  We'd like OpenGL to smoothly interpolate the colors between vertices.
    // This will help hide the polygon faces that approximate the curved surfaces.  That means we can use bigger polygons to improve performace without sacrificing quality.
    gluQuadricNormals(sphere, GLU_SMOOTH);
    gluQuadricNormals(cylinder, GLU_SMOOTH);


    //////////////////////////////////////////////////////////////////////////
    //                Drawing
    //////////////////////////////////////////////////////////////////////////    

    // Begin drawing routine.  We will begin by drawing the balls (atoms).

    /* The Name Stack commands are only executed during Selection Mode.  This code is ignored if we are just drawing the model.
       When we are done drawing, this function is executed once more, and we ask OpenGL to track the shapes it draws and tell us which ones
       are in a specific area (the ones the User is clicking on).  */

    // Cycle through the atoms in the Atlas Framework.
    for (int i = 0; i < PosX.size(); ++i)
    {
        // Name this object for OpenGL's Selection Mode.  Everything draw until this loop completes will be considered one object by OpenGL's selection
        // mode.  If any part of this object is within the viewing volume, it will return a hit to the selection buffer.  This command is only valid
        // during Selection Mode, otherwise it is ignored.
        glLoadName(i + 1);

        // Set the drawing to this color.
        glColor3f(Colors[i][0], Colors[i][1], Colors[i][2]);

        glPushMatrix();
            // The glu routine renders spheres at the origin as a default, so we need to ensure we center it on the atom's coordinates.
            glTranslatef(PosX[i], PosY[i], PosZ[i]);

            // Call the glu routine to render the sphere.
            // The parameters are: object_name, radius, # of slices, # of stacks
            gluSphere(sphere, Radii[i]*0.25, 24, 12);

        // Move back to the origin, so the next call will place the atom at the correct location.
        glPopMatrix();
    }


    // We can move on to drawing the bonds.
    // Cycle through the bonds listed in the Atlas Framework.

    // We specify that the bonds are all composed of a neutral color.  Here "Wheat" is specified.
    glColor3f(0.96078, 0.870588, 0.7019607);

    for (int i = 0; i < BondList.size(); ++i)
    {
        // Name this object for OpenGL's Selection Mode.  Everything draw until this loop completes will be considered one object by OpenGL's selection
        // mode.  If any part of this object is within the viewing volume, it will return a hit to the selection buffer.  This command is only valid
        // during Selection Mode, otherwise it is ignored.
        glLoadName(i + PosX.size() + 1);

        if (!selectRenderMode)
        {
            if (BondOrders[i] == 1)
            {
                // Single Bond

                glPushMatrix();
                    // Cylinders are drawn at the origin and radiating toward the positive z-axis by default.  Before drawing the bond, we have to move to the correct
                    // location before proceeding.
                    glTranslatef(PosX[BondList[i][0] - 1], PosY[BondList[i][0] - 1], PosZ[BondList[i][0] - 1]);

                    // We now rotate our orentation to align the z-axis with the bond vector.
                    glRotatef(BondNormals[i][2], BondNormals[i][0], BondNormals[i][1], 0);

                    // Now we draw the cylinder representing the bond by calling the glu routine.
                    // The parameters are: object_name, radius at bottom, radius at top, length of cylinder, # of slices, # of stacks
                    gluCylinder(cylinder, 0.1, 0.1, BondLengths[i], 10, 5);

                // Move back to the origin and starting orientation, so the next call will function properly.
                glPopMatrix();
            }
            else if (BondOrders[i] == 2)
            {
                // Double Bond...  We draw two skinny cylinder next to one another.

                glPushMatrix();
                    // Cylinders are drawn at the origin and radiating toward the positive z-axis by default.  Before drawing the bond, we have to move to the correct
                    // location before proceeding.
                    glTranslatef(PosX[BondList[i][0] - 1] + 0.1, PosY[BondList[i][0] - 1] + 0.1, PosZ[BondList[i][0] - 1]);

                    // We now rotate our orentation to align the z-axis with the bond vector.
                    glRotatef(BondNormals[i][2], BondNormals[i][0], BondNormals[i][1], 0);

                    // Now we draw the cylinder representing the bond by calling the glu routine.
                    // The parameters are: object_name, radius at bottom, radius at top, length of cylinder, # of slices, # of stacks
                    gluCylinder(cylinder, 0.075, 0.075, BondLengths[i], 10, 5);

                // Move back to the origin and starting orientation, so the next call will function properly.
                glPopMatrix();

                glPushMatrix();
                    // Cylinders are drawn at the origin and radiating toward the positive z-axis by default.  Before drawing the bond, we have to move to the correct
                    // location before proceeding.
                    glTranslatef(PosX[BondList[i][0] - 1] - 0.1, PosY[BondList[i][0] - 1] - 0.1, PosZ[BondList[i][0] - 1]);

                    // We now rotate our orentation to align the z-axis with the bond vector.
                    glRotatef(BondNormals[i][2], BondNormals[i][0], BondNormals[i][1], 0);

                    // Now we draw the cylinder representing the bond by calling the glu routine.
                    // The parameters are: object_name, radius at bottom, radius at top, length of cylinder, # of slices, # of stacks
                    gluCylinder(cylinder, 0.075, 0.075, BondLengths[i], 10, 5);

                // Move back to the origin and starting orientation, so the next call will function properly.
                glPopMatrix();
            }
            else if (BondOrders[i] == 3)
            {
                // Triple Bond...  We draw three skinny cylinders next to each other.

                glPushMatrix();
                    // Cylinders are drawn at the origin and radiating toward the positive z-axis by default.  Before drawing the bond, we have to move to the correct
                    // location before proceeding.
                    glTranslatef(PosX[BondList[i][0] - 1] + 0.15, PosY[BondList[i][0] - 1] + 0.15, PosZ[BondList[i][0] - 1]);

                    // We now rotate our orentation to align the z-axis with the bond vector.
                    glRotatef(BondNormals[i][2], BondNormals[i][0], BondNormals[i][1], 0);

                    // Now we draw the cylinder representing the bond by calling the glu routine.
                    // The parameters are: object_name, radius at bottom, radius at top, length of cylinder, # of slices, # of stacks
                    gluCylinder(cylinder, 0.06, 0.06, BondLengths[i], 10, 5);

                // Move back to the origin and starting orientation, so the next call will function properly.
                glPopMatrix();

                glPushMatrix();
                    // Cylinders are drawn at the origin and radiating toward the positive z-axis by default.  Before drawing the bond, we have to move to the correct
                    // location before proceeding.
                    glTranslatef(PosX[BondList[i][0] - 1], PosY[BondList[i][0] - 1], PosZ[BondList[i][0] - 1]);

                    // We now rotate our orentation to align the z-axis with the bond vector.
                    glRotatef(BondNormals[i][2], BondNormals[i][0], BondNormals[i][1], 0);

                    // Now we draw the cylinder representing the bond by calling the glu routine.
                    // The parameters are: object_name, radius at bottom, radius at top, length of cylinder, # of slices, # of stacks
                    gluCylinder(cylinder, 0.06, 0.06, BondLengths[i], 10, 5);

                // Move back to the origin and starting orientation, so the next call will function properly.
                glPopMatrix();

                glPushMatrix();
                    // Cylinders are drawn at the origin and radiating toward the positive z-axis by default.  Before drawing the bond, we have to move to the correct
                    // location before proceeding.
                    glTranslatef(PosX[BondList[i][0] - 1] - 0.15, PosY[BondList[i][0] - 1] - 0.15, PosZ[BondList[i][0] - 1]);

                    // We now rotate our orentation to align the z-axis with the bond vector.
                    glRotatef(BondNormals[i][2], BondNormals[i][0], BondNormals[i][1], 0);

                    // Now we draw the cylinder representing the bond by calling the glu routine.
                    // The parameters are: object_name, radius at bottom, radius at top, length of cylinder, # of slices, # of stacks
                    gluCylinder(cylinder, 0.06, 0.06, BondLengths[i], 10, 5);

                // Move back to the origin and starting orientation, so the next call will function properly.
                glPopMatrix();
            }
        }
        else
        {
            glPushMatrix();
                // Cylinders are drawn at the origin and radiating toward the positive z-axis by default.  Before drawing the bond, we have to move to the correct
                // location before proceeding.
                glTranslatef(PosX[BondList[i][0] - 1], PosY[BondList[i][0] - 1], PosZ[BondList[i][0] - 1]);

                // We now rotate our orentation to align the z-axis with the bond vector.
                glRotatef(BondNormals[i][2], BondNormals[i][0], BondNormals[i][1], 0);

                // Now we draw the cylinder representing the bond by calling the glu routine.
                // The parameters are: object_name, radius at bottom, radius at top, length of cylinder, # of slices, # of stacks
                gluCylinder(cylinder, 0.1, 0.1, BondLengths[i], 10, 5);

            // Move back to the origin and starting orientation, so the next call will function properly.
            glPopMatrix();
        }
    }


}

void CPSviewer::drawDreidingRepresentation()
{
    // Dreiding Model... Prefered method among the biological sciences. We're going to render a lot of spheres and cylinders, so we're going to rely on the handy routines provided by the
    // Graphics Library Utility (glu) to efficiently draw the model.

    ///////////////////////////////////////////////////////////////////////////
    //              SETUP
    ///////////////////////////////////////////////////////////////////////////



    // Enable anti-aliasing techniques to improve image quality.  For the Ball-and-Stick Representation, we need
    // to make sure OpenGL uses techniques designed for 3D surfaces because the intersection of solids looks really
    // bad, even when a lot of polygons are used.  We can turn off anti-aliasing methods for lines and points for this representation.
    if (glIsEnabled(GL_POINT_SMOOTH))
    {
        glDisable(GL_POINT_SMOOTH);
    }

    if (glIsEnabled(GL_LINE_SMOOTH))
    {
        glDisable(GL_LINE_SMOOTH);
    }

    glEnable(GL_MULTISAMPLE);
    glHint(GL_MULTISAMPLE, GL_NICEST);


    GLUquadricObj *sphere, *cylinder; // Quadric Object Pointers
    sphere = gluNewQuadric();  // initialize objects for the glu routine
    cylinder = gluNewQuadric();

    // Tell OpenGL about the normals we'd like to use for vertex shading.  We'd like OpenGL to smoothly interpolate the colors between vertices.
    // This will help hide the polygon faces that approximate the curved surfaces.  That means we can use bigger polygons to improve performace without sacrificing quality.
    gluQuadricNormals(sphere, GLU_SMOOTH);
    gluQuadricNormals(cylinder, GLU_SMOOTH);

    //////////////////////////////////////////////////////////////////////////
    //                Drawing
    //////////////////////////////////////////////////////////////////////////

    /* The Name Stack commands are only executed during Selection Mode.  This code is ignored if we are just drawing the model.
       When we are done drawing, this function is executed once more, and we ask OpenGL to track the shapes it draws and tell us which ones
       are in a specific area (the ones the User is clicking on).  */

    // Begin drawing routine.  We will begin by drawing the balls (atoms).
    // Cycle through the atoms in the Atlas Framework.
    for (int i = 0; i < PosX.size(); ++i)
    {
        // Save current location so we can undo our transformations.
        glPushMatrix();

            // Set the drawing to this color.
            glColor3f(Colors[i][0], Colors[i][1], Colors[i][2]);

            // The glu routine renders spheres at the origin as a default, so we need to ensure we center it on the atom's coordinates.
            glTranslatef(PosX[i], PosY[i], PosZ[i]);

            // Call the glu routine to render the sphere.
            // The parameters are: object_name, radius, # of slices, # of stacks
            gluSphere(sphere, 0.2, 24, 12);

        // Move back to the origin, so the next call will place the atom at the correct location.
        glPopMatrix();
    }


    // We can move on to drawing the bonds.
    // Cycle through the bonds listed in the Atlas Framework.
    for (int i = 0; i < BondList.size(); ++i)
    {
        glPushMatrix();
            // We specify that the bonds are colored to the midpoint according to the atom being represented.

            // Set the color to the first atom.
            glColor3f(Colors[BondList[i][0] - 1][0], Colors[BondList[i][0] - 1][1], Colors[BondList[i][0] - 1][2]);

            // Cylinders are drawn at the origin and radiating toward the positive z-axis by default.  Before drawing the bond, we have to move to the correct
            // location before proceeding.
            glTranslatef(PosX[BondList[i][0] - 1], PosY[BondList[i][0] - 1], PosZ[BondList[i][0] - 1]);

            // We now rotate our orentation to align the z-axis with the bond vector.
            glRotatef(BondNormals[i][2], BondNormals[i][0], BondNormals[i][1], 0);

            // Now we draw the cylinder representing the bond by calling the glu routine.
            // The parameters are: object_name, radius at bottom, radius at top, length of cylinder, # of slices, # of stacks
            gluCylinder(cylinder, 0.2, 0.2, BondLengths[i] / 2, 24, 5);

        // Move back to the origin and starting orientation, so the next call will function properly.
        glPopMatrix();

        // Now draw the second half of the bond.
        glPushMatrix();

            // Set the color to the second atom.
            glColor3f(Colors[BondList[i][1] - 1][0], Colors[BondList[i][1] - 1][1], Colors[BondList[i][1] - 1][2]);

            // Cylinders are drawn at the origin and radiating toward the positive z-axis by default.  Before drawing the bond, we have to move to the correct
            // location before proceeding.
            glTranslatef(PosX[BondList[i][1] - 1], PosY[BondList[i][1] - 1], PosZ[BondList[i][1] - 1]);

            // We now rotate our orentation to align the z-axis with the bond vector.
            glRotatef(BondNormals[i][2] + 180, BondNormals[i][0], BondNormals[i][1], 0);

            // Now we draw the cylinder representing the bond by calling the glu routine.
            // The parameters are: object_name, radius at bottom, radius at top, length of cylinder, # of slices, # of stacks
            gluCylinder(cylinder, 0.2, 0.2, BondLengths[i] / 2, 24, 5);

        // Move back to the origin and starting orientation, so the next call will function properly.
        glPopMatrix();

    }


}

void CPSviewer::drawSpaceFillingRepresentation()
{
    // Space-Filling Model...  We're going to need a lot of spheres.  We're going to rely on the handy routines provided by the
    // Graphics Library Utility (glu) to efficiently draw the model.

    ///////////////////////////////////////////////////////////////////////////
    //              SETUP
    ///////////////////////////////////////////////////////////////////////////

    // Enable anti-aliasing techniques to improve image quality.  For the Space Filling Representation, we need
    // to make sure OpenGL uses techniques designed for 3D surfaces because the intersection of spheres looks really
    // bad, even when a lot of polygons are used.  We can turn off anti-aliasing methods for lines and points for this representation.
    if (glIsEnabled(GL_POINT_SMOOTH))
    {
        glDisable(GL_POINT_SMOOTH);
    }

    if (glIsEnabled(GL_LINE_SMOOTH))
    {
        glDisable(GL_LINE_SMOOTH);
    }

    glEnable(GL_MULTISAMPLE);
    glHint(GL_MULTISAMPLE, GL_NICEST);


    GLUquadricObj *sphere; // Quadric Object Pointer
    sphere = gluNewQuadric();  // initialize object for the glu routine

    // Tell OpenGL about the normals we'd like to use for vertex shading.  We'd like OpenGL to smoothly interpolate the colors between vertices.
    // This will help hide the polygon faces that approximate the spheres.  That means we can use bigger polygons to improve performace without sacrificing quality.
    gluQuadricNormals(sphere, GLU_SMOOTH);




    //////////////////////////////////////////////////////////////////////////
    //                Drawing
    //////////////////////////////////////////////////////////////////////////

    /* The Name Stack commands are only executed during Selection Mode.  This code is ignored if we are just drawing the model.
       When we are done drawing, this function is executed once more, and we ask OpenGL to track the shapes it draws and tell us which ones
       are in a specific area (the ones the User is clicking on).  */

    // Begin drawing routine.  Cycle through the atoms in the Atlas Framework.
    for (int i = 0; i < PosX.size(); ++i)
    {
        // Name this object for OpenGL's Selection Mode.  Everything draw until this loop completes will be considered one object by OpenGL's selection
        // mode.  If any part of this object is within the viewing volume, it will return a hit to the selection buffer.  This command is only valid
        // during Selection Mode, otherwise it is ignored.
        glLoadName(i + 1);

        // Save current location so we can undo our transformations.
        glPushMatrix();

            // Set the drawing to this color.
            glColor3f(Colors[i][0], Colors[i][1], Colors[i][2]);

            // The glu routine renders spheres at the origin as a default, so we need to ensure we center it on the atom's coordinates.
            glTranslatef(PosX[i], PosY[i], PosZ[i]);

            // Call the glu routine to render the sphere.
            // The parameters are: object_name, radius, # of slices, # of stacks
            gluSphere(sphere, Radii[i], 48, 24);

        // Move back to the origin, so the next call will place the atom at the correct location.
        glPopMatrix();
    }
}

void CPSviewer::drawVectorRepresentation()
{
    // Vector Representation...  Lines represent the bonded atoms.  The lines are painted to their center to show more clear
    // demarcation of atom types.

    ///////////////////////////////////////////////////////////////////////////
    //              SETUP
    ///////////////////////////////////////////////////////////////////////////

    glLineWidth(1.0);

    // Enable anti-aliasing techniques to improve image quality.  For the Vector Representation, we need
    // to make sure OpenGL uses techniques designed for Lines because unaltered lines are rough to look at.
    // We need to turn off Multi-sampling, an anti-aliasing method for solid surfaces, because it inhibits calls to line and point AA,
    // and it is more expensive than the methods we'd like to use.
    if (glIsEnabled(GL_MULTISAMPLE))
    {
        glDisable(GL_MULTISAMPLE);
    }

    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH, GL_NICEST);



    //////////////////////////////////////////////////////////////////////////
    //                Drawing
    //////////////////////////////////////////////////////////////////////////

    // Loop over the vector that holds the paired atoms and draw lines between them.
    // bondData holds the atom ID numbers, and their positions are contained within posX, posY, and posZ.

    glBegin(GL_LINES); // Enter Drawing Routine.
    for (int i = 0; i < BondList.size(); ++i)
    {
        // It is entered compactly below, and without holder variables, but the ends of the line are given as the
        // positions of the atoms making the bonded pair.  [PosX, PosY, PosZ].  The integer given as an index for
        // PosX (or PosY or PosZ) is the integer atom ID stored within bondList.

        glColor3f(Colors[BondList[i][0] - 1][0], Colors[BondList[i][0] - 1][1], Colors[BondList[i][0] - 1][2]); // First atom's color is set.
        glVertex3f(PosX[BondList[i][0] - 1],    PosY[BondList[i][0] - 1],   PosZ[BondList[i][0] - 1]); // First Atom's Position
        glVertex3f(BondCenters[i][0], BondCenters[i][1], BondCenters[i][2]); // Bond Center

        glColor3f(Colors[BondList[i][1] - 1][0], Colors[BondList[i][1] - 1][1], Colors[BondList[i][1] - 1][2]); // Second atom's color is set.
        glVertex3f(BondCenters[i][0], BondCenters[i][1], BondCenters[i][2]); // Bond Center
        glVertex3f(PosX[BondList[i][1] - 1],   PosY[BondList[i][1] - 1],   PosZ[BondList[i][1] - 1]); // Second Atom's Position
    }

    glEnd();  // End Drawing Routine.
}

void CPSviewer::drawEnhancedVectorRepresentation()
{
    // "Enhanced" Vector Representation...  historically and techincally speaking, vetor representation should only include the lines
    // that represent the bonded atoms.  This has a little more flair since the atom coordinates are explicitly represented with points and
    // not just line junctions.

    ///////////////////////////////////////////////////////////////////////////
    //              SETUP
    ///////////////////////////////////////////////////////////////////////////

    glPointSize(5.0);   // Sets the dot size of the atom
    glLineWidth(1.0);

    // Enable anti-aliasing techniques to improve image quality.  For the Enhanced Vector Representation, we need
    // to make sure OpenGL uses techniques designed for Lines and Points because unaltered lines are rough to look at, and points come out as squares.
    // We need to turn off Multi-sampling, an anti-aliasing method for solid surfaces, because it inhibits calls to line and point AA,
    // and it is more expensive than the methods we'd like to use here.
    if (glIsEnabled(GL_MULTISAMPLE))
    {
        glDisable(GL_MULTISAMPLE);
    }

    glEnable(GL_POINT_SMOOTH);
    glHint(GL_POINT_SMOOTH, GL_NICEST);
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH, GL_NICEST);



    //////////////////////////////////////////////////////////////////////////
    //                Drawing
    //////////////////////////////////////////////////////////////////////////

    // Begin drawing routine.  Cycle through the atoms in the Atlas Framework. One point will be placed at every vertex listed between here and "glEnd"
    glBegin(GL_POINTS);
        for (int i = 0; i < PosX.size(); ++i)
        {
            // Set the color for drawing.
            glColor3f(Colors[i][0], Colors[i][1], Colors[i][2]);

            // Draw vertex.
            glVertex3f(PosX[i], PosY[i], PosZ[i]);
        }
    glEnd();  // End drawing atoms.


    // Now, we move on to bond vectors. Loop over the vector that holds the paired atoms and draw lines between them.
    // bondData holds the atom ID numbers, and their positions are contained within posX, posY, and posZ.

    glBegin(GL_LINES); // Enter Drawing Routine.
    for (int i = 0; i < BondList.size(); ++i)
    {
        // It is entered compactly below, and without holder variables, but the ends of the line are given as the
        // positions of the atoms making the bonded pair.  [PosX, PosY, PosZ].  The integer given as an index for
        // PosX (or PosY or PosZ) is the integer atom ID stored within bondList.

        glColor3f(Colors[BondList[i][0] - 1][0], Colors[BondList[i][0] - 1][1], Colors[BondList[i][0] - 1][2]); // First atom's color is set.
        glVertex3f(PosX[BondList[i][0] - 1],    PosY[BondList[i][0] - 1],   PosZ[BondList[i][0] - 1]); // First Atom's Position
        glVertex3f(BondCenters[i][0], BondCenters[i][1], BondCenters[i][2]); // Bond Center

        glColor3f(Colors[BondList[i][1] - 1][0], Colors[BondList[i][1] - 1][1], Colors[BondList[i][1] - 1][2]); // Second atom's color is set.
        glVertex3f(BondCenters[i][0], BondCenters[i][1], BondCenters[i][2]); // Bond Center
        glVertex3f(PosX[BondList[i][1] - 1],   PosY[BondList[i][1] - 1],   PosZ[BondList[i][1] - 1]); // Second Atom's Position
    }

    glEnd();  // End Drawing Routine.
}



//********************************************************************************************
// Functions in this section deal with drawing routines for the various model representations
// when part of the model is selected and highlighted on-screen.
//*********************************************************************************************

void CPSviewer::drawSelectedModel()
{
    // From here on, we need call OpenGL commands based on the representation method we're going after.
    // 1 = Ball-and-Stick
    // 2 = Licorice (Dreiding)
    // 3 = Space Filling (CPK)
    // 4 = Vector (Wireframe)
    // 5 = Vector with Points


    if (representationMethod == 0)
    {
        drawSelectedBallandStickRepresentation();
    }
    else if (representationMethod == 1)
    {
        drawSelectedDreidingRepresentation();
    }
    else if (representationMethod == 2)
    {
        drawSelectedSpaceFillingRepresentation();
    }
    else if (representationMethod == 3 | representationMethod == 4)
    {
        drawSelectedVectorRepresentation();
    }
}

void CPSviewer::drawSelectedBallandStickRepresentation()
{
    // Ball-and-Stick Model... A favorite staple of chemistry. We're going to render a lot of spheres and cylinders, so we're going to rely on the handy routines provided by the
    // Graphics Library Utility (glu) to efficiently draw the model.

    ///////////////////////////////////////////////////////////////////////////
    //              SETUP
    ///////////////////////////////////////////////////////////////////////////

    QVector<int>::Iterator it;

    // Enable anti-aliasing techniques to improve image quality.  For the Ball-and-Stick Representation, we need
    // to make sure OpenGL uses techniques designed for 3D surfaces because the intersection of solids looks really
    // bad, even when a lot of polygons are used.  We can turn off anti-aliasing methods for lines and points for this representation.
    if (glIsEnabled(GL_POINT_SMOOTH))
    {
        glDisable(GL_POINT_SMOOTH);
    }

    if (glIsEnabled(GL_LINE_SMOOTH))
    {
        glDisable(GL_LINE_SMOOTH);
    }

    glEnable(GL_MULTISAMPLE);
    glHint(GL_MULTISAMPLE, GL_NICEST);



    GLUquadricObj *sphere, *cylinder; // Quadric Object Pointers
    sphere = gluNewQuadric();  // initialize objects for the glu routine
    cylinder = gluNewQuadric();

    // Tell OpenGL about the normals we'd like to use for vertex shading.  We'd like OpenGL to smoothly interpolate the colors between vertices.
    // This will help hide the polygon faces that approximate the curved surfaces.  That means we can use bigger polygons to improve performace without sacrificing quality.
    gluQuadricNormals(sphere, GLU_SMOOTH);
    gluQuadricNormals(cylinder, GLU_SMOOTH);


    //////////////////////////////////////////////////////////////////////////
    //                Drawing
    //////////////////////////////////////////////////////////////////////////

    // Begin drawing routine.  We will begin by drawing the balls (atoms).

    // Cycle through the selected atoms in the Atlas Framework... the atoms listed here are atomIDs, but they are being used in indeces of atom position, radii, etc..  where
    // all the indeces start at position 0 or atomID number 1.

    if (!SelectedAtoms.isEmpty())
    {
        // Color this atom according to the highlight color.  The color here is Cerulean Blue.
        glColor4f(0.3, 0.92, 1.0, 0.5);

        for (it = SelectedAtoms.begin(); it != SelectedAtoms.end(); ++it)
        {
            glPushMatrix();
                // The glu routine renders spheres at the origin as a default, so we need to ensure we center it on the atom's coordinates.
                glTranslatef(PosX[*it - 1], PosY[*it - 1], PosZ[*it - 1]);

                // Call the glu routine to render the sphere.
                // The parameters are: object_name, radius, # of slices, # of stacks
                gluSphere(sphere, Radii[*it - 1]*0.35, 24, 12);

            // Move back to the origin, so the next call will place the atom at the correct location.
            glPopMatrix();
        }
    }


    // We can move on to drawing the bonds.
    // Cycle through the selected bonds in the Atlas Framework...
    if (!SelectedBonds.isEmpty())
    {
        // Color this bond according to the highlight color.  The color here is Cerulean Blue.
        glColor4f(0.3, 0.92, 1.0, 0.5);

        for (it = SelectedBonds.begin(); it != SelectedBonds.end(); ++it)
        {
            glPushMatrix();
                // GLU Cylinders are drawn at the origin and radiating toward the positive z-axis by default.  Before drawing the bond, we have to move to the correct
                // location before proceeding.
                glTranslatef(PosX[BondList[*it][0] - 1], PosY[BondList[*it][0] - 1], PosZ[BondList[*it][0] - 1]);

                // We now rotate our orentation to align the z-axis with the bond vector.
                glRotatef(BondNormals[*it][2], BondNormals[*it][0], BondNormals[*it][1], 0);

                // Now we draw the cylinder representing the bond by calling the glu routine.
                // The parameters are: object_name, radius at bottom, radius at top, length of cylinder, # of slices, # of stacks
                gluCylinder(cylinder, 0.15, 0.15, BondLengths[*it], 10, 5);

            // Move back to the origin and starting orientation, so the next call will function properly.
            glPopMatrix();
        }
    }


}

void CPSviewer::drawSelectedDreidingRepresentation()
{
    // Ball-and-Stick Model... A favorite staple of chemistry. We're going to render a lot of spheres and cylinders, so we're going to rely on the handy routines provided by the
    // Graphics Library Utility (glu) to efficiently draw the model.

    ///////////////////////////////////////////////////////////////////////////
    //              SETUP
    ///////////////////////////////////////////////////////////////////////////

    QVector<int>::Iterator it;

    // Enable anti-aliasing techniques to improve image quality.  For the Ball-and-Stick Representation, we need
    // to make sure OpenGL uses techniques designed for 3D surfaces because the intersection of solids looks really
    // bad, even when a lot of polygons are used.  We can turn off anti-aliasing methods for lines and points for this representation.
    if (glIsEnabled(GL_POINT_SMOOTH))
    {
        glDisable(GL_POINT_SMOOTH);
    }

    if (glIsEnabled(GL_LINE_SMOOTH))
    {
        glDisable(GL_LINE_SMOOTH);
    }

    glEnable(GL_MULTISAMPLE);
    glHint(GL_MULTISAMPLE, GL_NICEST);



    GLUquadricObj *sphere, *cylinder; // Quadric Object Pointers
    sphere = gluNewQuadric();  // initialize objects for the glu routine
    cylinder = gluNewQuadric();

    // Tell OpenGL about the normals we'd like to use for vertex shading.  We'd like OpenGL to smoothly interpolate the colors between vertices.
    // This will help hide the polygon faces that approximate the curved surfaces.  That means we can use bigger polygons to improve performace without sacrificing quality.
    gluQuadricNormals(sphere, GLU_SMOOTH);
    gluQuadricNormals(cylinder, GLU_SMOOTH);


    //////////////////////////////////////////////////////////////////////////
    //                Drawing
    //////////////////////////////////////////////////////////////////////////

    // Begin drawing routine.  We will begin by drawing the balls (atoms).

    // Cycle through the selected atoms in the Atlas Framework... the atoms listed here are atomIDs, but they are being used in indeces of atom position, radii, etc..  where
    // all the indeces start at position 0 or atomID number 1.

    if (!SelectedAtoms.isEmpty())
    {
        // Color this atom according to the highlight color.  The color here is Cerulean Blue.
        glColor4f(0.3, 0.92, 1.0, 0.5);

        for (it = SelectedAtoms.begin(); it != SelectedAtoms.end(); ++it)
        {
            glPushMatrix();

                // The glu routine renders spheres at the origin as a default, so we need to ensure we center it on the atom's coordinates.
                glTranslatef(PosX[*it - 1], PosY[*it - 1], PosZ[*it - 1]);

                // Call the glu routine to render the sphere.
                // The parameters are: object_name, radius, # of slices, # of stacks
                gluSphere(sphere, 0.4, 24, 12);

            // Move back to the origin, so the next call will place the atom at the correct location.
            glPopMatrix();
        }
    }


    // Disable backface culling just for drawing these bonds...  selecting bonds using the Dreiding Representation is ugly otherwise.
    glDisable(GL_CULL_FACE);

    // We can move on to drawing the bonds.
    // Cycle through the selected bonds in the Atlas Framework...
    if (!SelectedBonds.isEmpty())
    {
        // Color this bond according to the highlight color.  The color here is Cerulean Blue.
        glColor4f(0.3, 0.92, 1.0, 0.5);

        for (it = SelectedBonds.begin(); it != SelectedBonds.end(); ++it)
        {
            glPushMatrix();

                // GLU Cylinders are drawn at the origin and radiating toward the positive z-axis by default.  Before drawing the bond, we have to move to the correct
                // location before proceeding.
                glTranslatef(PosX[BondList[*it][0] - 1], PosY[BondList[*it][0] - 1], PosZ[BondList[*it][0] - 1]);

                // We now rotate our orentation to align the z-axis with the bond vector.
                glRotatef(BondNormals[*it][2], BondNormals[*it][0], BondNormals[*it][1], 0);

                // Now we draw the cylinder representing the bond by calling the glu routine.
                // The parameters are: object_name, radius at bottom, radius at top, length of cylinder, # of slices, # of stacks
                gluCylinder(cylinder, 0.25, 0.25, BondLengths[*it], 24, 5);

            // Move back to the origin and starting orientation, so the next call will function properly.
            glPopMatrix();
        }
    }

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);


}

void CPSviewer::drawSelectedSpaceFillingRepresentation()
{
    // Space-Filling Model...  We're going to need a lot of spheres.  We're going to rely on the handy routines provided by the
    // Graphics Library Utility (glu) to efficiently draw the model.

    ///////////////////////////////////////////////////////////////////////////
    //              SETUP
    ///////////////////////////////////////////////////////////////////////////

    QVector<int>::Iterator it;

    // Enable anti-aliasing techniques to improve image quality.  For the Space Filling Representation, we need
    // to make sure OpenGL uses techniques designed for 3D surfaces because the intersection of spheres looks really
    // bad, even when a lot of polygons are used.  We can turn off anti-aliasing methods for lines and points for this representation.
    if (glIsEnabled(GL_POINT_SMOOTH))
    {
        glDisable(GL_POINT_SMOOTH);
    }

    if (glIsEnabled(GL_LINE_SMOOTH))
    {
        glDisable(GL_LINE_SMOOTH);
    }

    glEnable(GL_MULTISAMPLE);
    glHint(GL_MULTISAMPLE, GL_NICEST);


    GLUquadricObj *sphere; // Quadric Object Pointer
    sphere = gluNewQuadric();  // initialize object for the glu routine

    // Tell OpenGL about the normals we'd like to use for vertex shading.  We'd like OpenGL to smoothly interpolate the colors between vertices.
    // This will help hide the polygon faces that approximate the spheres.  That means we can use bigger polygons to improve performace without sacrificing quality.
    gluQuadricNormals(sphere, GLU_SMOOTH);




    //////////////////////////////////////////////////////////////////////////
    //                Drawing
    //////////////////////////////////////////////////////////////////////////

    // Cycle through the selected atoms in the Atlas Framework... the atoms listed here are atomIDs, but they are being used in indeces of atom position, radii, etc..  where
    // all the indeces start at position 0 or atomID number 1.

    // Color this atom according to the highlight color.  The color here is Cerulean Blue.
    glColor4f(0.3, 0.92, 1.0, 0.5);

    for (it = SelectedAtoms.begin(); it != SelectedAtoms.end(); ++it)
    {
        // Save current location so we can undo our transformations.
        glPushMatrix();

            // The glu routine renders spheres at the origin as a default, so we need to ensure we center it on the atom's coordinates.
            glTranslatef(PosX[*it - 1], PosY[*it - 1], PosZ[*it - 1]);

            // Call the glu routine to render the sphere.
            // The parameters are: object_name, radius, # of slices, # of stacks
            gluSphere(sphere, Radii[*it - 1]*1.05, 48, 24);

        // Move back to the origin, so the next call will place the atom at the correct location.
        glPopMatrix();
    }

}

void CPSviewer::drawSelectedVectorRepresentation()
{
    // The highlight for vector representation will be an overlay of the Ball-and-Stick Model
    // We're going to render a lot of spheres and cylinders, so we're going to rely on the handy routines provided by the
    // Graphics Library Utility (glu) to efficiently draw the model.

    ///////////////////////////////////////////////////////////////////////////
    //              SETUP
    ///////////////////////////////////////////////////////////////////////////

    QVector<int>::Iterator it;

    // Enable anti-aliasing techniques to improve image quality.  For the Ball-and-Stick Representation, we need
    // to make sure OpenGL uses techniques designed for 3D surfaces because the intersection of solids looks really
    // bad, even when a lot of polygons are used.  We can turn off anti-aliasing methods for lines and points for this representation.
    if (glIsEnabled(GL_POINT_SMOOTH))
    {
        glDisable(GL_POINT_SMOOTH);
    }

    if (glIsEnabled(GL_LINE_SMOOTH))
    {
        glDisable(GL_LINE_SMOOTH);
    }

    glEnable(GL_MULTISAMPLE);
    glHint(GL_MULTISAMPLE, GL_NICEST);



    GLUquadricObj *sphere, *cylinder; // Quadric Object Pointers
    sphere = gluNewQuadric();  // initialize objects for the glu routine
    cylinder = gluNewQuadric();

    // Tell OpenGL about the normals we'd like to use for vertex shading.  We'd like OpenGL to smoothly interpolate the colors between vertices.
    // This will help hide the polygon faces that approximate the curved surfaces.  That means we can use bigger polygons to improve performace without sacrificing quality.
    gluQuadricNormals(sphere, GLU_SMOOTH);
    gluQuadricNormals(cylinder, GLU_SMOOTH);


    //////////////////////////////////////////////////////////////////////////
    //                Drawing
    //////////////////////////////////////////////////////////////////////////

    // Begin drawing routine.  We will begin by drawing the balls (atoms).

    // Cycle through the selected atoms in the Atlas Framework... the atoms listed here are atomIDs, but they are being used in indeces of atom position, radii, etc..  where
    // all the indeces start at position 0 or atomID number 1.

    if (!SelectedAtoms.isEmpty())
    {
        // Color this atom according to the highlight color.  The color here is Cerulean Blue.
        glColor4f(0.3, 0.92, 1.0, 0.5);

        for (it = SelectedAtoms.begin(); it != SelectedAtoms.end(); ++it)
        {
            glPushMatrix();
                // The glu routine renders spheres at the origin as a default, so we need to ensure we center it on the atom's coordinates.
                glTranslatef(PosX[*it - 1], PosY[*it - 1], PosZ[*it - 1]);

                // Call the glu routine to render the sphere.
                // The parameters are: object_name, radius, # of slices, # of stacks
                gluSphere(sphere, 0.1, 24, 12);

            // Move back to the origin, so the next call will place the atom at the correct location.
            glPopMatrix();
        }
    }

    // Temporarily disable back-face culling to improve the appearance of highlights in vector representation.
    glDisable(GL_CULL_FACE);

    // We can move on to drawing the bonds.
    // Cycle through the selected bonds in the Atlas Framework...
    if (!SelectedBonds.isEmpty())
    {
        // Color this bond according to the highlight color.  The color here is Cerulean Blue.
        glColor4f(0.3, 0.92, 1.0, 0.5);

        for (it = SelectedBonds.begin(); it != SelectedBonds.end(); ++it)
        {
            glPushMatrix();
                // GLU Cylinders are drawn at the origin and radiating toward the positive z-axis by default.  Before drawing the bond, we have to move to the correct
                // location before proceeding.
                glTranslatef(PosX[BondList[*it][0] - 1], PosY[BondList[*it][0] - 1], PosZ[BondList[*it][0] - 1]);

                // We now rotate our orentation to align the z-axis with the bond vector.
                glRotatef(BondNormals[*it][2], BondNormals[*it][0], BondNormals[*it][1], 0);

                // Now we draw the cylinder representing the bond by calling the glu routine.
                // The parameters are: object_name, radius at bottom, radius at top, length of cylinder, # of slices, # of stacks
                gluCylinder(cylinder, 0.05, 0.05, BondLengths[*it], 10, 5);

            // Move back to the origin and starting orientation, so the next call will function properly.
            glPopMatrix();
        }
    }

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

}



void CPSviewer::deepRefresh()
{
    /***********************************************************************************************************
     * This function allows us to ask for a refresh of CPSviewer's arrays before the scene is redrawn... this is
     * is especially handy if the User has asked for a change to colors, size, or is editing atoms/bonds.
     * Normally, the User's activities will just need OpenGL to draw the same object from a different angle,
     * so separating this recalculation of values in CPSviewer's arrays from the OpenGL refresh will let us
     * be more efficient where we can.
     **********************************************************************************************************/

    Pandora pandora;

    PosX = prometheusModel.getAtomPositionX();
    PosY = prometheusModel.getAtomPositionY();
    PosZ = prometheusModel.getAtomPositionZ();

    AtomNames = prometheusModel.getAtomName();
    FFTypes = prometheusModel.getAtomForceFieldType();
    PartialCharges = prometheusModel.getAtomPartialCharge();

    // Update the atomic radii array by first grabbing the list of elements (IDed by their atomic numbers)
    // and passing that to Pandora...  she will return a list of radii based on reference data stored within the Pandora Framework.
    AtomicNumbers = prometheusModel.getAtomAtomicNumber();
    Radii = pandora.findRadii(AtomicNumbers);

    // Now do the same for colors.
    Colors = pandora.findColor(AtomicNumbers);

    // Next, we pull the most current bond list from the Atlas Framework and perform calculations with the coordinates so we can display them usefully.
    BondList = prometheusModel.getBondList();
    BondOrders = prometheusModel.getBondOrder();

    BondLengths.clear();
    BondCenters.clear();
    BondNormals.clear();

    pandora.findBondData(BondList, activeModel, BondLengths, BondCenters, BondNormals);


    // Now that the model has been updated, ask OpenGL to redraw the scene, and we're done!
    update();
}

void CPSviewer::requestBuildElementChange(int atomicNumber)
{
    // This Slot takes a signal from the periodic table palette in the form of the desired atomic number.
    builderAtom = atomicNumber;
}

//  This function allows the drawing of an on-screen box for bulk selection.
void CPSviewer::drawSelectionBox(QPoint start, QPoint current)
{
    QVector<int> vertexBL, vertexTL, vertexTR, vertexBR;

    vertexBL.resize(2);
    vertexTL.resize(2);
    vertexTR.resize(2);
    vertexBR.resize(2);

    // The vertices establish the four corners of the on-screen selecton box.  Starting in the bottom
    // left (BL) they wrap clockwise to top left (TL), top right (TR), and bottom right (BR).

    // The User provides two of the corners with the position of the first click and the current cursor location.
    // However, depending on how the User draws the box, the provided corners could be any of the four mentioned above.
    // The first order of business is to figure out the box's corners.

    if (start.x() < current.x())
    {
        // The starting click maps to the left side of the box.

        if (start.y() < current.y())
        {
            // The starting click maps the bottom left corner.
            vertexBL[0] = start.x(); vertexBL[1] = start.y();
            vertexTL[0] = start.x(); vertexTL[1] = current.y();
            vertexTR[0] = current.x(); vertexTR[1] = current.y();
            vertexBR[0] = current.x(); vertexBR[1] = start.y();
        }
        else
        {
            // The starting click maps the top left corner.
            vertexBL[0] = start.x(); vertexBL[1] = current.y();
            vertexTL[0] = start.x(); vertexTL[1] = start.y();
            vertexTR[0] = current.x(); vertexTR[1] = start.y();
            vertexBR[0] = current.x(); vertexBR[1] = current.y();
        }
    }
    else
    {
        // The starting click maps to the right side of the box.

        if (start.y() < current.y())
        {
            // The starting click maps to the bottom right corner.
            vertexBL[0] = current.x(); vertexBL[1] = start.y();
            vertexTL[0] = current.x(); vertexTL[1] = current.y();
            vertexTR[0] = start.x(); vertexTR[1] = current.y();
            vertexBR[0] = start.x(); vertexBR[1] = start.y();
        }
        else
        {
            // The starting click maps to the top right corner.
            vertexBL[0] = current.x(); vertexBL[1] = current.y();
            vertexTL[0] = current.x(); vertexTL[1] = start.y();
            vertexTR[0] = start.x(); vertexTR[1] = start.y();
            vertexBR[0] = start.x(); vertexBR[1] = current.y();
        }
    }

    // Now that we have the box's coordinates, we draw them... but they are drawn in screen coordinates, not model coordinates.
    // Ask OpenGL for the Projection Matrix to access these coordinates.

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, this->width(), 0, this->height(), -1, 10);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glClear(GL_DEPTH_BUFFER_BIT);

    glColor4f(0.3, 0.92, 1.0, 1.0);  // Color of the Selection Box outline..  Cerulean Blue.
    glBegin(GL_LINE_LOOP);
        glVertex2f(vertexBL[0], vertexBL[1]);
        glVertex2f(vertexTL[0], vertexTL[1]);
        glVertex2f(vertexTR[0], vertexTR[1]);
        glVertex2f(vertexBR[0], vertexBR[1]);
    glEnd();


    glColor4f(0, 0, 1.0, 0.1);  // Color of the Selection Box..  Blue.
    glBegin(GL_QUADS);
        glVertex2f(vertexBR[0], vertexBR[1]);
        glVertex2f(vertexTR[0], vertexTR[1]);
        glVertex2f(vertexTL[0], vertexTL[1]);
        glVertex2f(vertexBL[0], vertexBL[1]);
    glEnd();

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);


}


void CPSviewer::drawLabels()
{

        // This function draws labels atom according to the method selected by the User.
        if (requestedLabelStyle == 0)
        {
            // Requested "None" -> Don't draw anything.
            return;
        }

        if (!AtomNames.isEmpty())
        {

            // Labels are drawn in screen coordinates, not model coordinates.
            // Ask OpenGL for the Projection Matrix to access these coordinates.

            glUniform1i(0, -2);

            glMatrixMode(GL_PROJECTION);
            glPushMatrix();
            glLoadIdentity();
            glOrtho(0, this->width(), 0, this->height(), -1, 10);
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();

            glClear(GL_DEPTH_BUFFER_BIT);

            // Get the atom's coordinates as they appear on-screen.
            onScreenPos.clear();
            locateOnScreen();

            glColor3f(0, 1, 0);
            glRasterPos2d(100, 100);


            GLubyte rasters1[20] = {252, 252, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 252, 252};
            GLubyte rasters2[20] = {192, 192, 192, 192, 48, 48, 63, 63, 48, 48, 12, 12, 12, 12, 12, 12, 3, 3, 3, 3};

            glBitmap(8, 20, 0, 0, -8, 0, rasters1);
            glBitmap(8, 20, 0, 0, -8, 0, rasters2);

            if (requestedLabelStyle == 1)
            {
                // Requested "Atomic Name" -> Pull data from AtomNames
                for (int i = 0; i < onScreenPos.size(); i++)
                {

                }
            }
            else if (requestedLabelStyle == 2)
            {
                // Requested "Element" -> Pull data from AtomicNumbers and use Pandora to translate.
                for (int i = 0; i < onScreenPos.size(); i++)
                {

                }
            }
            else if (requestedLabelStyle == 3)
            {
                // Requested "Force Field Type" -> Pull data from FFTypes
                for (int i = 0; i < onScreenPos.size(); i++)
                {

                }
            }
            else if (requestedLabelStyle == 4)
            {
                // Requested "Partial Charge" -> Pull data from PartialCharges
                for (int i = 0; i < onScreenPos.size(); i++)
                {

                }
            }

            glMatrixMode(GL_PROJECTION);
            glPopMatrix();
            glMatrixMode(GL_MODELVIEW);
        }
}

void CPSviewer::locateOnScreen()
{
    // This function locates the atoms on-screen using their position in model space and the camera's current view.

    QVector<int> temp;

    temp.push_back(200);
    temp.push_back(200);
    onScreenPos.push_back(temp);
}


void CPSviewer::drawBitmapGrid()
{
    float side;
    int count;

    if (this->width() / bitmapWidth <= this->height() / bitmapHeight)
    {
        side = float(this->width()) / bitmapWidth;
    }
    else
    {
        side = float(this->height()) / bitmapHeight;
    }

    glUniform1i(0, -3);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, this->width(), 0, this->height(), -1, 10);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glClear(GL_DEPTH_BUFFER_BIT);


    // Draw Lines
    glColor3f(1, 1, 1);
    glBegin(GL_LINES);
        for (int i = 0; i <= bitmapWidth; i++)
        {
            glVertex3f(i*side, 0, 0.1);
            glVertex3f(i*side, this->height(), 0.1);
        }

        for (int i = 0; i <= bitmapHeight; i++)
        {
            glVertex3f(0, i*side, 0.1);
            glVertex3f(this->width(), i*side, 0.1);
        }
    glEnd();

    // Draw Boxes
    count = 0;
    glBegin(GL_QUADS);
        for (int i = 0; i < bitmapHeight; i++)
        {
            for (int j = 0; j < bitmapWidth; j++)
            {
                if (pickedBits.contains(count))
                {
                    glColor3f(1, 0.6, 0.07);
                }
                else
                {
                    glColor3f(0, 0, 1);
                }
                glVertex2f((bitmapWidth-j)*side, i*side);
                glVertex2f((bitmapWidth-j)*side, (i+1)*side);
                glVertex2f((bitmapWidth-j-1)*side, (i+1)*side);
                glVertex2f((bitmapWidth-j-1)*side, i*side);

                count++;
            }
        }
    glEnd();

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

//********************************************
// This section deals with camera controls.
//********************************************

// Namely, we wish to accomplish the following goals with the main viewer.
// 1) Free rotation of the model by clicking and dragging on the viewer.
// 2) Zoom control by using the mouse wheel.
// 3) Not having this section of code interfere with other code designed for atom / electron
//     selection or manipulation.

void CPSviewer::setRotation(int X, int Y, int previousX, int previousY)
{
    // This function applies rotation according to the Arcball routine, famously established by Ken Shoemake.

    // Arcball applies rotation by simulating the user's mouse input as contact with a unit sphere that is free to rotate.
    // In our case, the silhouette of the hypothetical sphere is a circle inscribed in the CPS viewer.  The mouse draws an arc across its surface, and the angle
    // swept out by this determines how we rotate the model.  This sphere we're using for control is always centered at the
    // center of the viewer, even if the scene being rotated is off to the side.

    float startX, startY, endX, endY, radius, hypotenuse, scaleFactor;

    QVector3D  arcStart, arcEnd, perp;

    startX = previousX - width() / 2;
    startY = (previousY - height() / 2) * -1;
    endX = X - width() / 2;
    endY = (Y - height() / 2) * -1;

    // Use the dimensions of the viewer window to establish the Arcball sphere.
    if (width() >= height())
    {
        radius = height();
    }
    else
    {
        radius = width();
    }

    // We must formally establish the endpoints of the arc, in 3 dimensional space on a unit sphere, starting with where the arc begins.
    arcStart.setX(startX / radius);   // 3D x-coordinate of arc start point.
    arcStart.setY(startY / radius);  // 3D y-coordinate of arc end point.

    // Establish the z-coordinate of the arc's start point.  We need to assign whatever value will place the arc on the surface of a unit sphere.
    // Start by determining the hypotenuse of a right triangle composed of the x and y components.
    hypotenuse = arcStart.x()*arcStart.x() + arcStart.y()*arcStart.y();

    // If the hypotenuse is greater than 1, we'll need to scale down the x and y components because we're dealing with a unit vector.  Otherwise, we just make z big enough
    // to place the arc starting point on the unit sphere.
    if (hypotenuse > 1)
    {
        scaleFactor = 1 / qSqrt(hypotenuse);

        arcStart.setX(arcStart.x() * scaleFactor);
        arcStart.setY(arcStart.y() * scaleFactor);
        arcStart.setZ(0);
    }
    else
    {
        arcStart.setZ(qSqrt(1.0 - hypotenuse));
    }

    // Repeat the process for the endpoint of the arc.
    arcEnd.setX(endX / radius);   // 3D x-coordinate of arc start point.
    arcEnd.setY(endY / radius);  // 3D y-coordinate of arc end point.

    // Establish the z-coordinate of the arc's end point.  We need to assign whatever value will place the arc on the surface of a unit sphere.
    // Start by determining the hypotenuse of a right triangle composed of the x and y components.
    hypotenuse = arcEnd.x()*arcEnd.x() + arcEnd.y()*arcEnd.y();

    // If the hypotenuse is greater than 1, we'll need to scale down the x and y components so we're dealing with a unit sphere.  Otherwise, we just make z big enough
    // to place the arc end point on the unit sphere.
    if (hypotenuse > 1)
    {
        scaleFactor = 1 / qSqrt(hypotenuse);

        arcEnd.setX(arcEnd.x() * scaleFactor);
        arcEnd.setY(arcEnd.y() * scaleFactor);
        arcEnd.setZ(0);
    }
    else
    {
        arcEnd.setZ(qSqrt(1.0 - hypotenuse));
    }


    // Currently, the start and end points are represented as vectors originating from the unit sphere's center.  These two vectors contain all
    // the neccessary data to perform the rotation of the model.  Unfortunately, each time OpenGL updates the frame, it does not accumulate rotation....
    // it resets the axes.  To store and accumulate successive rotations, we follow the procedure outlined by Shoemake and exploit the properties of
    // quaternions, a concept similar to vectors, which were also created in the late 19th century.

    // Quaternions are composed of 4 elements:  A vector with an (1) x, (2) y, and (3) z component  and a (4) scalar component.
    // The vector is the axis of rotation, and the scalar component stores the angle of rotation, so quaternions are uniquely suited for describing rotation.

    // What makes them even better is that the product of quaternions describes cumulative rotation.
    // Example:  q1 describes a rotation from A to B.
    //           q2 describes a rotation from B to C.
    //    so, if q3 = q1 * q2  -> q3 describes a rotation from A to C.


    // Use the input vectors to create a quaternion.  The cross product of the start / end vectors yields a vector perpendicular to both.
    // This is the axis of rotation, which means we have our first three pieces.
    perp = QVector3D::crossProduct(arcStart, arcEnd);

    inputRotation.setX(perp.x());
    inputRotation.setY(perp.y());
    inputRotation.setZ(perp.z());

    // Next we set the scalar component.  This is the cosine of the angle between the start and end vectors.  This is given by the vectors' dot product.
    inputRotation.setScalar(QVector3D::dotProduct(arcStart, arcEnd));

    // Now we include this rotation with any we've already accumulated.  Quaterions automatically do this via their products.
    // Be careful. The order is (final * initial), and quaternion multiplication is NOT commutative.
    accumulatedRotation = inputRotation * accumulatedRotation;

    // And now we are done.  We ask OpenGL to update the viewer, and PaintGL will access the quaternion "accumulatedRotation" as a global variable to inform the drawing.
    update();
}

void CPSviewer::setXTranslation(float distance)
{
    // Test to see if the user is asking for another angle.  If so,
    // emit a signal to change the camera's orientation. updateGL is a built-in
    // GLWidget function that will update the drawing to the screen.
    if (distance != xTrans)
    {
        xTrans = distance;
        update();
    }
}

void CPSviewer::setYTranslation(float distance)
{
    // Test to see if the user is asking for another angle.  If so,
    // emit a signal to change the camera's orientation. updateGL is a built-in
    // GLWidget function that will update the drawing to the screen.
    if (distance != yTrans)
    {
        yTrans = distance;
        update();
    }
}






// This section handles the mouse interactions over the viewer's orientation.



void CPSviewer::mousePressEvent(QMouseEvent *event)
{
    /******************************************************************************************************************************
    // This Slot has access to the cursor's position on the screen when the mouse when it is initially clicked.
    // Also, it knows what button on the mouse was pushed.  We use that information to carry out the User's will depending on the
    // tool active at the moment.
    // ****************************************************************************************************************************/

    lastPos = event->pos();

    if (event->buttons() & Qt::LeftButton)
    {
        if (selectTool)
        {
            singleSelect = true;
            int selectX = event->x();
            int selectY = this->height() - event->y();
            selectObjects(selectX, selectY, 1.0, 1.0);
            initialPos = QPoint(selectX, selectY);
        }

        if (deleteTool)
        {
            singleSelect = true;
            int selectX = event->x();
            int selectY = this->height() - event->y();
            selectObjects(selectX, selectY, 1.0, 1.0);
        }

        if (atomTool)
        {
            singleSelect = true;
            atomBuilderMoved = false;
            mousePressed = false;
            int selectX = event->x();
            int selectY = this->height() - event->y();
            initialPos = QPoint(selectX, selectY);
            finalPos = QPoint(selectX, selectY);
            selectObjects(selectX, selectY, 1.0, 1.0);
        }

        if (incrementBOTool)
        {
            singleSelect = true;
            int selectX = event->x();
            int selectY = this->height() - event->y();
            selectObjects(selectX, selectY, 1.0, 1.0);
        }

        if (decrementBOTool)
        {
            singleSelect = true;
            int selectX = event->x();
            int selectY = this->height() - event->y();
            selectObjects(selectX, selectY, 1.0, 1.0);
        }


        if (bitmapMode)
        {
            int selectX = event->x();
            int selectY = this->height() - event->y();
            pickBitmapGrid(selectX, selectY);
        }
    }
}

// This function determines how much the scene should be rotated by looking at how much the mouse was
// dragged after being clicked.
void CPSviewer::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - lastPos.x();
    int dy = event->y() - lastPos.y();

    if (event->buttons() & Qt::LeftButton)
    {
        if (viewTool)
        {
            setRotation(event->x(), event->y(), lastPos.x(), lastPos.y());
        }

        if (selectTool)
        {
            finalPos = QPoint(event->x(), this->height() - event->y());

            float difference = qPow((finalPos.x() - initialPos.x()), 2) + qPow((finalPos.y() - initialPos.y()), 2);

            if (difference > 100)
            {
                // The mouse has moved enough to trigger bulk selection.  Set the singleSelect flag to false and ask OpenGL to redraw the scene
                // with a rectangle that highlights the selction area.
                singleSelect = false;
                update();
            }
        }

        if (atomTool)
        {
            finalPos = QPoint(event->x(), this->height() - event->y());

            float difference = qPow((finalPos.x() - initialPos.x()), 2) + qPow((finalPos.y() - initialPos.y()), 2);
            if (difference > 100)
            {
                // The mouse has moved enough to trigger the special function of the Atom Build Tool.  A new atom will be dropped into the model - bonded to whatever
                // atom was at initialPos.
                atomBuilderMoved = true;
                mousePressed = true;
                selectObjects(finalPos.x(), finalPos.y(), 1, 1);
            }
        }

    }
    else if (event->buttons() & Qt::RightButton)
    {
        setRotation(event->x(), event->y(), lastPos.x(), lastPos.y());
    }
    else if (event ->buttons() & Qt::MidButton)
    {
        setXTranslation(xTrans + (0.04 * dx) );
        setYTranslation(yTrans - (0.04 * dy) );
    }

    lastPos = event->pos();
}

void CPSviewer::mouseReleaseEvent(QMouseEvent *event)
{

    if (event->button() & Qt::LeftButton)
    {
        if (selectTool)
        {
            finalPos = QPoint(event->x(), this->height() - event->y());

            if (!singleSelect)
            {
                // If bulk selection has been triggered use the starting and final mouse coordinates to sweep out an area within the screen
                // and grab anything there.
                useSelectionBox(finalPos.x(), finalPos.y(), initialPos.x(), initialPos.y());
            }
        }

        if (atomTool)
        {
            finalPos = QPoint(event->x(), this->height() - event->y());

            if (atomBuilderMoved)
            {
                // If the Atom Build Tool's special function has been triggered, we will need to turn off the flag that keeps deleting atoms
                // we are placing.  While the mouse moves, the User sees an atom on-screen as placement feedback, but it is not intended to be permanent.
                // This one needs to be permanent.
                mousePressed = false;
                selectObjects(finalPos.x(), finalPos.y(), 1, 1);
                prometheusModel.setNumberOfAtoms();
                prometheusModel.setNumberOfBonds();
            }
        }
    }
}

// This function determines the scene zoom based on movements
// of the mouse wheel.

void CPSviewer::setZoom(float distance)
{
        int zTrans_save = zTrans;
        zTrans += distance;

        if (zTrans > 0)
        {
            zTrans = zTrans_save;
        }

        update();
}


void CPSviewer::wheelEvent(QWheelEvent *event)
{
    WheelDelta = float(event->delta()) / 8.0f;
    if (WheelDelta < 0)
    {
        setZoom(-1.0);
    }
    else
    {
        setZoom(1.0);
    }
    event->ignore();
}

void CPSviewer::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (selectTool)
    {
        singleSelect = true;
        doubleClick = true;
        prometheusModel.clearSelection();
        selectObjects(event->x(), this->height() - event->y(), 1.0, 1.0);
    }
}

void CPSviewer::useSelectionBox(int finalX, int finalY, int initialX, int initialY)
{
    /***************************************************************************************************
     * The purpose of this function is to allow the User to sweep out a box on-screen using the cursor.
     * Everything within the box, regardless of depth will be selected.  We need to call selectObjects
     * and pass to it the center of the selection box, as well as its dimensions.
    ***************************************************************************************************/
    int centerX, centerY, boxHeight, boxWidth;

    centerX = (finalX + initialX) / 2;
    centerY = (finalY + initialY) / 2;

    boxWidth = qAbs(finalX - initialX);
    boxHeight = qAbs(finalY - initialY);

    selectObjects(centerX, centerY, boxWidth, boxHeight);
}


void CPSviewer::pickBitmapGrid(int x, int y)
{
    int index;
    float side;


    if (this->width() / bitmapWidth <= this->height() / bitmapHeight)
    {
        side = float(this->width()) / bitmapWidth;
    }
    else
    {
        side = float(this->height()) / bitmapHeight;
    }


    for (int i = 0; i < bitmapHeight; i++)
    {
        if (y < side*(i+1))
        {
            index = i * bitmapWidth;
            break;
        }
    }

    for (int i = 0; i < bitmapWidth; i++)
    {
        if (x > side*(bitmapWidth-i-1))
        {
            index += i;
            break;
        }
    }

    if (pickedBits.contains(index))
    {
        for (int i = 0; i < pickedBits.size(); i++)
        {
            if (pickedBits[i] == index)
            {
                pickedBits.remove(i);
                break;
            }
        }
    }
    else
    {
        pickedBits.push_back(index);
    }

    update();
}


void CPSviewer::updateProjectionMethod(int index)
{
    // This is a Slot that accepts a Singal from the Main Window GUI.  It communicates the User's desired
    // projection Method to OpenGL.

    projectionMethod = index;
    update();
}

void CPSviewer::requestShallowRefresh()
{
    // This is a Slot that accepts a signal from the Main Window.  All it is meant to do is
    // force the CPS Viewer to update its view, plain and symbol.

    update();
}

void CPSviewer::requestDeepRefresh()
{
    // This is a Slot that accepts a singal from the Main Window.  This refresh asks OpenGL to redraw the scene, but not before
    // updating CPSviewer's Radii and Color storage arrays.

    deepRefresh();
}

void CPSviewer::requestLabelStyle(int label)
{
    requestedLabelStyle = label;
    update();
}


void CPSviewer::setBitmapMode(bool isChecked)
{
    if (isChecked)
    {
        bitmapMode = true;
    }
    else
    {
        bitmapMode = false;
    }

    update();
}

void CPSviewer::setBitGrid(int width, int height)
{
    bitmapWidth = width;
    bitmapHeight = height;

    pickedBits.clear();
    update();
}

void CPSviewer::printBitmap()
{
    int temp, numBytes;
    QVector<int> bitmapOutput1, bitmapOutput2;

    bitmapOutput1.clear();
    bitmapOutput2.clear();

    if ((bitmapHeight*bitmapWidth) % 8 == 0)
    {
        numBytes = (bitmapHeight * bitmapWidth) / 8;
    }
    else
    {
        numBytes = (bitmapHeight * bitmapWidth) / 8 + 1;
    }

    for (int i = 0; i < numBytes; i++)
    {
        temp = 0;

        if (pickedBits.contains(i*8))
        {
            temp += 1;
        }

        if (pickedBits.contains(i*8 + 1))
        {
            temp += 2;
        }

        if (pickedBits.contains(i*8 + 2))
        {
            temp += 4;
        }

        if (pickedBits.contains(i*8 + 3))
        {
            temp += 8;
        }

        if (pickedBits.contains(i*8 + 4))
        {
            temp += 16;
        }

        if (pickedBits.contains(i*8 + 5))
        {
            temp += 32;
        }

        if (pickedBits.contains(i*8 + 6))
        {
            temp += 64;
        }

        if (pickedBits.contains(i*8 + 7))
        {
            temp += 128;
        }

        if (i % 2 == 0)
        {
            bitmapOutput1.push_back(temp);
        }
        else
        {
            bitmapOutput2.push_back(temp);
        }
    }

    qDebug() << bitmapOutput1.size();
    qDebug() << bitmapOutput1;
    qDebug() << bitmapOutput2.size();
    qDebug() << bitmapOutput2;
}

//// Key handler
void CPSviewer::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
        case Qt::Key_Control:
            controlIsPressed = true;
            break;
        case Qt::Key_A:
            if (Qt::ControlModifier)
            {
                if (selectTool)
                {
                    controlAll = true;
                    // All atoms will be selected regardless, but we need to trigger selection, so we give the function dummy values.
                    selectObjects(1.0, 1.0, 1.0, 1.0);
                }
            }
            break;
        case Qt::Key_Backspace:
            prometheusModel.deleteHighlightedObjects();
            SelectedAtoms.clear();
            SelectedBonds.clear();
            deepRefresh();
            break;
        default:
            event->ignore();
    }
}

void CPSviewer::keyReleaseEvent(QKeyEvent *event)
{
    switch (event->key())
    {
        case Qt::Key_Control:
            controlIsPressed = false;
            controlAll = false;
            break;
        default:
            event->ignore();
    }
}


// These functions enable Drag and Drop capabilities of Prometheus.

void CPSviewer::dragEnterEvent(QDragEnterEvent *event)
{
    // This function is needed to enable dropEvent below.  Basically just ask Prometheus to accept whatever is dropped onto it.
    event->acceptProposedAction();
}

void CPSviewer::dropEvent(QDropEvent *event)
{
    // This function allows for Drag and Drop capabilities.  After this file is implemented, simply drop
    // a file onto the main viewer and the model should load.

    QString fileName = event->mimeData()->urls()[0].toLocalFile();
    emit dragAndDropFilename(fileName);

    // Send a request for Prometheus to fill in data for the atom's color and size based on User settings.
    // editAllAtomColors();
    // editAllAtomRadii();
}



// These Slots catch Signals from the tool GUI in MainWindow and inform OpenGL as to the User's desired interaction mode.

void CPSviewer::useViewTool()
{
    viewTool = true;
    selectTool = atomTool = deleteTool = measureTool = false;
}

void CPSviewer::useSelectTool()
{
    viewTool = false;
    selectTool = true;
    atomTool = deleteTool = measureTool = incrementBOTool = decrementBOTool = false;
}

void CPSviewer::useAtomTool()
{
    viewTool = false;
    atomTool = true;
    selectTool = deleteTool = measureTool = incrementBOTool = decrementBOTool = false;
}

void CPSviewer::useDeleteTool()
{
    viewTool = false;
    deleteTool = true;
    selectTool = atomTool = measureTool = incrementBOTool = decrementBOTool = false;
}

void CPSviewer::useMeasureTool()
{
    viewTool = false;
    measureTool = true;
    selectTool = atomTool = deleteTool = incrementBOTool = decrementBOTool = false;
}

void CPSviewer::useIncrementBOTool()
{
    viewTool = false;
    incrementBOTool = true;
    selectTool = atomTool = deleteTool = measureTool = decrementBOTool = false;
}

void CPSviewer::useDecrementBOTool()
{
    viewTool = false;
    decrementBOTool = true;
    selectTool = atomTool = deleteTool = measureTool = incrementBOTool = false;
}


void CPSviewer::selectObjects(int centerX, int centerY, int boxWidth, int boxHeight)
{
    /*******************************************************************************************************************************************
        This function is a piece of boiler-plating neccesary to enable object selection within the OpenGL context.
        Namely, we want the User to be able to pick objects within the OpenGL viewer using a mouse.  This is used to facilitate a wide
        variety of tools in Prometheus.
    *******************************************************************************************************************************************/
    GLint hitCount, view[4];



    // Set the maximum size of the Selection Buffer.
    selectionBufferSize = (prometheusModel.getNumberOfAtoms() + prometheusModel.getNumberOfBonds()) * 4;

    // In Selection Mode, OpenGL is able to provide a list of objects that are contained within an arbitrary volume.
    // but, before we engage the Selection Mode ability of OpenGL, we must first initialize an array capable of holding the list of objects found in the on-screen search.
    // This is refered to as the selction buffer.
    GLuint selectionBuffer[selectionBufferSize];
    glSelectBuffer(selectionBufferSize, selectionBuffer);

    if (controlAll)
    {
        /* If the User has pressed Ctrl + A (Command + A on Mac's), all atoms in the model must be selected.
         *We can skip the selection mode of OpenGL and manually fill the selection buffer with all objects. */

        processHits(hitCount, selectionBuffer);
    }
    else
    {
        // The User wishes to select only a portion of the objects on screen.  We utilize OpenGL's selection mode.


        // We ask OpenGL to give us the current viewport size parameters.  We save them now, because we are about to change them using gluPickMatrix, a handy glu library function.
        glGetIntegerv(GL_VIEWPORT, view);

        // We ask OpenGL to stop rendering images to the screen (Render Mode), and enter Selection Mode.
        glRenderMode(GL_SELECT);
        selectRenderMode = true;

        // Initialize the Name Stack... this names the objects within OpenGL, so it can name them in the Selection Buffer.
        glInitNames();
        // Place a number on the stack, glLoadName throws an error when used on an empty stack.
        glPushName(0);

        // We need to specify the viewing volume used by OpenGL to return selection hits.  This is done by restricting the viewing window itself, but the User shouldn't see anything different,
        // because this is all being done in Selection Mode.  The restriction is performed by the handy glu library function, gluPickMatrix.  We need the screen coordinates of the
        // cursor and the original view window size.
        glMatrixMode(GL_PROJECTION);

        // First save the current view, so we can restore it later.
        glPushMatrix();
        glLoadIdentity();
        gluPickMatrix(centerX, centerY, boxWidth, boxHeight, view);
        gluPerspective(45.0f, static_cast<GLfloat>(view[2])/view[3], 0.1f, 100.0f);

        // Now we're ready to "draw" objects to see what goes inside the selection buffer.  Switch over to the modelview matrix.
        glMatrixMode(GL_MODELVIEW);

        // If we've drawn a box over the model with bulk selection, we'll have to reset things...   otherwise, we can skip this.
        if (!singleSelect)
        {
            glTranslatef(xTrans, yTrans, zTrans);
            float angle = qAcos(accumulatedRotation.scalar()) * 114.592750;
            glRotatef(angle, accumulatedRotation.x(), accumulatedRotation.y(), accumulatedRotation.z());
        }


        /**********************************************************************************************************************************************************
         * Now, we just redraw the model.  Anything that gets drawn inside the viewing volume will be added to our "Hit List", the selection buffer. But to make
         * our lives easier, we choose to redraw the Ball-and-Stick Model.  With the exception of the CPK, Space Filling model, all representation methods conform
         * to the general size of the Ball and Stick Model...  what's more, that method guarantees that the atom position is always on top.  The Drieiding method's
         * tubes often block the selection of atom coordinates, and OpenGL cannot get hits of the thin lines availalbe in the vecor.  At no point does the User see
         * the Ball-and-Stick model Prometheus uses (unless that's their chosen model), but we can provide superior performance and simplicity of code this way.
        **********************************************************************************************************************************************************/

        if (atomTool)
        {
            // We need to update the viewer's copy of the model from the Atlas Framework...  we can draw atoms and bonds as feedback to the User, but we cannot let
            // that interfere with atom selection.
            Pandora pandora;

            PosX = prometheusModel.getAtomPositionX();
            PosY = prometheusModel.getAtomPositionY();
            PosZ = prometheusModel.getAtomPositionZ();

            // Update the atomic radii array by first grabbing the list of elements (IDed by their atomic numbers)
            // and passing that to Pandora...  she will return a list of radii based on reference data stored within the Pandora Framework.
            AtomicNumbers = prometheusModel.getAtomAtomicNumber();
            Radii = pandora.findRadii(AtomicNumbers);

             // Next, we pull the most current bond list from the Atlas Framework and perform calculations with the coordinates so we can display them usefully.
            BondList = prometheusModel.getBondList();

            BondLengths.clear();
            BondCenters.clear();
            BondNormals.clear();

            pandora.findBondData(BondList, activeModel, BondLengths, BondCenters, BondNormals);
        }


        if (representationMethod != 2)
        {
            drawBallandStickRepresentation();
        }
        else
        {
            // For the Space Filling (CPK) Method, we're better off dealing that method for selection and picking.
            drawSpaceFillingRepresentation();
        }

        // Okay, we're done filling the Selection Buffer.  Restore the view to the way it was, or we'll mess up how the model is drawn.  Recall, we saved the previous view
        // in the Projection Matrix...  switch over to that matrix.
        glMatrixMode(GL_PROJECTION);
        glPopMatrix();

        // And now place OpenGL back into its default use of the Model Matrix, since we're done messing with the Projection Matrix.
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        // Change back to the standard Render Mode.  This function returns the number of Hits found during the previous (Selection) mode.
        hitCount = glRenderMode(GL_RENDER);
        selectRenderMode = false;

        // Process the Hits and fill out the "selected atoms" portion of the prometheusModel if there are any.
        processHits(hitCount, selectionBuffer);
    }
}


void CPSviewer::processHits(GLint hitCount, GLuint *selectionBuffer)
{
    /*******************************************************************************************************************************************
       This function is meant to take any hits from the function selectObjects...  which looks for any objects near the cursor
       when the user clicks with the Select Tool...  and parse the data.  It loads the atom IDs into an array within the object prometheusModel.
       Then Prometheus can just iterate through that array and then perform whatever functions are neccessary.
    *******************************************************************************************************************************************/

    // This integer captures the ID of the selected atom... only intended to be used with the Build Tool.
    int editAtomID = -1;

    if (controlAll)
    {
        /* This is a special case of the Selection Tool.  The User has selected the Selection Tool and pressed Ctrl + A
         *(Command + A on Mac's), all atoms and all bonds in the model must be selected.
         *We can skip any decision making below because there is no need to interpret the Selection Buffer...  just highlight everything. */

        prometheusModel.clearSelection();

        for (int i = 1; i <= prometheusModel.getNumberOfAtoms(); ++i)
        {
            // Cycle through all atoms.
            prometheusModel.setSelectedAtom(i);
        }

        for (int i = 0; i < prometheusModel.getNumberOfBonds(); ++i)
        {
            // Cycle through all bonds.
            prometheusModel.setSelectedBond(i);
        }
    }
    else
    {

        // We must go through a somewhat tedious process of interpreting what came back from the Selection Buffer.

        if (hitCount > 0)
        {
            // We need to make sure that the atom/bond is not already highlighted.  We'll need to pull the current list.
            // Because this is passed by reference, we need only call for Prometheus's list once.

            QVector<int> currentAtomHighlights = prometheusModel.getSelectedAtom();
            QVector<int> currentBondHighlights = prometheusModel.getSelectedBond();
            int objectID;

            if (singleSelect)
            {
                /***************************************************************************************************************
                *If the User has not dragged out an area to select multiple objects, we must ensure that the User can only
                *select one object at a time.  OpenGL will return all objects along our z-axis line of sight, regardless of
                *whether the object is visible.  If multiple records are returned in the selection buffer, we must cull
                * them and seek the closest object.
                ***************************************************************************************************************/


                /* First step...  determine what the User is clicking.  Since the User is restricted to clicking only one object at a time,
                 * the closest object is typically what they're after.  However, we have to be careful with the Dreiding Model... the tubes that
                 * represent connectivity tend to block the view of the balls marking the atom's locations...  we'll need to do special work
                 * to override this bad behavior. */


                if (hitCount > 1)
                {
                    int closestObject = 0;  // Initialize objectID for comparisons below.

                    for (int i = 1; i < hitCount; ++i)
                    {
                        // Data in the selectionBuffer is in a one dimensional array, but each record has a stride of 4 fields.
                        // Compare the closest known object to the rest on the list and hold on to the winner.
                        if (selectionBuffer[closestObject * 4 + 1] > selectionBuffer[i * 4 + 1])
                        {
                            closestObject = i;
                        }
                    }

                    // We have the closest object now, so we can move on by placing the selected object into its special
                    // place within the Atlas Framework.

                    objectID = selectionBuffer[closestObject * 4 + 3];
                }
                else
                {
                    // There was only one object selected, so we only need to grab its ID.
                    objectID = selectionBuffer[3];
                }



                /* Now that we know for sure that the User is clicking on, that object will either be highlighted or un-highlighted,
                 * depending on the object's current state and other keys the User is pushing.  This section deals with this
                 * and modifies the Atlas Framework according to the result. */

                if (objectID <= prometheusModel.getNumberOfAtoms())
                {
                    // We're dealing with atoms.

                    if (atomTool)
                    {
                        // Override the normal selection behavior with the Build Atom Tool.  De-Select everything and capture the ID of the selected atom.
                        prometheusModel.clearSelection();
                        editAtomID = objectID;
                    }
                    else
                    {
                        // This the standard behavior.
                        if (currentAtomHighlights.contains(objectID) == false)
                        {
                            /* This atom is not currently highlighted.  There are several ways to proceed from here.
                        *  A1) Control is being held down...  other objects are selected currently    -> select the object being clicked.
                        *  A2) Control is being held down...  there are no other selected objects     -> select the object being clicked.
                        *  A3) Control is NOT being held down... other objects are selected currently -> deselect everything, then select ONLY the object being clicked.
                        *  A4) Control is NOT being held donw... there are no other selected objects  -> select the object being clicked. */

                            if (controlIsPressed)
                            {
                                // Situation A1 and A2 (see above) are the same here.
                                prometheusModel.setSelectedAtom(objectID);
                            }
                            else
                            {
                                if (currentAtomHighlights.size() + currentBondHighlights.size() > 0)
                                {
                                    // Situation A3 (see above)
                                    prometheusModel.clearSelection();
                                    prometheusModel.setSelectedAtom(objectID);
                                }
                                else
                                {
                                    // Situation A4 (see above)
                                    prometheusModel.setSelectedAtom(objectID);
                                }
                            }
                        }
                        else
                        {
                            /* This atom is already highlighted.  There are several ways to proceed from here.
                        *  B1) Control is being held down...  other objects are selected currently    -> deselect ONLY the object being clicked.
                        *  B2) Control is being held down...  there are no other selected objects     -> deselect the object being clicked.
                        *  B3) Control is NOT being held down... other objects are selected currently -> deselect everything EXCEPT the object being clicked.
                        *  B4) Control is NOT being held donw... there are no other selected objects  -> deselect the object being clicked. */

                            if (controlIsPressed)
                            {
                                // Situation B1 and B2 (see above) can be implemented together.
                                prometheusModel.unSelectAtom(objectID);
                            }
                            else
                            {
                                if (currentAtomHighlights.size()  + currentBondHighlights.size() > 1)
                                {
                                    // Situation B3 (see above)
                                    prometheusModel.clearSelection();
                                    prometheusModel.setSelectedAtom(objectID);
                                }
                                else
                                {
                                    // Situation B4 (see above)
                                    prometheusModel.clearSelection();
                                }
                            }
                        }
                    }
                }
                else
                {
                    // We're dealing with bonds...  adjust the hit record index.

                    objectID = objectID - prometheusModel.getNumberOfAtoms() - 1;

                    if (currentBondHighlights.contains(objectID) == false)
                    {
                        /* This bond is not currently highlighted.  There are several ways to proceed from here.
                        *  A1) Control is being held down...  other objects are selected currently    -> select the object being clicked.
                        *  A2) Control is being held down...  there are no other selected objects     -> select the object being clicked.
                        *  A3) Control is NOT being held down... other objects are selected currently -> deselect everything, then select ONLY the object being clicked.
                        *  A4) Control is NOT being held donw... there are no other selected objects  -> select the object being clicked. */

                        if (controlIsPressed)
                        {
                            // Situation A1 and A2 (see above) are the same here.
                            prometheusModel.setSelectedBond(objectID);
                        }
                        else
                        {
                            if (currentAtomHighlights.size() + currentBondHighlights.size() > 0)
                            {
                                // Situation A3 (see above)
                                prometheusModel.clearSelection();
                                prometheusModel.setSelectedBond(objectID);
                            }
                            else
                            {
                                // Situation A4 (see above)
                                prometheusModel.setSelectedBond(objectID);
                            }
                        }
                    }
                    else
                    {
                        /* This bond is already highlighted.  There are several ways to proceed from here.
                        *  B1) Control is being held down...  other objects are selected currently    -> deselect ONLY the object being clicked.
                        *  B2) Control is being held down...  there are no other selected objects     -> deselect the object being clicked.
                        *  B3) Control is NOT being held down... other objects are selected currently -> deselect everything EXCEPT the object being clicked.
                        *  B4) Control is NOT being held donw... there are no other selected objects  -> deselect the object being clicked. */

                        if (controlIsPressed)
                        {
                            // Situation B1 and B2 (see above) can be implemented together.
                            prometheusModel.unSelectBond(objectID);
                        }
                        else
                        {
                            if (currentAtomHighlights.size() + currentBondHighlights.size() > 1)
                            {
                                // Situation B3 (see above)
                                prometheusModel.clearSelection();
                                prometheusModel.setSelectedBond(objectID);
                            }
                            else
                            {
                                // Situation B4 (see above)
                                prometheusModel.clearSelection();
                            }
                        }
                    }
                }
            }
            else
            {

                /***************************************************************************************************************
                *In this section we are dealing with the selection properties of the viewer when the User drags out an area in
                *Selection mode.  Unless the control key is pressed, we must first clear out any previously selected atoms.
                *Then we grab anythng within Selection Buffer, regardless of its depth and place it within the Atlas Framework for highlighting.
                ***************************************************************************************************************/

                if (!controlIsPressed)
                {
                    prometheusModel.clearSelection();
                }

                for (int i = 0; i < hitCount; ++i)
                {

                    // The Selection Buffer holds four values for every entry.  The fourth one contains the Atom ID.
                    objectID = selectionBuffer[i * 4 + 3];

                    if (objectID <= prometheusModel.getNumberOfAtoms())
                    {
                        // We're dealing with atoms.
                        if (currentAtomHighlights.contains(objectID) == false)
                        {
                            prometheusModel.setSelectedAtom(objectID);
                        }
                    }
                    else
                    {
                        // We're dealing with bonds...  adjust the hit record index.

                        objectID = objectID - prometheusModel.getNumberOfAtoms() - 1;

                        if (currentBondHighlights.contains(objectID) == false)
                        {
                            prometheusModel.setSelectedBond(objectID);
                        }
                    }
                }
            }
        }
        else
        {
            // If there are no hits, make sure the arrays holding selected atoms and/or bonds is clear.
            prometheusModel.clearSelection();
        }
    }



    if (selectTool)
    {
        // If the User is using the Select Tool, we need to use updateGL to ask OpenGL to redraw
        // the model because we have updated the model inside the Atlas Framework.
        singleSelect = true;
        if (doubleClick)
        {
            if (hitCount > 0)
            {
                selectMolecule();
            }
            doubleClick = false;
        }

        // Update the list of selected atoms and bonds within the viewer class for rendering.
        SelectedAtoms = prometheusModel.getSelectedAtom();
        SelectedBonds = prometheusModel.getSelectedBond();

        update();
    }

    if (deleteTool)
    {
        // If the User is using the Delete Tool, we need to delete whatever object was just selected.  Then we ask
        // OpenGL to redraw the model we have just updated.
        prometheusModel.deleteHighlightedObjects();
        SelectedAtoms.clear();
        SelectedBonds.clear();
        deepRefresh();
    }

    if (atomTool)
    {
        // If the User is using the Atom Tool, then they wish to either place a new atom or change one they've selected.
        if (editAtomID == -1)
        {
            // The User has clicked empty space.  A new atom is to be placed here.
            prometheusModel.placeNewAtom(finalPos, this->width(), this->height(), xTrans, yTrans, zTrans, accumulatedRotation, builderAtom, atomBuilderMoved);
        }
        else
        {
            // The User has clicked an existing atom... change that atom to be one of the type the User desires.
            prometheusModel.editAtom(editAtomID, builderAtom, atomBuilderMoved);
        }
    }

    if (incrementBOTool)
    {
        // We'll take the selected bond and increase its bond order.
        QVector<int> currentBondHighlights = prometheusModel.getSelectedBond();

        if (!currentBondHighlights.isEmpty())
        {
            prometheusModel.incrementBond();
        }
        else
        {
            prometheusModel.clearSelection();
        }
    }

    if (decrementBOTool)
    {
        // We'll take the selected bond and decrease its bond order.
        QVector<int> currentBondHighlights = prometheusModel.getSelectedBond();

        if (!currentBondHighlights.isEmpty())
        {
            prometheusModel.decrementBond();
        }
        else
        {
            prometheusModel.clearSelection();
        }
    }

        // The model has been updated...  ask OpenGL to re-draw the scene.
        deepRefresh();

        if (atomTool & atomBuilderMoved & mousePressed)
        {
            // If the User is still holding down Their mouse to drag around a new atom, we want to show it on screen, but not permanently place the new atom there.
            // Since deepRefresh asks OpenGL to draw to the screen, the User as recieved Their placement feedback...  now we need to delete the atom we've just placed
            // from the framework.  When the User releases the mouse, mousePressed will become false, and this deletion fucntion will become inactive.

            prometheusModel.purgeLastRecord(editAtomID);
        }

}




void CPSviewer::selectMolecule()
{
    /*****************************************************************************************************************************
     * This function is meant to select all atoms and bonds within a single molecule selected atoms and/or bonds. The User is meant
     * to double click on any part of a molecule, and the entire molecule should become selected.  This will be accomplished via
     * a "fragmentation anaylsis", but it should be faster since we have the bond tables to look through.
    ******************************************************************************************************************************/

    // First we need to get an atom to start our fragment.  The User may have made our life easier and selected an atom, but they could
    // have also selected a bond.

    int seedAtom, seedBond;
    QVector<int> currentAtomHighlights = prometheusModel.getSelectedAtom();
    QVector<int> currentBondHighlights = prometheusModel.getSelectedBond();
    QVector<QVector<int> > bondList = prometheusModel.getBondList();

    if (currentBondHighlights.isEmpty())
    {
        // The User has selected an atom and our task is easier.
        seedAtom = currentAtomHighlights[0];
    }
    else
    {
        // The User has selected a bond, so we'll pick the first atom listed and work from there.
        seedAtom = bondList[currentBondHighlights[0]][0];
        seedBond = currentBondHighlights[0];
    }

    prometheusModel.clearSelection();
    // Because we clear these arrays, re-establish our connection to them.
    currentAtomHighlights = prometheusModel.getSelectedAtom();
    currentBondHighlights = prometheusModel.getSelectedBond();

    // Now, using the current atom, we perform recursive search for this atoms neighbors and their neighbors.
    QVector<int> oldList, newList, promoteList;
    oldList.clear();
    newList.clear();

    // oldList will hold the total list of atoms we are sure belong in the same molecule.  newList will hold any atoms that have been found
    // as part of our fragment, but we haven't checked them for neighbors.  We'll add the seedAtom to start our fragment and use a While Loop
    // to perform the recursive search.

    newList.push_back(seedAtom);

    while (!newList.isEmpty())
    {
        // promoteList stores atoms in the newList that will become part of the oldList once we've searched through all bonds.
        promoteList = newList;

        for (int i = 0; i < bondList.size(); i++)
        {
            for (int j = 0; j < promoteList.size(); j++)
            {
                // Cycle through the bondList and search for bonds that contain atoms in the newList.
                if (bondList[i][0] == promoteList[j])
                {
                    // The first atom in this bond is one of the new atoms we're looking for.  If the other atom in this bond hasn't been
                    // identified as part of the molecule, then add it to newList.

                    if (!oldList.contains(bondList[i][1]))
                    {
                        newList.push_back(bondList[i][1]);
                    }
                }

                if (bondList[i][1] == promoteList[j])
                {
                    // The second atom in this bond is one of the new atoms we're looking for.  If the other atom in this bond hasn't been
                    // identified as part of the molecule, then add it to newList.

                    if (!oldList.contains(bondList[i][0]))
                    {
                        newList.push_back(bondList[i][0]);
                    }
                }
            }
        }

        // Now that we've conducted a search through all bonds, remove the atoms that were originally in newList and place them in oldList.
        // If any atoms were added to newList, we'll have to repeat this proccess.
        for (int i = 0; i < promoteList.size(); i++)
        {
            for (int j = 0; j < newList.size(); j++)
            {
                if (promoteList[i] == newList[j])
                {
                    newList.remove(j);
                    oldList.push_back(promoteList[i]);
                }
            }
        }
    }

    // Now the recursive search is over...  all atoms on this molecule are contained within the list oldList.
    // Select them all.
    for (int i = 0; i < oldList.size(); i++)
    {
        prometheusModel.setSelectedAtom(oldList[i]);
    }

    // Now, we go through bondList and grab any bond that mentions atoms on oldList.
    for (int i = 0; i < oldList.size(); i++)
    {
        for (int j = 0; j < bondList.size(); j++)
        {
            if (bondList[j].contains(oldList[i]))
            {
                if(!currentBondHighlights.contains(j))
                {
                    prometheusModel.setSelectedBond(j);
                }
            }
        }
    }


}


// Various Characters stored as chunks one byte wide.
void CPSviewer::CharacterLookup()
{
    GLubyte chunk1[20];
    GLubyte chunk2[20];

    // Capital Letters...
/*
    // A
    chunk1 = {3, 3, 6, 6, 14, 12, 12, 12, 248, 248, 24, 48, 48, 32, 96, 96, 64, 192, 128, 128};
    chunk2 = {192, 192, 96, 96, 112, 48, 48, 48, 31, 31, 24, 12, 12, 4, 6, 6, 2, 3, 1, 1};

    // B
    chunk1 = {224, 240, 56, 28, 12, 12, 12, 28, 56, 112, 224, 192, 224, 96, 112, 48, 48, 48, 240, 224};
    chunk2 = {255, 255, 192, 192, 192, 192, 192, 192, 192, 192, 255, 255, 192, 192, 192, 192, 192, 192, 255, 255};

    // C
    chunk1 = {240, 248, 28, 14, 7, 3, 0, 0, 0, 0, 0, 0, 0, 0, 3, 7, 14, 28, 248, 240};
    chunk2 = {15, 31, 56, 112, 224, 192, 192, 192, 192, 192, 192, 192, 192, 192, 192, 224, 112, 56, 31, 15};

    // D
    chunk1 = {0, 128, 192, 224, 112, 56, 24, 28, 28, 28, 28, 28, 28, 24, 56, 112, 224, 192, 128, 0};
    chunk2 = {255, 255, 193, 192, 192, 192, 192, 192, 192, 192, 192, 192, 192, 192, 192, 192, 192, 193, 255, 255};

    // E
    chunk1 = {240, 240, 0, 0, 0, 0, 0, 0, 0, 0, 192, 192, 0, 0, 0, 0, 0, 0, 240, 240};
    chunk2 = {255, 255, 192, 192, 192, 192, 192, 192, 192, 192, 255, 255, 192, 192, 192, 192, 192, 192, 255, 255};

    // F
    chunk1 = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 192, 192, 0, 0, 0, 0, 0, 0, 240, 240};
    chunk2 = {192, 192, 192, 192, 192, 192, 192, 192, 192, 192, 255, 255, 192, 192, 192, 192, 192, 192, 255, 255};

    // G
    chunk1 = {243, 251, 31, 15, 3, 3, 3, 3, 127, 127, 0, 0, 0, 0, 3, 7, 14, 28, 248, 240};
    chunk2 = {15, 31, 56, 112, 224, 192, 192, 192, 192, 192, 192, 192, 192, 192, 192, 224, 112, 56, 31, 15};

    // H
    chunk1 = {3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 255, 255, 3, 3, 3, 3, 3, 3, 3, 3};
    chunk2 = {192, 192, 192, 192, 192, 192, 192, 192, 192, 192, 255, 255, 192, 192, 192, 192, 192, 192, 192, 192};

    // I
    chunk1 = {255, 255, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 255, 255};

    // J
    chunk1 = {0, 128, 192, 192, 192, 192, 192, 192, 192, 192, 192, 192, 192, 192, 192, 192, 192, 192, 192, 192};
    chunk2 = {63, 127, 225, 192, 192, 192, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    // K
    chunk1 = {14, 14, 12, 28, 56, 48, 48, 112, 224, 192, 192, 192, 128, 128, 192, 224, 112, 56, 28, 12};
    chunk2 = {192, 192, 192, 192, 192, 192, 192, 192, 240, 248, 221, 207, 199, 195, 193, 192, 192, 192, 192, 192};

    // L
    chunk1 = {252, 252, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    chunk2 = {255, 255, 192, 192, 192, 192, 192, 192, 192, 192, 192, 192, 192, 192, 192, 192, 192, 192, 192, 192};

    // M
    chunk1 = {3, 3, 3, 3, 3, 3, 3, 3, 3, 131, 227, 227, 115, 115, 59, 59, 31, 15, 15, 7};
    chunk2 = {192, 192, 192, 192, 192, 192, 192, 192, 192, 193, 199, 199, 206, 206, 220, 220, 248, 240, 240, 224};

    // N
    chunk1 = {15, 15, 31, 31, 59, 51, 115, 227, 227, 195, 131, 131, 3, 3, 3, 3, 3, 3, 3, 3};
    chunk2 = {192, 192, 192, 192, 192, 192, 192, 192, 192, 193, 195, 195, 199, 198, 206, 220, 248, 248, 240, 224};

    // O
    chunk1 = {224, 248, 60, 14, 6, 7, 3, 3, 3, 3, 3, 3, 3, 3, 7, 6, 14, 60, 248, 224};
    chunk2 = {7, 31, 60, 112, 96, 224, 192, 192, 192, 192, 192, 192, 192, 192, 224, 96, 112, 60, 31, 7};

    // P
    chunk1 = {0, 0, 0, 0, 0, 0, 0, 0, 224, 240, 56, 24, 28, 12, 12, 12, 28, 56, 240, 224};
    chunk2 = {192, 192, 192, 192, 192, 192, 192, 192, 255, 255, 192, 192, 192, 192, 192, 192, 192, 192, 255, 255};

    // Q
    chunk1 = {231, 254, 28, 62, 118, 231, 195, 131, 3, 3, 3, 3, 3, 3, 7, 6, 14, 60, 248, 224};
    chunk2 = {7, 31, 60, 112, 96, 224, 193, 192, 192, 192, 192, 192, 192, 192, 224, 96, 112, 60, 31, 7};

    // R
    chunk1 = {14, 14, 28, 28, 24, 56, 48, 112, 224, 224, 112, 56, 28, 12, 12, 28, 56, 112, 224, 192};
    chunk2 = {192, 192, 192, 192, 192, 192, 192, 192, 255, 255, 192, 192, 192, 192, 192, 192, 192, 192, 255, 255};

    // S
    chunk1 = {240, 248, 28, 12, 12, 12, 12, 60, 248, 224, 128, 0, 0, 0, 0, 0, 12, 28, 248, 240};
    chunk2 = {63, 127, 224, 192, 192, 128, 0, 0, 0, 3, 7, 31, 60, 112, 224, 192, 192, 224, 127, 63};

    // T
    chunk1 = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 252, 252};
    chunk2 = {3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 255, 255};

    // U
    chunk1 = {252, 254, 255, 15, 7, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3};
    chunk2 = {63, 127, 255, 240, 224, 192, 192, 192, 192, 192, 192, 192, 192, 192, 192, 192, 192, 192, 192, 192};

    // V
    chunk1 = {128, 192, 224, 112, 112, 48, 56, 56, 56, 24, 28, 28, 28, 12, 14, 14, 6, 7, 7, 3};
    chunk2 = {1, 3, 7, 14, 14, 12, 28, 28, 28, 24, 56, 56, 56, 48, 112, 112, 96, 224, 224, 192};

    // W
    chunk1 = {7, 15, 31, 27, 59, 51, 115, 99, 227, 195, 131, 3, 3, 3, 3, 3, 3, 3, 3, 3};
    chunk2 = {224, 240, 248, 216, 220, 204, 206, 198, 199, 195, 193, 192, 192, 192, 192, 192, 192, 192, 192, 192};

    // X
    chunk1 = {3, 7, 6, 14, 12, 28, 56, 112, 224, 192, 192, 224, 112, 56, 24, 28, 12, 14, 7, 3};
    chunk2 = {192, 224, 96, 112, 48, 56, 28, 14, 7, 3, 3, 7, 14, 28, 24, 56, 48, 112, 224, 192};

    // Y
    chunk1 = {0, 0, 0, 0, 0, 0, 0, 0, 0, 128, 192, 224, 112, 48, 56, 24, 28, 12, 12, 12};
    chunk2 = {3, 3, 3, 3, 3, 3, 3, 3, 3, 7, 15, 28, 56, 48, 112, 96, 224, 192, 192, 192};

    // Z
    chunk1 = {255, 255, 0, 0, 0, 0, 0, 0, 0, 128, 192, 224, 112, 56, 28, 14, 7, 3, 255, 255};
    chunk2 = {255, 255, 192, 224, 112, 56, 28, 14, 7, 3, 1, 0, 0, 0, 0, 0, 0, 0, 255, 255};


    // 0
    chunk1 = {224, 240, 56, 28, 12, 12, 12, 12, 12, 140, 140, 140, 204, 76, 108, 44, 60, 56, 240, 224};
    chunk2 = {7, 15, 28, 60, 54, 50, 51, 49, 49, 49, 48, 48, 48, 48, 48, 48, 56, 28, 15, 7};

    // 1
    chunk1 = {248, 240, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128};
    chunk2 = {31, 15, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 15, 7, 3, 1};

    // 2
    chunk1 = {248, 248, 0, 0, 0, 0, 128, 192, 224, 112, 56, 24, 24, 24, 24, 24, 24, 56, 240, 224};
    chunk2 = {31, 31, 24, 28, 14, 7, 3, 1, 0, 0, 0, 0, 0, 0, 24, 24, 24, 28, 15, 7};

    // 3
    chunk1 = {224, 240, 56, 24, 24, 24, 24, 24, 24, 56, 240, 240, 56, 24, 24, 24, 24, 56, 240, 224};
    chunk2 = {7, 15, 28, 24, 24, 24, 0, 0, 0, 0, 1, 1, 0, 0, 24, 24, 24, 28, 15, 7};

    // 4
    chunk1 = {96, 96, 96, 96, 96, 96, 248, 248, 96, 96, 96, 96, 96, 96, 96, 96, 96, 224, 224, 224};
    chunk2 = {0, 0, 0, 0, 0, 0, 31, 31, 16, 24, 24, 12, 12, 6, 6, 3, 3, 1, 1, 0};

    */
}
