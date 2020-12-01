#define _USE_MATH_DEFINES
#define _CRT_SECURE_NO_WARNINGS

#include <gl/glut.h>
#include <stdio.h>
#include <windows.h> // idle �Լ� ó���� ���� ���
#include <cmath>
#include <iostream>
#include "bmpfuncs.h"

using namespace std;

void init();
void setting_light();
void setting_quadric();
void draw();
void draw_text();
void draw_can();
void draw_disk();
void draw_teapot();
void draw_string(void*, const char*, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat);
void draw_axis();

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

double theta = 45;
double phi = 45;
double r = 17;
double x = 15, y = 15, z = 15;
double cam_uv = 1; // camera up vector (0, cam_uv, 0)�� ����

GLuint current_width, current_height;

GLUquadricObj* qobj = gluNewQuadric();
GLint pos;

GLfloat light_position[] = { 0.0f, 0.0f, 0.0f, 1.0f };

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(100, 100); // x, y
	glutCreateWindow("12161719 ����ȣ"); // window �̸�

	init();

	glutCreateMenu(main_menu);
	glutAddMenuEntry("Quit", 999);

	glutAttachMenu(GLUT_RIGHT_BUTTON);

	/*Callback �Լ� ����*/
	glutDisplayFunc(draw);
	glutMouseFunc(mouse); // ���콺 Ŭ�� �̺�Ʈ ó��
	glutMotionFunc(motion); // "Ŭ���� ����"���� ���콺 �������� ���� �� ȣ��Ǵ� ������ ����
	glutKeyboardFunc(keyboard); // Ű���� �Է� ó��
	glutSpecialFunc(specialKeyboard);
	glutReshapeFunc(resize);
	glutIdleFunc(idle);

	/*Looping ����*/
	glutMainLoop();

	return 0;
}

/*�ʱ�ȭ �Լ�*/
void init()
{
	print_manual();
	// ���� ����
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);

	// Quadric ������Ʈ�� �ؽ�ó ���� Ȱ��ȭ
	setting_quadric();

	// ���� ����
	setting_light();
}

void draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	getEyePosition(theta, phi);
	gluLookAt(y, z, x, 0, 0, 0, 0, cam_uv, 0);
	
	draw_text();
	draw_can();
	draw_disk();
	draw_teapot();
	
	glFlush();
	glutSwapBuffers();
}

void draw_text()
{
	glViewport(0, current_height / 2, current_width / 2, current_height / 2);

	draw_string(GLUT_BITMAP_TIMES_ROMAN_24, "Graphics are cool!!", -4.0, 3.0, 1.0f, 1.0f, 0);
	draw_string(GLUT_BITMAP_HELVETICA_18, "Multi-Veiwport Example!!", -4.0, 2.0, 0.5f, 1.0f, 0);

	glFlush();
}

void draw_can()
{	
	glViewport(0, 0, current_width / 2, current_height / 2);
	draw_axis();

	glPushMatrix();
	glRotatef(12.0f, 0, 1, 0);
	glTranslatef(0.0f, 0.0f, -2.0f);

	glPushMatrix();
	glColor3f(0.8f, 0.8f, 0.8f);
	gluDisk(qobj, 0.0f, 1.0f, 10, 10);
	glColor3f(0.67, 0.74, 0.32);
	gluCylinder(qobj, 1.0f, 1.0f, 4.0f, 10, 10);
	glTranslatef(0.0f, 0.0f, 4.0f);
	glColor3f(0.8f, 0.8f, 0.8f);
	gluDisk(qobj, 0.0f, 1.0f, 10, 10);
	glPopMatrix();

	glPopMatrix();

	glFlush();
}

void draw_disk()
{
	glViewport(current_width / 2, current_height / 2, current_width / 2, current_height / 2);
	draw_axis();

	glPushMatrix();
	glColor3f(0.8f, 0.3f, 0.1f);
	gluPartialDisk(qobj, 1.1f, 2.5f, 30, 30, 60, 280);
	glPopMatrix();

	glFlush();
}

void draw_teapot()
{
	glViewport(current_width / 2, 0, current_width / 2, current_height / 2);
	draw_axis();

	glPushMatrix();
	glColor3f(0.0f, 1.0f, 0.0f);
	glutWireTeapot(5);
	glPopMatrix();

	glFlush();
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

void setting_quadric()
{
	gluQuadricTexture(qobj, GL_TRUE); // �ؽ��� ���� �����Ѵ�.
	gluQuadricDrawStyle(qobj, GLU_FILL); // Solid ���·� ��ü�� �׸���.
	gluQuadricNormals(qobj, GLU_SMOOTH); // ���� �ε巴�� �׷������� �Ѵ�.
	gluQuadricOrientation(qobj, GLU_OUTSIDE); // ���� ���Ͱ� ��ü �ܺθ� ���ϵ��� �Ѵ�.
}

void setting_light()
{
	// ���� ����
	GLfloat light_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	GLfloat light_diffuse[] = { 0.7f, 0.7f, 0.7f, 1.0f };
	GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

	// ���� ����
	GLfloat ka[] = { 0.2f,0.2f,0.2f,1.0f };
	GLfloat kd[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat ks[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glMaterialfv(GL_FRONT, GL_AMBIENT, ka);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, kd); // �Ʒ��� glEnable(GL_COLOR_MATERIAL)�� �����Ƿ� �ʿ����
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
	cout << "++++++keyboard++++++\n0: ������ ������ ��ġ\n1 : ������ ť�꿡 ��ġ\n2: ������ ���̴ٿ� ��ġ\n3: ������ ������ ��ġ\n4: ���� (10, 5, 3)�� ��ġ\n\nr: Zoom Out\tc: Zoom-in\n";
	cout << "++++++++++++++++++++\n";
}