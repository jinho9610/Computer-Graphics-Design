#include <gl/glut.h>
#include <stdio.h>
#include <windows.h>

void init();
void resize(int, int);
void draw();
void draw_triangle();
void draw_axis();
void resize(int, int);

float p1[3] = { 3, -2, 1 };
float p2[3] = { -5, 3, -1 };
float p3[3] = { 0, 3, -1 };

bool alpha_blending_on = false;

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(500, 100);
	glutCreateWindow("12161719 ±Ë¡¯»£ HW2");

	init();

	glutDisplayFunc(draw);
	glutReshapeFunc(resize);

	glutMainLoop();
}

void init()
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void draw()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0, 1, 0);
	gluLookAt(7, 5, 8, 0, 0, 0, 0, 1, 0);

	draw_triangle();

	glTranslatef(0, -1, 0);
	draw_axis();

	glFlush();
	glutSwapBuffers();
}

void draw_triangle()
{
	glColor4f(0.4f, 0.0f, 0.5f, 0.7f);
	glBegin(GL_TRIANGLES);
	glVertex3fv(p1);
	glVertex3fv(p2);
	glVertex3fv(p3);
	glEnd();
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

void resize(int width, int height)
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, (float)width / (float)height, 1, 50);
	glMatrixMode(GL_MODELVIEW);
}
