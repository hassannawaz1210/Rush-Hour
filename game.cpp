//============================================================================
// Name        : Hassan nawaz.cpp
// Author      : FAST CS Department
// Version     : v5.0
// Copyright   : (c) Reserved
// Description : Basic 2D game of Rush Hour...
//============================================================================

#ifndef RushHour_CPP_
#define RushHour_CPP_
#include "util.h"
#include <iostream>
#include<string>
#include <ctime>
#include <cstdlib>
#include<cmath> // for basic math functions such as cos, sin, sqrt
using namespace std;


//---------------------- function prototypes
bool rightCollision(int, int);
bool leftCollision(int, int);
bool topCollision(int, int);
bool bottomCollision(int, int);
void moveUp();
void moveDown();
void moveRight();
void moveLeft();
void passengers(int [][2]);
bool vicinity(int, int);
void destination(int, int);
void trafficCars(int [][2]);

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::

//---------- walls coordinates
int coordinates[18][4] = { {100, 100, 30, 200},
						 {100, 100, 200, 30},
						 {920, 100, 30, 200},
						 {750, 100, 200, 30}, 
						 {100, 570, 30, 200},
						 {100, 740, 200, 30},
						 {920, 570, 30, 200}, 
						 {750, 740, 200, 30},
						 {250, 230, 30, 140},
						 {250, 230, 140, 30},
						 {740, 230, 30, 140},
						 {630, 230, 140, 30},
						 {250, 480, 30, 140},
						 {250, 590, 140, 30},
						 {740, 480, 30, 140},
						 {630, 590, 140, 30},
						 {490, 270, 30, 300},
						 {360, 400, 300, 30}
					   };


//---------- obstacles coordinates
int obstacles[12][2] = {{200, 690},
						{70,120},
						{70, 710},
						{450,460},
						{830,80},
						{800, 600},
						{320,290},
						{860,200},
						{900,420},
						{510,710},
						{670,110},
						{200,420}
						};


//:::::::::::::::::::::::::::::::::::::::::::::::::::::::


//-----------------cars coordinates
int totalScore=0, timeRemain=3;


//------ variables used in start menu
bool menuChanger = true, showMenu= true, taxiOption=true, yellowTaxi = false, redTaxi = false;


// our car/taxi's coordinates
int xI = 0, yI = 810;


// traffic cars coordinates
int traffic[100][2] = {0};

// destination coordinates
int xdestination, ydestination;
	bool destFlag = false;

//passengers random coordinates
	int xyCoords[4][2];
	string leaderboard[10];

//=====+++++++++++++++++----------------++++++++++++++++++++=============//

// seed the random numbers generator by current time (see the documentation of srand for further help)...

/* Function sets canvas size (drawing area) in pixels...
 *  that is what dimensions (x and y) your game will have
 *  Note that the bottom-left coordinate has value (0,0) and top-right coordinate has value (width-1,height-1)
 * */
void SetCanvasSize(int width, int height) {
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, width, 0, height, -1, 1); // set the screen size to given width and height.
	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity();
}

//-------------- this function generates coordinates for different arrays. 
void randomCoordinatesGenerator()
{
	const int MIN_VALUE = 30;
	const int MAX_VALUE = 820;
	unsigned seed = time(0);
	srand(seed);
	for(int i=0; i<4; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			xyCoords[i][j] = (rand() % (MAX_VALUE - MIN_VALUE + 1)) + MIN_VALUE;
		}
		cout << xyCoords[i][0] << '\t' << xyCoords[i][1] << '\n'; 
		
		if(vicinity( xyCoords[i][0] , xyCoords[i][1] ) == true)
		{
			i -= 1;
		}
		
	}
	for (int i = 0; i < 3; i++)
	{

		for (int j = 0; j < 2; j++)
		{
			traffic[i][j] = ( ((rand() % (MAX_VALUE - MIN_VALUE + 1)) + MIN_VALUE ) / 10)*10 ;
		}

		if(vicinity( traffic[i][0] , traffic[i][1] ) == true)
		{
			i -= 1;
		}
		
	}
	trafficCars(traffic);
	passengers(xyCoords);

}


