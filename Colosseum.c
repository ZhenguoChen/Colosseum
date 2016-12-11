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
 * 2. Have flash and when there is thunder there is flash.
 * 3. You can move the flashlight.
 * 4. The sky box can be light up with flashlight.
 */
#include <stdbool.h>
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_image.h>
#include "CSCIx229.h"

int axes=1;       //  Display axes
int move=1;       //  Move light
int d=20;
int th=-55;         //  Azimuth of view angle
int ph=15;         //  Elevation of view angle
int fov=60;       //  Field of view (for perspective)
int light=1;      //  Lighting
double asp=1;     //  Aspect ratio
double dim=3.0;   //  Size of world
// Light values
int one       =   1;  // Unit value
int distance  =   5;  // Light distance
int inc       =  10;  // Ball increment
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
float ylight  =   7.5;  // Elevation of light
//texture
unsigned int texture[3];
unsigned int sky[1];
unsigned int flash[1];	//the texture of flash
int showflash=0;	//show flash or not
int flashposition=35;
int showfog=0;
GLfloat density=0.0;	// the density of the fog

int Ph=0, Th=0;		// angle of light

extern int P_th;	//angle of person
extern int P_ph;
extern double P_x;
extern double P_y;
extern double P_z;
extern double P_Dir_x;
extern double P_Dir_y;
extern double P_Dir_z;
extern int collision;

// Do shadow mappiing
float  Lpos[4];	//light position
unsigned int framebuf=0;
double Svec[4];
double Tvec[4];
double Rvec[4];
double Qvec[4];
int    Width;	//window width
int    Height;	//window height
int shadowdim;	//size of shadow map texture
int shader;	//shader
int createshadow=1;
int shadow=0;

//mode=1, you can see the whole scene from different angle
//mode=0, what from the first person navigation perspective, but the scene is small
//mode=2, draw a bigger Colosseum for first person navigation
int mode=0;

int obj;	//colosseum obj
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

        glNormal3d(-1, 0, 0);
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

        glNormal3d(1, 0, 0);
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
 * draw the ground
 */
