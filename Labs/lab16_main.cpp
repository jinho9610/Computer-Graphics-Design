#define _USE_MATH_DEFINES
#define _CRT_SECURE_NO_WARNINGS

#include <GL/glut.h>
#include <stdio.h>
#include <windows.h> // idle 함수 처리를 위한 헤더
#include <cmath>
#include <iostream>
#include <GL/glext.h>
#include "bmpfuncs.h"

using namespace std;

void init();
void setting_light();
void setting_quadric();
void draw();
void draw_string(void*, const char*, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat);
void DrawSkybox();
void draw_axis();
void mouse(int, int, int, int);
void motion(int, int);
void keyboard(unsigned char, int, int);
void specialKeyboard(int, int, int);
void idle();
void resize(int, int);
void getEyePosition(double, double);
double getRadian(double);
void Cube_Texture();
void printMatrix(GLfloat*);

void main_menu(int);
void print_manual();
void InverseMat();

double theta = 90;
double phi = 0;
double r = 10;
double x, y, z;
double cam_uv = 1; // camera up vector (0, cam_uv, 0)을 결정

GLint pos;

GLuint g_nCubeTex;

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(300, 300); // x, y
	glutCreateWindow("12161719 김진호 Picking"); // window 이름

	init();

	/*Callback 함수 정의*/
	glutDisplayFunc(draw);
	glutMouseFunc(mouse); // 마우스 클릭 이벤트 처리
	glutMotionFunc(motion); // "클릭된 상태"에서 마우스 움직임이 있을 때 호출되는 것으로 보임
	glutKeyboardFunc(keyboard);
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
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	setting_light();
	Cube_Texture();
}

void draw()
{
	glDisable(GL_LIGHTING);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	getEyePosition(theta, phi);
	gluLookAt(y, z, x, 0, 0, 0, 0, cam_uv, 0);
	
	DrawSkybox();

	glMatrixMode(GL_TEXTURE);
	glPushMatrix();
	InverseMat();
	glMatrixMode(GL_MODELVIEW);

	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glEnable(GL_TEXTURE_GEN_R);
	glEnable(GL_TEXTURE_CUBE_MAP);
	glBindTexture(GL_TEXTURE_CUBE_MAP, g_nCubeTex);
	glutSolidSphere(1.0f, 30, 30);
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	glDisable(GL_TEXTURE_GEN_R);
	glDisable(GL_TEXTURE_CUBE_MAP);

	glMatrixMode(GL_TEXTURE);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);

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

void Cube_Texture()
{	
	uchar* data[6] = { NULL, };
	int width[6] = { 0, }, height[6] = { 0 }, channels[6] = { 0, };
	data[0] = readImageData("./img/right.bmp", &width[0], &height[0], &channels[0]);
	data[1] = readImageData("./img/left.bmp", &width[1], &height[1], &channels[1]);
	data[2] = readImageData("./img/top.bmp", &width[2], &height[2], &channels[2]);
	data[3] = readImageData("./img/bottom.bmp", &width[3], &height[3], &channels[3]);
	data[4] = readImageData("./img/back.bmp", &width[4], &height[4], &channels[4]);
	data[5] = readImageData("./img/front.bmp", &width[5], &height[5], &channels[5]);

	glGenTextures(1, &g_nCubeTex);
	glBindTexture(GL_TEXTURE_CUBE_MAP, g_nCubeTex);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA, width[0], height[0], 0, GL_RGB, GL_UNSIGNED_BYTE, data[0]);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA, width[1], height[1], 0, GL_RGB, GL_UNSIGNED_BYTE, data[1]);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA, width[2], height[2], 0, GL_RGB, GL_UNSIGNED_BYTE, data[2]);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA, width[3], height[3], 0, GL_RGB, GL_UNSIGNED_BYTE, data[3]);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA, width[4], height[4], 0, GL_RGB, GL_UNSIGNED_BYTE, data[4]);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA, width[5], height[5], 0, GL_RGB, GL_UNSIGNED_BYTE, data[5]);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
	glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
}