//--------------- this is another random coordinates generator fucntion, RCG2 = random coordinates generator 2
void RCG2(int &x, int &y)
{
	bool flag = false;
	const int MIN_VALUE = 30;
	const int MAX_VALUE = 820;
	unsigned seed = time(0);
	srand(seed);

	do {

	x = (rand() % (MAX_VALUE - MIN_VALUE + 1)) + MIN_VALUE;
	y = (rand() % (MAX_VALUE - MIN_VALUE + 1)) + MIN_VALUE;
	for (int i = 0; i < 4; i++)
	{

	 if(x == xyCoords[i][0] && y == xyCoords[i][1]) flag = true;
	}
	

	} 
	while(vicinity(x, y) == true || flag == true);

}


// ---------------- this function keeps the passengers and destination from spawning on the top of walls and obstacles
bool vicinity(int x, int y)
{
	for(int i=0; i<18; i++){
	if ( x >= coordinates[i][0]-30/*leaving some more space*/ && x<= coordinates [i][0]+coordinates[i][2] + 30 && 
		 y >= coordinates[i][1]-30 && y <= (coordinates[i][1] + coordinates[i][3]) + 30 ) return true;
	}
	for(int i=0; i<12; i++)
	{
		if( x>= obstacles[i][0]-30 && x<= obstacles[i][0]+30 && y>= obstacles[i][1]-30 && y<= obstacles[i][1]+30) 
		return true;
	}
	return false;
}


//------------- this function draws passengers
void passengers(int xyCoords[][2])
{
	
	for (int i = 0; i < 4; i++)
	{
		DrawCircle(xyCoords[i][0], xyCoords[i][1], 10, colors[RED]);
		// cout << xyCoords[i][0] << '\t' << xyCoords[i][1] << '\n'; 
	}
	glutPostRedisplay();

	
}


//--------- this function draws traffic cars
void trafficCars(int traffic[][2])
{

for (int i = 0; traffic[i][0] != 0 || traffic[i][1] != 0 ; i++)
{
	DrawSquare(traffic[i][0], traffic[i][1], 30, colors[BLUE]);

}

  glutPostRedisplay();

}

//---------------- this function draws destionation point on the map
void destination(int x, int y)
{
	DrawCircle(x, y, 10, colors[GREEN]);
	glutPostRedisplay();
}


//---------- this is our car/taxi

void drawCar() {
	//DrawSquare(xI, yI, 30, colors[GREEN]);
	if(yellowTaxi == true)
	DrawRectangle(xI, yI+10, 30, 20, colors[YELLOW]);
	else if (redTaxi == true)
	DrawRectangle(xI, yI+10, 30, 20, colors[RED]);

	DrawCircle(xI+5, yI+5, 5, colors[BLACK]);
	DrawCircle(xI+25, yI+5, 5, colors[BLACK]);


	
	glutPostRedisplay();
}


//----------- this function keeps the traffic in motion
void trafficMotion(int &x, int &y, int temp2)
{
if(temp2 == 1 && leftCollision(x, y) == false &&  x != 10) 
	{
		if(yellowTaxi == true)
		x -= 10;
		else if (redTaxi == true)
		x -= 5;
	}
if(temp2 == 2 && bottomCollision(x, y) == false &&  y !=0)
	{
			if(yellowTaxi ==true)
		 y -= 10;
		 else if(redTaxi == true)
		 y -= 5;
	}
 if(temp2 == 3 &&rightCollision(x, y) == false  &&  x != 990)
	{
		if(yellowTaxi == true)
		x += 10;
		else if(redTaxi == true)
		x += 5;
	}

if(temp2 == 4 && topCollision(x, y) == false &&  y != 810)
	{
		if(yellowTaxi == true)
		y += 10;
		 else if(redTaxi == true)
		 y  += 5;
	}
	glutPostRedisplay();

}

/*
 * Main Canvas drawing function.
 * */

