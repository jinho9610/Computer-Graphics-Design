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
void idle();
void keyboard(unsigned char, int, int);
void keyboard_direction(int, int, int);
void main_menu_function(int);
void sub_menu_function_shape(int);
void antialiasing();
void alpha_blending();

int type;
int point_color; // �׷��� ���� ���� ����ϴ� ����
int line_state; // �׷��� ���� Ÿ���� ����ϴ� ����
int polygon_state; // �׷��� ������ Ÿ���� ����ϴ� ����
int shape_type;
double spin;
bool isSpinning = false; // ȸ�� �÷���
bool antialiase_on = false; // ��Ƽ�ٸ���� �÷���
bool alpha_blending_on = false; // ���� ���� �÷���

int pivot_x = 500 / 2;
int pivot_y = 500 / 2; // �� ��Ʈ�� ���߾ӿ� �ʱ� pivot

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB); // �ִϸ��̼��� ���� ���� ���� ���
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(300, 300); // x, y
	glutCreateWindow("12161719 ����ȣ"); // window �̸�

	init();

	int shape = glutCreateMenu(sub_menu_function_shape);
	glutAddMenuEntry("triangle", 1);
	glutAddMenuEntry("quads", 2);
	glutAddMenuEntry("polygon", 3);

	// ���� �޴�
	glutCreateMenu(main_menu_function);
	glutAddMenuEntry("Quit", 999); // 999�� quit�� ���̵�
	glutAddMenuEntry("Go!", 11); // 11�� go�� ���̵�
	glutAddSubMenu("What you wanna draw?", shape);
	glutAttachMenu(GLUT_RIGHT_BUTTON); // ��Ŭ�� �� �޴� ��Ÿ��

	/*Callback �Լ� ����*/
	glutDisplayFunc(draw);
	glutMouseFunc(mouse); // ���콺 Ŭ�� �̺�Ʈ ó��
	glutMotionFunc(motion); // "Ŭ���� ����"���� ���콺 �������� ���� �� ȣ��Ǵ� ������ ����
	glutIdleFunc(idle);
	glutKeyboardFunc(keyboard); // Ű���� �Է� ó��
	glutIdleFunc(idle); // ��ǻ���� ���޽ð��� ȣ���
	glutSpecialFunc(keyboard_direction);

	/*Looping ����*/
	glutMainLoop();

	return EXIT_SUCCESS;
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

	glMatrixMode(GL_MODELVIEW); // �� �� ��Ʈ���� Ȱ��ȭ
	glLoadIdentity(); // M �ʱ�ȭ

	glColor3f(1.0f, 1.0f, 0);
	draw_string(GLUT_BITMAP_TIMES_ROMAN_24, "Graphics are cool!!", 20, 50);
	glColor3f(0.0f, 0.0f, 0.0f);
	draw_string(GLUT_BITMAP_TIMES_ROMAN_24, "12161719 Kim, Jin Ho", 250, 470);

	/* (250,250) pivot point translation */
	glTranslatef(pivot_x, pivot_y, 0); // x y �������� 250�̵�
	glRotatef(spin, 0, 0, 1); // z �� ���� 60�� ȸ��
	glTranslatef(-250, -250, 0); // x y �������� -250 �̵�
	/* ��������(�ؿ��� ����) ����� */

	switch (shape_type)
	{
	case 1:
		draw_triangle();
		break;
	case 2:
		draw_quads();
		break;
	case 3:
		draw_polygon();
		break;
	}

	glFlush(); // �����Ű��

	glutSwapBuffers(); // back buffer�� front buffer�� ��ü�ϴ� ��
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

	glBegin(line_type);
	glVertex2i(100, 200);
	glVertex2i(400, 200);
	glVertex2i(200, 350);
	glVertex2i(250, 50);
	glEnd();
}

void draw_triangle()
{
	/*���� ������ ���������*/
	glColor4f(1.0f, 1.0f, 0.0f, 0.25f);
	// GL_TRIANGLE_FAN	GL_QUADS	GL_QUAD_STRIP	GL_POLYGON
	glBegin(GL_TRIANGLES);
	glVertex2i(250, 100);
	glVertex2i(300, 350);
	glVertex2i(150, 250);
	glEnd();
	/*�� vertex ������ rgb ���� �ο��غ���*/
}

void draw_triangle_strip()
{
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

void antialiasing()
{
	if (antialiase_on)
	{
		glEnable(GL_POINT_SMOOTH);
		glEnable(GL_LINE_SMOOTH);
		glEnable(GL_POLYGON_SMOOTH);
	}
	else
	{
		glDisable(GL_POINT_SMOOTH);
		glDisable(GL_LINE_SMOOTH);
		glDisable(GL_POLYGON_SMOOTH);
	}
}

void alpha_blending()
{
	if (alpha_blending_on) glEnable(GL_BLEND);
	else glDisable(GL_BLEND);
}

void keyboard(unsigned char key, int x, int y)
{
	printf("You pressed %c\n", key);
	if (key == 's') isSpinning = !isSpinning;

	if (key == 'a')
	{
		antialiase_on = !antialiase_on;
		antialiasing();
		printf("%d\n", antialiase_on);
	}
	if (key == 'b')
	{
		alpha_blending_on = !alpha_blending_on;
		alpha_blending();
		printf("%d\n", alpha_blending_on);
	}

	glutPostRedisplay();
}

void keyboard_direction(int key, int x, int y)
{
	printf("You pressed %c\n", key);
	switch (key)
	{
	case GLUT_KEY_UP:
		printf("key_up\n");
		break;
	case GLUT_KEY_DOWN:
		printf("key_down\n");
		break;
	case GLUT_KEY_LEFT:
		printf("key_left\n");
		break;
	case GLUT_KEY_RIGHT:
		printf("key_right\n");
		break;
	default:
		break;
	}
}

void idle()
{
	if (isSpinning)
	{
		spin = spin + 0.1;
		if (spin > 360) spin -= 360;
		glutPostRedisplay();
	}
}

void main_menu_function(int option)
{
	printf("Main menu %d has been selected\n", option);
	if (option == 999) exit(0); // "QUIT" Ŭ���ϸ� ���α׷� ����
}

void sub_menu_function_shape(int option)
{
	if (option == 1) shape_type = 1;
	else if (option == 2) shape_type = 2;
	else shape_type = 3;
		
	glFlush();
	glutPostRedisplay();
}