void DrawSkybox()
{
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_CUBE_MAP);
	glBindTexture(GL_TEXTURE_CUBE_MAP, g_nCubeTex);
	GLfloat g_nSkySize = 50.0f;
	
	glBegin(GL_QUADS);
	// +x
	glTexCoord3f(1.0, -1.0, -1.0); glVertex3f(g_nSkySize, -g_nSkySize, -g_nSkySize);
	glTexCoord3f(1.0, -1.0, 1.0); glVertex3f(g_nSkySize, -g_nSkySize, g_nSkySize);
	glTexCoord3f(1.0, 1.0, 1.0); glVertex3f(g_nSkySize, g_nSkySize, g_nSkySize);
	glTexCoord3f(1.0, 1.0, -1.0); glVertex3f(g_nSkySize, g_nSkySize, -g_nSkySize);

	// -x
	glTexCoord3f(-1.0, -1.0, -1.0); glVertex3f(-g_nSkySize, -g_nSkySize, -g_nSkySize);
	glTexCoord3f(-1.0, -1.0, 1.0); glVertex3f(-g_nSkySize, -g_nSkySize, g_nSkySize);
	glTexCoord3f(-1.0, 1.0, 1.0); glVertex3f(-g_nSkySize, g_nSkySize, g_nSkySize);
	glTexCoord3f(-1.0, 1.0, -1.0); glVertex3f(-g_nSkySize, g_nSkySize, -g_nSkySize);

	// +y
	glTexCoord3f(1.0, 1.0, 1.0); glVertex3f(g_nSkySize, g_nSkySize, g_nSkySize);
	glTexCoord3f(-1.0, 1.0, 1.0); glVertex3f(-g_nSkySize, g_nSkySize, g_nSkySize);
	glTexCoord3f(-1.0, 1.0, -1.0); glVertex3f(-g_nSkySize, g_nSkySize, -g_nSkySize);
	glTexCoord3f(1.0, 1.0, -1.0); glVertex3f(g_nSkySize, g_nSkySize, -g_nSkySize);

	// -y
	glTexCoord3f(1.0, -1.0, 1.0); glVertex3f(g_nSkySize, -g_nSkySize, g_nSkySize);
	glTexCoord3f(-1.0, -1.0, 1.0); glVertex3f(-g_nSkySize, -g_nSkySize, g_nSkySize);
	glTexCoord3f(-1.0, -1.0, -1.0); glVertex3f(-g_nSkySize, -g_nSkySize, -g_nSkySize);
	glTexCoord3f(1.0, -1.0, -1.0); glVertex3f(g_nSkySize, -g_nSkySize, -g_nSkySize);

	// +z
	glTexCoord3f(1.0, -1.0, 1.0); glVertex3f(g_nSkySize, -g_nSkySize, g_nSkySize);
	glTexCoord3f(-1.0, -1.0, 1.0); glVertex3f(-g_nSkySize, -g_nSkySize, g_nSkySize);
	glTexCoord3f(-1.0, 1.0, 1.0); glVertex3f(-g_nSkySize, g_nSkySize, g_nSkySize);
	glTexCoord3f(1.0, 1.0, 1.0); glVertex3f(g_nSkySize, g_nSkySize, g_nSkySize);

	// -z
	glTexCoord3f(1.0, -1.0, -1.0); glVertex3f(g_nSkySize, -g_nSkySize, -g_nSkySize);
	glTexCoord3f(-1.0, -1.0, -1.0); glVertex3f(-g_nSkySize, -g_nSkySize, -g_nSkySize);
	glTexCoord3f(-1.0, 1.0, -1.0); glVertex3f(-g_nSkySize, g_nSkySize, -g_nSkySize);
	glTexCoord3f(1.0, 1.0, -1.0); glVertex3f(g_nSkySize, g_nSkySize, -g_nSkySize);

	glEnd();
}

void setting_light()
{
	// 조명 설정
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
	glutPostRedisplay();
}