void GameDisplay()/**/{
	// set the background color using function glClearColor.
	// to change the background play with the red, green and blue values below.
	// Note that r, g and b values must be in the range [0,1] where 0 means dim rid and 1 means pure red and so on.

	glClearColor(1/*Red Component*/, 1,	//148.0/255/*Green Component*/,
			1.0/*Blue Component*/, 0 /*Alpha component*/); // Red==Green==Blue==1 --> White Colour
	glClear (GL_COLOR_BUFFER_BIT); //Update the colors


	//--------------------------------------------
	DrawLine( 40 , 9000 ,  40 , 0 ,1 , colors[BLACK] );	
	DrawLine( 80 , 9000 ,  80 , 0 ,1 , colors[BLACK] );
	DrawLine( 120 , 9000 ,  120 , 0 ,1 , colors[BLACK] );
	DrawLine( 160 , 9000 ,  160 , 0 ,1 , colors[BLACK] );
	DrawLine( 200 , 9000 ,  200 , 0 ,1 , colors[BLACK] );
	DrawLine( 240 , 9000 ,  240 , 0 ,1 , colors[BLACK] );
	DrawLine( 280 , 9000 ,  280 , 0 ,1 , colors[BLACK] );
	DrawLine( 320 , 9000 ,  320 , 0 ,1 , colors[BLACK] );
	DrawLine( 360 , 9000 ,  360 , 0 ,1 , colors[BLACK] );
	DrawLine( 400 , 9000 ,  400 , 0 ,1 , colors[BLACK] );
	DrawLine( 440 , 9000 ,  440 , 0 ,1 , colors[BLACK] );
	DrawLine( 480 , 9000 ,  480 , 0 ,1 , colors[BLACK] );
	DrawLine( 520 , 9000 ,  520 , 0 ,1 , colors[BLACK] );
	DrawLine( 560 , 9000 ,  560 , 0 ,1 , colors[BLACK] );
	DrawLine( 600 , 9000 ,  600 , 0 ,1 , colors[BLACK] );
	DrawLine( 640 , 9000 ,  640 , 0 ,1 , colors[BLACK] );
	DrawLine( 680 , 9000 ,  680 , 0 ,1 , colors[BLACK] );
	DrawLine( 720 , 9000 ,  720 , 0 ,1 , colors[BLACK] );
	DrawLine( 760 , 9000 ,  760 , 0 ,1 , colors[BLACK] );
	DrawLine( 800 , 9000 ,  800 , 0 ,1 , colors[BLACK] );
	DrawLine( 840 , 9000 ,  840 , 0 ,1 , colors[BLACK] );
	DrawLine( 880 , 9000 ,  880 , 0 ,1 , colors[BLACK] );
	DrawLine( 920 , 9000 ,  920 , 0 ,1 , colors[BLACK] );
	DrawLine( 960 , 9000 ,  960 , 0 ,1 , colors[BLACK] );
	DrawLine( 1000 , 9000 ,  1000 , 0 ,1 , colors[BLACK] );
 	//-----------------------------------------------------------
	 //------------- obstacles
	for(int i=0; i<12; i++)
	{
		DrawCircle(obstacles[i][0], obstacles[i][1], 20, colors[GRAY]);
		DrawCircle(obstacles[i][0], obstacles[i][1], 10, colors[WHITE]);
	}



	 //------------------------------ 
	
//------------- Drawing walls
	for (int i = 0; i < 18; i++)
	{
		DrawRectangle(coordinates[i][0],coordinates[i][1],coordinates[i][2],coordinates[i][3], colors[BLACK]);
	}
	

//------------- time
string timeString;
	timeString = to_string(timeRemain);
	DrawString( 370, 820, "Remaining time=", colors[BLACK]);
	DrawString( 560, 820, timeString, colors[BLACK]);
	DrawString( 575, 820, "mins", colors[BLACK]);

//----------- score
	string scoreString;
	scoreString = to_string(totalScore);
	DrawString( 900, 820, "Score=", colors[BLACK]);
	DrawString( 970, 820, scoreString, colors[BLACK]);

//------------- destination circle 
	if(destFlag == true)
	{
		destination(xdestination, ydestination);
	}

//------------- 

	passengers(xyCoords);
	trafficCars(traffic);
	drawCar();

//------------------ choosing taxi, taxi menu
	if(taxiOption == true)
	{
		DrawRectangle(0, 0, 1020, 840, colors[BLACK]);

		DrawString(330,560, "Which taxi would you like to choose?", colors[WHITE]); 
		DrawString(370,480, "Red", colors[RED]);
		DrawString(370,440, "Yellow", colors[YELLOW]);
		DrawString(370,400, "Random color", colors[WHITE]);

	}


if(showMenu == true) {

	if(menuChanger == true)
	{
		DrawRectangle(0,0,1020,840,colors[BLACK]); // background
	//  DrawRectangle(480, 480, 60, 30, colors[RED]);
	// 	DrawRectangle(440, 440, 150, 30, colors[RED]);
	// 	DrawRectangle(480, 400, 50, 30, colors[RED]); //blocks
		DrawString(433,560, "RUSH HOUR", colors[WHITE]); 
		DrawString(480,480, "Start", colors[WHITE]);
		DrawString(440,440, "Leaderboard", colors[WHITE]);
		DrawString(480,400, "Quit", colors[WHITE]);
	

	}

	else if (menuChanger == false)
	{
		DrawRectangle(0,0,1020,840,colors[BLACK]); // background
		DrawString(440,680, "Leaderboard:", colors[WHITE]);
		DrawString(460,200, "Go back", colors[WHITE]);
	}	

}

 if(timeRemain <= 0 && totalScore >= 100)
{
		DrawRectangle(0, 0, 1020, 840, colors[BLACK]);
		DrawString(460,530, "You won.", colors[WHITE]); 
		DrawString(480,480, "Quit", colors[WHITE]);
}

else if(timeRemain <= 0 && totalScore <= 100)
{
		DrawRectangle(0, 0, 1020, 840, colors[BLACK]);
		DrawString(460,530, "You lost.", colors[WHITE]); 
		DrawString(480,480, "Quit", colors[WHITE]);
}

	glutSwapBuffers(); // do not modify this line..

}



