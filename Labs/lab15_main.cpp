#define _USE_MATH_DEFINES
#define _CRT_SECURE_NO_WARNINGS

#include <gl/glut.h>
#include <stdio.h>
#include <windows.h> // idle 함수 처리를 위한 헤더
#include <cmath>
#include <iostream>
#include "bmpfuncs.h"

using namespace std;

void init();
void setting_light();
void draw();
void draw_string(void*, const char*, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat);
void draw_axis();
void MakeCheckerBoard(GLint);
void MakeSincSurface(GLint);

void mouse(int, int, int, int);
void motion(int, int);
void keyboard(unsigned char, int, int);
void specialKeyboard(int, int, int);
void idle();
void resize(int, int);
void getEyePosition(double, double);
double getRadian(double);

void main_menu(int);
void print_manual();

void quiz1();
void quiz2_1();
void quiz2_2();
void quiz3();

double theta = 90;
double phi = 0;
double r = 17;
double x = 15, y = 15, z = 15;
double cam_uv = 1; // camera up vector (0, cam_uv, 0)을 결정

GLuint current_width, current_height;
GLint DispalyListStartIndex;

GLint pos;

GLfloat light_position[] = { 0.0f, 0.0f, 0.0f, 1.0f };

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(100, 100); // x, y
	glutCreateWindow("12161719 김진호"); // window 이름

	init();

	glutCreateMenu(main_menu);
	glutAddMenuEntry("Quit", 999);

	glutAttachMenu(GLUT_RIGHT_BUTTON);

	/*Callback 함수 정의*/
	glutDisplayFunc(draw);
	glutMouseFunc(mouse); // 마우스 클릭 이벤트 처리
	glutMotionFunc(motion); // "클릭된 상태"에서 마우스 움직임이 있을 때 호출되는 것으로 보임
	glutKeyboardFunc(keyboard); // 키보드 입력 처리
	glutSpecialFunc(specialKeyboard);
	glutReshapeFunc(resize);
	glutIdleFunc(idle);

	/*Looping 시작*/
	glutMainLoop();

	return 0;
}

/*초기화 함수*/
void init()
{
	// 배경색 설정
	glClearColor(0.0f, 0.0f, 1.0f, 0.0f);
	glClearDepth(1.0f);
	//glEnable(GL_DEPTH_TEST);

	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_NORMALIZE);
	
	// 조명 설정
	setting_light();

	DispalyListStartIndex = glGenLists(5);
	MakeCheckerBoard(DispalyListStartIndex);
	MakeSincSurface(DispalyListStartIndex + 1);
	//quiz1();
	//quiz2_1();
	//quiz2_2();
}

void draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(15, 15, 15, 0, 0, 0, 0, 1, 0);
	/*getEyePosition(theta, phi);
	gluLookAt(y, z, x, 0, 0, 0, 0, cam_uv, 0);*/
	
	glBegin(GL_POINT);
	glVertex2d(0, 0);
	glEnd();

	GLfloat LightPosition[] = { 0, 50, 0, 1 };
	glLightfv(GL_LIGHT0, GL_POSITION, LightPosition);

	//glCallList(2);
	//glCallList(3);
	//quiz3();

	//glCallList(DispalyListStartIndex);

	for (int i = -3; i < 3; i++)
	{
		for (int j = -3; j < 3; j++)
		{
			glPushMatrix();

			glTranslatef(10 * i, 0, 10 * j);
			glCallList(DispalyListStartIndex);

			glPopMatrix();
		}
	}

	glCallList(DispalyListStartIndex + 1);

	glFlush();
	glutSwapBuffers();
}

void quiz1()
{
	GLfloat color_vector[3] = { 0.0f, 0.0f, 0.0f };
	glNewList(1, GL_COMPILE);
	glColor3fv(color_vector);
	glutWireTeapot(3);
	glEndList();
	color_vector[0] = color_vector[1] = color_vector[2] = 1.0f;
}

