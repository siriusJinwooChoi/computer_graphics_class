#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define LEFT_BOUNDARY -0.62f
#define RIGHT_BOUNDARY 1.24f
#define TOP_BOUNDARY 0.44f
#define BOTTOM_BOUNDARY -1.42f
#define BAR_BASE_GAMMA_POSITION -1.12
#define BAR_BASE_DELTA_POSITION 0.0

void Bar();
void Ball();
void MyDisplay();
void MyTimer(int Value);
void MyKeyboard(unsigned char KeyPressed, int X, int Y);
void SpecialInput(int key, int x, int y);
void initView(int width, int height);
void ScoreTimer(int Value);

int width, height;
int score = 0;

GLfloat Delta = 0.0, Gamma = 0.0, xVector = 0.0, yVector = 0.0, xflag = 0.0, yflag = 0.0;

void Bar() {							//Draw Pingpong Bar
	glBegin(GL_POLYGON);
		glColor3f(0.4, 0.3, 0.2);
		glVertex3f(-0.2 + Delta, -0.73 + Gamma, 0.0);
		glColor3f(0.1, 0.2, 0.7);
		glVertex3f(0.2 + Delta, -0.73 + Gamma, 0.0);
		glColor3f(0.2, 0.8, 0.3);
		glVertex3f(0.2 + Delta, -0.68 + Gamma, 0.0);
		glColor3f(0.4, 0.3, 0.2);
		glVertex3f(-0.2 + Delta, -0.68 + Gamma, 0.0);
	glEnd();
	glFlush();
}

void Ball() {							//Draw Pingpong Ball, If Key Pressed 'S' -> Ball will be moving Start.
	int i;
	int max_angle = 360;
	float radius = 0.04;
	float radian = 3.141592 / 180;		// radian = pi(==180), pi / 180 = 1 degree
	
	glColor3f(0.1, 0.1, 1.0);	
	glBegin(GL_POLYGON);
	for (i = 0; i<max_angle; i++) {
		float angle1 = i*radian;
		glColor3f(0.1, 0.1 + (i * 2), 1.0 + (i * 8));
		glVertex3f(-0.3 + cos(angle1)*radius + xVector, 0.5 + sin(angle1)*radius + yVector, 0.0);
	}
	glEnd();
	glFlush();
}

void ScoreTimer(int Value) {			//Score Timer(1000ms = 1sec) -> score + 100
	glutPostRedisplay();
	score += 100;
	glutTimerFunc(1000, ScoreTimer, 1);
}

void MyDisplay() {
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glPushMatrix();				//About Pingpong Bar!
		Bar();
	glPopMatrix();

	glPushMatrix();				//About Pingpong Ball!
		Ball();
	glPopMatrix();

	glutSwapBuffers();
}

