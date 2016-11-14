/*
 *  homework 5
 *  Key bindings:
 *  l          Toggles lighting
 *  a/A        Decrease/increase ambient light
 *  d/D        Decrease/increase diffuse light
 *  s/S        Decrease/increase specular light
 *  e/E        Decrease/increase emitted light
 *  n/N        Decrease/increase shininess
 *  F1         Toggle smooth/flat shading
 *  F2         Toggle local viewer mode
 *  F3         Toggle light distance (1/5)
 *  F8         Change ball increment
 *  m          Toggles light movement
 *  []         Lower/rise light
 *  p          Toggles ortogonal/perspective projection
 *  +/-        Change field of view of perspective
 *  x          Toggle axes
 *  arrows     Change view angle
 *  PgDn/PgUp  Zoom in and out
 *  0          Reset view angle
 *  ESC        Exit

 * Features:
 * 1. Use SDL to add sound to the program
 */
#include<SDL/SDL.h>
#include<SDL/SDL_mixer.h>
#include "CSCIx229.h"

int axes=1;       //  Display axes
int mode=1;       //  Projection mode
int move=1;       //  Move light
int d=10;
int th=-55;         //  Azimuth of view angle
int ph=15;         //  Elevation of view angle
int fov=55;       //  Field of view (for perspective)
int light=1;      //  Lighting
double asp=1;     //  Aspect ratio
double dim=3.0;   //  Size of world
// Light values
int one       =   1;  // Unit value
int distance  =   5;  // Light distance
int inc       =  10;  // Ball increment
int smooth    =   1;  // Smooth/Flat shading
int local     =   0;  // Local Viewer Model
int emission  =   0;  // Emission intensity (%)
int ambient   =  30;  // Ambient intensity (%)
int diffuse   = 100;  // Diffuse intensity (%)
int specular  =   0;  // Specular intensity (%)
int shininess =   0;  // Shininess (power of two)
float shiny   =   1;  // Shininess (value)
float EXP     =   0;
int inf=1;
int zh        =  90;  // Light azimuth
float ylight  =   2.0;  // Elevation of light
//texture
unsigned int texture[4];
unsigned int sky[2];
unsigned int flash[1];	//the texture of flash
int showflash=0;	//show flash or not
int flashposition=35;

int Ph=0, Th=0;		// angle of light

extern int P_th;	//angle of person
extern int P_ph;
extern double P_x;
extern double P_y;
extern double P_z;
extern double P_Dir_x;
extern double P_Dir_y;
extern double P_Dir_z;

/* 
 *  Draw sky box
 */
static void Sky(double D)
{
	//  Set specular color to white
	float white[] = {1,1,1,1};
	float black[] = {0,0,0,1};
	int num=160;
	float mul=1.0/num;
	int i, j;
	glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
	glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
	glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);
	glPushMatrix();
	glRotated(45, 0, 1, 0);
	glEnable(GL_TEXTURE_2D);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	//  Sides
