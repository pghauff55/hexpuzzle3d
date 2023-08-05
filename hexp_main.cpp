//-------------------------------------------------------------------------------------------------------------------------
// HEX PLANET




// Author Pamela Hauff 2023

//HexPuzzlePlanet © 2023 by P. G. Hauff is licensed under Attribution-NonCommercial-NoDerivatives 4.0 International 
//------------------------------------------------------------------------------------------------------------------------




#include <iostream>
#include <vector>
#include <stdio.h>
#include <bits/stdc++.h>
#include <termios.h>
#include <cstdlib>
#include <ctime>
#include <algorithm>

// openGL and related includes
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <SOIL/SOIL.h>
// local includes
#include "hexplanet.h"
using namespace std;


//===========================================================================================================================

//												 Globals 

//===========================================================================================================================
static int SCREEN_WIDTH=1024;
static int SCREEN_HEIGHT=1024;
GLuint tex_2d;
  float A1x=0.0,A1y=0.0,A1z=0.0;

    double scale = 4.0;
int g_glutMainWin;
float g_aspect;
HexPlanet *m_planet;

  //set of sides that are travelable or not per type
  std::vector<unsigned int> vec_type1;
  std::vector<unsigned int> vec_type2;
  std::vector<unsigned int> vec_type3;
  std::vector<unsigned int> vec_type4;
  std::vector<unsigned int> vec_type0;
 

GLfloat light_diffuse[] = {1.0, 1.0, 1.0, 1.0};  /* Red diffuse light. */
GLfloat light_position[] = {1.0, 15.0, 5.0, 0.0};  /* Infinite light location. */
int mouse_x=0,mouse_y=0;

GLfloat texcoord[][2] = {{0.5, 0.0},
{0.0669875,0.25},
{0.0669875, 0.75},
{0.5, 1.0},
{0.9330125, 0.75},
{0.9330125, 0.25}};  

int tiles[3000][3];
GLuint textile[5];
GLuint number_tile[10];

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


Imath::V3f Align;


vector<int> hex_tile_list;
vector<int> hex_path;
vector<int>::iterator it;
int count_path=0;
int overflow_count=0;

map<int,int> tileNode_index_id;
vector<int> tileNode_id;
//----------------------------------------------reverse index id map----------------------------------------------------------------------------------------------------





static bool cmp(pair<int, int>& a,
			pair<int, int>& b)
	{
		return a.second < b.second;
	}
void sort(map<int, int>& M)
	{
	 
		// Declare vector of pairs
		vector<pair<int, int> > A;
	 
		// Copy key-value pair from Map
		// to vector of pairs
		for (auto& it : M) {
			A.push_back(it);
		}
	 
		// Sort using comparator function
		std::sort(A.begin(), A.end(), cmp);
	 
		// output the sorted values
		
		for (auto& it : A) {
	 
			tileNode_id.push_back(it.first);
			cout<<":"<<it.first;
			
		}
	}



//-----------------------------------------------------------------------------------------------------------------------------------------------


