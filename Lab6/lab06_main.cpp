#define _USE_MATH_DEFINES
#define _CRT_SECURE_NO_WARNINGS

#include <gl/glut.h>
#include <stdio.h>
#include <windows.h> // idle 함수 처리를 위한 헤더
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
double cam_uv = 1; // camera up vector (0, cam_uv, 0)을 결정
bool theta_direction_isReverse = false;

int main(int argc, char** argv)
{
	int submenu1;
	int submenu_color;
	int submenu_figure;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(300, 300); // x, y
	glutCreateWindow("12161719 김진호"); // window 이름

	init();

	glutCreateMenu(main_menu);
	glutAddMenuEntry("Init", 1);
	glutAddMenuEntry("Quit", 999);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	/*Callback 함수 정의*/
	glutDisplayFunc(draw);
	glutMouseFunc(mouse); // 마우스 클릭 이벤트 처리
	glutMotionFunc(motion); // "클릭된 상태"에서 마우스 움직임이 있을 때 호출되는 것으로 보임
	glutKeyboardFunc(keyboard); // 키보드 입력 처리
	glutSpecialFunc(specialKeyboard);
	glutReshapeFunc(resize);

	/*Looping 시작*/
	glutMainLoop();

	return 0;
}

/*초기화 함수*/
void init()
{	// 배경색 설정
	glClearColor(0.2f, 0.5f, 0.8f, 1.0f); // dark blue
}

void draw()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity(); // Local

	getEyePosition(theta, phi);
	gluLookAt(y, z, x, 0, 0, 0, 0, cam_uv, 0);
	cout << "r: " << r << "\tx: " << x << "\ty: " << y << "\tz: " << z
		<< "\ntheta: " << theta << "\tphi: " << phi << "\n\n";

	glColor3f(1, 1, 0);
	glutWireSphere(3, 20, 20);
	draw_axis();

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
		theta_direction_isReverse = true;
		break;
	case GLUT_KEY_DOWN:
		theta += 5.3;
		theta_direction_isReverse = false;
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
	printf("%d second has elapsed since the system was started\n", GetTickCount() / 1000);
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