void MyTimer(int Value) {
	if (Value == 1) {							//Firstcase, Goto Right-Bottom
		xflag = xVector;
		yflag = yVector;

		xVector += 0.02;
		yVector -= 0.02;

		if ((yVector > ((BAR_BASE_GAMMA_POSITION - 0.02) + Gamma)) && (yVector < (BAR_BASE_GAMMA_POSITION + Gamma))) {		//If Ball is located (y-axis of Bar),
			if ((xVector >(BAR_BASE_DELTA_POSITION + Delta)) && (xVector < ((BAR_BASE_DELTA_POSITION + 0.6) + Delta))) {		//and Ball is located (x-axis of Bar),
				if (xflag > xVector) {
					xflag = xVector;
					xVector -= 0.02;
					glutPostRedisplay();
					glutTimerFunc(40, MyTimer, 2);			
				}
				else if (xflag < xVector) {
					xflag = xVector;
					xVector += 0.02;
					glutPostRedisplay();
					glutTimerFunc(40, MyTimer, 2);
				}
			}
			else {
				yflag = yVector;
				yVector -= 0.02;
				glutPostRedisplay();
				glutTimerFunc(40, MyTimer, 1);
			}
		}

		else if (yVector < BOTTOM_BOUNDARY) {					//If Ball is located Bottom Boundry, Game will be finish
			printf("Ping pong Game is Over!\nYour Score is %d\n", score);
			glutPostRedisplay();
		}
		else if (yVector > TOP_BOUNDARY) {						//If Ball is located Top Boundry, Goto Bottom
			glutPostRedisplay();
			score += 100;
			glutTimerFunc(40, MyTimer, 3);
		}
		else if (xVector < LEFT_BOUNDARY) {					//If Ball is located Left Boundry, Goto Right
			glutPostRedisplay();
			score += 100;
			glutTimerFunc(40, MyTimer, 4);
		}
		else if (xVector > RIGHT_BOUNDARY) {						//If Ball is located Right Boundry, Goto Left
			glutPostRedisplay();
			score += 100;
			glutTimerFunc(40, MyTimer, 5);
		}
		else {
			glutPostRedisplay();
			glutTimerFunc(40, MyTimer, 1);
		}
	}

	else if (Value == 2) {								//About Top
		yflag = yVector;
		yVector += 0.02;
		
		if ((yVector > ((BAR_BASE_GAMMA_POSITION - 0.02) + Gamma)) && (yVector < (BAR_BASE_GAMMA_POSITION + Gamma))) {
			if ((xVector >(BAR_BASE_DELTA_POSITION + Delta)) && (xVector < ((BAR_BASE_DELTA_POSITION + 0.6) + Delta))) {
				if (xflag > xVector) {
					xflag = xVector;
					xVector -= 0.02;
					glutPostRedisplay();
					glutTimerFunc(40, MyTimer, 2);
				}
				else if (xflag < xVector) {
					xflag = xVector;
					xVector += 0.02;
					glutPostRedisplay();
					glutTimerFunc(40, MyTimer, 2);
				}
			}
			else {
				glutPostRedisplay();
				glutTimerFunc(40, MyTimer, 2);
			}
		}

		else if (yVector < BOTTOM_BOUNDARY) {
			printf("Ping pong Game is Over!\nYour Score is %d\n", score); //If Bottom-Boundry, Game Finish
			glutPostRedisplay();
		}
		else if (yVector > TOP_BOUNDARY) {
			glutPostRedisplay();
			score += 100;
			glutTimerFunc(40, MyTimer, 3);		//If Top is blocked, Goto Bottom
		}
		else if (xVector < LEFT_BOUNDARY) {
			glutPostRedisplay();
			score += 100;
			glutTimerFunc(40, MyTimer, 4);		//If Left is blocked, Goto Right
		}
		else if(xVector > RIGHT_BOUNDARY) {
			glutPostRedisplay();
			score += 100;
			glutTimerFunc(40, MyTimer, 5);		//If Right is blocked, Goto Left
		}

		else if (xflag > xVector) {				//Goto Left-Top
			xflag = xVector;
			xVector -= 0.02;
			glutPostRedisplay();
			glutTimerFunc(40, MyTimer, 2);
		}
		else if(xflag < xVector) {				//Goto Right-Top
			xflag = xVector;
			xVector += 0.02;
			glutPostRedisplay();
			glutTimerFunc(40, MyTimer, 2);
		}
	}

	else if (Value == 3) {						//About Bottom
		yflag = yVector;
		yVector -= 0.02;

		if ((yVector > ((BAR_BASE_GAMMA_POSITION - 0.02) + Gamma)) && (yVector < (BAR_BASE_GAMMA_POSITION + Gamma))) {
			if ((xVector >(BAR_BASE_DELTA_POSITION + Delta)) && (xVector < ((BAR_BASE_DELTA_POSITION + 0.6) + Delta))) {
				if (xflag > xVector) {
					xflag = xVector;
					xVector -= 0.02;
					glutPostRedisplay();
					glutTimerFunc(40, MyTimer, 2);
				}
				else if (xflag < xVector) {
					xflag = xVector;
					xVector += 0.02;
					glutPostRedisplay();
					glutTimerFunc(40, MyTimer, 2);
				}
			}
			else {
				yflag = yVector;
				yVector -= 0.02;
				glutPostRedisplay();
				glutTimerFunc(40, MyTimer, 3);
			}
		}

		else if (yVector < BOTTOM_BOUNDARY) {
			printf("Ping pong Game is Over!\nYour Score is %d\n", score);
			glutPostRedisplay();
		}
		else if (yVector > TOP_BOUNDARY) {
			score += 100;
			glutPostRedisplay();
			glutTimerFunc(40, MyTimer, 3);
		}
		else if (xVector < LEFT_BOUNDARY) {
			score += 100;
			glutPostRedisplay();
			glutTimerFunc(40, MyTimer, 4);		
		}
		else if (xVector > RIGHT_BOUNDARY) {
			score += 100;
			glutPostRedisplay();
			glutTimerFunc(40, MyTimer, 5);
		}
		
		else if (xflag < xVector) {				//Goto Right-Bottom
			xflag = xVector;
			xVector += 0.02;
			glutPostRedisplay();
			glutTimerFunc(40, MyTimer, 3);
		}
		else if (xflag > xVector) {				//Goto Left-Bottom
			xflag = xVector;
			xVector -= 0.02;
			glutPostRedisplay();
			glutTimerFunc(40, MyTimer, 3);
		}
	}

	else if (Value == 4) {						//About Right
		xflag = xVector;
		xVector += 0.02;

		if ((yVector > ((BAR_BASE_GAMMA_POSITION - 0.02) + Gamma)) && (yVector < (BAR_BASE_GAMMA_POSITION + Gamma))) {
			if ((xVector >(BAR_BASE_DELTA_POSITION + Delta)) && (xVector < ((BAR_BASE_DELTA_POSITION + 0.6) + Delta))) {
				if (xflag > xVector) {
					xflag = xVector;
					xVector -= 0.02;
					glutPostRedisplay();
					glutTimerFunc(40, MyTimer, 2);
				}
				else if (xflag < xVector) {
					xflag = xVector;
					xVector += 0.02;
					glutPostRedisplay();
					glutTimerFunc(40, MyTimer, 2);
				}
			}
			else {
				yflag = yVector;
				yVector -= 0.02;
				glutPostRedisplay();
				glutTimerFunc(40, MyTimer, 4);
			}
		}

		else if (yVector < BOTTOM_BOUNDARY) {
			printf("Ping pong Game is Over!\nYour Score is %d\n", score);
			glutPostRedisplay();
		}
		else if (yVector > TOP_BOUNDARY) {
			score += 100;
			glutPostRedisplay();
			glutTimerFunc(40, MyTimer, 3);
		}
		else if (xVector < LEFT_BOUNDARY) {
			score += 100;
			glutPostRedisplay();
			glutTimerFunc(40, MyTimer, 4);
		}
		else if (xVector > RIGHT_BOUNDARY) {
			score += 100;
			glutPostRedisplay();
			glutTimerFunc(40, MyTimer, 5);
		}

		else if (yflag > yVector) {				//Goto Right-Bottom
			yflag = yVector;
			yVector -= 0.02;
			glutPostRedisplay();
			glutTimerFunc(40, MyTimer, 4);
		}
		else if (yflag < yVector) {				//Goto Right-Top
			yflag = yVector;
			yVector += 0.02;
			glutPostRedisplay();
			glutTimerFunc(40, MyTimer, 4);
		}
	}

	else if (Value == 5) {						//About Left
		xflag = xVector;
		xVector -= 0.02;

		if ((yVector > ((BAR_BASE_GAMMA_POSITION - 0.02) + Gamma)) && (yVector < (BAR_BASE_GAMMA_POSITION + Gamma))) {
			if ((xVector >(BAR_BASE_DELTA_POSITION + Delta)) && (xVector < ((BAR_BASE_DELTA_POSITION + 0.6) + Delta))) {
				if (xflag > xVector) {
					xflag = xVector;
					xVector -= 0.02;
					glutPostRedisplay();
					glutTimerFunc(40, MyTimer, 2);
				}
				else if (xflag < xVector) {
					xflag = xVector;
					xVector += 0.02;
					glutPostRedisplay();
					glutTimerFunc(40, MyTimer, 2);
				}
			}
			else {
				yflag = yVector;
				yVector -= 0.02;
				glutPostRedisplay();
				glutTimerFunc(40, MyTimer, 5);
			}
		}
		else if (yVector < BOTTOM_BOUNDARY) {
			printf("Ping pong Game is Over!\nYour Score is %d\n", score);
			glutPostRedisplay();
		}
		else if (yVector > TOP_BOUNDARY) {
			score += 100;
			glutPostRedisplay();
			glutTimerFunc(40, MyTimer, 3);
		}
		else if (xVector < LEFT_BOUNDARY) {
			score += 100;
			glutPostRedisplay();
			glutTimerFunc(40, MyTimer, 4);
		}
		else if (xVector > RIGHT_BOUNDARY) {
			score += 100;
			glutPostRedisplay();
			glutTimerFunc(40, MyTimer, 5);
		}
		else if (yflag > yVector) {				//Goto Left-Bottom
			yflag = yVector;
			yVector -= 0.02;
			glutPostRedisplay();
			glutTimerFunc(40, MyTimer, 5);
		}
		else if(yflag < yVector) {				//Goto Left-Top
			yflag = yVector;
			yVector += 0.02;
			glutPostRedisplay();
			glutTimerFunc(40, MyTimer, 5);
		}
	}
	//printf("Value = %d, xflag = %lf, yflag = %lf, xVector = %lf, yVector = %lf\n", Value, xflag, yflag, xVector, yVector);	//Check Current Value.
}

