
#include <iostream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include "Solar.h"   
#include <GL/glut.h>	

using namespace std;

static GLenum spinMode = GL_TRUE;
static GLenum singleStep = GL_FALSE;


// These three variables control the animation's state and speed.
static float HourOfDay = 0.0;
static float DayOfYear = 0.0;

static float AnimateIncrement = 24.0;  // Time step for animation (hours)
size_t windowWidth = 800;
size_t windowHeight = 800;

/*velicina tocaka i linija*/

size_t pointSize = 10;
size_t lineWidth = 5;
size_t lineWidth2 = 2;

//const float kut = M_PI*5/180.0f;  //5 stupnjeva
//const float coskut = cos( kut );//0.999
//const float sinkut = sin( kut );//0.001522314371
int rot=0;

size_t numOfPts = 1;
//float tmpCos;

//bool mouseAlreadyDown = false;  //signalizira da korisnik drzi tocku i jos nije odlucio kamo je staviti
//bool changePointsMode = false; 
//float cosTheta = coskut;
//float sinTheta = sinkut;

struct Vektor
{
   Vektor() {}
	Vektor( float x, float y, float z = 0.0f ){ vec[0] = x; vec[1] = y; vec[2] = z; }
	inline float& operator[]( size_t index ){ return( vec[index-1] ); }
	float vec[3];
};
struct tocka
{
	tocka() {}
	tocka( float _x, float _y, float _z = 0.0f ) : x(_x), y(_y), z(_z) {}
	float x, y, z;
};

/*tocke parabole*/

Vektor vektor;

tocka T(1.0f,-1.0f,1.0f);

vector<tocka> Tocke;

GLdouble eps=0.03;

void myMouseFunc( GLint button, GLint state, GLint x, GLint y )
{
    if( button == GLUT_LEFT_BUTTON && state == GLUT_DOWN )
       {
          GLfloat xPos = GLfloat( x )/(GLfloat)windowWidth ;
          GLfloat yPos = 1-GLfloat( y )/(GLfloat)windowHeight; 

	  for( int i=0; i<Tocke.size(); ++i ) 
		if( Tocke[i].x+eps>xPos && Tocke[i].x-eps<xPos && Tocke[i].y+eps>yPos && Tocke[i].y-eps<yPos ) return;
		    
	if(Tocke.size()<2)
	  Tocke.push_back( tocka(xPos, yPos) );

       }

 
       glutPostRedisplay();

} 

static void Key_ubrzaj(void)
{	
	AnimateIncrement *= 2.0;
}

static void Key_uspori(void)
{

	AnimateIncrement /= 2.0;
}
static void Key_r(void)
{
	if ( singleStep ) {			// If ending single step mode
		singleStep = GL_FALSE;
		spinMode = GL_TRUE;		// Restart animation
	}
	else {
		spinMode = !spinMode;	// Toggle animation on and off.
	}
}

static void Key_s(void)
{
	singleStep = GL_TRUE;
	spinMode = GL_TRUE;
}
static void KeyPressFunc( unsigned char Key, int x, int y )
{
	switch ( Key ) {
	
	case '>':
		Key_ubrzaj();
		break;
	
	case '<':
		Key_uspori();
		break;
	 case ' ':
			rot=1;
			break;

	case 'r':
	case 'R':
		Key_r();
		break;
	
	case 's':
	case 'S':
		Key_s();
		break;
	
	case 27:	// Escape key
		exit(1);
	}
}




