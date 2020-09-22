#include <gl/glut.h>
#include <stdio.h>
#include <windows.h> // idle �Լ� ó���� ���� ���
#include <string.h>

void init();
void draw();
void draw_string(void*, const char*, int, int);
void draw_point(float, float, float);
void draw_line(int);
void draw_triangle();
void draw_triangle_strip();
void draw_triangle_fan();
void draw_quads();
void draw_quad_strip();
void draw_polygon();
void mouse(int, int, int, int);
void motion(int, int);
void keyboard(unsigned char, int, int);
void main_menu_function(int);
void sub_menu_function_point(int);
void sub_menu_function_line(int);
void sub_menu_function_polygon(int);

int type;
int point_color; // �׷��� ���� ���� ����ϴ� ����
int line_state; // �׷��� ���� Ÿ���� ����ϴ� ����
int polygon_state; // �׷��� ������ Ÿ���� ����ϴ� ����
bool antialiase_on = false;

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(300, 300); // x, y
	glutCreateWindow("12161719 ����ȣ"); // window �̸�

	init();

	int point = glutCreateMenu(sub_menu_function_point);
	glutAddMenuEntry("White", 1);
	glutAddMenuEntry("Green", 2);
	glutAddMenuEntry("Blue", 3);
	glutAddMenuEntry("all different colors", 4);

	int line = glutCreateMenu(sub_menu_function_line);
	glutAddMenuEntry("Line", 1);
	glutAddMenuEntry("Strip", 2);
	glutAddMenuEntry("Loop Line", 3);
	glutAddMenuEntry("Stipple", 4);

	int polygon = glutCreateMenu(sub_menu_function_polygon);
	glutAddMenuEntry("Triangle", 1);
	glutAddMenuEntry("Triangle Strip", 2);
	glutAddMenuEntry("Triangle Fan", 3);
	glutAddMenuEntry("Quads", 4);
	glutAddMenuEntry("Quad Strip", 5);
	glutAddMenuEntry("Polygon", 6);
	glutAddMenuEntry("GL_CCW", 7); // ������
	glutAddMenuEntry("GL_CW", 8); // �޼�
	glutAddMenuEntry("show Front", 9);
	glutAddMenuEntry("show Back", 10);

	// ���� �޴�
	glutCreateMenu(main_menu_function);
	glutAddMenuEntry("Quit", 999); // 999�� quit�� ���̵�
	glutAddMenuEntry("Go!", 11); // 11�� go�� ���̵�
	glutAddSubMenu("Point", point);
	glutAddSubMenu("Line", line);
	glutAddSubMenu("Polygon", polygon);
	glutAttachMenu(GLUT_RIGHT_BUTTON); // ��Ŭ�� �� �޴� ��Ÿ��

	/*Callback �Լ� ����*/
	glutDisplayFunc(draw);
	glutMouseFunc(mouse); // ���콺 Ŭ�� �̺�Ʈ ó��
	glutMotionFunc(motion); // "Ŭ���� ����"���� ���콺 �������� ���� �� ȣ��Ǵ� ������ ����
	glutKeyboardFunc(keyboard); // Ű���� �Է� ó��
	//glutIdleFunc(idle); // ��ǻ���� ���޽ð��� ȣ���

	/*Looping ����*/
	glutMainLoop();

	return 0;
}

/*�ʱ�ȭ �Լ�*/
void init()
{	// ���� ����
	glClearColor(0.57f, 0.5f, 0.5f, 1.0f); // dark blue

	glMatrixMode(GL_PROJECTION); // ���� ��ķ� ����
	glLoadIdentity(); // ���� ����� �׵� ��ķ� ����
	gluOrtho2D(0.0f, 500.0f, 500.0f, 0.0f); // 2���� viewport ���� // left right bottom top ����
	// ������ �׸��� �Ǵ� 2���� ������ �� viewport ���� ������ ����

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_DST_ALPHA);
	glHint(GL_POINT_SMOOTH, GL_NICEST);
	glHint(GL_LINE_SMOOTH, GL_NICEST);
	glHint(GL_POLYGON_SMOOTH, GL_NICEST);
}

