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
void TextureMapping_cube();
void TextureMapping_cider();
void TextureMapping_earth();
void setting_light();
void setting_quadric();
void draw();
void draw_string(void*, const char*, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat);
void draw_axis();
void draw_cube();
void draw_cider(GLfloat, GLfloat, GLfloat);
void draw_earth(GLfloat, GLfloat, GLfloat);
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
double cam_uv = 1; // camera up vector (0, cam_uv, 0)을 결정

GLuint tex_cube[6];
GLuint tex_cider[3];
GLuint tex_earth[1];

GLUquadricObj* qobj = gluNewQuadric();
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
	print_manual();
	// 배경색 설정
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);

	// Quadric 오브젝트의 텍스처 매핑 활성화
	setting_quadric();

	// 텍스처 맵핑
	TextureMapping_cube(); // 큐브
	TextureMapping_cider(); // 사이다
	TextureMapping_earth(); // 지구
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); // 조명 효과를 적용할 수 있도록 GL_MODULATE
	glEnable(GL_TEXTURE_2D);

	// 조명 설정
	setting_light();
}

void draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	getEyePosition(theta, phi);
	gluLookAt(y, z, x, 0, 0, 0, 0, cam_uv, 0);

	

	GLfloat pos0_position[] = { y, z, x, 1.0 };
	if (pos == 0) glLightfv(GL_LIGHT0, GL_POSITION, pos0_position);

	glPushMatrix();
	draw_cube();
	if (pos == 1) glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glPopMatrix();

	glPushMatrix();
	draw_cider(4.4, 2, 3.7);
	glPopMatrix();
	
	draw_earth(-3, 0.3, 4.1);

	if (pos == 4)
	{
		glTranslatef(10, 5, 3);
		glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	}

	glFlush();
	glutSwapBuffers();
}

void TextureMapping_cube()
{
	glGenTextures(6, tex_cube);
	int width = 0, height = 0, channels = 0;
	unsigned char* data = NULL;
	for (int i = 0; i < 6; i++)
	{
		glBindTexture(GL_TEXTURE_2D, tex_cube[i]);
		switch (i)
		{
		case 0:
			data = readImageData("./img/TexImage0.bmp", &width, &height, &channels);
			break;
		case 1:
			data = readImageData("./img/TexImage1.bmp", &width, &height, &channels);
			break;
		case 2:
			data = readImageData("./img/TexImage2.bmp", &width, &height, &channels);
			break;
		case 3:
			data = readImageData("./img/TexImage3.bmp", &width, &height, &channels);
			break;
		case 4:
			data = readImageData("./img/TexImage4.bmp", &width, &height, &channels);
			break;
		case 5:
			data = readImageData("./img/TexImage5.bmp", &width, &height, &channels);
			break;
		}
		glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	}
}

void TextureMapping_cider()
{
	glGenTextures(3, tex_cider);
	int width = 0, height = 0, channels = 0;
	unsigned char* data = NULL;
	for (int i = 0; i < 3; i++)
	{
		glBindTexture(GL_TEXTURE_2D, tex_cider[i]);
		switch (i)
		{
		case 0:
			data = readImageData("./img/CIDER_T.bmp", &width, &height, &channels);
			break;
		case 1:
			data = readImageData("./img/CIDER_S.bmp", &width, &height, &channels);
			break;
		case 2:
			data = readImageData("./img/CIDER_B.bmp", &width, &height, &channels);
			break;
		}
		glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	}
}

void TextureMapping_earth()
{
	glGenTextures(1, tex_earth);
	int width = 0, height = 0, channels = 0;
	unsigned char* data = NULL;

	glBindTexture(GL_TEXTURE_2D, tex_earth[0]);

	data = readImageData("./img/EARTH.bmp", &width, &height, &channels);

	glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}

