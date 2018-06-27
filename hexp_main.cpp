#include <iostream>
#include <vector>
#include <stdio.h>

#include <termios.h>

#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

// GL and related includes

#include <GL/gl.h>

#include <GL/glu.h>


#include <GL/glut.h>

#include "SOIL.h"
// Local includes
#include "hexplanet.h"
using namespace std;
using namespace cv;

//=========================================
// globals (mostly for dealing with glut/gui)

//=========================================
GLuint tex_2d;
  float A1x=0.0,A1y=0.0,A1z=0.0;
//string cascadeName = "./data/haarcascades/haarcascade_frontalface_alt.xml";
//string nestedCascadeName = "./data/haarcascades/haarcascade_eye_tree_eyeglasses.xml";
//VideoCapture cap(0);
//CascadeClassifier cascade, nestedCascade;
    double scale = 4.0;
int g_glutMainWin;
float g_aspect;
HexPlanet *m_planet;



GLfloat light_diffuse[] = {1.0, 1.0, 1.0, 1.0};  /* Red diffuse light. */
GLfloat light_position[] = {1.0, 15.0, 5.0, 0.0};  /* Infinite light location. */
int mouse_x=0,mouse_y=0;

GLfloat texcoord[][2] = {{0.5, 0.0},
{0.0669875,0.25},
{0.0669875, 0.75},
{0.5, 1.0},
{0.9330125, 0.75},
{0.9330125, 0.25}};  /* Red diffuse light. */

int tiles[300][2];
GLuint textile[5];

float anglex=0.0;
float angley=0.0;
float sz=1.0f;
float sx=0.0f;
float sy=0.0f;
float sX=0.0f,sY=0.0f,sZ=1.0f;
int current_axes=5;
int last_axes=5;
bool invert_y=false;
float theta2=atan(sz/sx);
Imath::V3f svec4,svec(0.0f,0.0f,1.0f),SVEC(0.0f,0.0f,1.0f);
Imath::V3f X(1.0f,0.0f,0.0f),Y(0.0f,1.0f,0.0f),Z(0.0f,0.0f,1.0f);

bool button_down=false;
bool flipped=false;
bool moving=false;
int select_hex=0;
int current_select_hex=0;