//	if(inf==1)
//		glColor3f(0,0,1);
//	else
//		glColor3f(46/255.0,46/255.0,46/255.0);
	glBindTexture(GL_TEXTURE_2D,sky[0]);
	glBegin(GL_QUADS);
	glNormal3d(0, 0, 1);
	for(i=0; i<num; i++)
                for(j=0; j<num; j++){
                glTexCoord2f(0.25*mul*(i+0),2/9.0*mul*(j+0)+0.43); glVertex3f(-D+2*D*mul*(i+0),Sin(-5)+D*mul*(j+0)*2/3,-D);
                glTexCoord2f(0.25*mul*(i+1),2/9.0*mul*(j+0)+0.43); glVertex3f(-D+2*D*mul*(i+1),Sin(-5)+D*mul*(j+0)*2/3,-D);
                glTexCoord2f(0.25*mul*(i+1),2/9.0*mul*(j+1)+0.43); glVertex3f(-D+2*D*mul*(i+1),Sin(-5)+D*mul*(j+1)*2/3,-D);
                glTexCoord2f(0.25*mul*(i+0),2/9.0*mul*(j+1)+0.43); glVertex3f(-D+2*D*mul*(i+0),Sin(-5)+D*mul*(j+1)*2/3,-D);
        }

        glNormal3d(1, 0, 0);
        for(i=0; i<num; i++)
                for(j=0; j<num; j++){
                glTexCoord2f(0.25*mul*(i+0)+0.25,2/9.0*mul*(j+0)+0.43); glVertex3f(+D,Sin(-5)+D*mul*(j+0)*2/3,-D+2*D*mul*(i+0));
                glTexCoord2f(0.25*mul*(i+1)+0.25,2/9.0*mul*(j+0)+0.43); glVertex3f(+D,Sin(-5)+D*mul*(j+0)*2/3,-D+2*D*mul*(i+1));
                glTexCoord2f(0.25*mul*(i+1)+0.25,2/9.0*mul*(j+1)+0.43); glVertex3f(+D,Sin(-5)+D*mul*(j+1)*2/3,-D+2*D*mul*(i+1));
                glTexCoord2f(0.25*mul*(i+0)+0.25,2/9.0*mul*(j+1)+0.43); glVertex3f(+D,Sin(-5)+D*mul*(j+1)*2/3,-D+2*D*mul*(i+0));
        }

        glNormal3d(0, 0, -1);
        for(i=0; i<num; i++)
                for(j=0; j<num; j++){
                glTexCoord2f(0.25*mul*(i+0)+0.50,2/9.0*mul*(j+0)+0.43); glVertex3f(+D-2*D*mul*(i+0),Sin(-5)+D*mul*(j+0)*2/3,+D);
                glTexCoord2f(0.25*mul*(i+1)+0.50,2/9.0*mul*(j+0)+0.43); glVertex3f(+D-2*D*mul*(i+1),Sin(-5)+D*mul*(j+0)*2/3,+D);
                glTexCoord2f(0.25*mul*(i+1)+0.50,2/9.0*mul*(j+1)+0.43); glVertex3f(+D-2*D*mul*(i+1),Sin(-5)+D*mul*(j+1)*2/3,+D);
                glTexCoord2f(0.25*mul*(i+0)+0.50,2/9.0*mul*(j+1)+0.43); glVertex3f(+D-2*D*mul*(i+0),Sin(-5)+D*mul*(j+1)*2/3,+D);
        }

        glNormal3d(-1, 0, 0);
        for(i=0; i<num; i++)
                for(j=0; j<num; j++){
                glTexCoord2f(0.25*mul*(i+0)+0.75,2/9.0*mul*(j+0)+0.43); glVertex3f(-D,Sin(-5)+D*mul*(j+0)*2/3,+D-2*D*mul*(i+0));
                glTexCoord2f(0.25*mul*(i+1)+0.75,2/9.0*mul*(j+0)+0.43); glVertex3f(-D,Sin(-5)+D*mul*(j+0)*2/3,+D-2*D*mul*(i+1));
                glTexCoord2f(0.25*mul*(i+1)+0.75,2/9.0*mul*(j+1)+0.43); glVertex3f(-D,Sin(-5)+D*mul*(j+1)*2/3,+D-2*D*mul*(i+1));
                glTexCoord2f(0.25*mul*(i+0)+0.75,2/9.0*mul*(j+1)+0.43); glVertex3f(-D,Sin(-5)+D*mul*(j+1)*2/3,+D-2*D*mul*(i+0));
        }
	glEnd();

	//  Top and bottom	
	glBegin(GL_QUADS);
	glNormal3d(0, -1, 0);
	for(i=0; i<num; i++)
		for(j=0; j<num; j++){
		glTexCoord2f(0.24*mul*(i+0)+0.25,0.34*mul*(j+0)+0.65); glVertex3f(+D-2*D*mul*(j+0),Sin(-5)+D*2/3,-D+2*D*mul*(i+0));
                glTexCoord2f(0.24*mul*(i+1)+0.25,0.34*mul*(j+0)+0.65); glVertex3f(+D-2*D*mul*(j+0),Sin(-5)+D*2/3,-D+2*D*mul*(i+1));
                glTexCoord2f(0.24*mul*(i+1)+0.25,0.34*mul*(j+1)+0.65); glVertex3f(+D-2*D*mul*(j+1),Sin(-5)+D*2/3,-D+2*D*mul*(i+1));
                glTexCoord2f(0.24*mul*(i+0)+0.25,0.34*mul*(j+1)+0.65); glVertex3f(+D-2*D*mul*(j+1),Sin(-5)+D*2/3,-D+2*D*mul*(i+0));
	}
	glEnd();

        glBegin(GL_QUADS);
	glNormal3d(0, 1, 0);
	for(i=0; i<num; i++)
                for(j=0; j<num; j++){
                glTexCoord2f(0.24*mul*(i+0)+0.25,0.33*mul*(j+0)); glVertex3f(+D-2*D*mul*(j+0),Sin(-5),-D+2*D*mul*(i+0));
                glTexCoord2f(0.24*mul*(i+1)+0.25,0.33*mul*(j+0)); glVertex3f(+D-2*D*mul*(j+0),Sin(-5),-D+2*D*mul*(i+1));
                glTexCoord2f(0.24*mul*(i+1)+0.25,0.33*mul*(j+1)); glVertex3f(+D-2*D*mul*(j+1),Sin(-5),-D+2*D*mul*(i+1));
                glTexCoord2f(0.24*mul*(i+0)+0.25,0.33*mul*(j+1)); glVertex3f(+D-2*D*mul*(j+1),Sin(-5),-D+2*D*mul*(i+0));
	}

	glEnd();

	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}