/*This function is called (automatically) whenever any non-printable key (such as up-arrow, down-arraw)
 * is pressed from the keyboard
 *
 * You will have to add the necessary code here when the arrow keys are pressed or any other key is pressed...
 *
 * This function has three argument variable key contains the ASCII of the key pressed, while x and y tells the
 * program coordinates of mouse pointer when key was pressed.
 *
 * */

//----------------- this function checks whether our car or traffic cars are colliding with themselves,walls and obstacles from left side
bool leftCollision(int x, int y)
{
	for (int i = 0; i < 18; i++)
	{
		if(x == coordinates[i][0]+coordinates[i][2] && (y>coordinates[i][1]-30 && y<coordinates[i][1] + coordinates[i][3]))
		return true;
	}
		for (int i = 0; traffic[i][0] !=  0 || traffic[i][1] !=  0; i++)
	{
		if(x == traffic[i][0]+30 && (y > traffic[i][1]-30 && y<traffic[i][1]+30))
		{
			if(x ==xI && y == yI && redTaxi == true) totalScore += -3;
			else if (x ==xI && y == yI && yellowTaxi == true) totalScore += -2;
			return true;
		}
	}
	for (int i = 0; i < 12; i++)
	{
		if(x == obstacles[i][0]+20 && (y > obstacles[i][1]-50 && y<obstacles[i][1]+20))
		{
		if(x ==xI && y == yI && redTaxi == true) totalScore += -2;
		else if(x ==xI && y == yI && yellowTaxi == true) totalScore += -4;
		return true;
		}
	}
	
		// //---------our car
	if(x == xI+30 && (y > yI-30 && y < yI+30))
	return true;

		
		
		// return true;
		// else 
		return false;
		
}

//----------------- this function checks whether our car or traffic cars are colliding with themselves,walls and obstacles from right side

bool rightCollision(int x, int y)
{
	for (int i = 0; i < 18; i++)
	{
		if(x == coordinates[i][0]-30 && (y>coordinates[i][1]-30 && y<coordinates[i][1]+coordinates[i][3]))
		return true;
	}
		for (int i = 0; traffic[i][0] !=  0 || traffic[i][1] !=  0; i++)
	{
		if(x == traffic[i][0]-30 && (y>traffic[i][1]-30 && y< traffic[i][1] +30))
		{
			if(x ==xI && y == yI && redTaxi == true) totalScore += -3;
			else if (x ==xI && y == yI && yellowTaxi == true) totalScore += -2;
			return true;
		}
	}
		for(int i=0; i<12; i++)
		{
			if (x == obstacles[i][0]-50 && (y>obstacles[i][1]-50 && y< obstacles[i][1]+20))
			{
				if(x ==xI && y == yI && redTaxi == true) totalScore += -2;
				else if(x ==xI && y == yI && yellowTaxi == true) totalScore += -4;
				return true;
			}
		}
		// //---------our car
		if (x == xI-30 && (y>yI-30 && y<yI+30))
		return true;

		// return true;
		// else 
		return false;
}