void draw()
{
	glClear(GL_COLOR_BUFFER_BIT); // ���� ���� Ŭ����

	if (type == 1)
	{
		if (point_color == 1) draw_point(1.0f, 1.0f, 1.0f);
		else if (point_color == 2) draw_point(0.0f, 1.0f, 0.0f);
		else if (point_color == 3) draw_point(0.0f, 0.0f, 1.0f);
		else
		{
			glClear(GL_COLOR_BUFFER_BIT);
			glPointSize(4.0f); // �� ũ��

			glBegin(GL_POINTS); // �� �׸� ���̶�� ����
			glColor3f(0.6f, 0.1f, 0.75f);
			glVertex2i(100, 200); // ���� x y ��ǥ
			glColor3f(0.3f, 0.7f, 0.3f);
			glVertex2i(400, 200);
			glColor3f(0.61f, 0.79f, 0.2f);
			glVertex2i(200, 350);
			glColor3f(0.36f, 0.28f, 0.82f);
			glVertex2i(250, 50);
			glEnd();
			//glutPostRedisplay();
		}
	}
	else if (type == 2)
	{
		draw_line(line_state);
	}
	else if (type == 3)
	{
		if (polygon_state == GL_TRIANGLES) draw_triangle();
		else if (polygon_state == GL_TRIANGLE_STRIP) draw_triangle_strip();
		else if (polygon_state == GL_TRIANGLE_FAN) draw_triangle_fan();
		else if (polygon_state == GL_QUADS) draw_quads();
		else if (polygon_state == GL_QUAD_STRIP) draw_quad_strip();
		else if (polygon_state == GL_POLYGON) draw_polygon();
		
	}
	else // ó�� default�� �׷����� ����
	{
		draw_point(1.0f, 0.0f, 0.0f);
	}

	glColor3f(1.0f, 1.0f, 0);
	draw_string(GLUT_BITMAP_TIMES_ROMAN_24, "Graphics are cool!!", 20, 50);
	glColor3f(0.0f, 0.0f, 0.0f);
	draw_string(GLUT_BITMAP_TIMES_ROMAN_24, "12161719 Kim, Jin Ho", 250, 470);

	glFlush(); // �����Ű��
}

void draw_string(void* font, const char* str, int x, int y)
{
	unsigned int i;
	glRasterPos2i(x, y);
	for (i = 0; i < strlen(str); i++)
		glutBitmapCharacter(font, str[i]);
}

void draw_point(float r, float g, float b)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(r, g, b); // �� ����
	glPointSize(4.0f); // �� ũ��

	glBegin(GL_POINTS); // �� �׸� ���̶�� ����
	glVertex2i(100, 200); // ���� x y ��ǥ
	glVertex2i(400, 200);
	glVertex2i(200, 350);
	glVertex2i(250, 50);
	glEnd();
}

void draw_line(int line_type)
{
	glLineWidth(2.0f);
	//glEnable(GL_LINE_STIPPLE);
	glLineStipple(3, 0xAAAA);
	glColor3f(1.0f, 1.0f, 0.0f);

	glBegin(line_type); // GL_LINES, GL_LINE_STRIP ���߼�, GL_LINE_LOOP �����������ձ�
	glVertex2i(100, 200); // ���� x y ��ǥ
	glVertex2i(400, 200);
	glVertex2i(200, 350);
	glVertex2i(250, 50);
	glEnd();

	/*glLineStipple(GLint factor, Glushort patter)���� �� ���� ���� ����
	factor�� 1~256 ������ ���� ������ default�� 1�̴�.
	glEnable(GL_LINE_STIPPLE)�� ���־����*/
}

void draw_triangle()
{
	glClear(GL_COLOR_BUFFER_BIT);
	/*���� ������ ���������*/
	glColor3f(1.0f, 1.0f, 0.0f);
	// GL_TRIANGLE_FAN	GL_QUADS	GL_QUAD_STRIP	GL_POLYGON
	glBegin(GL_TRIANGLES);
	glVertex2i(50, 50);
	glVertex2i(400, 100);
	glVertex2i(250, 450);
	glEnd();
	/*�� vertex ������ rgb ���� �ο��غ���*/
}