/***    drawing text on screen  ***/     
void drawString(char *string)
{
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();             
    glLoadIdentity();   
    int w = glutGet( GLUT_WINDOW_WIDTH );
    int h = glutGet( GLUT_WINDOW_HEIGHT );
    glOrtho( 0, w, 0, h, -1, 1 );

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glDisable( GL_DEPTH_TEST ); 

    glDisable( GL_LIGHTING );
    glColor3f(1, 0, 0);

    glRasterPos2i(20, 20);
    void *font = GLUT_BITMAP_HELVETICA_18; 
    for (char* c=string; *c != '\0'; c++) 
    {
        glutBitmapCharacter(font, *c); 
    }

    glEnable( GL_LIGHTING );

    glEnable (GL_DEPTH_TEST);     

    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();  
}
//=========================================
// glut Display Func
//=========================================
void glut_Display( void )
{  

glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode( GL_PROJECTION );
  glLoadIdentity();
  gluPerspective( 50.0f, 1.5f, 1.0f, 5000.0f );

  
 float dt=0.000007f;
float dsx=0,dsy=0,dsz=0;


 dsx=(mouse_x-800)*dt;

 dsy=-(mouse_y-450.0)*dt;




if(abs(SVEC[1])>0.707f){

Imath::V3f x,y,z,Yt,Xt,Zt;

printf("   SVEC(%2.3f  %2.3f   %2.3f ) X(%2.3f   %2.3f   %2.3f )  Y(%2.3f  %2.3f   %2.3f )   Z(%2.3f  %2.3f   %2.3f )\n",SVEC[0],SVEC[1],SVEC[2],X[0],X[1],X[2],Y[0],Y[1],Y[2],Z[0],Z[1],Z[2]);

y[0]=-SVEC[0];
if(SVEC[1]>0.0f)
y[1]=sin(M_PI/2.0f-atan( abs(SVEC[1])/sqrt(SVEC[0]*SVEC[0]+SVEC[2]*SVEC[2]) ) );
else
y[1]=-sin(M_PI/2.0f-atan( abs(SVEC[1])/sqrt(SVEC[0]*SVEC[0]+SVEC[2]*SVEC[2]) ) );


y[2]=-SVEC[2];
y.normalize();

printf("(%2.3f   %2.3f   %2.3f ) ",y[0],y[1],y[2]);
z=SVEC;
z.normalize();
x=y%z;
x.normalize();

//y=z%x;
//y.normalize();

float square_angle=acos((y^z)/(sqrt(z^z)*sqrt(y^y)))*180.0f/M_PI;

printf("  %2.3f   ",square_angle);


Yt[0]=y[0]*X[0]+y[1]*Y[0]+y[2]*Z[0];
Yt[1]=y[0]*X[1]+y[1]*Y[1]+y[2]*Z[1];
Yt[2]=y[0]*X[2]+y[1]*Y[2]+y[2]*Z[2];

Zt[0]=z[0]*X[0]+z[1]*Y[0]+z[2]*Z[0];
Zt[1]=z[0]*X[1]+z[1]*Y[1]+z[2]*Z[1];
Zt[2]=z[0]*X[2]+z[1]*Y[2]+z[2]*Z[2];

Xt[0]=x[0]*X[0]+x[1]*Y[0]+x[2]*Z[0];
Xt[1]=x[0]*X[1]+x[1]*Y[1]+x[2]*Z[1];
Xt[2]=x[0]*X[2]+x[1]*Y[2]+x[2]*Z[2];

X=Xt;Y=Yt;Z=Zt;

X.normalize();
Y.normalize();
Z.normalize();



printf("   SVEC(%2.3f  %2.3f   %2.3f ) X(%2.3f   %2.3f   %2.3f )  Y(%2.3f  %2.3f   %2.3f )   Z(%2.3f  %2.3f   %2.3f )\n",SVEC[0],SVEC[1],SVEC[2],X[0],X[1],X[2],Y[0],Y[1],Y[2],Z[0],Z[1],Z[2]);


float SY,SX,SZ;
SVEC[0]=0.0f;
SVEC[1]=0.0f;
SVEC[2]=1.0f;
SX=SVEC[0]*X[0]+SVEC[1]*Y[0]+SVEC[2]*Z[0];
SY=SVEC[0]*X[1]+SVEC[1]*Y[1]+SVEC[2]*Z[1];
SZ=SVEC[0]*X[2]+SVEC[1]*Y[2]+SVEC[2]*Z[2];
printf("SX SY SZ (%2.3f   %2.3f   %2.3f ) ",SX,SY,SZ);


getchar();
}



//up vector
Imath::V3f SVEC2;
SVEC2=SVEC-Imath::V3f(0.0f,1.0f,0.0f);

 SVEC2.normalize();

Imath::V3f SVEC3=SVEC%SVEC2;
SVEC3.normalize();

Imath::V3f SVEC5;
SVEC5=SVEC%SVEC3;
SVEC5.normalize();

Imath::V3f SVEC_POINTER,svec_pointer;

SVEC_POINTER[0]=SVEC[0]+250.0f*dsx*SVEC3[0]+250.0f*dsy*SVEC5[0];
SVEC_POINTER[1]=SVEC[1]+250.0f*dsx*SVEC3[1]+250.0f*dsy*SVEC5[1];
SVEC_POINTER[2]=SVEC[2]+250.0f*dsx*SVEC3[2]+250.0f*dsy*SVEC5[2];

svec_pointer[0]=SVEC_POINTER[0]*X[0]+SVEC_POINTER[1]*Y[0]+SVEC_POINTER[2]*Z[0];
svec_pointer[1]=SVEC_POINTER[0]*X[1]+SVEC_POINTER[1]*Y[1]+SVEC_POINTER[2]*Z[1];
svec_pointer[2]=SVEC_POINTER[0]*X[2]+SVEC_POINTER[1]*Y[2]+SVEC_POINTER[2]*Z[2];

SVEC[0]+=dsx*SVEC3[0]+dsy*SVEC5[0];
SVEC[1]+=dsx*SVEC3[1]+dsy*SVEC5[1];
SVEC[2]+=dsx*SVEC3[2]+dsy*SVEC5[2];

SVEC.normalize();



sx=SVEC[0]*X[0]+SVEC[1]*Y[0]+SVEC[2]*Z[0];
sy=SVEC[0]*X[1]+SVEC[1]*Y[1]+SVEC[2]*Z[1];
sz=SVEC[0]*X[2]+SVEC[1]*Y[2]+SVEC[2]*Z[2];

Imath::V3f svec3;
svec3[0]=SVEC3[0]*X[0]+SVEC3[1]*Y[0]+SVEC3[2]*Z[0];
svec3[1]=SVEC3[0]*X[1]+SVEC3[1]*Y[1]+SVEC3[2]*Z[1];
svec3[2]=SVEC3[0]*X[2]+SVEC3[1]*Y[2]+SVEC3[2]*Z[2];

Imath::V3f svec5;
svec5[0]=SVEC5[0]*X[0]+SVEC5[1]*Y[0]+SVEC5[2]*Z[0];
svec5[1]=SVEC5[0]*X[1]+SVEC5[1]*Y[1]+SVEC5[2]*Z[1];
svec5[2]=SVEC5[0]*X[2]+SVEC5[1]*Y[2]+SVEC5[2]*Z[2];

Imath::V3f svec4(sx,sy,sz);

svec=svec4;

svec.normalize();
svec5.normalize();

char string1[100];
sprintf(string1," tile index: %d (%2.3f   %2.3f   %2.3f )  (%2.3f  %2.3f   %2.3f )   (%2.3f  %2.3f   %2.3f )",select_hex,SVEC[0],SVEC[1],SVEC[2],X[0],X[1],X[2],Y[0],Y[1],Y[2]);
drawString(string1);



gluLookAt(3.0f*svec[0], 3.0f*svec[1], 3.0f*svec[2], 0.0, 0.0, 0.0,svec5[0],svec5[1],svec5[2]);


light_position[0]=40.0f*svec[0];
light_position[1]=40.0f*svec[1];
light_position[2]=40.0f*svec[2];

  glLightfv(GL_LIGHT0, GL_POSITION, light_position);

 glMatrixMode( GL_MODELVIEW );
  glLoadIdentity();






glPushMatrix();
glScalef(1.3,1.3,1.3);
glDisable(GL_LIGHTING);
glBegin( GL_LINES );
	
	glColor3f( 1.0f, 0.0f, 0.0f );
	glVertex3f( 0.0f, 0.0f, 0.0f );
	glVertex3f( 1.0f, 0.0f, 0.0f );


	glColor3f( 0.0f, 1.0f, 0.0f );
	glVertex3f( 0.0f, 0.0f, 0.0f );
	glVertex3f( 0.0f, 1.0f, 0.0f );

	glColor3f( 0.0f, 0.0f, 1.0f );
	glVertex3f( 0.0f, 0.0f, 0.0f );
	glVertex3f( 0.0f, 0.0f, 1.0f );

	glColor3f( 1.0f, 0.0f, 0.0f );
	glVertex3f( 0.0f, 0.0f, 0.0f );
	glVertex3f( X[0], X[1], X[2] );


	glColor3f( 0.0f, 1.0f, 0.0f );
	glVertex3f( 0.0f, 0.0f, 0.0f );
	glVertex3f( Y[0], Y[1], Y[2] );

	glColor3f( 0.0f, 0.0f, 1.0f );
	glVertex3f( 0.0f, 0.0f, 0.0f );
	glVertex3f( Z[0], Z[1], Z[2] );

	//glScalef(25.0f,25.0f,25.0f);
glColor3f( 0.0f, 1.0f, 1.0f );
	glVertex3f( svec[0], svec[1], svec[2] );
	glVertex3f( svec[0]+10.0f*dsy*svec5[0], svec[1]+10.0f*dsy*svec5[1], svec[2]+10.0f*dsy*svec5[2] );
	glVertex3f( svec[0], svec[1], svec[2] );
	glVertex3f( svec[0]+10.0f*dsx*svec3[0], svec[1]+10.0f*dsx*svec3[1], svec[2]+10.0f*dsx*svec3[2] );

	
	


glEnd();
glPopMatrix();
glEnable(GL_LIGHTING);
glScalef(0.1,0.1,0.1);





std::vector<Imath::V3f> Poly;

select_hex=m_planet->getHexIndexFromPoint(svec_pointer);

m_planet->getPolygon(m_planet->m_hexes[select_hex],Poly,1.1);

if(button_down){

tiles[select_hex][1]=(tiles[select_hex][1]+1)%Poly.size();
button_down=false;
}



glDisable(GL_TEXTURE_2D);
glDisable(GL_LIGHTING);
glColor3f(0,1,0);
glLineWidth(5.0);
glBegin(GL_LINE_LOOP);

for(int i=0;i<Poly.size();i++){
	Imath::V3f p=Poly[i];
	p.normalize();
	p *= m_planet->kPlanetRadius + 1.3;
	glVertex3f(p[0],p[1],p[2]);
}
glEnd();

glEnable(GL_TEXTURE_2D);
glEnable(GL_LIGHTING);
//printf("%d\n",m_planet->m_hexes.size());
for(int j=0;j<m_planet->m_hexes.size();j++){

glColor3f(1,1,0.5);
glEnable(GL_TEXTURE_2D);
glBindTexture(GL_TEXTURE_2D,textile[tiles[j][0]]);

//int j=16;
m_planet->getPolygon(m_planet->m_hexes[j],Poly,1.1);
Imath::V3f p2=m_planet->m_hexes[j].m_vertPos;
p2.normalize();

if( (p2[0]<0.3&&p2[0]>=0.0) && (p2[1]<0.3&&p2[1]>=0.0) && (p2[2]>0.8&&p2[2]<=1.0)){
//printf("*%d",j);

}
glBegin(GL_POLYGON);
glNormal3f(p2[0],p2[1],p2[2]);
//printf("-----------------\n %g %g %g",p2[0],p2[1],p2[2]);
for(int i=0;i<Poly.size();i++){
	Imath::V3f p=Poly[i];
p.normalize();
		p *= m_planet->kPlanetRadius + 1.1;
	
if(p[0]==p2[0] && p[1]==p2[1] && p[2]==p2[2] )printf("*");
	glTexCoord2f(texcoord[(i+tiles[j][1])%Poly.size()][0],texcoord[(i+tiles[j][1])%Poly.size()][1]);
	glVertex3f(p[0],p[1],p[2]);
//printf("%g %g %g\n",p[0],p[1],p[2]);
}



glEnd();
glDisable(GL_TEXTURE_2D);


glColor3f(1,0,0);
glLineWidth(5.0);
glBegin(GL_LINE_LOOP);

for(int i=0;i<Poly.size();i++){
	Imath::V3f p=Poly[i];
p.normalize();
		p *= m_planet->kPlanetRadius + 1.2;
	

	
	glVertex3f(p[0],p[1],p[2]);

}



glEnd();
}




//GLUquadricObj *quadric;
//quadric = gluNewQuadric();

//gluQuadricDrawStyle(quadric, GLU_FILL );
//gluSphere( quadric , .5 , 36 , 18 );
  glutSwapBuffers(); 
glutPostRedisplay();
}