//----------------- this function checks whether our car or traffic cars are colliding with themselves,walls and obstacles from top side


bool topCollision(int x, int y)
{
	for (int i = 0; i < 18; i++)
	{
		if( y == coordinates[i][1]-30 && (x>coordinates[i][0]-30 && x<coordinates[i][0]+coordinates[i][2]))
		return true;
	}
	for (int i = 0; traffic[i][0] != 0 || traffic[i][1] != 0; i++)
	{
		if (y ==  traffic[i][1]-30 && (x > traffic[i][0]-30 && x < traffic[i][0]+30))
		{
			if(x ==xI && y == yI && redTaxi == true) totalScore += -3;
			else if (x ==xI && y == yI && yellowTaxi == true) totalScore += -2;
			return true;
		}
	}
	for (int i = 0; i < 12; i++)
	{
		if(y == obstacles[i][1]-50 && (x > obstacles [i][0]-50 && x < obstacles[i][0]+20))
		{
			if(x ==xI && y == yI && redTaxi == true) totalScore += -2;
			else if(x ==xI && y == yI && yellowTaxi == true) totalScore += -4;
			return true;
		}
	}
	


		// //----------our car
		if(y == yI-30 && (x > xI-30 && x<xI+30))

		 return true;
		// else 
		return false;
}
//----------------- this function checks whether our car or traffic cars are colliding with themselves,walls and obstacles from bottom side


bool bottomCollision(int x, int y)
{
	for (int i = 0; i < 18; i++)
	{
		if( y == coordinates[i][1] + coordinates[i][3] && (x > coordinates[i][0]-30 && x < coordinates[i][0] + coordinates [i][2]) )
		return true;
	}
		for (int i = 0; traffic[i][0] !=  0 || traffic[i][1] !=  0; i++)
	{
		if (y == traffic[i][1]+30 && (x>traffic[i][0]-30 && x<traffic[i][0]+30))
		{
			if(x ==xI && y == yI && redTaxi == true) totalScore += -3;
			else if (x ==xI && y == yI && yellowTaxi == true) totalScore += -2;
			return true;
		}
	}	
	for (int i = 0; i < 12; i++)
	{
		if (y == obstacles[i][1]+20 && (x>obstacles[i][0]-50 && x<obstacles[i][0]+20))
		{
		if(x ==xI && y == yI && redTaxi == true) totalScore += -2;
		else if(x ==xI && y == yI && yellowTaxi == true) totalScore += -4;
		return true;
		}
	}
	

// 		//----------our car
		if(y == yI+30 && (x>xI-30 && x<xI+30))

 		return true;
		// else 
		return false;
}

void NonPrintableKeys(int key, int x, int y) {
	if (key
			== GLUT_KEY_LEFT /*GLUT_KEY_LEFT is constant and contains ASCII for left arrow key*/) {
		// what to do when left key is pressed...
		if(leftCollision(xI,yI) == true || xI == 0) {}
		else if(yellowTaxi == true)
		xI -= 10;
		else if(redTaxi == true)
		xI -= 5;


	} else if (key
			== GLUT_KEY_RIGHT /*GLUT_KEY_RIGHT is constant and contains ASCII for right arrow key*/) {


		if(rightCollision(xI, yI) == true || xI == 990) {}
		else if(yellowTaxi == true)
		xI += 10;
		else if(redTaxi == true)
		xI += 5;
		

	} else if (key
			== GLUT_KEY_UP/*GLUT_KEY_UP is constant and contains ASCII for up arrow key*/) {


		

		if(topCollision(xI, yI) == true || yI == 810) {}
		else if(yellowTaxi == true)
		yI += 10;
		else if(redTaxi == true)
		yI += 5;


	} else if (key
			== GLUT_KEY_DOWN/*GLUT_KEY_DOWN is constant and contains ASCII for down arrow key*/) 
	{

	
		if(bottomCollision(xI, yI) == true ||  yI == 0) {}
		else if(yellowTaxi == true)
		yI -= 10;
		else if(redTaxi == true)
		yI -= 5;

	}

	//--------------------- SIDEWAYSSSSSSSSSSSSSSSSS


	/* This function calls the Display function to redo the drawing. Whenever you need to redraw just call
	 * this function*/

	glutPostRedisplay();


}
bool flag = false;
/*This function is called (automatically) whenever any printable key (such as x,b, enter, etc.)
 * is pressed from the keyboard
 * This function has three argument variable key contains the ASCII of the key pressed, while x and y tells the
 * program coordinates of mouse pointer when key was pressed.
 * */