/*
 *  Draw vertex in polar coordinates with normal
 */
static void Vertex(double th,double ph)
{
	double x = Sin(th)*Cos(ph);
	double y = Sin(ph);
	double z = Cos(th)*Cos(ph);
	
	glVertex3d(x,y,z);
}

/*
 *  Draw a ball
 *     at (x,y,z)
 *     radius (r)
 */
static void ball(double x,double y,double z,double r)
{
	int th,ph;
	float yellow[] = {1.0,1.0,0.0,1.0};
	float Emission[]  = {0.0,0.0,0.01*emission,1.0};
	//  Save transformation
	glPushMatrix();
	//  Offset, scale and rotate
	glTranslated(x,y,z);
	glScaled(r,r,r);
	//  White ball
	glColor3f(1,1,1);
	glMaterialf(GL_FRONT,GL_SHININESS,shiny);
	glMaterialfv(GL_FRONT,GL_SPECULAR,yellow);
	glMaterialfv(GL_FRONT,GL_EMISSION,Emission);
	//  Bands of latitude
	for (ph=-90;ph<90;ph+=inc)
	{
		glBegin(GL_QUAD_STRIP);
		for (th=0;th<=360;th+=2*inc)
		{
			Vertex(th,ph);
			Vertex(th,ph+inc);
		}
		glEnd();
	}
	//  Undo transofrmations
	glPopMatrix();
}

/*
 * draw the ground
 */
static void Ground(double x,double y,double z,
                 double r)
{
	int th, ph;
	int d=5;
	float rep=5.0;

	//  Save transformation
	glPushMatrix();
   	//  Transform cube
   	glTranslated(x, y, z);
   	glScaled(r, r, r);
	
	//set texture
	glEnable(GL_TEXTURE_2D);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBindTexture(GL_TEXTURE_2D, texture[1]);

	glColor3f(92/255.0,92/255.0,92/255.0);
	glNormal3d(0, 1, 0);
	//upper ground
	glBegin(GL_TRIANGLE_FAN);
	glTexCoord2f(0.5, 0.5); glVertex3f(0, 0, 0);
	for (th=0;th<=360;th+=d)
	{
		glTexCoord2f(rep/2*Cos(th)+0.5, rep/2*Sin(th)+0.5);
		glVertex3f(Cos(th), 0, Sin(th));
		//Vertex(th, 0);
	}
	glEnd();

   	//side ground
	ph=-5;
	glColor3f(205/255.0, 170/255.0, 125/255.0);
   	glBegin(GL_QUAD_STRIP);
	for (th=0;th<=360;th+=d)
	{
		glNormal3d(Sin(th)*Cos(ph), Sin(ph),Cos(th)*Cos(ph));
		glTexCoord2f(th%(2*d)?0.0: 1.0, 0.0);
		Vertex(th,0);
		glTexCoord2f(th%(2*d)?0.0: 1.0, 1.0);
		Vertex(th,ph);
	}
	//  End
   	glEnd();

	/*
	//buttom ground
	glColor3f(0.5,0.5,0);
	glNormal3d(0, -1, 0);
	glBegin(GL_TRIANGLE_FAN);
	glTexCoord2f(0.5, 0.5); glVertex3f(0, Sin(ph), 0);
	//Vertex(0,0);
	for (th=0;th<=360;th+=d)
	{
		glTexCoord2f(rep/2*Cos(th)+0.5, rep/2*Sin(th)+0.5);
		glVertex3f(Sin(th)*Cos(ph), Sin(ph), Cos(th)*Cos(ph));
		//Vertex(th, ph);
	}
	glEnd();
	*/
   	//  Undo transformations
   	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
}

