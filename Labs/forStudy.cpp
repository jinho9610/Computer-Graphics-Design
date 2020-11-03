#include <gl/glut.h>
#include <stdio.h>
#include <iostream>

using namespace std;

void init();
void resize(int, int);
void draw();
void printMatrix();
void check_MODELVIEW();
void check_PROJECTION();

int main(int argc, char** argv)
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(400, 300);
	glutInitWindowPosition(150, 150);
	glutCreateWindow("12161719 김진호");

	init();

	glutDisplayFunc(draw);
	glutReshapeFunc(resize);

	glutMainLoop();

	return 0;
}

void init()
{
	// 색상의 원활한 확인을 위해 배경을 흰색으로 설정
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	// 조명 특성 설정
	GLfloat light_ambient[] = { 0.2f,0.2f,0.2f,1.0f };
	GLfloat light_diffuse[] = { 1.0f,1.0f,1.0f,1.0f };
	GLfloat light_specular[] = { 1.0f,1.0f,1.0f,1.0f };
	// 조명 특성 적용
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	// 재질 특성 설정
	GLfloat material_ambient[] = { 0.7f,0.7f,0.7f,1.0f };
	GLfloat material_diffuse[] = { 1.0f,0.0f,0.0f,1.0f };
	GLfloat material_specular[] = { 0.7f,0.7f,0.7f,1.0f };
	// 재질 특성 적용
	glMaterialfv(GL_FRONT, GL_AMBIENT, material_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, material_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, material_specular);
	glMaterialf(GL_FRONT, GL_SHININESS, 16.0);

	// global ambient light 성분 제거 및 local viewer 활성화
	GLfloat lmodel_ambient[] = { 0.0f,0.0f,0.0f,1.0f };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);

	// 조명 활성화
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	// DEPTH 버퍼 초기화
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);

	// polygon의 법선벡터를 단위 벡터로 계산
	glEnable(GL_NORMALIZE);
}

void draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity(); // 모델뷰 행렬 초기화
	// 시점 위치, 카메라 좌표계
	gluLookAt(2, 2, 0, 0, 0, 0, 0, 1, 0);

	// 조명 위치
	GLfloat light_position[] = { -2.0f,1.0f,0.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	// 점 그리기
	glPointSize(1000);
	glBegin(GL_POINTS);
	glNormal3f(0.0f, 1.0f, 0.0f); // 법선 벡터 지정
	glVertex3f(0.0f, 0.0f, 0.0f); // 점 좌표 지정
	glEnd();

	glFlush();
	glutSwapBuffers();
}

void resize(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	cout << "(1)" << endl;
	printMatrix();
	gluPerspective(90, (float)w / (float)h, 1, 100);
	cout << "(2)" << endl;
	printMatrix();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	cout << "(3)" << endl;
	printMatrix();
	glTranslatef(0.0f, 0.0f, -50.0f);
	cout << "(4)" << endl;
	printMatrix();
	glPushMatrix();
	cout << "(5)" << endl;
	printMatrix();
	glRotatef(30.0f, 0.0f, 0.0f, 1.0f);
	cout << "(6)" << endl;
	printMatrix();
	glPopMatrix();
	cout << "(7)" << endl;
	printMatrix();

}

void printMatrix()
{
	cout << "===============모델링 행렬===============\n";
	check_MODELVIEW();
	cout << "===============투영 행렬===============\n";
	check_PROJECTION();
}

void check_MODELVIEW()
{
	GLfloat m[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, m);
	cout << m[0] << ' ' << m[4] << ' ' << m[8] << ' ' << m[12] << endl;
	cout << m[1] << ' ' << m[5] << ' ' << m[9] << ' ' << m[13] << endl;
	cout << m[2] << ' ' << m[6] << ' ' << m[10] << ' ' << m[14] << endl;
	cout << m[3] << ' ' << m[7] << ' ' << m[11] << ' ' << m[15] << endl;
}

void check_PROJECTION()
{
	GLfloat m[16];
	glGetFloatv(GL_PROJECTION_MATRIX, m);
	cout << m[0] << ' ' << m[4] << ' ' << m[8] << ' ' << m[12] << endl;
	cout << m[1] << ' ' << m[5] << ' ' << m[9] << ' ' << m[13] << endl;
	cout << m[2] << ' ' << m[6] << ' ' << m[10] << ' ' << m[14] << endl;
	cout << m[3] << ' ' << m[7] << ' ' << m[11] << ' ' << m[15] << endl;
}