void PrintableKeys(unsigned char key, int x, int y) {
	if (key == 27/* Escape key ASCII*/) {
		exit(1); // exit the program when escape key is pressed.
		cout << "Closing game...." << endl;
	}

	if (key == ' ') 
	{
		int x, y;
		static int var=0, appendingTraffic=0, count=0;

		
			for (int i=0; i<4; i++)
			{		//-20 compensates for the bringing the coordinate of circle to bottom left corner 
				if(xI >= (xyCoords[i][0]-30) && xI<= (xyCoords[i][0]+10) && yI >= (xyCoords[i][1]-30) && yI <= (xyCoords[i][1]+10) && flag == false)
					{
						cout << "Passenger has been picked.\n";
						flag = true;
						xyCoords[i][0] = 10000;
						xyCoords[i][1] = 10000;
						var = i;
						destFlag = true;
						RCG2(xdestination, ydestination);//getting random coordinates for destination
					}
			
			
				else if (xI >= (xyCoords[i][0]-20) && xI<= (xyCoords[i][0]+10) && yI >= (xyCoords[i][1]-20) && yI <= (xyCoords[i][1]+10) && flag == true) 
				{
					cout << "A player has already been picked\n";
				}
			}
			
			if(xI >= xdestination-20 && xI <= xdestination+10 && yI >= ydestination-20 && yI <= ydestination+10 && flag == true)
			{
				
				cout << "You have dropped the passenger at his destination\n";
				flag = false;
				destFlag = false;
				totalScore += 10;
				RCG2( x, y );
				cout << x << '\t' << y  << endl;
				xyCoords[var][0] = x;
				xyCoords[var][1] = y;
				count++;
				if(count == 2)
				{
				appendingTraffic++;
				count=0;

				do{
				x =(((rand() % (990 - 0 + 1)) + 0)/10)*10;
				y =(((rand() % (990 - 0 + 1)) + 0)/10)*10;
				} 
				while(vicinity(x,y) == true);

				
				traffic[2+appendingTraffic][0] = x;
				traffic[2+appendingTraffic][1] = y;
				}
			}
			else if(flag == false) {}
			else cout << "you can only drop the passenger at his destination\n";

		
	}
	glutPostRedisplay();
}



/*
 * This function is called after every 1000.0/FPS milliseconds
 * (FPS is defined on in the beginning).
 * You can use this function to animate objects and control the
 * speed of different moving objects by varying the constant FPS.
 *
 * */
void Timer(int m) {
	int temp2;
	const int MIN_VALUE = 1;
	const int MAX_VALUE = 4;
	unsigned seed = time(0);
	srand(seed);


	
	int xcoord, ycoord;
	for(int i=0; traffic[i][0] != 0 || traffic [i][1] != 0; i++)
	{
		temp2= (rand() % (MAX_VALUE - MIN_VALUE + 1)) + MIN_VALUE;
		trafficMotion(traffic[i][0], traffic[i][1], temp2);
	}

	// once again we tell the library to call our Timer function after next 1000/FPS
	glutTimerFunc(100, Timer, 0);
}

/*This function is called (automatically) whenever your mouse moves witin inside the game window
 *
 * You will have to add the necessary code here for finding the direction of shooting
 *
 * This function has two arguments: x & y that tells the coordinate of current position of move mouse
 *
 * */
void MousePressedAndMoved(int x, int y) {
	glutPostRedisplay();
}
void MouseMoved(int x, int y) {
	glutPostRedisplay();
}