static void Animate(void)
{
	
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
   if (spinMode) {
		
   
        DayOfYear += AnimateIncrement/24.0;


        //DayOfYear = DayOfYear - ((int)(DayOfYear/365))*365;
		}
	

    glLoadIdentity();
    glTranslatef ( 0.0, 0.0, -3.0 );
    //gluLookAt (5.0, 3.0, 2.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    glRotatef( 40.0, 1.0, 0.0, 0.0 );
    
    glColor3f( 0.45, 0.65, 1.0 );//koordinatni
	glLineWidth( lineWidth );
	glBegin( GL_LINES );
		
		glVertex3f( 0.0, 5.0,0.0 );
		glVertex3f( 0.0, -5.0,0.0 );
		glVertex3f( 5.0, 0.0,0.0 );
		glVertex3f( -5.0, 0.0,0.0 );
		glVertex3f( 0.0, 0.0,5.0 );
		glVertex3f( 0.0, 0.0,-5.0 );

		
		glColor3f( 0.0f, 1.0f, 0.0f );
		glVertex3f( -vektor[1], -vektor[2], -vektor[3] );
		glVertex3f( vektor[1], vektor[2], vektor[3] );

		glEnd();

     
	



	glPushMatrix();	
	
	glPointSize(10);

	if(rot==1){
		
		
		glRotatef( 360.0*DayOfYear/365.0,  vektor[1], vektor[2], vektor[3] );
		
		}

	glPointSize(10);
	glBegin(GL_POINTS);
		glColor3f( 0.0, 1.0, 1.0 );
		for( int i=0; i<Tocke.size(); ++i ) glVertex3f( Tocke[i].x, Tocke[i].y, 0.0f);
		glVertex3f( T.x, T.y,T.z );
	glEnd();	

	glPointSize(5);
	GLint n=Tocke.size()-1;
	glColor3f( 1.0f, 1.0f, 1.0f );


	glBegin( GL_LINE_STRIP ); 
        
		for( float i = 0.01f, t = 0.0f; t <= 1.0f; t += i )

			for( int j=0; j<n;++j) 
			glVertex3f((1-t)*(1-t)*Tocke[j].x + 2.0f*(1-t)*t*T.x + t*t*Tocke[j+1].x,(1-t)*(1-t)*Tocke[j].y + 2.0f*(1-t)*t*T.y + 				t*t*Tocke[j+1].y,(1-t)*(1-t)*0.0f + 2.0f*(1-t)*t*T.z + t*t*0.0f);
	glEnd();
	
	


	
	glPopMatrix();



    	glFlush();
    	glutSwapBuffers();
	
   
	
	if ( singleStep ) {
		spinMode = GL_FALSE;
	}
	glutPostRedisplay();		// Request a re-draw for animation purposes

}

void ucitajVektor( const char* filename )
{
	FILE* file = fopen( filename, "r" );

	if( file == NULL )
	{
		fprintf( stderr, "datoteku ne mogu otvoriti\n" );
		exit( 0 );
	}

	// (x, y, z)
	if( fscanf( file, "(%f, %f, %f)", &vektor[1], &vektor[2], &vektor[3] ) != 3 )
	{
		
      fprintf( stderr, "datoteku ne mogu procitati\n" );
		
		exit(0 );
	}
	
	//normiranje vektora
 	float n = sqrt( vektor[1]*vektor[1] + vektor[2]*vektor[2] + vektor[3]*vektor[3] );
 	

 	vektor[1] = vektor[1]/n;
 	vektor[2] = vektor[2]/n;
 	vektor[3] = vektor[3]/n;

	
}


void OpenGLInit(void)
{
    glShadeModel( GL_FLAT );
    glClearColor( 0.65, 0.15, 0.5, 0.2 );
   
    glClearDepth( 1.0 );
    glEnable( GL_DEPTH_TEST );
}

static void ResizeWindow(int w, int h)
{
    float aspectRatio;
	h = (h == 0) ? 1 : h;
	w = (w == 0) ? 1 : w;
	glViewport( 0, 0, w, h);	// View port uses whole window
	aspectRatio = (float)w/(float)h;

    glMatrixMode( GL_PROJECTION ); 
    //glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
   
    //gluPerspective( 60.0, aspectRatio/2, 1.0, 30.0 );
	if(w<h)
		glFrustum(-1, 1., -1./aspectRatio, 1./aspectRatio, 1., 30.);
	else
		glFrustum(-1.*aspectRatio, 1.*aspectRatio, -1., 1., 1., 30.);
	
	
	// Select the Modelview matrix
    glMatrixMode( GL_MODELVIEW );
}


int main( int argc, char** argv )
{
	// Need to double buffer for animation
	glutInit(&argc,argv);
    	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );

	// Create and position the graphics window
    	glutInitWindowPosition( 0, 0 );
    	glutInitWindowSize( windowWidth, windowHeight);
    	glutCreateWindow( "rotacija parabole oko vektora" );

	// Initialize OpenGL.
    	OpenGLInit();

	// Set up callback functions for key presses
	glutKeyboardFunc( KeyPressFunc );
	
	glutMouseFunc(myMouseFunc);
	// Set up the callback function for resizing windows
	ucitajVektor( "os.txt" );
    	glutReshapeFunc( ResizeWindow );

	// Callback for graphics image redrawing
    	glutDisplayFunc( Animate );
	
	// Start the main loop.  glutMainLoop never returns.
	glutMainLoop(  );

    return(0);			// Compiler requires this to be here. (Never reached)
}