static void Ground(double x,double y,double z,
                 double r)
{
	int th, ph;
	int d=5;
	float rep=5.0;
	int num=r/0.05, i;

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
		glVertex3f(Cos(th)*r/num, 0, Sin(th)*r/num);
		//Vertex(th, 0);
	}
	glEnd();
	//draw rings
	glBegin(GL_QUAD_STRIP);
	for (i=0; i<num-1; i++){
		// as the radius of the rings become larger, the texture would become sparse and also affect the lighting. So, change d to fix this.
		d=d-(num/10)>0?d-(num/10):1;
		for (th=0;th<=360;th+=d)
        	{
        	        //glTexCoord2f(th%(2*d)?0.0:1.0, 0.0);
        	        glTexCoord2f(th%(2*d)?10.0/num*(i+0):10.0/num*(i+1), 10.0/num*(i+0));
                	glVertex3d((i+2)*Sin(th)/num, 0, (i+2)*Cos(th)/num);
                	//glTexCoord2f(th%(2*d)?0.0:1.0, 1);
                	glTexCoord2f(th%(2*d)?10.0/num*(i+0):10.0/num*(i+1), 10.0/num*(i+1));
                	glVertex3d((i+1)*Sin(th)/num, 0, (i+1)*Cos(th)/num);
        	}
	}
	glEnd();

   	//side ground
	ph=-5;
	//glColor3f(205/255.0, 170/255.0, 125/255.0);
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
	//float rep=2;
	int num=r/0.25, i;

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
	glNormal3f(0, 1, 0);
        for (i=width*num-1; i<num-1; i++){
                for (th=0;th<=360;th+=d)
                {
                        glTexCoord2f(th%(2*d)?0.0:1.0, 0.0);
                        glVertex3d((i+2)*Sin(th)/num, h, (i+2)*Cos(th)/num);
                        glTexCoord2f(th%(2*d)?0.0:1.0, 1);
                        glVertex3d((i+1)*Sin(th)/num, h, (i+1)*Cos(th)/num);
                }
        }

	glEnd();

   	//side floor
	//glColor3f(184/255.0, 134/255.0, 11/255.0);
	//glColor3f(205/255.0, 170/255.0, 125/255.0);
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
	//glColor3f(205/255.0, 170/255.0, 125/255.0);
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
	for (i=width*num-1; i<num-1; i++){
                for (th=0;th<=360;th+=d)
                {
                        glTexCoord2f(th%(2*d)?0.0:1.0, 0.0);
                        glVertex3d((i+2)*Sin(th)/num, 0, (i+2)*Cos(th)/num);
                        glTexCoord2f(th%(2*d)?0.0:1.0, 1);
                        glVertex3d((i+1)*Sin(th)/num, 0, (i+1)*Cos(th)/num);
                }
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
	int num=40;
	float mul=1.0/num;
	//  Set specular color to white
	float white[] = {1,1,1,1};
	float black[] = {0,0,0,1};
	int i, j, k;
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
   	
	glColor3f(205/255.0, 201/255.0, 201/255.0);
	glNormal3f(0, 0, 1);
   	glBegin(GL_QUADS);
   	//  Front
   	
	for(i=0; i<num; i++)
		for(j=0, k=0; j<num; j++, k+=4){
   		glTexCoord2f(mul*(i+0),mul*(k+0)); 
		glVertex3f(-width+2*width*mul*(i+0), length*mul*(j+0), width);
   		glTexCoord2f(mul*(i+1),mul*(k+0)); 
		glVertex3f(-width+2*width*mul*(i+1), length*mul*(j+0), width);
   		glTexCoord2f(mul*(i+1),mul*(k+4)); 
		glVertex3f(-width+2*width*mul*(i+1), length*mul*(j+1), width);
   		glTexCoord2f(mul*(i+0),mul*(k+4)); 
		glVertex3f(-width+2*width*mul*(i+0), length*mul*(j+1), width);
	}
   	//  Back
   	glColor3f(205/255.0, 201/255.0, 201/255.0);
	glNormal3f( 0, 0,-1);
	for(i=0; i<num; i++)
                for(j=0, k=0; j<num; j++, k+=4){
                glTexCoord2f(mul*(i+0),mul*(k+0)); 
                glVertex3f(width-2*width*mul*(i+0), length*mul*(j+0), -width);
                glTexCoord2f(mul*(i+1),mul*(k+0)); 
                glVertex3f(width-2*width*mul*(i+1), length*mul*(j+0), -width);
                glTexCoord2f(mul*(i+1),mul*(k+4)); 
                glVertex3f(width-2*width*mul*(i+1), length*mul*(j+1), -width);
                glTexCoord2f(mul*(i+0),mul*(k+4)); 
                glVertex3f(width-2*width*mul*(i+0), length*mul*(j+1), -width);
        }

   	//  Right
   	glColor3f(205/255.0, 201/255.0, 201/255.0);
	glNormal3f(+1, 0, 0);
	for(i=0; i<num; i++)
                for(j=0, k=0; j<num; j++, k+=4){
                glTexCoord2f(mul*(i+0),mul*(k+0)); 
                glVertex3f(width, length*mul*(j+0), width-2*width*mul*(i+0));
                glTexCoord2f(mul*(i+1),mul*(k+0)); 
                glVertex3f(width, length*mul*(j+0), width-2*width*mul*(i+1));
                glTexCoord2f(mul*(i+1),mul*(k+4)); 
                glVertex3f(width, length*mul*(j+1), width-2*width*mul*(i+1));
                glTexCoord2f(mul*(i+0),mul*(k+4)); 
                glVertex3f(width, length*mul*(j+1), width-2*width*mul*(i+0));
        }

   	//  Left
   	glColor3f(205/255.0, 201/255.0, 201/255.0);
	glNormal3f(-1, 0, 0);
	for(i=0; i<num; i++)
                for(j=0, k=0; j<num; j++, k+=4){
                glTexCoord2f(mul*(i+0),mul*(k+0));
                glVertex3f(-width, length*mul*(j+0), -width+2*width*mul*(i+0));
                glTexCoord2f(mul*(i+1),mul*(k+0));
                glVertex3f(-width, length*mul*(j+0), -width+2*width*mul*(i+1));
                glTexCoord2f(mul*(i+1),mul*(k+4));
                glVertex3f(-width, length*mul*(j+1), -width+2*width*mul*(i+1));
                glTexCoord2f(mul*(i+0),mul*(k+4));
                glVertex3f(-width, length*mul*(j+1), -width+2*width*mul*(i+0));
        }

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
	
	d=3*d;
	glTexCoord2f(0,0); glVertex3f(-d*Sin(flashposition),+0,-d*Cos(flashposition));
	glTexCoord2f(1,0); glVertex3f(+d*Sin(flashposition),+0,+d*Cos(flashposition));
	glTexCoord2f(1,1); glVertex3f(+d*Sin(flashposition),+6,+d*Cos(flashposition));
	glTexCoord2f(0,1); glVertex3f(-d*Sin(flashposition),+6,-d*Cos(flashposition));
	
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
	int d=10;
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
	Floor(0, 1.2, 0, 1.5, 0.9, 0.05, 122/255.0, 122/255.0, 122/255.0);

	//draw fence
	Fence(0, 0, 0, 0.5, 0.9, 0.05, 0.4, 0.3, 1.0);

	//draw flash, when create shadow, do not display this
	if(showflash==1&&mode==0){
		DrawFlash(5);
	}
}