/*
 * draw the floor
 */
static void Floor(double x,double y,double z,
		double r, double width,double h,
		double color_r, double color_g, double color_b)
{
	int th;
	int d=5;
	float rep=width;

	//  Save transformation
	glPushMatrix();
   	//  Transform cube
   	glTranslated(x, y, z);
   	glScaled(r, r, r);
	
	//set texture
	glEnable(GL_TEXTURE_2D);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBindTexture(GL_TEXTURE_2D, texture[1]);

	glColor3f(color_r, color_g, color_b);
	glNormal3d(0, 1, 0);
	//upper floor
	glBegin(GL_QUAD_STRIP);
	glTexCoord2f(0.5, 0.5);
	for (th=0;th<=360;th+=d)
	{
		glTexCoord2f(th%(2*d)?0.0: rep, 0.0);	
		glVertex3d(Sin(th), h, Cos(th));
		glTexCoord2f(th%(2*d)?0.0: rep, rep);
		glVertex3d(width*Sin(th), h, width*Cos(th));
	}
	glEnd();

   	//side floor
	//glColor3f(184/255.0, 134/255.0, 11/255.0);
	glColor3f(205/255.0, 170/255.0, 125/255.0);
   	glBegin(GL_QUAD_STRIP);
	for (th=0;th<=360;th+=d)
	{
		glNormal3d(Sin(th), 0, Cos(th));
		glTexCoord2f(th%(2*d)?0.0: 1.0, 0.0);
		glVertex3d(Sin(th), 0, Cos(th));
		glTexCoord2f(th%(2*d)?0.0: 1.0, 1.0);
		glVertex3d(Sin(th), h, Cos(th));
	}
	//  End
   	glEnd();

	//inner side floor
	//glColor3f(184/255.0, 134/255.0, 11/255.0);
	glColor3f(205/255.0, 170/255.0, 125/255.0);
   	glBegin(GL_QUAD_STRIP);
	for (th=0;th<=360;th+=d)
	{
		glNormal3d(-Sin(th), 0, -Cos(th));
		glTexCoord2f(th%(2*d)?0.0: 1.0, 0.0);
		glVertex3d(width*Sin(th), 0, width*Cos(th));
		glTexCoord2f(th%(2*d)?0.0: 1.0, 1.0);
		glVertex3d(width*Sin(th), h, width*Cos(th));
	}
	//  End
   	glEnd();

	//buttom floor
	glColor3f(color_r, color_g, color_b);
	glNormal3d(0, -1, 0);
	glBegin(GL_QUAD_STRIP);
	for (th=0;th<=360;th+=d)
	{
		glVertex3d(Sin(th), 0, Cos(th));
		glVertex3d(width*Sin(th), 0, width*Cos(th));
	}
	glEnd();
   	//  Undo transformations
   	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
}

/*
 * draw fence for the colosseum
 */
static void Fence(double x,double y,double z,
		double r, double width,double h,
		double color_r, double color_g, double color_b)
{
	Floor(x, y, z, r, width, h, color_r, color_g, color_g);
}

/*
 * draw a pillar in (x, y, z) with length=l
 */
