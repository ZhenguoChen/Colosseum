#include"CSCIx229.h"
const double Speed=0.1;	//the speed person moves
const double RotateSpeed=5;
int P_th=45;		//the angle of person rotate
int P_ph=0;		//
double P_x=5;		//the position of person
const double P_y=0.35;	//suppose the height of human
double P_z=5;
double P_Dir_x=0;	//direction vector point at where you look at
double P_Dir_y=0;
double P_Dir_z=0;
/*
 * Set eye position when navigation
 * Move Forward
 */
void FPN_Forward()
{
	//the move direction of person
	double move_x=-Sin(P_th)*Cos(P_ph);
	double move_y=Sin(P_ph);
	double move_z=-Cos(P_th)*Cos(P_ph);

	//person next position
	P_x=P_x+Speed*move_x;
	P_z=P_z+Speed*move_z;
	
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
 * Move Backward
 */
void FPN_Backward()
{
	//the move direction of person
	double move_x=-Sin(P_th)*Cos(P_ph);
	double move_y=Sin(P_ph);
	double move_z=-Cos(P_th)*Cos(P_ph);

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