/*
 * draw a bigger colosseum for first person navigation
 */
static void Colosseum_FPN()
{
        int th=0;
        double dx, dy, dz;
        int d=20;
        dx=dy=dz=1;

        //draw ground
        Ground(0, 0, 0, 3.0);

        //draw pillars
        //first floor pillars
        for(th=0; th<360; th+=d)
        {
                Pillar(2.9*Sin(th), 0 , 2.9*Cos(th), dx, dy, dz, th);
                Pillar(2.9*Sin(th)*0.6, 0 , 2.9*Cos(th)*0.6, dx, dy, dz, th);
        }

        //draw second floor pillars
        for(th=0; th<360; th+=d)
        {
                Pillar(2.9*Sin(th), 0.95 , 2.9*Cos(th), dx, dy, dz, th);
                Pillar(2.9*Sin(th)*0.8, 0.95 , 2.9*Cos(th)*0.8, dx, dy, dz, th);
        }

        //draw third floor pillars
        for(th=0; th<360; th+=d)
        {
                Pillar(2.9*Sin(th), 1.9 , 2.9*Cos(th), dx, dy, dz, th);
        }

        //draw second floor
        Floor(0, 0.8, 0, 3.0, 0.5, 0.05, 139/255.0, 131/255.0, 134/255.0);
        //draw third floor
        Floor(0, 1.75, 0, 3.0, 0.7, 0.05, 122/255.0, 122/255.0, 122/255.0);
        //top
        Floor(0, 2.7, 0, 3.0, 0.9, 0.05, 171/255.0, 171/255.0, 171/255.0);

        //draw fence
        Fence(0, 0, 0, 1.0, 0.9, 0.05, 0.4, 0.3, 1.0);
}

/*
 *  OpenGL (GLUT) calls this routine to display the scene
 */