static void Pillar(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th)
{
	double width=0.1;		//width of the pillar
	double length=0.8;		//length of the pillar
	//  Set specular color to white
	float white[] = {1,1,1,1};
	float black[] = {0,0,0,1};
	glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
	glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
	glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);
	//  Save transformation
	glPushMatrix();
   	//  Transform cube
   	glTranslated(x,y,z);
   	glRotated(th,0,1,0);
   	glScaled(dx,dy,dz);
	
	//enable textures
	glEnable(GL_TEXTURE_2D);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBindTexture(GL_TEXTURE_2D, texture[0]);
   	//  Cube
   	glBegin(GL_QUADS);
   	//  Front
   	glColor3f(205/255.0, 201/255.0, 201/255.0);
	glNormal3f(0, 0, 1);
   	glTexCoord2f(0,0); glVertex3f(-width, 0, width);
   	glTexCoord2f(1,0); glVertex3f(+width, 0, width);
   	glTexCoord2f(1,4); glVertex3f(+width,+length, width);
   	glTexCoord2f(0,4); glVertex3f(-width,+length, width);
   	//  Back
   	glColor3f(205/255.0, 201/255.0, 201/255.0);
	glNormal3f( 0, 0,-1);
   	glTexCoord2f(0,0); glVertex3f(+width, 0,-width);
   	glTexCoord2f(1,0); glVertex3f(-width, 0,-width);
   	glTexCoord2f(1,4); glVertex3f(-width,+length,-width);
   	glTexCoord2f(0,4); glVertex3f(+width,+length,-width);
   	//  Right
   	glColor3f(139/255.0, 115/255.0, 85/255.0);
	glNormal3f(+1, 0, 0);
   	glTexCoord2f(0,0); glVertex3f(+width, 0,+width);
   	glTexCoord2f(1,0); glVertex3f(+width, 0,-width);
   	glTexCoord2f(1,4); glVertex3f(+width,+length,-width);
   	glTexCoord2f(0,4); glVertex3f(+width,+length,+width);
   	//  Left
   	glColor3f(139/255.0, 115/255.0, 85/255.0);
	glNormal3f(-1, 0, 0);
   	glTexCoord2f(0,0); glVertex3f(-width, 0,-width);
   	glTexCoord2f(1,0); glVertex3f(-width, 0,+width);
   	glTexCoord2f(1,4); glVertex3f(-width,+length,+width);
   	glTexCoord2f(0,4); glVertex3f(-width,+length,-width);
   	//  Top
   	glColor3f(0,1,1);
	glNormal3f( 0,+1, 0);
   	glVertex3f(-width,+length,+width);
   	glVertex3f(+width,+length,+width);
   	glVertex3f(+width,+length,-width);
   	glVertex3f(-width,+length,-width);
   	//  Bottom
   	glColor3f(1,0,1);
	glNormal3f( 0,-1, 0);
	glVertex3f(-width, 0,-width);
      	glVertex3f(+width, 0,-width);
      	glVertex3f(+width, 0,+width);
      	glVertex3f(-width, 0,+width);
   	//  End
   	glEnd();
	glDisable(GL_TEXTURE_2D);

   	//  Undo transformations
   	glPopMatrix();
}

void DrawFlash(double d){
	glPushMatrix();
	
	glEnable(GL_BLEND);
	glColor4f(69/255.0, 139/255.0, 116/255.0, 1);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	//glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glDepthMask(0);
	glEnable(GL_TEXTURE_2D);
	glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
	glBindTexture(GL_TEXTURE_2D, flash[0]);

	glBegin(GL_QUADS);
	glNormal3f(0,0,+1);
	
	d=4*d;
	glTexCoord2f(0,0); glVertex3f(-d*Sin(flashposition),+0,-d*Cos(flashposition));
	glTexCoord2f(1,0); glVertex3f(+d*Sin(flashposition),+0,+d*Cos(flashposition));
	glTexCoord2f(1,1); glVertex3f(+d*Sin(flashposition),+4,+d*Cos(flashposition));
	glTexCoord2f(0,1); glVertex3f(-d*Sin(flashposition),+4,-d*Cos(flashposition));
	
	/*int th=-55;
	glTexCoord2f(0,0); glVertex3f(2*dim*Sin(th)-w*Cos(th),+0.5, 2*dim*Cos(th)+w*Sin(th));
        glTexCoord2f(1,0); glVertex3f(2*dim*Sin(th)+w*Cos(th),+0.5, 2*dim*Cos(th)-w*Sin(th));
        glTexCoord2f(1,1); glVertex3f(2*dim*Sin(th)+w*Cos(th),+4  , 2*dim*Cos(th)-w*Sin(th));
        glTexCoord2f(0,1); glVertex3f(2*dim*Sin(th)-w*Cos(th),+4  , 2*dim*Cos(th)+w*Sin(th));
	*/
	glEnd();
	glDisable(GL_BLEND);
	glDepthMask(1);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}

/*
 * draw the colosseum
 */