void quiz2_1()
{
	glNewList(2, GL_COMPILE);
	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_POLYGON);
	glVertex2f(0.0, 0.0);
	glVertex2f(1.0, 0.0);
	glVertex2f(0.0, 1.0);
	glEnd();
	glTranslatef(1.5, 0.0, 0.0);
	glEndList();
}

void quiz2_2()
{
	glNewList(3, GL_COMPILE);
	glPushMatrix();
	glPushAttrib(GL_CURRENT_BIT);
	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_POLYGON);
	glVertex2f(0.0, 0.0);
	glVertex2f(1.0, 0.0);
	glVertex2f(0.0, 1.0);
	glEnd();
	glTranslatef(1.5, 0.0, 0.0);
	glPopAttrib();
	glPopMatrix();
	glEndList();
}

void quiz3()
{
	for (int i = 0; i < 4; i++)
		glCallList(3);
		//glCallList(2)

	glColor3f(0.0, 1.0, 0.0);
	glBegin(GL_LINES);
	glVertex2f(0.5, 0.0);
	glVertex2f(5.0, 0.0);
	glEnd();
}

void MakeCheckerBoard(GLint ListIndex)
{
	glNewList(ListIndex, GL_COMPILE);

	glPushAttrib(GL_LIGHTING_BIT);
	glDisable(GL_LIGHTING);

	for (int i = -5; i < 5; i++)
	{
		for (int j = -5; j < 5; j++)
		{
			glPushMatrix();

			glTranslatef(i, 0, j);
			if ((i + j + 100) % 2 == 1) glColor3f(1, 1, 0);
			else glColor3f(0, 0, 0);
			glBegin(GL_QUADS);
			glVertex3f(0.5f, 0, 0.5f);
			glVertex3f(-0.5f, 0, 0.5f);
			glVertex3f(-0.5f, 0, -0.5f);
			glVertex3f(0.5f, 0, -0.5f);
			glEnd();

			glPopMatrix();
		}
	}
	glPopAttrib();
	glEndList();
}

void MakeSincSurface(GLint ListIndex)
{
	GLfloat x, z, d1, d2, d3, d4, y1, y2, y3, y4;
	GLfloat step = 0.2, freq = 3.0, peak = 10;

	glNewList(ListIndex, GL_COMPILE);
	glColor3f(0, 1, 0);
	for (x = -10; x < 10; x += step)
	{
		for (z = -10; z < 10; z += step)
		{
			d1 = sqrt(x * x + z * z);
			d2 = sqrt((x + step) * (x + step) + z * z);
			d3 = sqrt((x + step) * (x + step) + (z + step) * (z + step));
			d4 = sqrt(x * x + (z + step) * (z + step));

			if (d1 == 0) y1 = peak;
			else y1 = peak * sin(freq * d1) / (freq * d1);

			if (d2 == 0) y2 = peak;
			else y2 = peak * sin(freq * d2) / (freq * d2);

			if (d3 == 0) y3 = peak;
			else y3 = peak * sin(freq * d3) / (freq * d3);

			if (d4 == 0) y4 = peak;
			else y4 = peak * sin(freq * d4) / (freq * d4);

			glBegin(GL_TRIANGLES);
			glNormal3f(step, step, y3 - y1);
			glVertex3f(x, y1, z);
			glVertex3f(x + step, y2, z);
			glVertex3f(x + step, y3, z + step);

			glVertex3f(x + step, y3, z + step);
			glVertex3f(x, y4, z + step);
			glVertex3f(x, y1, z);

			glEnd();
		}
	}
	glEndList();
}

void draw_axis()
{
	glLineWidth(3);
	glBegin(GL_LINES);
	glColor3f(1, 0, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(4, 0, 0);

	glColor3f(0, 1, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 4, 0);

	glColor3f(0, 0, 1);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, 4);
	glEnd();
	glLineWidth(1);
}