void OnMouseClick(int button, int state, int x, int y)
{
  if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) 
  { 
		button_down=true;
  }	
}

void glut_Motion(int x, int y)
{

mouse_x = x;
mouse_y = y;
glutPostRedisplay();
}


//=========================================
// Main program
//=========================================
int main( int argc, char *argv[])
{




  //if( !cascade.load( cascadeName ) )
 //  {
  //   printf("ERROR: Could not load classifier cascade");
        
  //      return -1;
   /// }

//cap.set(CV_CAP_PROP_FRAME_WIDTH,1280);
//cap.set(CV_CAP_PROP_FRAME_HEIGHT,720);
	// Todo: check args for fullscreen

	m_planet  = new HexPlanet( 3, 0.17,0.5 );

for(int j=0;j<m_planet->m_hexes.size();j++){

tiles[j][1]=rand() % 6 + 1;//direction
tiles[j][0]=rand() % 5;//tile type
}
	// Initialize glut
  glutInit(&argc, argv);
	glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH );
	glutInitWindowPosition( 0, 0 );
	glutInitWindowSize( 800, 600 );
 
	g_glutMainWin = glutCreateWindow( "Hex Planet Demo" );

	   glutFullScreen();  
	glutDisplayFunc( glut_Display );
	glutPassiveMotionFunc( glut_Motion );
 glutMouseFunc(OnMouseClick); 
	    glEnable( GL_BLEND );
    //glDisable( GL_BLEND );
    //	straight alpha
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    //	premultiplied alpha (remember to do the same in glColor!!)
    //glBlendFunc( GL_ONE, GL_ONE_MINUS_SRC_ALPHA );

    //	do I want alpha thresholding?
    glEnable( GL_ALPHA_TEST );
    glAlphaFunc( GL_GREATER, 0.5f );
	/* load an image file directly as a new OpenGL texture */
