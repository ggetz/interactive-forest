#include "Angel.h"  //includes gl.h, glut.h and other stuff...
#include "Light.h"
#include "Camera.h"
#include "Ground.h"
#include "Cube.h"

//----------------------------------------------------------------------------
//foward declarations of functions
void init();
void draw();
void resize(int width, int height);
void keyboard(unsigned char, int, int);
void keyBoardSpecial (int key, int xx, int yy);
void mouseClicked(GLint button, GLint state, GLint x, GLint y);
void update(int value);
void close();

void createTree(vec4 location);
void initShadowMapping();
void generateShadowMap();


//----------------------------------------------------------------------------
// GLOBALS
DirectionalLight sun;
vector<Camera> cameras;
Mesh *ground;
vector<Mesh*> meshes;
Camera currentCam;
bool isCam1_ON;

GLuint _depthBuffer;
GLuint _shadowMap;
GLuint _shadowMapSize = 1024;

// window size
int HEIGHT = 512;
int WIDTH = 512;


// flags and variables
int theta;

//----------------------------------------------------------------------------
int main( int argc, char **argv )
{
    glutInit( &argc, argv );	//initialize glut
#ifdef __APPLE__
    glutInitDisplayMode( GLUT_3_2_CORE_PROFILE|GLUT_RGBA | GLUT_SINGLE|GLUT_DEPTH);
#else
    glutInitDisplayMode( GLUT_RGBA | GLUT_SINGLE|GLUT_DEPTH);
#endif
    glutInitWindowSize( WIDTH, HEIGHT );	//set window size to be 512x512 pixels

    glutCreateWindow( "Interactive Forest" );

	//initialize glew if necessary (don't need to on Macs)
	#ifndef __APPLE__
	GLenum err = glewInit();
	#endif

    init();  //do some initialize for our program

    //set up the callback functions
    glutDisplayFunc( draw );  //REQUIRED.  What to do when it's time to draw
    glutKeyboardFunc( keyboard );  //What to do if a keyboard event is detected
    glutSpecialFunc(keyBoardSpecial);
    glutMouseFunc(mouseClicked); // mouse clicked
    glutTimerFunc(50, update, 50); // animation, for sun movement
    glutWMCloseFunc(close); // default close func, make sure to delete resources
    glutReshapeFunc(resize);  //use for recomputing projection matrix on reshape
    glutMainLoop();  //start infinite loop, listening for events
    return 0;
}


//Initialization
void init()
{
    cameras.push_back(Camera());
    cameras.push_back(Camera());
    
    sun = DirectionalLight();
    sun.shadow = (vec4(0.5, 0.5, 0.7, 1.0));
    
	//set up the cameras
    //vec4(4, 2, 1, 0)
	cameras[0].positionCamera(vec4(0, 0, 1, 0), vec4(0, 1, 0, 0), vec4(0, 0, -1, 0), vec4(1, 0, 0, 0));
    cameras[1].positionCamera(vec4(1, 0, 0, 0), vec4(0, 1, 0, 0), vec4(0, 0, -1, 0), vec4(1, 0, 0, 0));
    isCam1_ON = true;
    currentCam = cameras[0];
    
    Material m = Material();
    m.texturePath = "grass256by256.ppm";
    m.ambient = vec4(0.5, 0.5, 0.5, 1.0);
    m.diffuse = vec4(0.8, 0.8, 0.8, 1.0);
    
    ground = new Ground();
    ground->setMaterial(m);
    //ground->setPosition(vec4(0, 0.5, -7, 1));
    ground->init();
    
    m.texturePath = "crate_texture.ppm";
    Mesh *cube = new Cube();
    cube->setMaterial(m);
    cube->setPosition(vec4(-1, -2, 1, 1));
    meshes.push_back(cube);
    
    // initialize all meshes
    for (auto &mesh : meshes)
    {
        mesh->init();
    }
    
    initShadowMapping();
    
	
	glEnable(GL_DEPTH_TEST); //since we're doing 3D graphics, enable the Z-buffer depth test
	
	//define color to use when color buffer is cleared
    glClearColor( 1.0, 1.0, 1.0, 1.0 ); 
}

//----------------------------------------------------------------------------

void draw( void )
{
    // Shadow pass
    glCullFace(GL_FRONT);
    
    generateShadowMap();
    
    glCullFace(GL_NONE);
    
    // Render pass
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    ground->draw(currentCam, sun, _shadowMap);
    
    // draw all meshes
    for (auto &mesh : meshes)
    {
        mesh->draw(currentCam, sun, _shadowMap);
    }
		
    //glutSwapBuffers();
	glFlush();
}

//----------------------------------------------------------------------------