//-----------------------------------------------------tileNode Class--------------------------------------------------------------------------------
class tileNode{
	protected:
	int n_sides=6;
	int adjacent_nodes_by_id[6];
	float angles[6];
	int id;
	int index=0;
	Imath::V3f center,align,vectors[6];
	map<int,float> adjacent_nodes;
	map<int,float>::iterator adjacent_nodes_it;
	int adjactent_ids[6];
	bool activated=false;
	int rotation_position;
	int tile_type;
	int next_id;
	public:
	tileNode(int id){
		this->id=id;
		
		}
	void add(int id){
		this->adjacent_nodes_by_id[this->index]=id;
		this->index++;
	}
	void addCenterVert(Imath::V3f center){
		this->center=center;
	}
	Imath::V3f getCenter(){
		return this->center;
	}
	void addAlignVert0(Imath::V3f align){
		this->align=(align-center).normalize();
	}
	void addAlignVert(Imath::V3f align){
		this->align=align;
	}
	Imath::V3f calcAlignVectors();
	void randomise(){
		if(this->index==6){
			this->rotation_position=rand() % this->index + 1;
			this->tile_type=rand()%5;
			}
		else {
			this->rotation_position=0;
			this->tile_type=0;
			}
	
	}
	void deactivate(){
		this->activated=false;
	}
	void activate(){
		this->activated=true;
	}
	bool is_activated(){
		return this->activated;
	}
	int get_position(int i){
		return (i+this->rotation_position)%this->index;
	}
	int get_type(){
		return this->tile_type;
	}
	void advance_position(){
		if(this->tile_type!=0){
			this->rotation_position++;
			this->rotation_position=this->rotation_position%this->index;
			}
		
	}
	int Id(){
		return this->id;
	}
	int Index(){
		return this->index;
	}
	int getAdjacentId(int i){
		
		return this->adjactent_ids[i];
		
	}
	std::vector<unsigned int> getTypeVec(){
		
		
		switch(this->tile_type){
				case 0:
					return vec_type0;
					
				case 1:
					return vec_type1;
					
				case 2:
					return vec_type2;
					
				case 3:
					return vec_type3;
					
				case 4:
					return vec_type4;
					
			}
			return vec_type0;
		}
	Imath::V3f getAlignVec(){
		return this->align;
	}
	protected:
	void sort(map<int, float>& M){
	 
		// Declare vector of pairs
		vector<pair<int, float> > A;
	 
		// Copy key-value pair from Map
		// to vector of pairs
		for (auto& it : M) {
			A.push_back(it);
		}
	 
		// Sort using comparator function
		std::sort(A.begin(), A.end(), cmp);
	 
		// Print the sorted value
		cout<<"sorted ids:[";
		int Acounter=0;
		for (auto& it : A) {
			cout<<it.first<<";";
			this->adjactent_ids[Acounter]=it.first;
			Acounter++;
			
		}
		cout<<"]"<<endl;
	}
	static bool cmp(pair<int, float>& a,pair<int, float>& b){
		return a.second < b.second;
	}
	
};
//END CLASS------------------------------------------------------------------------------------------------------------------------------------------
vector<tileNode*> tileNode_list;
tileNode *get_tileNodefromId(int id){
	return tileNode_list[tileNode_id[id]];
}

Imath::V3f tileNode::calcAlignVectors(){
		Imath::V3f compare=this->align%this->center;
		cout<<"angles:[ ";
		for(int m=0;m<this->index;m++){
			this->vectors[m]=(m_planet->m_hexes[adjacent_nodes_by_id[m]].m_vertPos-this->center).normalize();
			
			
			this->angles[m]=180.0f*atan2f((this->vectors[m]%this->align).length(),this->vectors[m]^this->align)/M_PI;
			if((this->vectors[m]^compare)<0.0f)this->angles[m]=360.0f-this->angles[m];
			cout<<this->adjacent_nodes_by_id[m]<<" , "<<this->angles[m]<<" : ";
			if(this->angles[m]<0.0f)angles[m]+=360.0f;
			this->adjacent_nodes.insert(make_pair(this->adjacent_nodes_by_id[m],this->angles[m]));
		}
		cout<<" ]"<<endl;
			
		
		
	
		for(this->adjacent_nodes_it=this->adjacent_nodes.begin();this->adjacent_nodes_it!=this->adjacent_nodes.end();this->adjacent_nodes_it++){
			
			if(this->adjacent_nodes_it->second>335.0f)this->adjacent_nodes_it->second=0.0f;
			
		}
		
		
		this->sort(this->adjacent_nodes);
		
		cout<<endl;
		
		return (tileNode_list[tileNode_id[this->adjactent_ids[0]]]->center-this->center).normalize();
		
	}

//==================================================================================================================================================


//											 		recurse hex tiles



//==================================================================================================================================================

void recurse_hex_tiles(int j){

	


	//find hex tile j and remove from hex_tile_list
	
	
	if((it=std::find(hex_tile_list.begin(), hex_tile_list.end(), j))!=hex_tile_list.end()){
		hex_tile_list.erase(it);
		//std::cout<<"^";
	}
	else {
		//std::cout<<"&";
		//path_num++;
		overflow_count++;
		return;
		
	}
  //get neighbours
	std::vector<size_t> list_neighbours;
	m_planet->getNeighbors((size_t)(j),list_neighbours);
	
	
	hex_path.push_back(j);
    cout<<":"<<j;
    
	  //int break_count=0;
	  while(list_neighbours.size()>0){
			int x =  std::rand() / ((RAND_MAX + 1u) / list_neighbours.size()); 
	        //std::cout<<"{"<<x<<"}";
	
			
			
			if(std::find(hex_tile_list.begin(), hex_tile_list.end(),list_neighbours[x] )!=hex_tile_list.end()){
				count_path++;
			  recurse_hex_tiles(list_neighbours[x]);
			  //break_count=10;
			  //std::cout<<"+";
			}
			else {
				list_neighbours.erase(list_neighbours.begin() + x);
				//std::cout<<"-";
			}
			//break_count++;
	
		}
		
}



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
  gluPerspective( 50.0f, 1.0f, 1.0f, 5000.0f );

  
 float dt=0.000007/2;
