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
void draw_string(void*, const char*, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat);
void DrawSphere();
void DrawBackground();
void draw_axis();
void Picking(GLint, GLint);
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

double theta = 70;
double phi = 10;
double r = 17;
double x = 15, y = 15, z = 15;
double cam_uv = 1; // camera up vector (0, cam_uv, 0)�� ����

GLint pos;

GLfloat g_nGLWidth = 500, g_nGLHeight = 500;
GLint g_nX, g_nY; // ���콺 Ŭ�� ��ġ
GLint g_nSelect; // ���õ� ��ü id

GLfloat g_pPosition[6] = { 0.0f, 144.0f, -100.0f, -100.0f, 100.0f, -100.0f }; // position matrix

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(g_nGLWidth, g_nGLHeight);
	glutInitWindowPosition(300, 300); // x, y
	glutCreateWindow("12161719 ����ȣ Picking"); // window �̸�

	init();

	/*Callback �Լ� ����*/
	glutDisplayFunc(draw);
	glutMouseFunc(mouse); // ���콺 Ŭ�� �̺�Ʈ ó��
	glutMotionFunc(motion); // "Ŭ���� ����"���� ���콺 �������� ���� �� ȣ��Ǵ� ������ ����
	glutReshapeFunc(resize);
	glutIdleFunc(idle);

	/*Looping ����*/
	glutMainLoop();

	return 0;
}

/*�ʱ�ȭ �Լ�*/
void init()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);

	setting_light();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//glOrtho(-g_nGLWidth / 2.0f, g_nGLWidth / 2.0f, -g_nGLHeight / 2.0f, g_nGLHeight / 2.0f, -100, 100);
	//gluPerspective(45, 1, 1, 500);

}

void draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(100, 100, 100, 0, 0, 0, 0, 1, 0);

	DrawBackground();
	DrawSphere();
	//DrawOthers();

	glFlush();
	glutSwapBuffers();
}

void DrawBackground()
{
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glDisable(GL_LIGHTING);
	glColor3f(1.0f, 1.0f, 1.0f);
	int i;
	int offset = 50;
	glBegin(GL_LINES);
	for (i = -g_nGLWidth / 2.0f; i < g_nGLWidth / 2.0f; i += offset)
	{
		glVertex3f(i, -g_nGLHeight / 2.0f, 0.0f);
		glVertex3f(i, g_nGLHeight / 2.0f, 0.0f);
	}
	for (i = -g_nGLHeight / 2.0f; i < g_nGLHeight; i += offset)
	{
		glVertex3f(-g_nGLWidth / 2.0f, i, 0.0f);
		glVertex3f(g_nGLWidth / 2.0f, i, 0.0f);
	}
	glEnd();
	glPopAttrib();
}

void DrawSphere()
{
	glLoadName(1);
	glPushMatrix();
	glTranslatef(g_pPosition[0], g_pPosition[1], -10.0f);
	glColor3f(1, 0, 0);
	glutSolidSphere(25.0f, 30, 30);
	glPopMatrix();

	glLoadName(2);
	glPushMatrix();
	glTranslatef(g_pPosition[2], g_pPosition[3], 0.0f);
	glColor3f(0, 1, 0);
	glutSolidSphere(25.0f, 30, 30);
	glPopMatrix();

	glLoadName(3);
	glPushMatrix();
	glTranslatef(g_pPosition[4], g_pPosition[5], 10.0f);
	glColor3f(0, 0, 1);
	glutSolidSphere(25.0f, 30, 30);
	glPopMatrix();
}

