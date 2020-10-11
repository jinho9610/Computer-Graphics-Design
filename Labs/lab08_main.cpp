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

GLdouble m_RotateAngle = 0.0;

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(100, 100); // x, y
	glutCreateWindow("12161719 김진호"); // window 이름

	init();

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
	GLfloat ambientLight[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	GLfloat diffuseLight[] = { 0.7f, 0.7f, 0.7f, 1.0f };
	GLfloat specularLight[] = { 0.9f, 0.9f, 0.9f, 1.0f };
	GLfloat specularMaterial[] = { 1.0f,1.0f,1.0f,1.0f };
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specularMaterial);
	glMateriali(GL_FRONT, GL_SHININESS, 20);

	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glFrontFace(GL_CW); // SolidTeapot은 GL_CW로 설정하고 그려야 주전자 외피가 앞면이 됨
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	
	glPolygonMode(GL_BACK, GL_LINE);
	glEnable(GL_CULL_FACE); // 뒷 면을 그리지 않는다
}

void draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // draw할 때 마다 depth buffer 비워줌 // 이 때, glClearDepth의 인자 값으로 초기화
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	draw_axis();
	gluLookAt(20, 20, 20, 0, 0, 0, 0, 1, 0);

	glPushMatrix();
	glRotatef(m_RotateAngle, 0, 1, 0);
	GLdouble eq[4] = { 1.0f, 0.0f, 0.0f, 0.0f }; // clipping plane 법선 벡터
	GLdouble another_eq[4] = { 0.0f, 0.0f, 1.0f, 0.0f };
	GLdouble eq_for_cube[4] = { 0.5f, 1.0f, -0.3f, 0.0f };
	glClipPlane(GL_CLIP_PLANE0, eq);
	//glClipPlane(GL_CLIP_PLANE1, another_eq);
	//glClipPlane(GL_CLIP_PLANE2, eq_for_cube);
	glEnable(GL_CLIP_PLANE0);
	//glEnable(GL_CLIP_PLANE1);
	//glEnable(GL_CLIP_PLANE2);

	glutSolidTeapot(4);
	//glutSolidCube(10);

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

void specialKeyboard(int key, int x, int y)
{
	glutPostRedisplay();
}

void idle()
{
	m_RotateAngle += 0.1;
	if (m_RotateAngle > 360) m_RotateAngle -= 360;
	glutPostRedisplay();
}

void resize(int width, int height)
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, (float)width / (float)height, 1, 500);
	glMatrixMode(GL_MODELVIEW);
}