#include "cpsviewer.h"
#include <QKeyEvent>


#include <OpenGL/glu.h>
#include <QtGui>
#include <QtOpenGL>
#include <QGLBuffer>
#include <qmath.h>



// Constructor
CPSviewer::CPSviewer(QWidget *parent) :
    QGLWidget(parent)
{
    // Set the widget to Double Buffer mode.  This will be needed for animation and efficient realtime rendering.
    // This is technically not a OpenGL command because it depends on the windowing context (QGLWidget), but it has an important effect on all the other OpenGL commands.
    setFormat(QGLFormat(QGL::DoubleBuffer | QGL::DepthBuffer | QGL::Rgba | QGL::SampleBuffers));

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





    // Create Vertex Buffer Objects.

    float positionData[] = {
                -0.8f, -0.8f, 0.0f,
                 0.8f, -0.8f, 0.0f,
                 0.0f,  0.8f, 0.0f };

    float colorData[] = {
          1.0f, 0.0f, 0.0f,
          0.0f, 1.0f, 0.0f,
          0.0f, 0.0f, 1.0f };


    QGLBuffer positionBuffer(QGLBuffer::VertexBuffer);
    QGLBuffer colorBuffer(QGLBuffer::VertexBuffer);

    positionBuffer.create();
    colorBuffer.create();

    positionBuffer.bind();
    colorBuffer.bind();

    positionBuffer.setUsagePattern(QGLBuffer::DynamicDraw);
    colorBuffer.setUsagePattern(QGLBuffer::DynamicDraw);

    positionBuffer.allocate(&positionData, 9*sizeof(float));
    colorBuffer.allocate(&colorData, 9*sizeof(float));

    // Create and set-up a vertex array object, which stores connections between the data in these buffers and the shader inputs.



}

// This is called when the OpenGL window is resized.  It handles redrawing the window,
// so we can resize the window without worry.
void CPSviewer::resizeGL(int width, int height)
{
    // Prevent divide by zero (in the gluPerspective call)
    if (height == 0)
        height = 1;

    glViewport(0, 0, width, height); // Reset current viewport

//    int side = qMin(width, height);
//    glViewport((width - side) / 2, (height - side) / 2, side, side);  // reset current viewport.  It is square.

    glMatrixMode(GL_PROJECTION); // Select projection matrix
    glLoadIdentity(); // Reset projection matrix

    gluPerspective(45.0f, static_cast<GLfloat>(width)/height, 0.1f, 100.0f); // Calculate aspect ratio

    /*
      TO DO - WISH LIST:
      We need to be able to switch between perspective and orthographic projections at will.
      Figure out how to make orthographic possible.

    GLfloat aspectRatio = width / height;
    if (width <= height)
    {
        glOrtho (-100.0, 100.0, -100 / aspectRatio, 100.0 / aspectRatio, 1.0, -1.0);
    }
    else
    {
        glOrtho (-100.0 * aspectRatio, 100.0 * aspectRatio, -100.0, 100.0, 1.0, -1.0);
    }
    */

    glMatrixMode(GL_MODELVIEW); // Select modelview matrix
    glLoadIdentity(); // Reset modelview matrix



}