void setting_light()
{
	// 조명 설정
	GLfloat light_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	GLfloat light_diffuse[] = { 0.7f, 0.7f, 0.7f, 1.0f };
	GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

	// 재질 설정
	GLfloat ka[] = { 0.2f,0.2f,0.2f,1.0f };
	GLfloat kd[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat ks[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glMaterialfv(GL_FRONT, GL_AMBIENT, ka);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, kd); // 아래의 glEnable(GL_COLOR_MATERIAL)이 있으므로 필요없음
	glMaterialfv(GL_FRONT, GL_SPECULAR, ks);
	glMaterialf(GL_FRONT, GL_SHININESS, 64.0); // shiness factor

	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
}

void draw_string(void* font, const char* str, GLfloat x, GLfloat y, GLfloat r, GLfloat g, GLfloat b)
{
	glPushAttrib(GL_LIGHTING_BIT);
	glDisable(GL_LIGHTING);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(-5, 5, -5, 5);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glColor3f(r, g, b);
	glRasterPos3f(x, y, 0);
	for (unsigned int i = 0; i < strlen(str); i++)
		glutBitmapCharacter(font, str[i]);
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopAttrib();
}

void mouse(int button, int state, int x, int y)
{
	printf("Mouse button is clicked! (%d, %d, %d, %d)\n", button, state, x, y);
}

void motion(int x, int y)
{
	printf("Mouse is moving! (%d, %d)\n", x, y);
}

void keyboard(unsigned char key, int x, int y)
{
	printf("You pressed %c\n", key);
	if (key == '0') pos = 0;
	else if (key == '1') pos = 1;
	else if (key == '2') pos = 2;
	else if (key == '3') pos = 3;
	else if (key == '4') pos = 4;

	if (key == 'r') r++; // farther
	if (key == 'c') r--; // closer

	cout << "pos:" << pos << endl;
	glutPostRedisplay();
}

void specialKeyboard(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_LEFT:
		phi -= 5.3;
		break;
	case GLUT_KEY_RIGHT:
		phi += 5.3;
		break;
	case GLUT_KEY_UP:
		theta -= 5.3;
		break;
	case GLUT_KEY_DOWN:
		theta += 5.3;
		break;
	}

	if (phi > 360) phi -= 360;
	else if (phi < -360) phi += 360;
	if (theta > 360) theta -= 360;
	else if (theta < -360) theta += 360;

	glutPostRedisplay();
}

void idle()
{
	glutPostRedisplay();
}

void getEyePosition(double a, double b)
{
	if (a < 0)
	{
		if (a > -180) cam_uv = -1;
		else cam_uv = 1; //a < -180

		a = -a;
		b += 180;
	}
	else
	{
		if (a > 180)
		{
			cam_uv = -1;
			a = 360 - a;
			b += 180;
			//cout << b << endl;
		}
		else cam_uv = 1;
	}
	//cout << "cam_uv: " << cam_uv << endl;
	if (b > 360) b -= 360;
	else if (b < -360) b += 360;

	x = r * sin(getRadian(a)) * cos(getRadian(b));
	y = r * sin(getRadian(a)) * sin(getRadian(b));
	z = r * cos(getRadian(a));
}

double getRadian(double angle)
{
	return angle * M_PI / 180;
}

void sub_menu_function(int option)
{
	printf("Submenu %d has been selected\n", option);
}

void resize(int width, int height)
{
	current_width = width, current_height = height;
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, (float)width / (float)height, 1, 500);
	glMatrixMode(GL_MODELVIEW);
}

void main_menu(int option)
{
	if (option == 999) exit(0);

	glutPostRedisplay();
}

void print_manual()
{
	cout << "++++++keyboard++++++\n0: 광원이 시점에 위치\n1 : 광원이 큐브에 위치\n2: 광원이 사이다에 위치\n3: 광원이 지구에 위치\n4: 광원 (10, 5, 3)에 위치\n\nr: Zoom Out\tc: Zoom-in\n";
	cout << "++++++++++++++++++++\n";
}