void draw_cube()
{
	glColor3f(1.0f, 1.0f, 1.0f);
	// left
	glBindTexture(GL_TEXTURE_2D, tex_cube[0]);
	glBegin(GL_QUADS);
	glNormal3f(-1, 0, 0);
	glTexCoord2f(0, 0); glVertex3f(-1.0f, 1.0f, -1.0f);
	glTexCoord2f(1, 0); glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(1, 1); glVertex3f(-1.0f, -1.0f, 1.0f);
	glTexCoord2f(0, 1); glVertex3f(-1.0f, 1.0f, 1.0f);
	glEnd();

	// right
	glBindTexture(GL_TEXTURE_2D, tex_cube[1]);
	glBegin(GL_QUADS);
	glNormal3f(1, 0, 0);
	glTexCoord2f(0, 0); glVertex3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(1, 0); glVertex3f(1.0f, -1.0f, 1.0f);
	glTexCoord2f(1, 1); glVertex3f(1.0f, -1.0f, -1.0f);
	glTexCoord2f(0, 1); glVertex3f(1.0f, 1.0f, -1.0f);
	glEnd();

	// bottom
	glBindTexture(GL_TEXTURE_2D, tex_cube[2]);
	glBegin(GL_QUADS);
	glNormal3f(0, -1, 0);
	glTexCoord2f(0, 0); glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(1, 0); glVertex3f(1.0f, -1.0f, -1.0f);
	glTexCoord2f(1, 1); glVertex3f(1.0f, -1.0f, 1.0f);
	glTexCoord2f(0, 1); glVertex3f(-1.0f, -1.0f, 1.0f);
	glEnd();

	// top
	glBindTexture(GL_TEXTURE_2D, tex_cube[3]);
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);
	glTexCoord2f(0, 0); glVertex3f(-1.0f, 1.0f, 1.0f);
	glTexCoord2f(1, 0); glVertex3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(1, 1); glVertex3f(1.0f, 1.0f, -1.0f);
	glTexCoord2f(0, 1); glVertex3f(-1.0f, 1.0f, -1.0f);
	glEnd();

	// near
	glBindTexture(GL_TEXTURE_2D, tex_cube[4]);
	glBegin(GL_QUADS);
	glNormal3f(0, 0, 1);
	glTexCoord2f(0, 0); glVertex3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(1, 0); glVertex3f(-1.0f, 1.0f, 1.0f);
	glTexCoord2f(1, 1); glVertex3f(-1.0f, -1.0f, 1.0f);
	glTexCoord2f(0, 1); glVertex3f(1.0f, -1.0f, 1.0f);
	glEnd();

	// far
	glBindTexture(GL_TEXTURE_2D, tex_cube[5]);
	glBegin(GL_QUADS);
	glNormal3f(0, 0, -1);
	glTexCoord2f(0, 0); glVertex3f(1.0f, 1.0f, -1.0f);
	glTexCoord2f(1, 0); glVertex3f(-1.0f, 1.0f, -1.0f);
	glTexCoord2f(1, 1); glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(0, 1); glVertex3f(1.0f, -1.0f, -1.0f);
	glEnd();
}

void draw_cider(GLfloat cider_x, GLfloat cider_y, GLfloat cider_z)
{
	glColor3f(1.0f, 1.0f, 1.0f);

	glPushMatrix();
	glTranslatef(cider_x, cider_y, cider_z);
	glRotatef(90.0f, 1, 0, 0);
	glBindTexture(GL_TEXTURE_2D, tex_cider[1]);
	gluCylinder(qobj, 1.0f, 1.0f, 4.0f, 16, 4);
	if (pos == 2) glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glPopMatrix();

	glPushMatrix();
	glTranslatef(cider_x, cider_y, cider_z);
	glRotatef(90.0f, 1, 0, 0);
	glBindTexture(GL_TEXTURE_2D, tex_cider[0]);
	gluDisk(qobj, 0.0f, 1.0f, 16, 4);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(cider_x, cider_y - 4.0f, cider_z);
	glRotatef(90.0f, 1, 0, 0);
	glBindTexture(GL_TEXTURE_2D, tex_cider[2]);
	gluDisk(qobj, 0.0f, 1.0f, 16, 4);
	glPopMatrix();
}

