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
void idle();
void resize(int, int);

void main_menu(int);
void submenu_light_position(int);
void submenu_attenuation(int);

GLint pos = 1;
GLint atten = 0;

float rotate_angle[5];
float revolve_angle[5];

GLfloat light_position[] = { 0.0f, 0.0f, 0.0f, 1.0f };

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(100, 100); // x, y
	glutCreateWindow("12161719 ����ȣ"); // window �̸�

	init();

	int light_position = glutCreateMenu(submenu_light_position);
	glutAddMenuEntry("Sun", 1);
	glutAddMenuEntry("Earth", 2);
	glutAddMenuEntry("Moon", 3);

	int attenuation = glutCreateMenu(submenu_attenuation);
	glutAddMenuEntry("Dim", 1);
	glutAddMenuEntry("Dimmer", 2);
	glutAddMenuEntry("attenuation effect off", 3);

	glutCreateMenu(main_menu);
	glutAddMenuEntry("Quit", 999);
	glutAddSubMenu("Light Position", light_position);
	glutAddSubMenu("Attenuation type", attenuation);

	glutAttachMenu(GLUT_RIGHT_BUTTON);

	/*Callback �Լ� ����*/
	glutDisplayFunc(draw);
	glutMouseFunc(mouse); // ���콺 Ŭ�� �̺�Ʈ ó��
	glutMotionFunc(motion); // "Ŭ���� ����"���� ���콺 �������� ���� �� ȣ��Ǵ� ������ ����
	glutKeyboardFunc(keyboard); // Ű���� �Է� ó��
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
	//GLfloat light_ambient[] = { 1.0f, 0.0f, 0.2f, 1.0f };
	//GLfloat light_ambient[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat light_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	//GLfloat light_diffuse[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	GLfloat light_diffuse[] = { 1.0f, 0.0f, 0.0f, 1.0f };
	GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	//GLfloat light_specular[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

	//glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 45.0);
	//GLfloat spot_direction[] = { 1.0f,1.0f,0.0f,0.0f };
	//glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spot_direction);
	//glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 4.0);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glMateriali(GL_FRONT, GL_SHININESS, 128); // ����

	glMaterialfv(GL_FRONT, GL_SPECULAR, light_specular); // specular light

	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
}

void draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (atten == 1)
	{
		glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 1.15);
		glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.15);
		glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.0);
	}
	else if (atten == 2)
	{
		glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 1.15);
		glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.15);
		glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.01);
	}
	else
	{
		glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 1.0);
		glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.0);
		glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.0);
	}
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(15, 15, 15, 0, 0, 0, 0, 1, 0); // world ��ǥ���� �߽�
	if (pos == 1) glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	draw_axis();
	glRotatef(rotate_angle[Sun], 0, 1, 0);
	glColor3f(1, 0, 0);
	glutSolidSphere(2, 30, 30); // �¾�
	glPushMatrix(); // ���� M ��Ʈ���� ����

	glRotatef(revolve_angle[Earth], 0, 1, 0); // ���� ����
	glTranslatef(8, 0, 0); // ���� ��ġ �̵�
	glRotatef(rotate_angle[Earth], 0, 1, 0); //���� ����
	glColor3f(0, 0, 1);
	glutSolidSphere(1, 30, 30); // ����
	if (pos == 2) glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glRotatef(revolve_angle[Moon], 0, 1, 0); // �� ����
	glTranslatef(2, 0, 0); // �� ��ġ �̵�
	glRotatef(rotate_angle[Moon], 0, 1, 0); // �� ����
	glColor3f(0, 1, 0);
	glutSolidSphere(0.5, 30, 30); // ��
	if (pos == 3) glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glPopMatrix(); // M ��Ʈ���� ������
	glPushMatrix(); // �������� ����� �� �ֵ��� M ��Ʈ���� ����

	glRotatef(revolve_angle[Mars], 0, 1, 0);
	glTranslatef(11, 0, 0);
	glRotatef(rotate_angle[Mars], 0, 1, 0);
	glColor3f(1, 0.4, 0.8);
	glutWireSphere(1.4, 50, 10); // ȭ��

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
	revolve_angle[Mars] += 0.05; // ȭ�� ����
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

void main_menu(int option)
{
	if (option == 999) exit(0);

	glutPostRedisplay();
}

void submenu_light_position(int option)
{
	if (option == 1)
	{
		pos = 1;
		printf("Light at SUN position\n");
	}
	else if (option == 2)
	{
		pos = 2;
		printf("Light at EARTH position\n");
	}
	else
	{
		pos = 3;
		printf("Light at MOON position\n");
	}

	glutPostRedisplay();
}

void submenu_attenuation(int option)
{
	atten = option;
	glutPostRedisplay();
}