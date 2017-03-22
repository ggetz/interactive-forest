#include "Angel.h"  //includes gl.h, glut.h and other stuff...
#include "Light.h"
#include "Camera.h"
#include "Ground.h"

//----------------------------------------------------------------------------
//foward declarations of functions
void init();
void draw();
void resize(int width, int height);
void keyboard(unsigned char, int, int);
void keyBoardSpecial (int key, int xx, int yy);
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

GLuint _depthBuffer;
GLuint _shadowMap;
GLuint _shadowMapSize = 1024;


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
    glutDisplayFunc( draw );  //REQUIRED.  What to do when it's time to draw
    glutKeyboardFunc( keyboard );  //What to do if a keyboard event is detected
    glutSpecialFunc(keyBoardSpecial);
    glutTimerFunc(50, update, 50);
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
    
    sun = DirectionalLight();
    sun.shadow = (vec4(0.5, 0.5, 0.7, 1.0));
    
	//set up the camera
	cameras[0].positionCamera(vec4(0, 2, 0, 0), vec4(0, 1, 0, 0), vec4(0, 0, -1, 0), vec4(1, 0, 0, 0));
    
    Material m = Material();
    m.texturePath = "grass256by256.ppm";
    m.ambient = vec4(0.5, 0.5, 0.5, 1.0);
    m.diffuse = vec4(0.8, 0.8, 0.8, 1.0);
    
    ground = new Ground();
    ground->setMaterial(m);
    ground->setPosition(vec4(0, 0.5, -7, 1));
    ground->init();
    
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

    ground->draw(cameras[0], sun, _shadowMap);
		
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
    cameras[0].changeProjection(w, h);
	
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
    
//    for (auto &mesh : meshes)
//    {
//        if (mesh != plane) // don't have the ground plan cast shadows
//        {
//            mesh->drawShadowMap(sun);
//        }
//    }
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0,0, 512, 512);
}


//----------------------------------------------------------------------------
void keyboard( unsigned char key, int x, int y )
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
void update(int value)
{
    // "rising and setting the sun"
    theta++;
    if (theta > 360)
    {
        theta = 0;
    }
    
    float rad = theta/2*3.1459;
    
//    lights[1].changeLightProps(vec4(theta, -theta/20, -10, 1), vec4(0.1f, 0.1f, 0.1f, 1), vec4(0.1, 0.1, 0.1, 1), vec4(0.1, 0.1, 0.1, 1));
//    sun.changeLightProps(vec4(cos(rad), 10, sin(rad) , 1), vec4(0.1f, 0.1f, 0.1f, 1), vec4(0.1, 0.1, 0.1, 1), vec4(0.1, 0.1, 0.1, 1));
    //sun.direction = vec3(cos(rad), sin(rad), 1);

    
    glutPostRedisplay();
    glutTimerFunc(50, update, value);
}

//----------------------------------------------------------------------------

void close(){
    delete ground;
}

//----------------------------------------------------------------------------