static void Colosseum()
{
	int th=0;
	double dx, dy, dz;
	dx=dy=dz=0.5;

	//draw ground
	Ground(0, 0, 0, 1.5);
	
	//draw pillars
	//first floor pillars
	for(th=0; th<360; th+=d)
	{
		Pillar(1.4*Sin(th), 0 , 1.4*Cos(th), dx, dy, dz, th);
		Pillar(1.4*Sin(th)*0.6, 0 , 1.4*Cos(th)*0.6, dx, dy, dz, th);
	}
	
	//draw second floor pillars
	for(th=0; th<360; th+=d)
	{
		Pillar(1.4*Sin(th), 0.40 , 1.4*Cos(th), dx, dy, dz, th);
		Pillar(1.4*Sin(th)*0.8, 0.40 , 1.4*Cos(th)*0.8, dx, dy, dz, th);
	}
	
	//draw third floor pillars
	for(th=0; th<360; th+=d)
	{
		Pillar(1.4*Sin(th), 0.8 , 1.4*Cos(th), dx, dy, dz, th);
	}

	//draw second floor
	Floor(0, 0.35, 0, 1.5, 0.5, 0.05, 139/255.0, 131/255.0, 134/255.0);
	//draw third floor
	Floor(0, 0.8, 0, 1.5, 0.7, 0.05, 122/255.0, 122/255.0, 122/255.0);
	//top
	Floor(0, 1.2, 0, 1.5, 0.9, 0.05, 171/255.0, 171/255.0, 171/255.0);

	//draw fence
	Fence(0, 0, 0, 0.5, 0.9, 0.05, 0.4, 0.3, 1.0);

	//draw flash
	if(showflash==1)
		DrawFlash(2.5);	
}


/*
 *  OpenGL (GLUT) calls this routine to display the scene
 */
void display()
{
	const double len=2.0;  //  Length of axes
	//  Erase the window and the depth buffer
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	//  Enable Z-buffering in OpenGL
	glEnable(GL_DEPTH_TEST);

	//  Undo previous transformations
	glLoadIdentity();
	//  Perspective - set eye position
	/*
	double Ex = -2*dim*Sin(th)*Cos(ph);
	double Ey = +2*dim        *Sin(ph);
	double Ez = +2*dim*Cos(th)*Cos(ph);
	*/
	//gluLookAt(Ex,Ey,Ez , 0,0,0 , 0,Cos(ph),0);
	InitFPN();

	//  Flat or smooth shading
	glShadeModel(smooth ? GL_SMOOTH : GL_FLAT);
	//  Light switch
	if (light)
	{
		//  Translate intensity to color vectors
		float Ambient[]   = {0.01*ambient ,0.01*ambient ,0.01*ambient ,1.0};
		float Diffuse[]   = {0.01*diffuse ,0.01*diffuse ,0.01*diffuse ,1.0};
		float Specular[]  = {0.01*specular,0.01*specular,0.01*specular,1.0};
		//  Light position
		float Position[4];
		float Direction[]={-Sin(P_th)*Cos(P_ph), Sin(P_ph), -Cos(P_th)*Cos(P_ph), 0};
		if(showflash==0)//showflash==1 showflash
		{
			Position[0]=P_x;Position[1]=P_y;Position[2]=P_z;Position[3]=1.0;
		}
		else
		{
			Position[0]=P_x;Position[1]=P_y;Position[2]=P_z;Position[3]=0;
			Direction[0]=-P_x;
			Direction[1]=-P_y;
			Direction[2]=-P_z;
		}
		//printf("direction:%f,%f,%f\n", Direction[0], Direction[1], Direction[2]);
		//float Direction[]={Cos(Th)*Sin(Ph), Sin(Th)*Sin(Ph), -Cos(Ph), 0};
		//  Draw light position as ball (still no lighting here)
		glColor3f(1,1,1);
		ball(Position[0],Position[1],Position[2] , 0.1);
		//  OpenGL should normalize normal vectors
		glEnable(GL_NORMALIZE);
		//  Enable lighting
		glEnable(GL_LIGHTING);
		//  Location of viewer for specular calculations
		glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,local);
		//  glColor sets ambient and diffuse color materials
		glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
		glEnable(GL_COLOR_MATERIAL);
		//  Enable light 0
		glEnable(GL_LIGHT0);
		//  Set ambient, diffuse, specular components and position of light 0
		glLightfv(GL_LIGHT0,GL_AMBIENT ,Ambient);
		glLightfv(GL_LIGHT0,GL_DIFFUSE ,Diffuse);
		glLightfv(GL_LIGHT0,GL_SPECULAR,Specular);
		glLightfv(GL_LIGHT0,GL_POSITION,Position);

		glLightfv(GL_LIGHT0,GL_SPOT_DIRECTION,Direction);
		glLightf(GL_LIGHT0,GL_SPOT_CUTOFF,5);
		glLightf(GL_LIGHT0,GL_SPOT_EXPONENT,0.8);

		glLightf(GL_LIGHT0,GL_QUADRATIC_ATTENUATION,3/100.0);
		//glLightf(GL_LIGHT0,GL_LINEAR_ATTENUATION,10/100.0);
	}
	else
	glDisable(GL_LIGHTING);

	/*
	GLuint fogMode[]={GL_EXP, GL_EXP2, GL_LINEAR};
	GLfloat fogColor[4]={0.3, 0.6, 0.6, 1.0};
	GLfloat density=1.0;
	
	glFogi(GL_FOG_MODE, fogMode[0]);
	glFogfv(GL_FOG_COLOR, fogColor);
	glFogf(GL_FOG_DENSITY, density);
	glHint(GL_FOG_HINT, GL_DONT_CARE);
	glFogf(GL_FOG_START, 50.0);	//start depth
	glFogf(GL_FOG_END, 400.0);	//end depth
	glEnable(GL_FOG);
	*/
	//draw the sky box
	Sky(5*dim);
	//  Draw scene
	Colosseum();

	//  Draw axes - no lighting from here on
	glDisable(GL_LIGHTING);
	glColor3f(1,1,1);
	if (!axes)
	{
		glBegin(GL_LINES);
		glVertex3d(0.0,0.0,0.0);
		glVertex3d(len,0.0,0.0);
		glVertex3d(0.0,0.0,0.0);
		glVertex3d(0.0,len,0.0);
		glVertex3d(0.0,0.0,0.0);
		glVertex3d(0.0,0.0,len);
		glEnd();
		//  Label axes
		glRasterPos3d(len,0.0,0.0);
		Print("X");
		glRasterPos3d(0.0,len,0.0);
		Print("Y");
		glRasterPos3d(0.0,0.0,len);
		Print("Z");
	}

	//  Display parameters
	glWindowPos2i(5,5);
	Print("Angle=%d,%d  Person Angle=%d,%d FOV=%d Projection=%s Light=%s",th,ph,P_th,P_ph,fov,mode?"Perpective":"Orthogonal",light?"On":"Off");
	if (light)
	{
		glWindowPos2i(5,45);
		Print("Model=%s LocalViewer=%s Distance=%d Elevation=%.1f",smooth?"Smooth":"Flat",local?"On":"Off",distance,ylight);
		glWindowPos2i(5,25);
		Print("Ambient=%d  Diffuse=%d Specular=%d Emission=%d Shininess=%.0f",ambient,diffuse,specular,emission,shiny);
	}

	//  Render the scene and make it visible
	ErrCheck("display");
	glFlush();
	SDL_GL_SwapBuffers();
}
/*
 *  GLUT calls this routine when a key is pressed
 */