void draw_earth(GLfloat earth_x, GLfloat earth_y, GLfloat earth_z)
{
	glColor3f(1.0f, 1.0f, 1.0f);

	glPushMatrix();
	glTranslatef(earth_x, earth_y, earth_z);
	glBindTexture(GL_TEXTURE_2D, tex_earth[0]);
	gluSphere(qobj, 1.5f, 24, 24);
	if (pos == 3) glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glPopMatrix();
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
	gluQuadricTexture(qobj, GL_TRUE); // 텍스쳐 맵을 적용한다.
	gluQuadricDrawStyle(qobj, GLU_FILL); // Solid 형태로 객체를 그린다.
	gluQuadricNormals(qobj, GLU_SMOOTH); // 면이 부드럽게 그려지도록 한다.
	gluQuadricOrientation(qobj, GLU_OUTSIDE); // 법선 벡터가 객체 외부를 향하도록 한다.
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
	glMaterialf(GL_FRONT, GL_SHININESS, 140.0); // shiness factor

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
	void TextureMapping_cube();
	void TextureMapping_cider();
	void TextureMapping_earth();
	void setting_light();
	void setting_quadric();
	void draw();
	void draw_string(void*, const char*, GLfloat, GLfloat, GLfloat);
	void draw_axis();
	void draw_cube();
	void draw_cider(GLfloat, GLfloat, GLfloat);
	void draw_earth(GLfloat, GLfloat, GLfloat);
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
	double cam_uv = 1; // camera up vector (0, cam_uv, 0)을 결정

	GLuint tex_cube[6];
	GLuint tex_cider[3];
	GLuint tex_earth[1];

	GLUquadricObj* qobj = gluNewQuadric();
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
		print_manual();
		// 배경색 설정
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClearDepth(1.0f);
		glEnable(GL_DEPTH_TEST);

		// Quadric 오브젝트의 텍스처 매핑 활성화
		setting_quadric();

		// 텍스처 맵핑
		TextureMapping_cube(); // 큐브
		TextureMapping_cider(); // 사이다
		TextureMapping_earth(); // 지구
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); // 조명 효과를 적용할 수 있도록 GL_MODULATE
		glEnable(GL_TEXTURE_2D);

		// 조명 설정
		setting_light();
	}

	void draw()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		getEyePosition(theta, phi);
		gluLookAt(y, z, x, 0, 0, 0, 0, cam_uv, 0);

		GLfloat pos0_position[] = { y, z, x, 1.0 };
		if (pos == 0) glLightfv(GL_LIGHT0, GL_POSITION, pos0_position);

		glPushMatrix();
		draw_cube();
		if (pos == 1) glLightfv(GL_LIGHT0, GL_POSITION, light_position);
		glPopMatrix();

		glPushMatrix();
		draw_cider(4.4, 2, 3.7);
		glPopMatrix();

		draw_earth(-3, 0.3, 4.1);

		if (pos == 4)
		{
			glTranslatef(10, 5, 3);
			glLightfv(GL_LIGHT0, GL_POSITION, light_position);
		}

		glFlush();
		glutSwapBuffers();
	}

	void TextureMapping_cube()
	{
		glGenTextures(6, tex_cube);
		int width = 0, height = 0, channels = 0;
		unsigned char* data = NULL;
		for (int i = 0; i < 6; i++)
		{
			glBindTexture(GL_TEXTURE_2D, tex_cube[i]);
			switch (i)
			{
			case 0:
				data = readImageData("./img/TexImage0.bmp", &width, &height, &channels);
				break;
			case 1:
				data = readImageData("./img/TexImage1.bmp", &width, &height, &channels);
				break;
			case 2:
				data = readImageData("./img/TexImage2.bmp", &width, &height, &channels);
				break;
			case 3:
				data = readImageData("./img/TexImage3.bmp", &width, &height, &channels);
				break;
			case 4:
				data = readImageData("./img/TexImage4.bmp", &width, &height, &channels);
				break;
			case 5:
				data = readImageData("./img/TexImage5.bmp", &width, &height, &channels);
				break;
			}
			glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		}
	}

	void TextureMapping_cider()
	{
		glGenTextures(3, tex_cider);
		int width = 0, height = 0, channels = 0;
		unsigned char* data = NULL;
		for (int i = 0; i < 3; i++)
		{
			glBindTexture(GL_TEXTURE_2D, tex_cider[i]);
			switch (i)
			{
			case 0:
				data = readImageData("./img/CIDER_T.bmp", &width, &height, &channels);
				break;
			case 1:
				data = readImageData("./img/CIDER_S.bmp", &width, &height, &channels);
				break;
			case 2:
				data = readImageData("./img/CIDER_B.bmp", &width, &height, &channels);
				break;
			}
			glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		}
	}

	void TextureMapping_earth()
	{
		glGenTextures(1, tex_earth);
		int width = 0, height = 0, channels = 0;
		unsigned char* data = NULL;

		glBindTexture(GL_TEXTURE_2D, tex_earth[0]);

		data = readImageData("./img/EARTH.bmp", &width, &height, &channels);

		glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	}

	void draw_cube()
	{
		glColor3f(1.0f, 1.0f, 1.0f);
		// left
		glBindTexture(GL_TEXTURE_2D, tex_cube[0]);
		glBegin(GL_QUADS);
		glNormal3f(-1, 0, 0);
		glTexCoord2f(0, 0); glVertex3f(-1.0f, 1.0f, -1.0f);
		glTexCoord2f(1, 0); glVertex3f(-1.0f, -1.0f, -1.0f);
		glTexCoord2f(1, 1); glVertex3f(-1.0f, -1.0f, 1.0f);
		glTexCoord2f(0, 1); glVertex3f(-1.0f, 1.0f, 1.0f);
		glEnd();

		// right
		glBindTexture(GL_TEXTURE_2D, tex_cube[1]);
		glBegin(GL_QUADS);
		glNormal3f(1, 0, 0);
		glTexCoord2f(0, 0); glVertex3f(1.0f, 1.0f, 1.0f);
		glTexCoord2f(1, 0); glVertex3f(1.0f, -1.0f, 1.0f);
		glTexCoord2f(1, 1); glVertex3f(1.0f, -1.0f, -1.0f);
		glTexCoord2f(0, 1); glVertex3f(1.0f, 1.0f, -1.0f);
		glEnd();

		// bottom
		glBindTexture(GL_TEXTURE_2D, tex_cube[2]);
		glBegin(GL_QUADS);
		glNormal3f(0, -1, 0);
		glTexCoord2f(0, 0); glVertex3f(-1.0f, -1.0f, -1.0f);
		glTexCoord2f(1, 0); glVertex3f(1.0f, -1.0f, -1.0f);
		glTexCoord2f(1, 1); glVertex3f(1.0f, -1.0f, 1.0f);
		glTexCoord2f(0, 1); glVertex3f(-1.0f, -1.0f, 1.0f);
		glEnd();

		// top
		glBindTexture(GL_TEXTURE_2D, tex_cube[3]);
		glBegin(GL_QUADS);
		glNormal3f(0, 1, 0);
		glTexCoord2f(0, 0); glVertex3f(-1.0f, 1.0f, 1.0f);
		glTexCoord2f(1, 0); glVertex3f(1.0f, 1.0f, 1.0f);
		glTexCoord2f(1, 1); glVertex3f(1.0f, 1.0f, -1.0f);
		glTexCoord2f(0, 1); glVertex3f(-1.0f, 1.0f, -1.0f);
		glEnd();

		// near
		glBindTexture(GL_TEXTURE_2D, tex_cube[4]);
		glBegin(GL_QUADS);
		glNormal3f(0, 0, 1);
		glTexCoord2f(0, 0); glVertex3f(1.0f, 1.0f, 1.0f);
		glTexCoord2f(1, 0); glVertex3f(-1.0f, 1.0f, 1.0f);
		glTexCoord2f(1, 1); glVertex3f(-1.0f, -1.0f, 1.0f);
		glTexCoord2f(0, 1); glVertex3f(1.0f, -1.0f, 1.0f);
		glEnd();

		// far
		glBindTexture(GL_TEXTURE_2D, tex_cube[5]);
		glBegin(GL_QUADS);
		glNormal3f(0, 0, -1);
		glTexCoord2f(0, 0); glVertex3f(1.0f, 1.0f, -1.0f);
		glTexCoord2f(1, 0); glVertex3f(-1.0f, 1.0f, -1.0f);
		glTexCoord2f(1, 1); glVertex3f(-1.0f, -1.0f, -1.0f);
		glTexCoord2f(0, 1); glVertex3f(1.0f, -1.0f, -1.0f);
		glEnd();
	}

	void draw_cider(GLfloat cider_x, GLfloat cider_y, GLfloat cider_z)
	{
		glColor3f(1.0f, 1.0f, 1.0f);

		glPushMatrix();
		glTranslatef(cider_x, cider_y, cider_z);
		glRotatef(90.0f, 1, 0, 0);
		glBindTexture(GL_TEXTURE_2D, tex_cider[1]);
		gluCylinder(qobj, 1.0f, 1.0f, 4.0f, 16, 4);
		if (pos == 2) glLightfv(GL_LIGHT0, GL_POSITION, light_position);

		glPopMatrix();

		glPushMatrix();
		glTranslatef(cider_x, cider_y, cider_z);
		glRotatef(90.0f, 1, 0, 0);
		glBindTexture(GL_TEXTURE_2D, tex_cider[0]);
		gluDisk(qobj, 0.0f, 1.0f, 16, 4);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(cider_x, cider_y - 4.0f, cider_z);
		glRotatef(90.0f, 1, 0, 0);
		glBindTexture(GL_TEXTURE_2D, tex_cider[2]);
		gluDisk(qobj, 0.0f, 1.0f, 16, 4);
		glPopMatrix();
	}

	void draw_earth(GLfloat earth_x, GLfloat earth_y, GLfloat earth_z)
	{
		glColor3f(1.0f, 1.0f, 1.0f);

		glPushMatrix();
		glTranslatef(earth_x, earth_y, earth_z);
		glBindTexture(GL_TEXTURE_2D, tex_earth[0]);
		gluSphere(qobj, 1.5f, 24, 24);
		if (pos == 3) glLightfv(GL_LIGHT0, GL_POSITION, light_position);
		glPopMatrix();
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
		gluQuadricTexture(qobj, GL_TRUE); // 텍스쳐 맵을 적용한다.
		gluQuadricDrawStyle(qobj, GLU_FILL); // Solid 형태로 객체를 그린다.
		gluQuadricNormals(qobj, GLU_SMOOTH); // 면이 부드럽게 그려지도록 한다.
		gluQuadricOrientation(qobj, GLU_OUTSIDE); // 법선 벡터가 객체 외부를 향하도록 한다.
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
		glMaterialf(GL_FRONT, GL_SHININESS, 140.0); // shiness factor

		glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
		glEnable(GL_COLOR_MATERIAL);

		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
	}

	void draw_string(void* font, const char* str, GLfloat x, GLfloat y, GLfloat z)
	{
		unsigned int i;
		glRasterPos3i(x, y, z);
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