/*This function is called (automatically) whenever your mouse button is clicked witin inside the game window
 *
 * You will have to add the necessary code here for shooting, etc.
 *
 * This function has four arguments: button (Left, Middle or Right), state (button is pressed or released),
 * x & y that tells the coordinate of current position of move mouse
 *
 * */
void MouseClicked(int button, int state, int x, int y) {

	if (button == GLUT_LEFT_BUTTON) // dealing only with left button
			{
		cout << GLUT_DOWN << " " << GLUT_UP << endl;
		
	//----------------------- start menu
		if(x>=480 && x<=480+60 && y>=330 && y<=330+30 && showMenu == true) 
		{
			showMenu = false;
		}
		else if (x>=440 && x<=440+150 && y>=370 && y<=370+30 && showMenu == true)
		{
			menuChanger = false;
		}
		else if (x>=480 && x<=480+50 && y>=410 && y<=410+30 && showMenu == true)
		{
			exit(1);
		}

		else if(x>=470 && x<= 530 && y>= 620 && y<= 640 && showMenu == true && menuChanger == false)
		{
			menuChanger = true;
		}


		//---------- taxi menu
		if(showMenu == false && taxiOption == true && 
			x>= 360 && x<= 400 && y>= 330 && y<=370)
		{
		taxiOption = false;
		redTaxi = true;
		}
		else if(showMenu == false && taxiOption == true && 
			x>= 360 && x<= 430 && y>= 385 && y<=410 )
		{
		taxiOption = false;
		yellowTaxi = true;
		}

		else if(showMenu == false && taxiOption == true && 
			x>= 360 && x<= 510 && y>= 425 && y<=460 )
		{
			int randomColor;
			const int MIN_VALUE = 0;
			const int MAX_VALUE = 1;
			unsigned seed = time(0);
			srand(seed);
			randomColor = (rand() % (MAX_VALUE - MIN_VALUE + 1)) + MIN_VALUE;
			if (randomColor == 0)
				yellowTaxi = true;
			else
				redTaxi = true;

			taxiOption = false;

		}

		

		if(showMenu == false && taxiOption == false && timeRemain <= 0)
		{
			if(x>=480 && x<=480+40 && y>=340 && y<=330+40) 
		{
			exit(1);
		}
		}

		
//-------------- RIGHT CLICKKKKKKK
	} else if (button == GLUT_RIGHT_BUTTON) // dealing with right button
			{
			cout<<"Right Button Pressed"<<endl;
		
	}
	glutPostRedisplay();
}


//------------ this function keeps the timer running
void timeUpdate(int m)
{
	if(yellowTaxi == true || redTaxi == true)
	timeRemain -= 1;
	glutTimerFunc(60000.0, timeUpdate, 0);

}

int main(int argc, char*argv[]) {

	int width = 1020, height = 840; // i have set my window size to be 800 x 600

	InitRandomizer(); // seed the random number generator...
	glutInit(&argc, argv); // initialize the graphics library...
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // we will be using color display mode
	glutInitWindowPosition(50, 50); // set the initial position of our window
	glutInitWindowSize(width, height); // set the size of our window
	glutCreateWindow("Rush Hour by Hassan Nawaz"); // set the title of our game window
	SetCanvasSize(width, height); // set the number of pixels...

	// Register your functions to the library,
	// you are telling the library names of function to call for different tasks.
	//glutDisplayFunc(display); // tell library which function to call for drawing Canvas.

	glutDisplayFunc(GameDisplay); // tell library which function to call for drawing Canvas.
	glutSpecialFunc(NonPrintableKeys); // tell library which function to call for non-printable ASCII characters
	glutKeyboardFunc(PrintableKeys); // tell library which function to call for printable ASCII characters
	// This function tells the library to call our Timer function after 1000.0/FPS milliseconds...
	glutTimerFunc(1000.0, Timer, 0);
	glutTimerFunc(60000.0,  timeUpdate, 0);


	glutMouseFunc(MouseClicked);
	glutPassiveMotionFunc(MouseMoved); // Mouse
	glutMotionFunc(MousePressedAndMoved); // Mouse
	randomCoordinatesGenerator();

	// now handle the control to library and it will call our registered functions when
	// it deems necessary...
	glutMainLoop();
	return 1;
}
#endif /* RushHour_CPP_ */
