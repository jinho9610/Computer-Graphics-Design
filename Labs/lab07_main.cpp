#define _USE_MATH_DEFINES
#define _CRT_SECURE_NO_WARNINGS

#define Sun 0
#define Venus 1
#define Earth 2
#define Moon 3
#define Mars 4

#include <gl/glut.h>
#include <stdio.h>
#include <windows.h> // idle �Լ� ó���� ���� ���
#include <cmath>
#include <iostream>

using namespace std;

void init();
void draw();
void draw_string(void*, const char*, int, int);
void draw_axis();
void mouse(int, int, int, int);
void motion(int, int);
void keyboard(unsigned char, int, int);
void specialKeyboard(int, int, int);
void idle();
void resize(int, int);
double getRadian(double);
void getEyePosition(double, double);

void main_menu(int);

double theta = 45;
double phi = 45;
double r = 13;
double x, y, z;
double cam_uv = 1; // camera up vector (0, cam_uv, 0)�� ����

float rotate_angle[5];
float revolve_angle[5];

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(100, 100); // x, y
	glutCreateWindow("12161719 ����ȣ"); // window �̸�

	init();

	glutCreateMenu(main_menu);
	glutAddMenuEntry("Init", 1);
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
{	// ���� ����
	glClearColor(0.2f, 0.5f, 0.8f, 1.0f);
}

void draw()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0, 30, 0, 0, 0, 0, 1, 0, 0);

	draw_axis();
	glRotatef(rotate_angle[Sun], 0, 1, 0);
	glColor3f(1, 0, 0);
	glutWireSphere(2, 50, 10); // �¾�
	glPushMatrix(); // ���� M ��Ʈ���� ����

	glRotatef(revolve_angle[Earth], 0, 1, 0); // ���� ����
	glTranslatef(8, 0, 0); // ���� ��ġ �̵�
	glRotatef(rotate_angle[Earth], 0, 1, 0); //���� ����
	glColor3f(0, 0, 1);
	glutWireSphere(1, 50, 10); // ����

	glRotatef(revolve_angle[Moon], 0, 1, 0); // �� ����
	glTranslatef(2, 0, 0); // �� ��ġ �̵�
	glRotatef(rotate_angle[Moon], 0, 1, 0); // �� ����
	glColor3f(0, 1, 0);
	glutWireSphere(0.5, 20, 5); // ��

	glPopMatrix(); // M ��Ʈ���� ������
	glPushMatrix(); // �������� ����� �� �ֵ��� M ��Ʈ���� ����

	glRotatef(revolve_angle[Mars], 0, 1, 0);
	glTranslatef(11, 0, 0);
	glRotatef(rotate_angle[Mars], 0, 1, 0);
	glColor3f(1, 0.4, 0.8);
	glutWireSphere(1.4, 50, 10); // ȭ��

	glPopMatrix(); 
	glRotatef(revolve_angle[Venus], 0, 1, 0);
	glTranslatef(-4, 3, 0);
	glRotatef(rotate_angle[Venus], 0, 1, 0);
	glColor3f(0, 1, 1);
	glutWireSphere(0.8, 20, 5); // �ݼ�

	glFlush();
	glutSwapBuffers();
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

void draw_string(void* font, const char* str, int x, int y)
{
	unsigned int i;
	glRasterPos2i(x, y);
	for (i = 0; i < strlen(str); i++)
		glutBitmapCharacter(font, str[i]);
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
	if (key == 'f') r += 1.5;
	else if (key == 'c') r -= 1.5;;
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
	rotate_angle[Sun] += 0.01; // �¾� ����
	if (rotate_angle[Sun] > 360) rotate_angle[Sun] -= 360;

	rotate_angle[Earth] += 0.01; // ���� ����
	if (rotate_angle[Earth] > 360) rotate_angle[Earth] -= 360;
	revolve_angle[Earth] += 0.03; // ���� ����
	if (revolve_angle[Earth] > 360) revolve_angle[Earth] -= 360;

	rotate_angle[Moon] += 0.01; // �� ����
	if (rotate_angle[Moon] > 360) rotate_angle[Moon] -= 360;
	revolve_angle[Moon] += 0.1; // �� ����
	if (revolve_angle[Moon] > 360) revolve_angle[Moon] -= 360;

	rotate_angle[Mars] += 0.01; // ȭ�� ����
	if (rotate_angle[Mars] > 360) rotate_angle[Mars] -= 360;
	revolve_angle[Mars] += 0.037; // ȭ�� ����
	if (revolve_angle[Mars] > 360) revolve_angle[Mars] -= 360;

	rotate_angle[Venus] += 0.01; // �ݼ� ����
	if (rotate_angle[Venus] > 360) rotate_angle[Venus] -= 360;
	revolve_angle[Venus] += 0.05; // �ݼ� ����
	if (revolve_angle[Venus] > 360) revolve_angle[Venus] -= 360;

	glutPostRedisplay();
}

void sub_menu_function(int option)
{
	printf("Submenu %d has been selected\n", option);
}

void resize(int width, int height)
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, (float)width / (float)height, 1, 500);
	glMatrixMode(GL_MODELVIEW);
}

double getRadian(double angle)
{
	return angle * M_PI / 180;
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
			cout << b << endl;
		}
		else cam_uv = 1;
	}
	cout << "cam_uv: " << cam_uv << endl;
	if (b > 360) b -= 360;
	else if (b < -360) b += 360;

	x = r * sin(getRadian(a)) * cos(getRadian(b));
	y = r * sin(getRadian(a)) * sin(getRadian(b));
	z = r * cos(getRadian(a));
}

void main_menu(int option)
{
	if (option == 1)
	{
		theta = 45;
		phi = 45;
		cam_uv = 1;
	}
	else if (option == 999) exit(0);

	glutPostRedisplay();
}