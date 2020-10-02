#include <gl/glut.h>
#include <stdio.h>
#include <windows.h>

void init();
void resize(int, int);
void draw();
void draw_triangle();
void draw_axis();
void mouse(int, int, int, int);
void keyboard(unsigned char, int, int);
void resize(int, int);
void alpha_blending();
void printVertex();

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
	glutMouseFunc(mouse);
	glutKeyboardFunc(keyboard);
	glutReshapeFunc(resize);

	glutMainLoop();
}

void init()
{
	glClearColor(0.2f, 0.5f, 0.8f, 1.0f);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	printVertex();
}

void draw()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0, 1, 0);
	gluLookAt(7, 5, 8, 0, 0, 0, 0, 1, 0);

	draw_axis();
	draw_triangle();

	printVertex();

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

void alpha_blending()
{
	if (alpha_blending_on) glEnable(GL_BLEND);
	else glDisable(GL_BLEND);
}

void keyboard(unsigned char key, int x, int y)
{
	if (key == 'b')
	{
		alpha_blending_on = !alpha_blending_on;
		alpha_blending();
		printf("%d\n", alpha_blending_on);
	}
	glutPostRedisplay();
}

void mouse(int button, int state, int x, int y)
{
	printf("Mouse button is clicked! (%d, %d, %d, %d)\n", button, state, x, y);
	glutPostRedisplay();
}

void resize(int width, int height)
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, (float)width / (float)height, 1, 50);
	glMatrixMode(GL_MODELVIEW);
}

void printVertex()
{
	printf("P1 (%f, %f, %f), (%f, %f, %f), (%f, %f, %f)\n\n",
		p1[0], p1[1], p1[2], p2[0], p2[1], p2[2], p3[0], p3[1], p3[2]);
}