void display()
{
	int id;
	//  Erase the window and the depth buffer
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	//  Enable Z-buffering in OpenGL
	glEnable(GL_DEPTH_TEST);

	//  Undo previous transformations
	glLoadIdentity();	

	//  Perspective - set eye position
	if(mode==1){
		double Ex = -2*dim*Sin(th)*Cos(ph);
		double Ey = +2*dim        *Sin(ph);
		double Ez = +2*dim*Cos(th)*Cos(ph);
	
		gluLookAt(Ex,Ey,Ez , 0,0,0 , 0,Cos(ph),0);
	}
	else{
		InitFPN();
	}
	
	/*
	 * start drawing shadow
	 */
	if (showflash==1&&shadow==1){
		glUseProgram(shader);
		id=glGetUniformLocation(shader, "tex");
		if(id>=0)glUniform1i(id,0);
		id=glGetUniformLocation(shader, "depth");
		if(id>=0)glUniform1i(id,1);

		glActiveTexture(GL_TEXTURE1);
		glTexGendv(GL_S, GL_EYE_PLANE, Svec);
		glTexGendv(GL_T, GL_EYE_PLANE, Tvec);
		glTexGendv(GL_R, GL_EYE_PLANE, Rvec);
		glTexGendv(GL_Q, GL_EYE_PLANE, Qvec);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
		glActiveTexture(GL_TEXTURE0);
	}

	//  Flat or smooth shading
	glShadeModel(GL_SMOOTH);
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
			//Position[0]=P_x;Position[1]=P_y;Position[2]=P_z;Position[3]=1.0;
			Position[0]=P_x; Position[1]=P_y; Position[2]=P_z; Position[3]=1.0;
		}
		else
		{
			Position[0]=6;Position[1]=0.35;Position[2]=6;Position[3]=0;
			Direction[0]=-6;
			Direction[1]=-0.35;
			Direction[2]=-6;
		}

		glColor3f(1,1,1);

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
		glLightf(GL_LIGHT0,GL_SPOT_CUTOFF,4);
		glLightf(GL_LIGHT0,GL_SPOT_EXPONENT,0.8);

		glLightf(GL_LIGHT0,GL_QUADRATIC_ATTENUATION,10/100.0);
		glLightf(GL_LIGHT0,GL_LINEAR_ATTENUATION,10/100.0);
		glLightf(GL_LIGHT0,GL_CONSTANT_ATTENUATION, 10/100.0);
	}
	else
	glDisable(GL_LIGHTING);

	if(showfog==1){
		GLuint fogMode[]={GL_EXP, GL_EXP2, GL_LINEAR};
		GLfloat fogColor[4]={1.0, 1.0, 1.0, 1.0};
		
		glFogi(GL_FOG_MODE, fogMode[0]);
		glFogfv(GL_FOG_COLOR, fogColor);
		if(showflash==1)
			glFogf(GL_FOG_DENSITY, 0.02);
		else
			glFogf(GL_FOG_DENSITY, density);
		glHint(GL_FOG_HINT, GL_DONT_CARE);
		glFogf(GL_FOG_START, 5.0);	//start depth
		glFogf(GL_FOG_END, 10.0);	//end depth
		glEnable(GL_FOG);
	}
	//draw the sky box
	Sky(5*dim);

	//  Draw scene
	if(mode==2)
		Colosseum_FPN();
	else
	{
		/*
		glPushMatrix();
		glTranslated(P_x, P_y, P_z);
		glScaled(0.1, 0.1, 0.1);
		glCallList(obj);
		glPopMatrix();
		*/
		Colosseum();
	}

	if(showflash==1&&shadow==1)
		glUseProgram(0);

	if(collision==1){
		glWindowPos2i(5,5);
   		Print("Be careful! Do not hit the wall\n");
	}

	glDisable(GL_LIGHTING);
	glDisable(GL_FOG);
	glColor3f(1,1,1);
	
	Project(fov,asp,dim);
	//  Render the scene and make it visible
	ErrCheck("display");
	glFlush();
	SDL_GL_SwapBuffers();
}

/*
 *  Build Shadow Map
 */
void ShadowMap(void)
{
	double Lmodel[16];  //  Light modelview matrix
	double Lproj[16];   //  Light projection matrix
	double Tproj[16];   //  Texture projection matrix
	double Dim=2.0;     //  Bounding radius of scene
	double Ldist;       //  Distance from light to scene center
	Lpos[0] = 2;
	Lpos[1] = ylight;
	Lpos[2] = 0;
	Lpos[3] = 1;

	//  Save transforms and modes
	glPushMatrix();
	glPushAttrib(GL_TRANSFORM_BIT|GL_ENABLE_BIT);
	//  No write to color buffer and no smoothing
	glShadeModel(GL_FLAT);
	glColorMask(0,0,0,0);
	// Overcome imprecision
	glEnable(GL_POLYGON_OFFSET_FILL);

	//  Turn off lighting and set light position
	glDisable(GL_LIGHTING);
	glDisable(GL_COLOR_MATERIAL);
	glDisable(GL_NORMALIZE);

	//  Light distance
	Ldist = sqrt(Lpos[0]*Lpos[0] + Lpos[1]*Lpos[1] + Lpos[2]*Lpos[2]);
	if (Ldist<1.1*Dim) Ldist = 1.1*Dim;

	//  Set perspective view from light position
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(114.6*atan(Dim/Ldist),1,Ldist-Dim,Ldist+Dim);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(Lpos[0],Lpos[1],Lpos[2] , 0,0,0 , 0,1,0);
	//  Size viewport to desired dimensions
	glViewport(0,0,shadowdim,shadowdim);

	// Redirect traffic to the frame buffer
	glBindFramebuffer(GL_FRAMEBUFFER,framebuf);

	// Clear the depth buffer
	glClear(GL_DEPTH_BUFFER_BIT);
	// Draw all objects that can cast a shadow
	Colosseum();

	//  Retrieve light projection and modelview matrices
	glGetDoublev(GL_PROJECTION_MATRIX,Lproj);
	glGetDoublev(GL_MODELVIEW_MATRIX,Lmodel);

	// Set up texture matrix for shadow map projection,
	// which will be rolled into the eye linear
	// texture coordinate generation plane equations
	glLoadIdentity();
	glTranslated(0.5,0.5,0.5);
	glScaled(0.5,0.5,0.5);
	glMultMatrixd(Lproj);
	glMultMatrixd(Lmodel);

	// Retrieve result and transpose to get the s, t, r, and q rows for plane equations
	glGetDoublev(GL_MODELVIEW_MATRIX,Tproj);
	Svec[0] = Tproj[0];    Tvec[0] = Tproj[1];    Rvec[0] = Tproj[2];    Qvec[0] = Tproj[3];
	Svec[1] = Tproj[4];    Tvec[1] = Tproj[5];    Rvec[1] = Tproj[6];    Qvec[1] = Tproj[7];
	Svec[2] = Tproj[8];    Tvec[2] = Tproj[9];    Rvec[2] = Tproj[10];   Qvec[2] = Tproj[11];
	Svec[3] = Tproj[12];   Tvec[3] = Tproj[13];   Rvec[3] = Tproj[14];   Qvec[3] = Tproj[15];

	// Restore normal drawing state
	glShadeModel(GL_SMOOTH);
	glColorMask(1,1,1,1);
	glDisable(GL_POLYGON_OFFSET_FILL);
	glPopAttrib();
	glPopMatrix();
	glBindFramebuffer(GL_FRAMEBUFFER,0);

	//  Check if something went wrong
	ErrCheck("ShadowMap");
}

