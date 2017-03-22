#include "Angel.h"  //includes gl.h, glut.h and other stuff...
#include "Light.h"
#include "Camera.h"
#include "Cube.h"

//----------------------------------------------------------------------------
//foward declarations of functions
void init();
void display();
void resize(int width, int height);
void keyboard(unsigned char, int, int);
void keyBoardSpecial (int key, int xx, int yy);
void timerCallBack(int value);
void close();


//----------------------------------------------------------------------------
// GLOBALS
DirectionalLight* sun;
vector<Camera> cameras;
Cube* myCube;

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
    glutInitWindowSize( 512, 512 );	//set window size to be 512x512 pixels

    glutCreateWindow( "Interactive Forest" );

	//initialize glew if necessary (don't need to on Macs)
	#ifndef __APPLE__
	GLenum err = glewInit();
	#endif

    init();  //do some initialize for our program

    //set up the callback functions
    glutDisplayFunc( display );  //REQUIRED.  What to do when it's time to draw
    glutKeyboardFunc( keyboard );  //What to do if a keyboard event is detected
    glutSpecialFunc(keyBoardSpecial);
    glutTimerFunc(50, timerCallBack, 50);
    glutWMCloseFunc(close);
    glutReshapeFunc(resize);  //use for recomputing projection matrix on reshape
    glutMainLoop();  //start infinite loop, listening for events
    return 0;
}


//Initialization
void init()
{
    cameras.push_back(Camera());
    cameras.push_back(Camera());
    
	//set up the camera
	cameras[0].positionCamera(vec4(0, 2, 0, 1), vec4(0, 1, 0, 0), vec4(0, 0, -1, 0), vec4(1, 0, 0, 0));
    
    myCube = new Cube();
	
	glEnable(GL_DEPTH_TEST); //since we're doing 3D graphics, enable the Z-buffer depth test
	
	//define color to use when color buffer is cleared
    glClearColor( 1.0, 1.0, 1.0, 1.0 ); 
}

//----------------------------------------------------------------------------

void display( void )
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );  //clear out the color of the framebuffer and the depth info from the depth buffer
		
    //myCube->draw();
		
    glutSwapBuffers();
	glFlush();
}

//----------------------------------------------------------------------------

void resize(int w, int h){
	glViewport(0,0,(GLsizei) w, (GLsizei) h);
	//cam.setProjection(Perspective(65.0, GLfloat(w / h), 1.0, 100.0));
}

//----------------------------------------------------------------------------
void keyboard( unsigned char key, int x, int y )
{
    if (key == ' ')
    {
//        if (flashlight_ON == true)
//        {
//            // flashlight off
//            //flashlight_ON = false;
//        }
//        else
//        {
//            // flashlight on
//            //flashlight_ON = true;
//            
//        }
    }
    else
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
    
    if (key == 033 or key == 'q' or key == 'Q')
    {
        exit( EXIT_SUCCESS );
    }
}

void keyBoardSpecial (int key, int xx, int yy)
{
    switch (key) {
            
        case GLUT_KEY_UP :
            // forward
            //cam.moveCamera(-1);
            cout << "MOVED IN" << endl;
            break;
            
        case GLUT_KEY_DOWN :
            // backward
            //cam.moveCamera(1);
            cout << "MOVED OUT" << endl;
            break;
    }
    
}

//----------------------------------------------------------------------------
// animation/timer callback function
void timerCallBack(int value)
{
//    if (flashlight_ON)
//    {
//        // turn on flashlight
//        //myCube->enableLight1(true);
//        //mySphere->enableLight1(true);
//        //myPlane->enableLight1(true);
//    }
//    else
//    {
//        // turn off flashlight
//        //myCube->enableLight1(false);
//        //mySphere->enableLight1(false);
//        //myPlane->enableLight1(false);
//    }
    
    // "rising and setting the sun"
    theta++;
    if (theta > 360)
    {
        theta = 0;
    }
    
    float rad = theta/2*3.1459;
    
//    lights[1].changeLightProps(vec4(theta, -theta/20, -10, 1), vec4(0.1f, 0.1f, 0.1f, 1), vec4(0.1, 0.1, 0.1, 1), vec4(0.1, 0.1, 0.1, 1));
//    sun.changeLightProps(vec4(cos(rad), 10, sin(rad) , 1), vec4(0.1f, 0.1f, 0.1f, 1), vec4(0.1, 0.1, 0.1, 1), vec4(0.1, 0.1, 0.1, 1));
    sun->direction = vec3(cos(rad), sin(rad), 1);

    
    glutPostRedisplay();
    glutTimerFunc(50, timerCallBack, value);
}

//----------------------------------------------------------------------------

void close(){
    delete myCube; //, myPlane;
    
}

//----------------------------------------------------------------------------