tex_2d = SOIL_load_OGL_texture
	(
		"shape_hexagon.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);
/* check for an error during the load process */
if( 0 == tex_2d )
{
	printf( "SOIL loading error: '%s'\n", SOIL_last_result() );
}
else printf("LOADED TEXTURE\n");


textile[0]= SOIL_load_OGL_texture
	(
		"shape_hexagon_empty.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);
/* check for an error during the load process */
if( 0 == textile[0] )
{
	printf( "SOIL loading error: '%s'\n", SOIL_last_result() );
}
else printf("LOADED TEXTURE\n");

textile[1]= SOIL_load_OGL_texture
	(
		"shape_hexagon2.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);
/* check for an error during the load process */
if( 0 == textile[1] )
{
	printf( "SOIL loading error: '%s'\n", SOIL_last_result() );
}
else printf("LOADED TEXTURE\n");
textile[2]= SOIL_load_OGL_texture
	(
		"shape_hexagon3.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);
/* check for an error during the load process */
if( 0 == textile[2] )
{
	printf( "SOIL loading error: '%s'\n", SOIL_last_result() );
}
else printf("LOADED TEXTURE\n");
textile[3]= SOIL_load_OGL_texture
	(
		"shape_hexagon4.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);
/* check for an error during the load process */
if( 0 == textile[3] )
{
	printf( "SOIL loading error: '%s'\n", SOIL_last_result() );
}
else printf("LOADED TEXTURE\n");

textile[4]= SOIL_load_OGL_texture
	(
		"shape_hexagon-straight.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);
/* check for an error during the load process */
if( 0 == textile[4] )
{
	printf( "SOIL loading error: '%s'\n", SOIL_last_result() );
}
else printf("LOADED TEXTURE\n");



 /* Enable a single OpenGL light. */
GLfloat light_ambient[] =
  {0.2, 0.2, 0.2, 1.0};
  GLfloat light_diffuse[] =
  {1.0, 1.0, 1.0, 1.0};
  GLfloat light_specular[] =
  {1.0, 1.0, 1.0, 1.0};


  glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
  glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);

  glEnable(GL_LIGHT0);
  glDepthFunc(GL_LESS);
  glEnable(GL_DEPTH_TEST);
 glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
glEnable(GL_COLOR_MATERIAL);
  glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
  glLightfv(GL_LIGHT1, GL_POSITION, light_position);
  glEnable(GL_LIGHT1);
  glEnable(GL_LIGHTING);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	// Call glut main loop  
	glutMainLoop();

//SOIL_free_image_data(tex_2d);

	return 1;
}