void Picking(GLint x, GLint y)
{
	printf("\n\nPICKING...\n");
	GLuint selectBuf[100] = { 0, };

	glSelectBuffer(100, selectBuf); // picking�� ��ü�� ���� ���� ����
	glRenderMode(GL_SELECT); // GL_SELECT�� ����Ͽ� ���� ��� ����
	glMatrixMode(GL_PROJECTION); // ������ ó���� ���� ��ǥ�迡�� �ǽ��ϹǷ� ���� ��ȯ ��� ����
	glInitNames(); // 
	glPushName(-1); // ��ü�� �̸� // �ӽ� -1 push

	glPushMatrix();
	glLoadIdentity();
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport); // ���� viewport ������ viewport �迭�� ����
	gluPickMatrix(x, y, 0.1, 0.1, viewport);

	glOrtho(-g_nGLWidth / 2.0f, g_nGLWidth / 2.0f, -g_nGLHeight / 2.0f, g_nGLHeight / 2.0f, -100, 100);
	//gluPerspective(45, (float)g_nGLWidth / (float)g_nGLHeight, 1, 500);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	DrawSphere();

	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glFlush();
	
	GLint hits = glRenderMode(GL_RENDER); // ���콺 ��Ŭ������ ���õ� ��ü�� ����

	if (hits <= 0) return; // �ƹ��͵� ���õ��� ���� ���
	
	int stack = selectBuf[0]; // name stack�� �ִ� name�� ����
	GLuint zMin = selectBuf[1]; // ���õ� ��ü�� �ּ� z��
	GLuint zMax = selectBuf[2]; // ���õ� ��ü�� �ִ� z��
	g_nSelect = selectBuf[3]; // ���õ� ��ü�� id

	int index = 3 + stack;
	for (int i = 1; i < hits; i++)
	{
		stack = selectBuf[index];

		/* zMin�� �̿��ؼ� ���� ����� �� pick �ϱ� */
		if (zMin > selectBuf[index + 1])
		{
			zMin = selectBuf[index + 1];
			g_nSelect = selectBuf[index + 3];
		}

		///* zMax�� �̿��ؼ� ���� �� �� pick �ϱ� */
		//if (zMax < selectBuf[index + 2])
		//{   // ������ ���õ� ��쿡�� zMax�� ���� ū �� ��, ���� �ָ��ִ� �ͺ��� �����Ѵ�.
		//	zMax = selectBuf[index + 2];
		//	g_nSelect = selectBuf[index + 3]; // ************************************ key point
		//}

		///* zMin�� �̿��ؼ� ���� �� �� pick �ϱ� */
		//if (zMin < selectBuf[index + 1])
		//{
		//	zMin = selectBuf[index + 1];
		//	g_nSelect = selectBuf[index + 3];
		//}

		///* zMax�� �̿��ؼ� ���� ����� �� pick �ϱ� */
		//if (zMax > selectBuf[index + 2])
		//{
		//	zMin = selectBuf[index + 2];
		//	g_nSelect = selectBuf[index + 3];
		//}

		index += 3 + stack;
	}

	printf("hits:%d   name id: %d   zMin: %u   zMax: %u   ���õ� ��ü: %d", hits, stack, zMin, zMax, g_nSelect);
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
	// ���� ����
	GLfloat light_ambient[] = { 0.1f,0.1f,0.1f,1.0f };
	GLfloat light_diffuse[] = { 1.0f,1.0f,1.0f,1.0f };
	GLfloat light_speuclar[] = { 1.0f,1.0f,1.0f,1.0f };
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_speuclar);

	GLfloat light_position[] = { 1.0f,1.0f,1.0f,0.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	GLfloat mat_speuclar[] = { 1.0f,1.0f,1.0f,1.0f };
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_speuclar);
	glMaterialf(GL_FRONT, GL_SHININESS, 64);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);
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
	printf("x:%d\ty:%d\n", x, y);
	y = g_nGLHeight - y;
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		Picking(x, y);
		g_nX = x;
		g_nY = y;
	}
	else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		g_nSelect = 0;
	}
}

void motion(int x, int y)
{
	y = g_nGLHeight - y;
	if (g_nSelect > 0)
	{
		g_pPosition[(g_nSelect - 1) * 2 + 0] += x - g_nX;
		g_pPosition[(g_nSelect - 1) * 2 + 1] += y - g_nY;
		g_nX = x;
		g_nY = y;
		glutPostRedisplay();
	}
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
	g_nGLWidth = width;
	g_nGLHeight = height;
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//glOrtho(-width / 2.0f, width / 2.0f, -height / 2.0f, height / 2.0f, -100, 100);
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