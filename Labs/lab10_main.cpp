#define _USE_MATH_DEFINES
#define _CRT_SECURE_NO_WARNINGS

#define Sun 0
#define Venus 1
#define Earth 2
#define Moon 3
#define Mars 4

#include <gl/glut.h>
#include <stdio.h>
#include <windows.h> // idle 함수 처리를 위한 헤더
#include <cmath>
#include <iostream>

using namespace std;

void init();
void draw();
void draw_string(void*, const char*, GLfloat, GLfloat, GLfloat);
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
void submenu_light_position(int);
void submenu_attenuation(int);

GLint pos = 0;
GLint atten = 0;

float rotate_angle[5];
float revolve_angle[5];

GLfloat light_position[] = { 0.0f, 0.0f, 0.0f, 1.0f };

double theta = 45;
double phi = 45;
double r = 20;
double x = 15, y = 15, z = 15;
double cam_uv = 1; // camera up vector (0, cam_uv, 0)을 결정

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(100, 100); // x, y
	glutCreateWindow("12161719 김진호"); // window 이름

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
{	// 배경색 설정
	glClearColor(0.2f, 0.5f, 0.8f, 1.0f);
	
	// 조명 설정
	GLfloat light_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	GLfloat light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

	// 재질 설정
	GLfloat ambient_Sun[] = { 0.2f,0.2f,0.2f,1.0f };
	GLfloat diffuse_Sun[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	GLfloat specular_Sun[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_Sun);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_Sun); // 아래의 glEnable(GL_COLOR_MATERIAL)이 있으므로 필요없음
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular_Sun);
	glMaterialf(GL_FRONT, GL_SHININESS, 125.0); // shiness factor

	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
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

	getEyePosition(theta, phi);
	gluLookAt(y, z, x, 0, 0, 0, 0, cam_uv, 0);

	glColor3f(0.0f, 0.0f, 0.0f);
	draw_string(GLUT_BITMAP_TIMES_ROMAN_24, "12161719 Kim, Jin Ho", -21, 0, 1);

	GLfloat pos0_position[] = { x, y, z, 1.0 };
	if (pos == 0) glLightfv(GL_LIGHT0, GL_POSITION, pos0_position);

	GLfloat emission[] = { 0.0f,0.0f,0.0f,1.0f };

	draw_axis();
	glPushMatrix(); // 현재 M 매트릭스 저장
	//glPushMatrix(); // 현재 M 매트릭스 저장
	//glPushMatrix(); // 현재 M 매트릭스 저장

	if (pos == 1)
	{
		emission[0] = 1.0f;
		glMaterialfv(GL_FRONT, GL_EMISSION, emission);
	}
	
	glRotatef(rotate_angle[Sun], 0, 1, 0);
	glColor3f(1, 0, 0);
	glutSolidSphere(2, 30, 30); // 태양
	if (pos == 1) // 내가 광원이 되면 그때는 emission이 필요함
	{
		emission[0] = 0.0f;
		glMaterialfv(GL_FRONT, GL_EMISSION, emission);
		glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	}

	if (pos == 2)
	{
		emission[2] = 1.0f;
		glMaterialfv(GL_FRONT, GL_EMISSION, emission);
	}
	glRotatef(revolve_angle[Earth], 0, 1, 0); // 지구 공전
	glTranslatef(8, 0, 0); // 지구 위치 이동
	glRotatef(rotate_angle[Earth], 0, 1, 0); //지구 자전
	glColor3f(0, 0, 1);
	glutSolidSphere(1, 30, 30); // 지구
	if (pos == 2)
	{
		emission[2] = 0.0f;
		glMaterialfv(GL_FRONT, GL_EMISSION, emission);
		glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	}

	if (pos == 3)
	{
		emission[1] = 1.0f;
		emission[2] = 1.0f;
		glMaterialfv(GL_FRONT, GL_EMISSION, emission);
	}
	glRotatef(revolve_angle[Moon], 0, 1, 0); // 달 공전
	glTranslatef(2, 0, 0); // 달 위치 이동
	glRotatef(rotate_angle[Moon], 0, 1, 0); // 달 자전
	glColor3f(0, 1, 1);
	glutSolidSphere(0.5, 30, 30); // 달
	emission[1] = 0;
	if (pos == 3)
	{
		emission[1] = 0.0f;
		emission[2] = 0.0f;
		glMaterialfv(GL_FRONT, GL_EMISSION, emission);
		glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	}
	
	glPopMatrix(); // M 매트릭스 꺼내기
	glPushMatrix();
	GLfloat m[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, m);
	cout << m[0] << ' ' << m[4] << ' ' << m[8] << ' ' << m[12] << endl;
	cout << m[1] << ' ' << m[5] << ' ' << m[9] << ' ' << m[13] << endl;
	cout << m[2] << ' ' << m[6] << ' ' << m[10] << ' ' << m[14] << endl;
	cout << m[3] << ' ' << m[7] << ' ' << m[11] << ' ' << m[15] << endl;

	glRotatef(revolve_angle[Mars], 0, 1, 0);
	glTranslatef(11, 0, 0);
	glRotatef(rotate_angle[Mars], 0, 1, 0);
	glColor3f(1, 0.4, 0.8);
	glutSolidSphere(1.4, 30, 30); // 화성

	if (pos == 4) // world 좌표계의 (10, 5, 3)에 조명이 위치하게 됨
	{
		emission[0] = 0.0f, emission[1] = 0.0f, emission[2] = 0.0f, emission[3] = 1.0f;
		glMaterialfv(GL_FRONT, GL_EMISSION, emission);
		glPopMatrix();
		glTranslatef(10, 5, 3);
		glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	}

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
	if (key == 48)
	{
		pos = 0;
	}
	else if (key == 49)
	{
		pos = 1;
	}
	else if (key == 50)
	{
		pos = 2;
	}
	else if (key == 51)
	{
		pos = 3;
	}
	else if (key == 52)
	{
		pos = 4;
	}
	cout << "현재 키보드 옵션: " << pos << endl;
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
	rotate_angle[Sun] += 0.01; // 태양 자전
	if (rotate_angle[Sun] > 360) rotate_angle[Sun] -= 360;

	rotate_angle[Earth] += 0.01; // 지구 자전
	if (rotate_angle[Earth] > 360) rotate_angle[Earth] -= 360;
	revolve_angle[Earth] += 0.03; // 지구 공전
	if (revolve_angle[Earth] > 360) revolve_angle[Earth] -= 360;

	rotate_angle[Moon] += 0.01; // 달 자전
	if (rotate_angle[Moon] > 360) rotate_angle[Moon] -= 360;
	revolve_angle[Moon] += 0.1; // 달 공전
	if (revolve_angle[Moon] > 360) revolve_angle[Moon] -= 360;

	rotate_angle[Mars] += 0.01; // 화성 자전
	if (rotate_angle[Mars] > 360) rotate_angle[Mars] -= 360;
	revolve_angle[Mars] += 0.05; // 화성 공전
	if (revolve_angle[Mars] > 360) revolve_angle[Mars] -= 360;

	rotate_angle[Venus] += 0.01; // 금성 자전
	if (rotate_angle[Venus] > 360) rotate_angle[Venus] -= 360;
	revolve_angle[Venus] += 0.05; // 금성 공전
	if (revolve_angle[Venus] > 360) revolve_angle[Venus] -= 360;

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