void InitMap()
{
	unsigned int shadowtex; //  Shadow buffer texture id
	int n;

	//  Make sure multi-textures are supported
	glGetIntegerv(GL_MAX_TEXTURE_UNITS,&n);
	if (n<2) Fatal("Multiple textures not supported\n");

	//  Get maximum texture buffer size
	glGetIntegerv(GL_MAX_TEXTURE_SIZE,&shadowdim);
	//  Limit texture size to maximum buffer size
	glGetIntegerv(GL_MAX_RENDERBUFFER_SIZE,&n);
	if (shadowdim>n) shadowdim = n;
	//  Limit texture size to 2048 for performance
	if (shadowdim>2048) shadowdim = 2048;
	if (shadowdim<512) Fatal("Shadow map dimensions too small %d\n",shadowdim);

	//  Do Shadow textures in MultiTexture 1
	glActiveTexture(GL_TEXTURE1);

	//  Allocate and bind shadow texture
	glGenTextures(1,&shadowtex);
	glBindTexture(GL_TEXTURE_2D,shadowtex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, shadowdim, shadowdim, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

	//  Map single depth value to RGBA (this is called intensity)
	glTexParameteri(GL_TEXTURE_2D,GL_DEPTH_TEXTURE_MODE,GL_INTENSITY);

	//  Set texture mapping to clamp and linear interpolation
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

	//  Set automatic texture generation mode to Eye Linear
	glTexGeni(GL_S,GL_TEXTURE_GEN_MODE,GL_EYE_LINEAR);
	glTexGeni(GL_T,GL_TEXTURE_GEN_MODE,GL_EYE_LINEAR);
	glTexGeni(GL_R,GL_TEXTURE_GEN_MODE,GL_EYE_LINEAR);
	glTexGeni(GL_Q,GL_TEXTURE_GEN_MODE,GL_EYE_LINEAR);

	// Switch back to default textures
	glActiveTexture(GL_TEXTURE0);

	// Attach shadow texture to frame buffer
	glGenFramebuffers(1,&framebuf);
	glBindFramebuffer(GL_FRAMEBUFFER,framebuf);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowtex, 0);
	//  Don't write or read to visible color buffer
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	//  Make sure this all worked
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) Fatal("Error setting up frame buffer\n");
	glBindFramebuffer(GL_FRAMEBUFFER,0);

	//  Check if something went wrong
	ErrCheck("InitMap");

	//  Create shadow map
	ShadowMap();
}

//
//  Read text file
//
static char* ReadText(const char *file)
{
	int   n;
	char* buffer;
	//  Open file
	FILE* f = fopen(file,"rt");
	if (!f) Fatal("Cannot open text file %s\n",file);
	//  Seek to end to determine size, then rewind
	fseek(f,0,SEEK_END);
	n = ftell(f);
	rewind(f);
	//  Allocate memory for the whole file
	buffer = (char*)malloc(n+1);
	if (!buffer) Fatal("Cannot allocate %d bytes for text file %s\n",n+1,file);
	//  Snarf the file
	if (fread(buffer,n,1,f)!=1) Fatal("Cannot read %d bytes for text file %s\n",n,file);
	buffer[n] = 0;
	//  Close and return
	fclose(f);
	return buffer;
}