int key()
{
	Uint8* keys=SDL_GetKeyState(NULL);
	int shift=SDL_GetModState()&KMOD_SHIFT;
	//  Exit on ESC
	if (keys[SDLK_ESCAPE])
		exit(0);
	//  Reset view angle
	else if (keys[SDLK_0])
		th = ph = 0;
	//  Toggle axes
	else if (keys[SDLK_x])
		axes = 1-axes;
	//  Toggle lighting
	else if (keys[SDLK_l])
		light = 1-light;
	//  Switch projection mode
	else if (keys[SDLK_p])
		mode = 1-mode;
	//  Toggle light movement
	else if (keys[SDLK_m])
		move = 1-move;
	//  Move light
	else if (keys[SDLK_LEFTBRACKET])
		zh += 1;
	else if (keys[SDLK_RIGHTBRACKET])
		zh -= 1;
	//  Change field of view angle
	else if (keys[SDLK_KP_MINUS]||keys[SDLK_MINUS])
		fov--;
	else if (keys[SDLK_KP_PLUS]||keys[SDLK_EQUALS])
		fov++;
	//  Light elevation
	else if(keys[SDLK_w])
	{
		FPN_Forward();
	}
	else if(keys[SDLK_s])
	{
		FPN_Backward();
	}
	else if(keys[SDLK_a])
	{
		P_th+=5;
		FPN_Refresh();
	}
	else if(keys[SDLK_d])
	{
		P_th-=5;
		FPN_Refresh();
	}
	//  Shininess level
	/*
	else if (keys[SDLK_n] && shininess>-1)
		shininess -= 1;
	else if (keys[SDLK_n] && shift && shininess<7)
		shininess += 1;
	else if (keys[SDLK_i])
		printf("i pressed\n");
	*/
	 //  Increase/decrease asimuth
	else if (keys[SDLK_RIGHT])
		th += 5;
	else if (keys[SDLK_LEFT])
		th -= 5;
	//  Increase/decrease elevation
	else if (keys[SDLK_UP])
	{
		P_ph += 5;
		FPN_Refresh();
	}
	else if (keys[SDLK_DOWN])
	{
		P_ph -= 5;
		FPN_Refresh();
	}
	//  Translate shininess power to value (-1 => 0)
	shiny = shininess<0 ? 0 : pow(2.0,shininess);
	//  Reproject
	Project(fov,asp,dim);
	//  Animate if requested
	return 1;
}