float dsx=0,dsy=0,dsz=0;


 dsx=(mouse_x-SCREEN_WIDTH/2)*dt;

 dsy=-(mouse_y-SCREEN_HEIGHT/2)*dt;




if(fabsf(SVEC[1])>0.207f){

Imath::V3f x,y,z,Yt,Xt,Zt;

//printf("   SVEC(%2.3f  %2.3f   %2.3f ) X(%2.3f   %2.3f   %2.3f )  Y(%2.3f  %2.3f   %2.3f )   Z(%2.3f  %2.3f   %2.3f )\n",SVEC[0],SVEC[1],SVEC[2],X[0],X[1],X[2],Y[0],Y[1],Y[2],Z[0],Z[1],Z[2]);


if(SVEC[1]>0.0f){
y[0]=-SVEC[0];

y[1]=(SVEC[0]*SVEC[0]+SVEC[2]*SVEC[2])/SVEC[1];


y[2]=-SVEC[2];
}
else {
y[0]=SVEC[0];

y[1]=-(SVEC[0]*SVEC[0]+SVEC[2]*SVEC[2])/SVEC[1];


y[2]=SVEC[2];
	
	
	
}


y.normalize();




//printf("(%2.3f   %2.3f   %2.3f ) ",y[0],y[1],y[2]);
z=SVEC;
z.normalize();
x=y%z;
x.normalize();

//y=z%x;
//y.normalize();

float square_angle=acos((y^z)/(sqrt(z^z)*sqrt(y^y)))*180.0f/M_PI;

//printf("  %2.3f   ",square_angle);


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



//printf("   SVEC(%2.3f  %2.3f   %2.3f ) X(%2.3f   %2.3f   %2.3f )  Y(%2.3f  %2.3f   %2.3f )   Z(%2.3f  %2.3f   %2.3f )\n",SVEC[0],SVEC[1],SVEC[2],X[0],X[1],X[2],Y[0],Y[1],Y[2],Z[0],Z[1],Z[2]);


float SY,SX,SZ;
SVEC[0]=0.0f;
SVEC[1]=0.0f;
SVEC[2]=1.0f;
SX=SVEC[0]*X[0]+SVEC[1]*Y[0]+SVEC[2]*Z[0];
SY=SVEC[0]*X[1]+SVEC[1]*Y[1]+SVEC[2]*Z[1];
SZ=SVEC[0]*X[2]+SVEC[1]*Y[2]+SVEC[2]*Z[2];
printf("SX SY SZ (%2.3f   %2.3f   %2.3f ) ",SX,SY,SZ);


//getchar();
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
glScalef(0.105,0.105,0.105);



std::vector<Imath::V3f> Poly;

select_hex=m_planet->getHexIndexFromPoint(svec_pointer);
m_planet->getPolygon(m_planet->m_hexes[select_hex],Poly,1.1);

	// Imath::V3f center1=m_planet->m_hexes[select_hex].m_vertPos.normalize();
	 //Imath::V3f center3=Poly[0].normalize();

tileNode *selected_tile=get_tileNodefromId(select_hex);

	//std::cout<<"finished"<<std::endl;

//----------------------------------------------------------------------------------------------------------------------------------------------------

for(int j=0;j<tileNode_list.size();j++){

	tileNode *tileA=get_tileNodefromId(j);
	tileA->deactivate();
	std::vector<unsigned int> vec_typeA=tileA->getTypeVec();
	for(unsigned int i = 0; i < tileA->Index(); i++){
			  
			  tileNode *tileB=get_tileNodefromId(tileA->getAdjacentId(i));
			  std::vector<unsigned int> vec_typeB=tileB->getTypeVec();
			  if(vec_typeA[tileA->get_position(i)]==1 && vec_typeB[tileB->get_position(i)]==1){
						  tileA->activate();
						  tileB->activate();
				  }
		  }

}

//----------------------------------------------------------------------------------------------------------------------------------------------------


//----------------------------------------------------------------------------------------------------------------------------------------------------

if(button_down){
	selected_tile->advance_position();
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
glEnable(GL_BLEND); 
glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  


//-------------------------------------------------------------------------
// Render ALL hex tiles																			//printf("%d\n",m_planet->m_hexes.size());
//------------------------------------------------------------------------
for(int j=0;j<m_planet->m_hexes.size();j++){
	
	tileNode *tile=get_tileNodefromId(j);
	
	glEnable( GL_BLEND );


    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	
	//--------------------------Texture type of 5 textures types--------
	glEnable(GL_TEXTURE_2D);//-------------------------------------------
	glBindTexture(GL_TEXTURE_2D,textile[tile->get_type()]);

	//int j=16;
	m_planet->getPolygon(m_planet->m_hexes[j],Poly,1.1);
	Imath::V3f p2=tile->getCenter();
	p2.normalize();
																									//if( (p2[0]<0.3&&p2[0]>=0.0) && (p2[1]<0.3&&p2[1]>=0.0) && (p2[2]>0.8&&p2[2]<=1.0)){
																									//printf("*%d",j);
																								     //}
	glBegin(GL_POLYGON);//------------------------------------------------
	glNormal3f(p2[0],p2[1],p2[2]);
																									//printf("-----------------\n %g %g %g",p2[0],p2[1],p2[2]);
	for(int i=0;i<Poly.size();i++){
		Imath::V3f p=Poly[i];
		p.normalize();
		p *= m_planet->kPlanetRadius + 1.0;
		
		//if(p[0]==p2[0] && p[1]==p2[1] && p[2]==p2[2] )printf("*");
		glTexCoord2f(texcoord[tile->get_position(i)][0],texcoord[tile->get_position(i)][1]);
		glVertex3f(p[0],p[1],p[2]);
		//printf("%g %g %g\n",p[0],p[1],p[2]);
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);//------------------------------------------
		//----------------------FILL ACTIVATED/NON-ACTIVATED TILE--WITH COLOR-------------
	float radius_p=0.0;//------------------------------------------------
	if(tile->is_activated()){
		glColor3f(0,0,1);
		radius_p=1.05;/////// BLUE   ACTIVATED
	}
	else {
		glColor3f(1,1,1);/////WHITE   NON-ACTIVATED
		radius_p=1.15;
	}
	glBegin(GL_POLYGON);
	glNormal3f(p2[0],p2[1],p2[2]);
	//-----                                                                       --------printf("-----------------\n %g %g %g",p2[0],p2[1],p2[2]);
	for(int i=0;i<Poly.size();i++){
		Imath::V3f p=Poly[i];
		p.normalize();
		p *= m_planet->kPlanetRadius + 1.0;
		
		glVertex3f(p[0],p[1],p[2]);

	}
	glEnd();//------------------------------------------------------------

    glDisable( GL_BLEND );

// -------------------------Tile Border----------------------------
	glColor3f(1,0,0);//----------RED-----------------------------------
	glLineWidth(4.0);
	glBegin(GL_LINE_LOOP);

	for(int i=0;i<Poly.size();i++){
		Imath::V3f p=Poly[i];
		p.normalize();
		p *= m_planet->kPlanetRadius +radius_p;
		glVertex3f(p[0],p[1],p[2]);
	}
	glEnd();//------------------------------------------------------
}

//--------------------------------------------------------------------------------------------------------------------------------
// Selected Tile
//--------------------------------------------------------------------------------------------------------------------------------
 int j=select_hex;
	
	tileNode *tileA=selected_tile;
	Imath::V3f center1=tileA->getCenter();
	tileA->deactivate();
	std::vector<unsigned int> vec_typeA=tileA->getTypeVec();
	for(unsigned int i = 0; i < tileA->Index(); i++){
			  
			  tileNode *tileB=get_tileNodefromId(tileA->getAdjacentId(i));
			  std::vector<unsigned int> vec_typeB=tileB->getTypeVec();
			  if(vec_typeA[tileA->get_position(i)]==1 && vec_typeB[tileB->get_position(i)]==1){
						  tileA->activate();
						  tileB->activate();
		
						  float kr=0.3;
						  Imath::V3f center2=tileB->getCenter();
					      Imath::V3f vec0=(center2-center1).normalize();
					      Imath::V3f vec01=vec0;
					      vec0[0]*=0.1;vec0[1]*=0.1;vec0[2]*=0.1;
					      vec01[0]*=kr;vec01[1]*=kr;vec01[2]*=kr;
					      Imath::V3f vec1=center1+vec0;
					      vec1*= m_planet->kPlanetRadius + 1.5;
					      Imath::V3f vec2=(vec1%vec0).normalize();
					       vec2[0]*=kr;vec2[1]*=kr;vec2[2]*=kr;
					      glPointSize(2);
					      glColor3f(0.5,0.5,0.0);
					      glBegin( GL_POINTS);
						  glVertex3f( vec1[0],vec1[1],vec1[2]);
						  glEnd();
						  //std::cout<<"*";
						  glEnable(GL_TEXTURE_2D);
							glBindTexture(GL_TEXTURE_2D,number_tile[i]);
							//glDisable(GL_TEXTURE_2D);
							glBegin(GL_QUADS);
							glNormal3f(vec1[0],vec1[1],vec1[2]);
							glTexCoord2f(0,0);
							 glVertex3f( vec01[0]+vec1[0]+vec2[0],vec01[1]+vec1[1]+vec2[1],vec01[2]+vec1[2]+vec2[2]);
							glTexCoord2f(0,1);
							 glVertex3f( vec01[0]+vec1[0]-vec2[0],vec01[1]+vec1[1]-vec2[1],vec01[2]+vec1[2]-vec2[2]);
							 glTexCoord2f(1,1);
							 glVertex3f( -vec01[0]+vec1[0]-vec2[0],-vec01[1]+vec1[1]-vec2[1],-vec01[2]+vec1[2]-vec2[2]);
							 glTexCoord2f(1,0);
							 glVertex3f( -vec01[0]+vec1[0]+vec2[0],-vec01[1]+vec1[1]+vec2[1],-vec01[2]+vec1[2]+vec2[2]);
							glEnd();
							glDisable(GL_TEXTURE_2D);
				  }
				 // std::cout<<std::endl;
				

		  }
	
//--------------------------------------------------------------------------------------------------------------------------------------------
																									//GLUquadricObj *quadric;
																									//quadric = gluNewQuadric();

																									//gluQuadricDrawStyle(quadric, GLU_FILL );
																									//gluSphere( quadric , .5 , 36 , 18 );
//---------------------------------------------------------------------------------------------------------------------------------------------


  glutSwapBuffers(); 
glutPostRedisplay();
}
//----------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------
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

int load_texture(const char *name){
 tex_2d=SOIL_load_OGL_texture
	(
		name,
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);
	/* check for an error during the load process */
	if( 0 == tex_2d )
	{
		cout<<"SOIL loading error:"<< SOIL_last_result()<<endl;
	}
	else {
		cout<<"LOADED TEXTURE"<<endl;

		
	}
return tex_2d;
}

//==================================================================================================================================================


//											 		Main program



//==================================================================================================================================================
int main( int argc, char *argv[])
{

  //set of sides that are travelable or not per type
   
   vec_type1.push_back(1);vec_type1.push_back(0);vec_type1.push_back(0);vec_type1.push_back(0);vec_type1.push_back(0);vec_type1.push_back(1);
   vec_type2.push_back(1);vec_type2.push_back(0);vec_type2.push_back(0);vec_type2.push_back(1);vec_type2.push_back(1);vec_type2.push_back(1);
   vec_type3.push_back(1);vec_type3.push_back(1);vec_type3.push_back(1);vec_type3.push_back(1);vec_type3.push_back(1);vec_type3.push_back(1);
   vec_type4.push_back(1);vec_type4.push_back(0);vec_type4.push_back(0);vec_type4.push_back(1);vec_type4.push_back(0);vec_type4.push_back(0);
   vec_type0.push_back(0);vec_type0.push_back(0);vec_type0.push_back(0);vec_type0.push_back(0);vec_type0.push_back(0);vec_type0.push_back(0);

  



	m_planet  = new HexPlanet( 4, 0.17,0.5 );/////////////////////PLANET


	std::srand(std::time(nullptr));








	//find path



	std::cout<<"init vectors"<<std::endl;

	

	for(int j=0;j<m_planet->m_hexes.size();j++){
			hex_tile_list.push_back(j);
		}
	std::cout<<"---------------------------------------------------------"<<std::endl;
	int start=0;
		
		
	std::cout<<"[";
		
		while(count_path==0 && overflow_count<10){
				recurse_hex_tiles(hex_tile_list[start]);
			 //std::cout<<".";
		}
			
		
		
	cout<<"]"<<std::endl;
	cout<<"tiles left"<<hex_tile_list.size()<<endl;

		






	cout<<"Hex Path size:"<<hex_path.size()<<endl;

	//vector<int>::iterator hex_path_it=hex_path.begin();

	cout<<"start";
	int I0=(int)hex_path.size();
	Align=(m_planet->m_hexes[1].m_vertPos-m_planet->m_hexes[hex_path[0]].m_vertPos).normalize();
	for(int i=0; i<I0; i++){

		cout<<":"<<hex_path[i];
		vector<size_t> list_neighbours;
			
		m_planet->getNeighbors((size_t)(hex_path[i]),list_neighbours);
		
		vector<size_t>::iterator it=list_neighbours.begin();
		
		tileNode *tile=new tileNode(hex_path[i]);
		
		for(;it<list_neighbours.end();it++){
			tile->add((int)*it);
		}
		tile->addCenterVert(m_planet->m_hexes[hex_path[i]].m_vertPos.normalize());
		

		
		//tile->addAlignVert(Align);
		
		//tile->calcAlignVectors();
		
		tile->randomise();
		
		
		
		
		tileNode_list.push_back(tile);
		
		
	}

	
	
	I0=(int)hex_path.size();
	for(int i=0;i<I0;i++){
		tileNode_index_id.insert( make_pair(i,tileNode_list[i]->Id()) );
	}
	cout<<"Id to Index:[";
	sort(tileNode_index_id);
	cout<<endl;
		
		
		
		
		
	I0=(int)hex_path.size();
	Align=(m_planet->m_hexes[1].m_vertPos-m_planet->m_hexes[hex_path[0]].m_vertPos).normalize();

	for(int i=1; i<I0; i++){
		tileNode *tile=get_tileNodefromId(hex_path[i]);
		if(tile->Index()==6){
			tile->addAlignVert(Align);
			
			Align=tile->calcAlignVectors();
		}
		else {
			tile->addAlignVert(Align);
			tile->calcAlignVectors();
		}
		
	}	
		
		
	
	
	
	// Initialize glut
	glutInit(&argc, argv);
	glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH );
	glutInitWindowPosition( 0, 0 );
	glutInitWindowSize( SCREEN_WIDTH, SCREEN_HEIGHT );
 
	g_glutMainWin = glutCreateWindow( "HexPuzzlePlanet" );

	//glutFullScreen();  
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
    glPointSize(4);
    
    
	/* load an image file directly as a new OpenGL texture */

	tex_2d=load_texture("shape_hexagon.png");

	textile[0]=load_texture("shape_hexagon_empty.png");
	textile[1]=load_texture("shape_hexagon2.png");
	textile[2]=load_texture("shape_hexagon3.png");
	textile[3]=	load_texture("shape_hexagon4.png");
	textile[4]=load_texture("shape_hexagon-straight.png");


	number_tile[0]=load_texture("n0.png");
	number_tile[1]=load_texture("n1.png");
	number_tile[2]=load_texture("n2.png");
	number_tile[3]=load_texture("n3.png");
	number_tile[4]=load_texture("n4.png");
	number_tile[5]=load_texture("n5.png");
	number_tile[6]=load_texture("n6.png");
	number_tile[7]=load_texture("n7.png");
	number_tile[8]=load_texture("n8.png");
	number_tile[9]=load_texture("n9.png");

	 /* Enable a single OpenGL light. */
	GLfloat light_ambient[] =
	  {0.2, 0.2, 0.2, 0.2};
	  GLfloat light_diffuse[] =
	  {1.0, 1.0, 1.0, 0.8};
	  GLfloat light_specular[] =
	  {1.0, 1.0, 1.0, 0.9};


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



	return 1;
}