//
//  Print Shader Log
//
static void PrintShaderLog(int obj,const char* file)
{
	int len=0;
	glGetShaderiv(obj,GL_INFO_LOG_LENGTH,&len);
	if (len>1)
	{
	int n=0;
	char* buffer = (char *)malloc(len);
	if (!buffer) Fatal("Cannot allocate %d bytes of text for shader log\n",len);
	glGetShaderInfoLog(obj,len,&n,buffer);
	fprintf(stderr,"%s:\n%s\n",file,buffer);
	free(buffer);
	}
	glGetShaderiv(obj,GL_COMPILE_STATUS,&len);
	if (!len) Fatal("Error compiling %s\n",file);
}

//
//  Print Program Log
//
void PrintProgramLog(int obj)
{
	int len=0;
	glGetProgramiv(obj,GL_INFO_LOG_LENGTH,&len);
	if (len>1)
	{
	int n=0;
	char* buffer = (char *)malloc(len);
	if (!buffer) Fatal("Cannot allocate %d bytes of text for program log\n",len);
	glGetProgramInfoLog(obj,len,&n,buffer);
	fprintf(stderr,"%s\n",buffer);
	}
	glGetProgramiv(obj,GL_LINK_STATUS,&len);
	if (!len) Fatal("Error linking program\n");
}

//
//  Create Shader
//
void CreateShader(int prog,const GLenum type,const char* file)
{
	//  Create the shader
	int shader = glCreateShader(type);
	//  Load source code from file
	char* source = ReadText(file);
	glShaderSource(shader,1,(const char**)&source,NULL);
	free(source);
	//  Compile the shader
	glCompileShader(shader);
	//  Check for errors
	PrintShaderLog(shader,file);
	//  Attach to shader program
	glAttachShader(prog,shader);
}

//
//  Create Shader Program
//
int CreateShaderProg(const char* VertFile,const char* FragFile)
{
	//  Create program
	int prog = glCreateProgram();
	//  Create and compile vertex shader
	if (VertFile) CreateShader(prog,GL_VERTEX_SHADER,VertFile);
	//  Create and compile fragment shader
	if (FragFile) CreateShader(prog,GL_FRAGMENT_SHADER,FragFile);
	//  Link program
	glLinkProgram(prog);
	//  Check for errors
	PrintProgramLog(prog);
	//  Return name
	return prog;
}

/*
 *  GLUT calls this routine when a key is pressed
 */
int key()
{
	Uint8* keys=SDL_GetKeyState(NULL);
	//int shift=SDL_GetModState()&KMOD_SHIFT;
	//  Exit on ESC
	if (keys[SDLK_ESCAPE])
		exit(0);
	//  Reset view angle
	else if (keys[SDLK_0])
		th = ph = 0;
	//  Toggle axes
	else if (keys[SDLK_x])
		shadow=1-shadow;
	else if (keys[SDLK_y]){
		ylight+=0.5;
		//InitMap();
	}
	else if (keys[SDLK_z])
		Lpos[2]+=0.5;
	//  Toggle lighting
	else if (keys[SDLK_l])
		light = 1-light;
	//  Move light
	else if (keys[SDLK_LEFTBRACKET])
		zh += 1;
	else if (keys[SDLK_RIGHTBRACKET])
		zh -= 1;
	else if (keys[SDLK_m])
		mode=(mode+1)%3;
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
		P_th+=3;
		FPN_Refresh();
	}
	else if(keys[SDLK_d])
	{
		P_th-=3;
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
		ph+=3;
		P_ph += 5;
		FPN_Refresh();
	}
	else if (keys[SDLK_DOWN])
	{
		ph-=3;
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
	sky[0]=LoadTexBMP("skybox2.bmp");
	flash[0]=LoadTexBMP("flash3.bmp");
	
	//load obj
	//obj=LoadOBJ("hand_light.obj");
	
	//enable Z-buffer
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glPolygonOffset(4,0);
	shader=CreateShaderProg("shadow.vert", "shadow.frag");
	InitMap();
	//ShadowMap();
	reshape(screen->w, screen->h);
	
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
		if(t>30.0&&density<0.1){
			showfog=1;
			density+=0.001;
		}
		if(t>40&&density==0.02)
			density+=0.001;
		if(t>50&&density==0.03)
			density+=0.001;
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