/*
 *  GLUT calls this routine when the window is resized
 */
void reshape(int width,int height)
{
	//  Ratio of the width to the height of the window
	asp = (height>0) ? (double)width/height : 1;
	//  Set the viewport to the entire window
	glViewport(0,0, width,height);
	//  Set projection
	Project(mode?fov:0,asp,dim);
}

/*
 *  Start up GLUT and tell it what to do
 */
int main(int argc,char* argv[])
{
	int run=1;
	double t0=0;
	
	Mix_Music* music;
	SDL_Surface* screen;
	
	//Initialize SDL
	SDL_Init(SDL_INIT_VIDEO);
	//Set size, resizable and double buffering
	screen=SDL_SetVideoMode(600, 600, 0, SDL_OPENGL|SDL_RESIZABLE|SDL_DOUBLEBUF);
	if(!screen) Fatal("Cannot set SDL video mode\n");
	//set window and icon labels
	SDL_WM_SetCaption("Colosseum -Zhenguo Chen", "dsl1.2");
	//set screen size
	reshape(screen->w, screen->h);
	
	//Load textures
	texture[0]=LoadTexBMP("wall.bmp");
	texture[1]=LoadTexBMP("stone3.bmp");
	texture[2]=LoadTexBMP("brickSingle2.bmp");
	texture[3]=LoadTexBMP("skybox.bmp");
	sky[0]=LoadTexBMP("skybox2.bmp");
	sky[1]=LoadTexBMP("sky1.bmp");
	flash[0]=LoadTexBMP("flash3.bmp");
	
	//Initialize audio
	if (Mix_OpenAudio(44100, AUDIO_S16SYS, 2, 4096)) Fatal("cannot initialize audio\n");
	music= Mix_LoadMUS("thunder.mp3");
	if(!music) Fatal("cannot load thunder.mp3\n");
	Mix_PlayMusic(music, -1);
	
	//SDL event loop
	ErrCheck("init");
	//limit the number of flash happen in a period of time
	int flashtime=0;
	double lasttime=0;	//flash last time
	double intervals[]={0.05, 1.0, 5.0, 2.0, 8.0, 1.0, 6.0, 0.05};

	while(run)
	{
		//elaspsed time in seconds
		double t=SDL_GetTicks()/1000.0;
		//Process all pending events
		SDL_Event event;
		while(SDL_PollEvent(&event))
			switch(event.type)
			{
				case SDL_VIDEORESIZE:
					screen=SDL_SetVideoMode(event.resize.w, event.resize.h, 0, SDL_OPENGL|SDL_RESIZABLE|SDL_DOUBLEBUF);
					reshape(screen->w, screen->h);
					break;
				case SDL_QUIT:
					run=0;
					break;
				case SDL_KEYDOWN:
					run=key();
					t0=t+0.5;
					break;
				default:
					//do nothing
					break;
			}
		//repeat key every 50 ms
		if(t-t0>0.05)
		{
			run=key();
			t0=t;
		}
		if(t-lasttime>intervals[flashtime])
		{
			inf=1-inf;
			showflash=1-showflash;
			flashtime=(flashtime+1)%8;
			lasttime=t;
			flashposition=(flashposition-30)%360;
		}
		//display
		display();
		//slow down display rate to about 100 fps by sleeping 5ms
		SDL_Delay(5);
	}
	//shut down SDL
	Mix_CloseAudio();
	SDL_Quit();
	return 0;
}
