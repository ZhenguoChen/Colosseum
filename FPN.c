#include"CSCIx229.h"
const double Speed=0.1;	//the speed person moves
const double RotateSpeed=5;
int P_th=45;		//the angle of person rotate
int P_ph=0;		//
double P_x=6;		//the position of person
const double P_y=0.4;	//suppose the height of human
double P_z=6;
double P_Dir_x=0;	//direction vector point at where you look at
double P_Dir_y=0.35;
double P_Dir_z=0;
int collision=0;	//test if hit the pillar

#define Atan(th) 180*atan(th)/3.1415926

/*
 * hit the wall return 1
 * did not hit the wall return 0
 */
int CollisionDetection(double x, double y, double z)
{
	//printf("x:%f, z:%f\n", x, z);
	double distance=sqrt(x*x+z*z);
	if((distance<3.9&&distance>2.5)||(distance<2.2&&distance>1.2))
		if(abs((int)(Atan(x/z)))%20<=5||abs((int)(Atan(x/z)))%20>=15){
			//printf("%d\n", abs(Atan(x/z)));
			//printf("%d\n", abs((int)(Atan(x/z))%20));
			//printf("hit the wall\n");
			collision=1;
			return 1;
		}
	//printf("did not hit the wall\n");
	//printf("%d\n", abs(Atan(x/z)));
	//printf("%d\n", abs((int)(Atan(x/z)))%20);
	collision=0;
	return 0;
}

/*
 * Set eye position when navigation
 * Move Forward
 * return 1 when hit the wall
 * return 0 when did not hit the wall
 */
int FPN_Forward()
{
	//the move direction of person
	double move_x=-Sin(P_th)*Cos(P_ph);
	double move_z=-Cos(P_th)*Cos(P_ph);

	//collision detected
	if(CollisionDetection(P_x+Speed*move_x, P_y, P_z+Speed*move_z)==1)
		return 1;
	//person next position
	P_x=P_x+Speed*move_x;
	P_z=P_z+Speed*move_z;
		
	//look at this direction
	P_Dir_x=P_x+move_x;
	//P_Dir_y=P_y+move_y;
	P_Dir_z=P_z+move_z;

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//set look at
	gluLookAt(P_x, P_y, P_z, P_Dir_x, P_Dir_y, P_Dir_z, 0, Cos(P_ph), 0);
	return 0;
}

/*
 * Set eye position when navigation
 * Move Backward
 */
int FPN_Backward()
{
	//the move direction of person
	double move_x=-Sin(P_th)*Cos(P_ph);
	double move_y=Sin(P_ph);
	double move_z=-Cos(P_th)*Cos(P_ph);

	//collision detected
	if(CollisionDetection(P_x-Speed*move_x, P_y, P_z-Speed*move_z)==1)
		return 1;

	//person next position
	P_x=P_x-Speed*move_x;
	P_z=P_z-Speed*move_z;
	
	//look at this direction
	P_Dir_x=P_x+move_x;
	P_Dir_y=P_y+move_y;
	P_Dir_z=P_z+move_z;

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//set look at
	gluLookAt(P_x, P_y, P_z, P_Dir_x, P_Dir_y, P_Dir_z, 0, Cos(P_ph), 0);
	return 0;
}

/*
 * Refresh when change direction
 */
void FPN_Refresh()
{
	//the move direction of person
	double move_x=-Sin(P_th)*Cos(P_ph);
	double move_y=Sin(P_ph);
	double move_z=-Cos(P_th)*Cos(P_ph);
	
	//look at this direction
	P_Dir_x=P_x+move_x;
	P_Dir_y=P_y+move_y;
	P_Dir_z=P_z+move_z;

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//set look at
	gluLookAt(P_x, P_y, P_z, P_Dir_x, P_Dir_y, P_Dir_z, 0, Cos(P_ph), 0);
	//printf("position of person: %f,%f,%f\n", P_x, P_y, P_z);
}

/*
 * Set eye position when navigation
 * Turn left
 */
void FPN_Left()
{
	//the move direction of person
	double move_x=-Sin(P_th)*Cos(P_ph);
	double move_y=Sin(P_ph);
	double move_z=-Cos(P_th)*Cos(P_ph);
	
	//look at this direction
	P_Dir_x=P_x+move_x;
	P_Dir_y=P_y+move_y;
	P_Dir_z=P_z+move_z;

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//set look at
	gluLookAt(P_x, P_y, P_z, P_Dir_x, P_Dir_y, P_Dir_z, 0, Cos(P_ph), 0);
}

/*
 * Set eye position when navigation
 * Turn Right
 */
void FPN_Right()
{
	//the move direction of person
	double move_x=-Sin(P_th)*Cos(P_ph);
	double move_y=Sin(P_ph);
	double move_z=-Cos(P_th)*Cos(P_ph);
	
	//look at this direction
	P_Dir_x=P_x+move_x;
	P_Dir_y=P_y+move_y;
	P_Dir_z=P_z+move_z;

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//set look at
	gluLookAt(P_x, P_y, P_z, P_Dir_x, P_Dir_y, P_Dir_z, 0, Cos(P_ph), 0);
}

void InitFPN()
{
	//manipulate the projection matrix
	glMatrixMode(GL_PROJECTION);
	//undo previous transformations
	glLoadIdentity();
	//perspective transformation
	
	gluPerspective(20, 1, 6.0/16, 16*6.0);
	
	glMatrixMode(GL_MODELVIEW);
	//undo previous transformation
	glLoadIdentity();

	gluLookAt(P_x, P_y, P_z, P_Dir_x, P_Dir_y, P_Dir_z, 0, Cos(P_ph), 0);
}
