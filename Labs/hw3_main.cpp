#include <gl/glut.h>
#include <stdio.h>

void init();
void resize(int, int);
void draw();

int main(int argc, char** argv)
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(150, 150);
	glutCreateWindow("12161719 ����ȣ");

	init();

	glutDisplayFunc(draw);
	glutReshapeFunc(resize);

	glutMainLoop();

	return 0;
}

void init()
{
	// ������ ��Ȱ�� Ȯ���� ���� ����� ������� ����
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	
	// ���� Ư�� ����
	GLfloat light_ambient[] = { 0.2f,0.2f,0.2f,1.0f };
	GLfloat light_diffuse[] = { 1.0f,1.0f,1.0f,1.0f };
	GLfloat light_specular[] = { 1.0f,1.0f,1.0f,1.0f };
	// ���� Ư�� ����
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	// ���� Ư�� ����
	GLfloat material_ambient[] = { 0.7f,0.7f,0.7f,1.0f };
	GLfloat material_diffuse[] = { 1.0f,0.0f,0.0f,1.0f };
	GLfloat material_specular[] = { 0.7f,0.7f,0.7f,1.0f };
	// ���� Ư�� ����
	glMaterialfv(GL_FRONT, GL_AMBIENT, material_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, material_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, material_specular);
	glMaterialf(GL_FRONT, GL_SHININESS, 16.0);

	// global ambient light ���� ���� �� local viewer Ȱ��ȭ
	GLfloat lmodel_ambient[] = { 0.0f,0.0f,0.0f,1.0f };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);

	// ���� Ȱ��ȭ
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	// DEPTH ���� �ʱ�ȭ
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glFrontFace(GL_CCW);

	// polygon�� �������͸� ���� ���ͷ� ���
	glEnable(GL_NORMALIZE); 
}

void draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity(); // �𵨺� ��� �ʱ�ȭ

	// ���� ��ġ, ī�޶� ��ǥ��
	gluLookAt(2, 2, 0, 0, 0, 0, 0, 1, 0);

	// ���� ��ġ
	GLfloat light_position[] = { -2.0f,1.0f,0.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	// �� �׸���
	glPointSize(1000);
	glBegin(GL_POINTS);
	glNormal3f(0.0f, 1.0f, 0.0f); // ���� ���� ����
	glVertex3f(0.0f, 0.0f, 0.0f); // �� ��ǥ ����
	glEnd();
	
	glFlush();
	glutSwapBuffers();
}

void resize(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, (float)w / (float)h, 1, 500);
	glMatrixMode(GL_MODELVIEW);
}