void initShadowMapping()
{
    glGenFramebuffers(1, &_depthBuffer);
    glGenTextures(1, &_shadowMap);
    
    glBindTexture(GL_TEXTURE_2D, _shadowMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, _shadowMapSize, _shadowMapSize, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
    
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
}

void resize(int w, int h){
	glViewport(0,0,(GLsizei) w, (GLsizei) h);
    currentCam.changeProjection(w, h);
	
}

void generateShadowMap()
{
    glBindFramebuffer(GL_FRAMEBUFFER, _depthBuffer);
    glViewport(0, 0, _shadowMapSize, _shadowMapSize);
    
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    
    glBindTexture(GL_TEXTURE_2D, _shadowMap);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _shadowMap, 0);
    glClear(GL_DEPTH_BUFFER_BIT);
    
    // draw all mesh shadow maps
    for (auto &mesh : meshes)
    {
        mesh->drawShadowMap(sun);
    }
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0,0, WIDTH, HEIGHT);
}


//----------------------------------------------------------------------------
void keyboard( unsigned char key, int x, int y )
{
    if (key == ' ')
    {
        if (isCam1_ON == true)
        {
            // switch to camera 1
            isCam1_ON = false;
            cout << "cam1" << endl;
            
        }
        else
        {
            // switch to camera 2
            isCam1_ON = true;
            cout << "cam2" << endl;
        }
    }
    else
    {
        if (isCam1_ON)
        {
            if (key == 'X')
            {
                // pitch down
                cameras[0].pitch(-10);
            }
            else if (key == 'x')
            {
                // pitch up
                cameras[0].pitch(10);
            }
            else if (key == 'C')
            {
                // yaw clockwise in un plane
                cameras[0].yaw(10);
            }
            else if (key == 'c')
            {
                // yaw counter-clockwise in un plane
                cameras[0].yaw(-10);
            }
            else if (key == 'Z')
            {
                // roll clockwise in the uv plane
                cameras[0].roll(10);
            }
            else if (key == 'z')
            {
                // roll counter-clockwise in the uv plane
                cameras[0].roll(-10);
            }
        }
    }

    
    if (key == 033 or key == 'q' or key == 'Q')
    {
        exit( EXIT_SUCCESS );
    }
    
    glutPostRedisplay();
}

void keyBoardSpecial (int key, int xx, int yy)
{
    switch (key) {
            
        case GLUT_KEY_UP :
            // forward
            cameras[0].moveCamera(-1);
            break;
            
        case GLUT_KEY_DOWN :
            // backward
            cameras[0].moveCamera(1);
            break;
    }
    
    glutPostRedisplay();
}

// mouse click callback function
void mouseClicked(GLint button, GLint state, GLint x, GLint y)
{
    // pFront
    float xCam = (2 * ((float)x / ((float)WIDTH))) - 1;
    float yCam = (2 * (((float)HEIGHT) - (float)y) / ((float)HEIGHT)) - 1;
    
    if (button == GLUT_LEFT_BUTTON)
    {
        //cube->setPosition(vec4(xCam, yCam, 1, 1));
        //cube->setPosition(vec4(1, -2, 1, 1));
        vec4 worldLoc = currentCam.getPickingLocation(vec2(xCam, yCam));
        vec4 onGround = vec4(worldLoc.x, worldLoc.y, 1, 1);
        
        Material m = Material();
        m.texturePath = "crate_texture.ppm";
        m.ambient = vec4(0.5, 0.5, 0.5, 1.0);
        m.diffuse = vec4(0.8, 0.8, 0.8, 1.0);
        
        Mesh *cube = new Cube();
        cube->setMaterial(m);
        cube->setPosition(onGround);
        cube->init();
        meshes.push_back(cube);
        
    }
    else if (button == GLUT_RIGHT_BUTTON)
    {
//        Triangle* tri1 = new Triangle(xCam, yCam);
//        objs.push_back(tri1);
    }
    
    glutPostRedisplay();
}

//----------------------------------------------------------------------------
// animation/timer callback function
void update(int value)
{
//    // "rising and setting the sun"
//    theta++;
//    if (theta > 360)
//    {
//        theta = 0;
//    }
//    
//    float rad = theta/2*3.1459;
//    
//    sun.direction = vec3(-5, cos(rad), -10);
    
    // set camera
//    if (isCam1_ON)
//    {
//        // flying camera
//        currentCam = cameras[0];
//    }
//    else
//    {
//        // overhead camera
//        currentCam = cameras[1];
//    }
    
    
    glutPostRedisplay();
    glutTimerFunc(500, update, value);
}

//----------------------------------------------------------------------------

void close(){
    
    delete ground;
    
    // call all mesh destructors
    for (auto &mesh : meshes)
    {
        delete mesh;
    }
}

//----------------------------------------------------------------------------