void draw_triangle_strip()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0f, 1.0f, 0.0f);
	glBegin(GL_TRIANGLE_STRIP);
	glVertex2i(50, 50);
	glVertex2i(200, 380);
	glVertex2i(360, 100);
	glVertex2i(420, 270);
	glEnd();
}

void draw_triangle_fan()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0f, 1.0f, 0.0f);
	glBegin(GL_TRIANGLE_FAN);
	glVertex2i(250, 450);
	glVertex2i(50, 50);
	glVertex2i(400, 100);
	glVertex2i(200, 380);
	glEnd();
}

void draw_quads()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0f, 1.0f, 0.0f);
	glBegin(GL_QUADS);
	glVertex2i(100, 100);
	glVertex2i(400, 100);
	glVertex2i(400, 400);
	glVertex2i(100, 400);
	glEnd();
}

void draw_quad_strip()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0f, 1.0f, 0.0f);
	glBegin(GL_QUAD_STRIP);
	glVertex2i(100, 100);
	glVertex2i(300, 100);
	glVertex2i(100, 300);
	glVertex2i(300, 300);
	glVertex2i(150, 350);
	glVertex2i(250, 350);
	glEnd();
}

void draw_polygon()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0f, 1.0f, 0.0f);
	glBegin(GL_POLYGON);
	glVertex2i(100, 100);
	glVertex2i(300, 100);
	glVertex2i(300, 300);
	glVertex2i(200, 400);
	glVertex2i(100, 300);
	glEnd();
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
{	// x, y �� Ű���� �Է� ������ ���콺 ��ġ
	printf("You pressed %c\n", key);
}

void main_menu_function(int option)
{
	printf("Main menu %d has been selected\n", option);
	if (option == 999) exit(0); // "QUIT" Ŭ���ϸ� ���α׷� ����
}

void sub_menu_function_point(int option)
{
	type = 1;
	glClear(GL_COLOR_BUFFER_BIT);
	if (option == 1) point_color = 1;
	else if (option == 2) point_color = 2;
	else if (option == 3) point_color = 3;
	else point_color = 4;
	glFlush();
	glutPostRedisplay();
}


void sub_menu_function_line(int option)
{
	glClear(GL_COLOR_BUFFER_BIT);
	if (option == 1)
	{
		type = 2;
		glDisable(GL_LINE_STIPPLE);
		line_state = GL_LINES;
	}
	else if (option == 2)
	{
		type = 2;
		glDisable(GL_LINE_STIPPLE);
		line_state = GL_LINE_STRIP;
	}
	else if (option == 3)
	{
		type = 2;
		glDisable(GL_LINE_LOOP);
		line_state = GL_LINE_LOOP;
	}
	else
	{
		type = 2;
		glLineStipple(1, 0xAAAA);
		glEnable(GL_LINE_STIPPLE);
	}
	glFlush();
	glutPostRedisplay();
}

void sub_menu_function_polygon(int option)
{
	glClear(GL_COLOR_BUFFER_BIT);
	type = 3;
	glDisable(GL_CULL_FACE); // default
	if (option == 1) polygon_state = GL_TRIANGLES;
	else if (option == 2) polygon_state = GL_TRIANGLE_STRIP;
	else if (option == 3) polygon_state = GL_TRIANGLE_FAN;
	else if (option == 4) polygon_state = GL_QUADS;
	else if (option == 5) polygon_state = GL_QUAD_STRIP;
	else if (option == 6) polygon_state = GL_POLYGON;
	else if (option == 7)
	{
		glFrontFace(GL_CCW); // �ݽð� ���� // default
	}
	else if (option == 8)
	{
		glFrontFace(GL_CW); // �ð� ���� 
	}
	else if (option == 9)
	{
		glCullFace(GL_BACK);
		glEnable(GL_CULL_FACE);
	}
	else if (option == 10)
	{
		glCullFace(GL_FRONT);
		glEnable(GL_CULL_FACE);
	}
	glutPostRedisplay();
}