// OpenGL painting code goes here.
void CPSviewer::paintGL()
{
    int representationMethod;
    float angle;

    // First off, we have to decide how the model will be displayed.  This integer will inform this function's drawing method.
    // 1 = Ball-and-Stick, 2 = Dreiding (Licorice), 3 = Space Filling (CPK), 4 = Vector (Wireframe), 5 = Vector with Points
    representationMethod = prometheusModel.getDrawingStyle();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear screen and depth buffer
    glLoadIdentity(); // Reset current modelview matrix

    // This line handles where you've dragged / zoomed the model.
      // xTrans and yTrans are assigned from dragging functions.
      // zTrans is assigned from the Zoom function.
    glTranslatef(xTrans, yTrans, zTrans);

    // Rotation...  We ask OpenGL to rotate the model using the function glRotatef.  The paramters are angle of rotation in degrees, and the x-y-z components of a vector describing
    // the axis of rotation.  These values are stored within the quaternion "accumulatedRotation".  See the function "setRotation()" below for its calculation.
    // Quaternions hold cos(angle / 2).  In order to retrieve the angle in degrees, we multiply the conversion factor by 2.
    angle = qAcos(accumulatedRotation.scalar()) * 360 / 3.14159;
    glRotatef(angle, accumulatedRotation.x(), accumulatedRotation.y(), accumulatedRotation.z());

    // Now that the view has been established, the remaining code in this function is dedicated to designating what is actually being displayed



    // From here on, we need call OpenGL commands based on the representation method we're going after.
    // 1 = Ball-and-Stick, 2 = Licorice (Dreiding), 3 = Space Filling (CPK), 4 = Vector (Wireframe), 5 = Vector with Points
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



//********************************************************************************************
// Functions in this section deal with drawing routines for the various model representations.
//*********************************************************************************************


void CPSviewer::drawBallandStickRepresentation()
{
    // Ball-and-Stick Model... A favorite staple of chemistry. We're going to render a lot of spheres and cylinders, so we're going to rely on the handy routines provided by the
    // Graphics Library Utility (glu) to efficiently draw the model.

    ///////////////////////////////////////////////////////////////////////////
    //              SETUP
    ///////////////////////////////////////////////////////////////////////////

    QVector<float> color;

    // First thing's first...   Retrieve the coordinates from the Atlas framework.
    QVector<float> posX = prometheusModel.getAtomPositionX();
    QVector<float> posY = prometheusModel.getAtomPositionY();
    QVector<float> posZ = prometheusModel.getAtomPositionZ();

    // Retrieve the bond data from the Atlas framework. Stored as a vector of pairs of atom ID integers.
    QVector<QVector<int> > bondList = prometheusModel.getBondList();

    // Also retrieve bond lengths, and vectors normal to the bond and z-axis
    QVector<QVector<float> > rotationList = prometheusModel.getBondNormalList();
    QVector<float> length = prometheusModel.getBondLengthList();

    // Retrieve specified atomic radii from the Atlas framework.
    QVector<float> radii = prometheusModel.getAtomRadius();

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

    /*

    // We need to set up some lighting calls to OpenGL.  Also, tell OpenGL to shade the model by considering the material's color
    // and the incident light through color tracking.
    glEnable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL);

    // Specify light sources.
    GLfloat ambientLight[] = {0.3, 0.3, 0.3, 1.0};  // Medium ambient light (white in color).
    GLfloat diffuseLight[] = {0.7, 0.7, 0.7, 1.0};  // Farily strong directed light (white in color).
    GLfloat specularLight[] = {1.0, 1.0, 1.0, 1.0};  // Strong, specular light source (white in color).
    glLightfv(GL_LIGHT0,GL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT0,GL_DIFFUSE, diffuseLight);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);

    // Describe the material illuminated.
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    GLfloat specularReflectance[] = {1.0, 1.0, 1.0, 1.0};  // High specular reflectance.
    glMaterialfv(GL_FRONT, GL_SPECULAR, specularReflectance);
    glMateriali(GL_FRONT,GL_SHININESS,128);  // Focuses the specular effects to a small area.

    // Enable the light source.
    glEnable(GL_LIGHT0);

    */

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
    // Cycle through the atoms in the Atlas Framework.
    for (int i = 0; i < posX.size(); ++i)
    {
        // Retrieve the RGB color assigned to this atom.
        color = prometheusModel.getAtomColor(i + 1);

        // Set the drawing to this color.
        glColor3f(color[0], color[1], color[2]);

        // The glu routine renders spheres at the origin as a default, so we need to ensure we center it on the atom's coordinates.
        glTranslatef(posX[i], posY[i], posZ[i]);

        // Call the glu routine to render the sphere.
        // The parameters are: object_name, radius, # of slices, # of stacks
        gluSphere(sphere, radii[i]*0.25, 24, 12);

        // Move back to the origin, so the next call will place the atom at the correct location.
        glTranslatef(-posX[i], -posY[i], -posZ[i]);
    }


    // We can move on to drawing the bonds.
    // Cycle through the bonds listed in the Atlas Framework.
    for (int i = 0; i < bondList.size(); ++i)
    {
        glPushMatrix();
            // We specify that the bonds are all composed of a neutral color.  Here "Wheat" is specified.
            glColor3f(0.96078, 0.870588, 0.7019607);

            // Cylinders are drawn at the origin and radiating toward the positive z-axis by default.  Before drawing the bond, we have to move to the correct
            // location before proceeding.
            glTranslatef(posX[bondList[i][0] - 1], posY[bondList[i][0] - 1], posZ[bondList[i][0] - 1]);

            // We now rotate our orentation to align the z-axis with the bond vector.
            glRotatef(rotationList[i][2], rotationList[i][0], rotationList[i][1], 0);

            // Now we draw the cylinder representing the bond by calling the glu routine.
            // The parameters are: object_name, radius at bottom, radius at top, length of cylinder, # of slices, # of stacks
            gluCylinder(cylinder, 0.1, 0.1, length[i], 10, 5);

        // Move back to the origin and starting orientation, so the next call will function properly.
        glPopMatrix();
    }


}

void CPSviewer::drawDreidingRepresentation()
{
    // Dreiding Model... Prefered method among the biological sciences. We're going to render a lot of spheres and cylinders, so we're going to rely on the handy routines provided by the
    // Graphics Library Utility (glu) to efficiently draw the model.

    ///////////////////////////////////////////////////////////////////////////
    //              SETUP
    ///////////////////////////////////////////////////////////////////////////

    QVector<float> colorA, colorB;

    // First thing's first...   Retrieve the coordinates from the Atlas framework.
    QVector<float> posX = prometheusModel.getAtomPositionX();
    QVector<float> posY = prometheusModel.getAtomPositionY();
    QVector<float> posZ = prometheusModel.getAtomPositionZ();

    // Retrieve the bond data from the Atlas framework. Stored as a vector of pairs of atom ID integers.
    QVector<QVector<int> > bondList = prometheusModel.getBondList();

    // Also retrieve bond lengths, and vectors normal to the bond and z-axis
    QVector<QVector<float> > rotationList = prometheusModel.getBondNormalList();
    QVector<float> length = prometheusModel.getBondLengthList();

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

    /*
    // We need to set up some lighting calls to OpenGL.  Also, tell OpenGL to shade the model by considering the material's color
    // and the incident light through color tracking.
    glEnable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL);

    // Specify light sources.
    GLfloat ambientLight[] = {0.3, 0.3, 0.3, 1.0};  // Medium ambient light (white in color).
    GLfloat diffuseLight[] = {0.7, 0.7, 0.7, 1.0};  // Farily strong directed light (white in color).
    GLfloat specularLight[] = {1.0, 1.0, 1.0, 1.0};  // Strong, specular light source (white in color).
    glLightfv(GL_LIGHT0,GL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT0,GL_DIFFUSE, diffuseLight);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);


    // Describe the material illuminated.
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    GLfloat specularReflectance[] = {1.0, 1.0, 1.0, 1.0};  // High specular reflectance.
    glMaterialfv(GL_FRONT, GL_SPECULAR, specularReflectance);
    glMateriali(GL_FRONT,GL_SHININESS,128);  // Focuses the specular effects to a small area.

    // Enable the light source.
    glEnable(GL_LIGHT0);

    */

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
    // Cycle through the atoms in the Atlas Framework.
    for (int i = 0; i < posX.size(); ++i)
    {
        // Save current location so we can undo our transformations.
        glPushMatrix();

            // Retrieve the RGB color assigned to this atom.
            colorA = prometheusModel.getAtomColor(i + 1);

            // Set the drawing to this color.
            glColor3f(colorA[0], colorA[1], colorA[2]);

            // The glu routine renders spheres at the origin as a default, so we need to ensure we center it on the atom's coordinates.
            glTranslatef(posX[i], posY[i], posZ[i]);

            // Call the glu routine to render the sphere.
            // The parameters are: object_name, radius, # of slices, # of stacks
            gluSphere(sphere, 0.2, 24, 12);

        // Move back to the origin, so the next call will place the atom at the correct location.
        glPopMatrix();
    }


    // We can move on to drawing the bonds.
    // Cycle through the bonds listed in the Atlas Framework.
    for (int i = 0; i < bondList.size(); ++i)
    {
        glPushMatrix();
            // We specify that the bonds are colored to the midpoint according to the atom being represented.
            // Retrieve the RGB color assigned to each atom pair.
            colorA = prometheusModel.getAtomColor(bondList[i][0]);
            colorB = prometheusModel.getAtomColor(bondList[i][1]);

            // Set the color to the first atom.
            glColor3f(colorA[0], colorA[1], colorA[2]);

            // Cylinders are drawn at the origin and radiating toward the positive z-axis by default.  Before drawing the bond, we have to move to the correct
            // location before proceeding.
            glTranslatef(posX[bondList[i][0] - 1], posY[bondList[i][0] - 1], posZ[bondList[i][0] - 1]);

            // We now rotate our orentation to align the z-axis with the bond vector.
            glRotatef(rotationList[i][2], rotationList[i][0], rotationList[i][1], 0);

            // Now we draw the cylinder representing the bond by calling the glu routine.
            // The parameters are: object_name, radius at bottom, radius at top, length of cylinder, # of slices, # of stacks
            gluCylinder(cylinder, 0.2, 0.2, length[i] / 2, 24, 5);

        // Move back to the origin and starting orientation, so the next call will function properly.
        glPopMatrix();

        // Now draw the second half of the bond.
        glPushMatrix();

            // Set the color to the first atom.
            glColor3f(colorB[0], colorB[1], colorB[2]);

            // Cylinders are drawn at the origin and radiating toward the positive z-axis by default.  Before drawing the bond, we have to move to the correct
            // location before proceeding.
            glTranslatef(posX[bondList[i][1] - 1], posY[bondList[i][1] - 1], posZ[bondList[i][1] - 1]);

            // We now rotate our orentation to align the z-axis with the bond vector.
            glRotatef(rotationList[i][2] + 180, rotationList[i][0], rotationList[i][1], 0);

            // Now we draw the cylinder representing the bond by calling the glu routine.
            // The parameters are: object_name, radius at bottom, radius at top, length of cylinder, # of slices, # of stacks
            gluCylinder(cylinder, 0.2, 0.2, length[i] / 2, 24, 5);

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

    QVector<float> color;

    // First thing's first...   Retrieve the coordinates from the Atlas framework.
    QVector<float> posX = prometheusModel.getAtomPositionX();
    QVector<float> posY = prometheusModel.getAtomPositionY();
    QVector<float> posZ = prometheusModel.getAtomPositionZ();

    // Also retrieve specified atomic radii from the Atlas framework.
    QVector<float> radii = prometheusModel.getAtomRadius();

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

    /*
    // We need to set up some lighting calls to OpenGL.  Also, tell OpenGL to shade the model by considering the material's color
    // and the incident light through color tracking.
    glEnable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL);

    // Specify light sources.
    GLfloat ambientLight[] = {0.3, 0.3, 0.3, 1.0};  // Low ambient light (white in color).
    GLfloat diffuseLight[] = {0.7, 0.7, 0.7, 1.0};  // Farily strong directed light (white in color).
    GLfloat specularLight[] = {1.0, 1.0, 1.0, 1.0};  // Strong, specular light source (white in color).
    glLightfv(GL_LIGHT0,GL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT0,GL_DIFFUSE, diffuseLight);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);

    // Describe the material illuminated.
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    GLfloat specularReflectance[] = {1.0, 1.0, 1.0, 1.0};  // High specular reflectance.
    glMaterialfv(GL_FRONT, GL_SPECULAR, specularReflectance);
    glMateriali(GL_FRONT,GL_SHININESS,128);  // Focuses the specular effects to a small area.

    // Enable the light source.
    glEnable(GL_LIGHT0);

    */

    GLUquadricObj *sphere; // Quadric Object Pointer
    sphere = gluNewQuadric();  // initialize object for the glu routine

    // Tell OpenGL about the normals we'd like to use for vertex shading.  We'd like OpenGL to smoothly interpolate the colors between vertices.
    // This will help hide the polygon faces that approximate the spheres.  That means we can use bigger polygons to improve performace without sacrificing quality.
    gluQuadricNormals(sphere, GLU_SMOOTH);




    //////////////////////////////////////////////////////////////////////////
    //                Drawing
    //////////////////////////////////////////////////////////////////////////

    // Begin drawing routine.  Cycle through the atoms in the Atlas Framework.
    for (int i = 0; i < posX.size(); ++i)
    {
        // Save current location so we can undo our transformations.
        glPushMatrix();

            // Retrieve the RGB color assigned to this atom.
            color = prometheusModel.getAtomColor(i + 1);

            // Set the drawing to this color.
            glColor3f(color[0], color[1], color[2]);

            // The glu routine renders spheres at the origin as a default, so we need to ensure we center it on the atom's coordinates.
            glTranslatef(posX[i], posY[i], posZ[i]);

            // Call the glu routine to render the sphere.
            // The parameters are: object_name, radius, # of slices, # of stacks
            gluSphere(sphere, radii[i], 48, 24);

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

    QVector<float> colorA, colorB;

    // First thing's first...   Retrieve the coordinates from the Atlas framework.
    QVector<float> posX = prometheusModel.getAtomPositionX();
    QVector<float> posY = prometheusModel.getAtomPositionY();
    QVector<float> posZ = prometheusModel.getAtomPositionZ();

    // Retrieve the bond data from the Atlas framework. Stored as a vector of pairs of atom ID integers.
    QVector<QVector<int> > bondList = prometheusModel.getBondList();

    // Also retrieve the position of the center of the bond.
    QVector<QVector<float> > bondCenter = prometheusModel.getBondCenterList();

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

    /*
    // Turn off any lights if they've been activated for other methods
    if (glIsEnabled(GL_LIGHTING))
    {
        glDisable(GL_LIGHTING);
    }

    if (glIsEnabled(GL_COLOR_MATERIAL))
    {
        glDisable(GL_COLOR_MATERIAL);
    }
    */

    //////////////////////////////////////////////////////////////////////////
    //                Drawing
    //////////////////////////////////////////////////////////////////////////

    // Loop over the vector that holds the paired atoms and draw lines between them.
    // bondData holds the atom ID numbers, and their positions are contained within posX, posY, and posZ.

    glBegin(GL_LINES); // Enter Drawing Routine.
    for (int i = 0; i < bondList.size(); ++i)
    {
        // Before we give the endpoints of the line we will draw, we must give the color of that part of line.
        colorA = prometheusModel.getAtomColor(bondList[i][0]);  // First Atom's Color.
        colorB = prometheusModel.getAtomColor(bondList[i][1]);  // Second Atom's Color.

        // It is entered compactly below, and without holder variables, but the ends of the line are given as the
        // positions of the atoms making the bonded pair.  [posX, posY, posZ].  The integer given as an index for
        // posX (or posY or posZ) is the integer atom ID stored within bondList.

        glColor3f(colorA[0], colorA[1], colorA[2]); // First atom's color is set.
        glVertex3f(posX[bondList[i][0] - 1],   posY[bondList[i][0] - 1],   posZ[bondList[i][0] - 1]); // First Atom's Position
        glVertex3f(bondCenter[i][0], bondCenter[i][1], bondCenter[i][2]); // Bond Center

        glColor3f(colorB[0], colorB[1], colorB[2]); // Second atom's color is set.
        glVertex3f(bondCenter[i][0], bondCenter[i][1], bondCenter[i][2]); // Bond Center
        glVertex3f(posX[bondList[i][1] - 1],   posY[bondList[i][1] - 1],   posZ[bondList[i][1] - 1]); // Second Atom's Position
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

    QVector<float> color, colorA, colorB;

    // First thing's first...   Retrieve the coordinates from the Atlas framework.
    QVector<float> posX = prometheusModel.getAtomPositionX();
    QVector<float> posY = prometheusModel.getAtomPositionY();
    QVector<float> posZ = prometheusModel.getAtomPositionZ();

    glPointSize(5.0);   // Sets the dot size of the atom

    // Retrieve the bond data from the Atlas framework. Stored as a vector of pairs of atom ID integers.
    QVector<QVector<int> > bondList = prometheusModel.getBondList();

    // Also retrieve the position of the center of the bond.
    QVector<QVector<float> > bondCenter = prometheusModel.getBondCenterList();


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

/*
    // Turn off any lights if they've been activated for other methods
    if (glIsEnabled(GL_LIGHTING))
    {
        glDisable(GL_LIGHTING);
    }

    if (glIsEnabled(GL_COLOR_MATERIAL))
    {
        glDisable(GL_COLOR_MATERIAL);
    }
*/

    //////////////////////////////////////////////////////////////////////////
    //                Drawing
    //////////////////////////////////////////////////////////////////////////

    // Begin drawing routine.  Cycle through the atoms in the Atlas Framework. One point will be placed at every vertex listed between here and "glEnd"
    glBegin(GL_POINTS);
        for (int i = 0; i < posX.size(); ++i)
        {
            // Retrieve desired color for this atom.
            color = prometheusModel.getAtomColor(i + 1);

            // Set the color for drawing.
            glColor3f(color[0], color[1], color[2]);

            // Draw vertex.
            glVertex3f(posX[i], posY[i], posZ[i]);
        }
    glEnd();  // End drawing atoms.


    // Now, we move on to bond vectors. Loop over the vector that holds the paired atoms and draw lines between them.
    // bondData holds the atom ID numbers, and their positions are contained within posX, posY, and posZ.

    glBegin(GL_LINES); // Enter Drawing Routine.
    for (int i = 0; i < bondList.size(); ++i)
    {
        // Before we give the endpoints of the line we will draw, we must give the color of that part of line.
        colorA = prometheusModel.getAtomColor(bondList[i][0]);  // First Atom's Color.
        colorB = prometheusModel.getAtomColor(bondList[i][1]);  // Second Atom's Color.

        // It is entered compactly below, and without holder variables, but the ends of the line are given as the
        // positions of the atoms making the bonded pair.  [posX, posY, posZ].  The integer given as an index for
        // posX (or posY or posZ) is the integer atom ID stored within bondList.

        glColor3f(colorA[0], colorA[1], colorA[2]); // First atom's color is set.
        glVertex3f(posX[bondList[i][0] - 1],   posY[bondList[i][0] - 1],   posZ[bondList[i][0] - 1]); // First Atom's Position
        glVertex3f(bondCenter[i][0], bondCenter[i][1], bondCenter[i][2]); // Bond Center

        glColor3f(colorB[0], colorB[1], colorB[2]); // Second atom's color is set.
        glVertex3f(bondCenter[i][0], bondCenter[i][1], bondCenter[i][2]); // Bond Center
        glVertex3f(posX[bondList[i][1] - 1],   posY[bondList[i][1] - 1],   posZ[bondList[i][1] - 1]); // Second Atom's Position
    }

    glEnd();  // End Drawing Routine.
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
    // quaternions, a similar concept to vectors also created in the late 19th century.

    // Quaternions are composed of 4 elements:  A vector with an (1) x, (2) y, and (3) z component  and (4) a scalar component.
    // The vector is the axis of rotation, and the scalar component stores the angle of rotation, so quaternions are uniquely suited for describing rotation.

    // What makes them even better is that the product of quaternions describes cumulative rotation.
    // Example:  q1 describes a rotation from A to B.
    //           q2 describes a rotation from B to C.
    //    therefore, q3 = q1 * q2  -> describes a rotation from A to C.


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
    updateGL();

}


void CPSviewer::setXTranslation(float distance)
{
    // Test to see if the user is asking for another angle.  If so,
    // emit a signal to change the camera's orientation. updateGL is a built-in
    // GLWidget function that will update the drawing to the screen.
    if (distance != xTrans)
    {
        xTrans = distance;
        updateGL();
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
        updateGL();
    }
}






// This section handles the mouse interactions over the viewer's orientation.


// This function simply records the position of the mouse when it is initially clicked.
void CPSviewer::mousePressEvent(QMouseEvent *event)
{
    lastPos = event->pos();
}

// This function determines how much the scene should be rotated by looking at how much the mouse was
// dragged after being clicked.
void CPSviewer::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - lastPos.x();
    int dy = event->y() - lastPos.y();

    if (event->buttons() & Qt::LeftButton)
    {
        setRotation(event->x(), event->y(), lastPos.x(), lastPos.y());
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

// This function determines the scene zoom based on movements
// of the mouse wheel.

void CPSviewer::setZoom(float distance)
{
        zTrans += distance;
        updateGL();
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


//// Key handler
void CPSviewer::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
        case Qt::Key_Escape:
            close(); // Quit on Escape
            break;
        case Qt::Key_F1:
            setWindowState(windowState() ^ Qt::WindowFullScreen); // Toggle fullscreen on F1
            break;
        case Qt::Key_B:
            setXTranslation(-1.0);
            break;
        default:
            QGLWidget::keyPressEvent(event); // Let base class handle the other keys
    }
}

//void CPSviewer::changeEvent(QEvent *event) {
//    switch (event->type()) {
//    case QEvent::WindowStateChange:
//        // Hide cursor if the window is fullscreen, otherwise show it
//        if (windowState() == Qt::WindowFullScreen)
//            setCursor(Qt::BlankCursor);
//        else
//            unsetCursor();
//        break;
//    default:
//        break;
//    }
//}