void MyKeyboard(unsigned char KeyPressed, int X, int Y) {		//General Key Function
	switch (KeyPressed) {
		//Program Start! ('S', 's', 'Space Key')
		case 'S':
			glutTimerFunc(40, MyTimer, 1);
			//glutTimerFunc(40, ScoreTimer, 1);			//If Start Game, Checking the Score.
			break;
		case 's':
			glutTimerFunc(40, MyTimer, 1);
			//glutTimerFunc(40, ScoreTimer, 1);
			break;
		case 32:				//Space Key
			glutTimerFunc(40, MyTimer, 1);
			//glutTimerFunc(40, ScoreTimer, 1);
			break;

		//Program Restart! ('R', 'r')
		case 'R':
			score = 0;
			initView(500, 500);
			break;
		case 'r':
			score = 0;
			initView(500, 500);
			break;

		//Quit Program('Q', 'q', 'ESC Key')
		case 'Q':
			exit(0); break;
		case 'q':
			exit(0); break;
		case 27:                //'esc' Value of ASCII Code
			exit(0); break;
		}
}

void SpecialInput(int key, int x, int y) {						//Special Key Function
	switch (key) {
	case GLUT_KEY_LEFT:		//Left Key
		Delta = Delta - 0.04;
		glutPostRedisplay();
		break;
	case GLUT_KEY_RIGHT:	//Right Key
		Delta = Delta + 0.04;
		glutPostRedisplay();
		break;
	case GLUT_KEY_UP:		//Up Key
		Gamma = Gamma + 0.04;
		glutPostRedisplay();
		break;
	case GLUT_KEY_DOWN:		//Down Key
		Gamma = Gamma - 0.04;
		glutPostRedisplay();
		break;
	}
	glutPostRedisplay();
}

void initView(int width, int height) {
	Delta = 0, Gamma = 0, xVector = 0, yVector = 0, score = 0;
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(width, height);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Homework_1 PingPong Game");

	glClearColor(1.0, 1.0, 1.0, 0.0);
	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();
	glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
	glutDisplayFunc(MyDisplay);

	glutKeyboardFunc(MyKeyboard);
	glutSpecialFunc(SpecialInput);
	glutMainLoop();
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	width = 500, height = 500;
	initView(width, height);

	return 0;
}