void motion(int x, int y)
{
	glutPostRedisplay();
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
	cout << "r:" << r << endl;
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

void printMatrix(GLfloat* m)
{
	cout << m[0] << ' ' << m[4] << ' ' << m[8] << ' ' << m[12] << endl;
	cout << m[1] << ' ' << m[5] << ' ' << m[9] << ' ' << m[13] << endl;
	cout << m[2] << ' ' << m[6] << ' ' << m[10] << ' ' << m[14] << endl;
	cout << m[3] << ' ' << m[7] << ' ' << m[11] << ' ' << m[15] << endl;
}

void InverseMat() {
	GLfloat m[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, m);

	GLfloat inv[16];

	inv[0] =
		m[5] * m[10] * m[15]
		- m[5] * m[11] * m[14]
		- m[9] * m[6] * m[15]
		+ m[9] * m[7] * m[14]
		+ m[13] * m[6] * m[11]
		- m[13] * m[7] * m[10];

	inv[1] =
		-m[1] * m[10] * m[15]
		+ m[1] * m[11] * m[14]
		+ m[9] * m[2] * m[15]
		- m[9] * m[3] * m[14]
		- m[13] * m[2] * m[11]
		+ m[13] * m[3] * m[10];

	inv[2] =
		m[1] * m[6] * m[15]
		- m[1] * m[7] * m[14]
		- m[5] * m[2] * m[15]
		+ m[5] * m[3] * m[14]
		+ m[13] * m[2] * m[7]
		- m[13] * m[3] * m[6];

	inv[3] =
		-m[1] * m[6] * m[11]
		+ m[1] * m[7] * m[10]
		+ m[5] * m[2] * m[11]
		- m[5] * m[3] * m[10]
		- m[9] * m[2] * m[7]
		+ m[9] * m[3] * m[6];

	inv[4] =
		-m[4] * m[10] * m[15]
		+ m[4] * m[11] * m[14]
		+ m[8] * m[6] * m[15]
		- m[8] * m[7] * m[14]
		- m[12] * m[6] * m[11]
		+ m[12] * m[7] * m[10];

	inv[5] =
		m[0] * m[10] * m[15]
		- m[0] * m[11] * m[14]
		- m[8] * m[2] * m[15]
		+ m[8] * m[3] * m[14]
		+ m[12] * m[2] * m[11]
		- m[12] * m[3] * m[10];

	inv[6] =
		-m[0] * m[6] * m[15]
		+ m[0] * m[7] * m[14]
		+ m[4] * m[2] * m[15]
		- m[4] * m[3] * m[14]
		- m[12] * m[2] * m[7]
		+ m[12] * m[3] * m[6];

	inv[7] =
		m[0] * m[6] * m[11]
		- m[0] * m[7] * m[10]
		- m[4] * m[2] * m[11]
		+ m[4] * m[3] * m[10]
		+ m[8] * m[2] * m[7]
		- m[8] * m[3] * m[6];

	inv[8] =
		m[4] * m[9] * m[15]
		- m[4] * m[11] * m[13]
		- m[8] * m[5] * m[15]
		+ m[8] * m[7] * m[13]
		+ m[12] * m[5] * m[11]
		- m[12] * m[7] * m[9];

	inv[9] =
		-m[0] * m[9] * m[15]
		+ m[0] * m[11] * m[13]
		+ m[8] * m[1] * m[15]
		- m[8] * m[3] * m[13]
		- m[12] * m[1] * m[11]
		+ m[12] * m[3] * m[9];

	inv[10] =
		m[0] * m[5] * m[15]
		- m[0] * m[7] * m[13]
		- m[4] * m[1] * m[15]
		+ m[4] * m[3] * m[13]
		+ m[12] * m[1] * m[7]
		- m[12] * m[3] * m[5];

	inv[11] =
		-m[0] * m[5] * m[11]
		+ m[0] * m[7] * m[9]
		+ m[4] * m[1] * m[11]
		- m[4] * m[3] * m[9]
		- m[8] * m[1] * m[7]
		+ m[8] * m[3] * m[5];

	inv[12] =
		-m[4] * m[9] * m[14]
		+ m[4] * m[10] * m[13]
		+ m[8] * m[5] * m[14]
		- m[8] * m[6] * m[13]
		- m[12] * m[5] * m[10]
		+ m[12] * m[6] * m[9];

	inv[13] =
		m[0] * m[9] * m[14]
		- m[0] * m[10] * m[13]
		- m[8] * m[1] * m[14]
		+ m[8] * m[2] * m[13]
		+ m[12] * m[1] * m[10]
		- m[12] * m[2] * m[9];

	inv[14] =
		-m[0] * m[5] * m[14]
		+ m[0] * m[6] * m[13]
		+ m[4] * m[1] * m[14]
		- m[4] * m[2] * m[13]
		- m[12] * m[1] * m[6]
		+ m[12] * m[2] * m[5];

	inv[15] =
		m[0] * m[5] * m[10]
		- m[0] * m[6] * m[9]
		- m[4] * m[1] * m[10]
		+ m[4] * m[2] * m[9]
		+ m[8] * m[1] * m[6]
		- m[8] * m[2] * m[5];

	glMultMatrixf(inv);
}