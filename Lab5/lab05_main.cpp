#define _CRT_SECURE_NO_WARNINGS

#include <gl/glut.h>
#include <stdio.h>
#include <windows.h> // idle �Լ� ó���� ���� ���
#include <string.h>
#include <string>
#include <iostream>

using namespace std;

void init();
void draw();
void draw_string(void*, const char*, int, int);
void draw_point(float, float, float);
void draw_line(int);
void draw_triangle();
void draw_quads();
void draw_polygon();
void mouse(int, int, int, int);
void motion(int, int);
void idle();
void keyboard(unsigned char, int, int);
void keyboard_direction(int, int, int);
void main_menu_function(int);
void sub_menu_function_shape(int);
void sub_menu_function_matrix_order(int);
void antialiasing();
void alpha_blending();
void adjusting_angle_rate(double);

int shape_type; // � ���� �׸� ������
int matrixOrder = 1; // ��Ʈ���� ���� // 1�� pivot rotating�� ���� ���� ���� // 2�� �ݴ�

double spin;
double spin_increment = 0.1; // ������ �����ϴ� ���� // adjusting_angle_rate�� ��Ʈ��
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

	int order = glutCreateMenu(sub_menu_function_matrix_order);
	glutAddMenuEntry("Appropriate Pivot-rotating Matrix", 1);
	glutAddMenuEntry("Wrong Pivot-rotating Matrix", 2);
	glutAddMenuEntry("Rotating-first Matrix", 3); // ȸ�� ���� �ϴ� �̻��� ��ȯ

	// ���� �޴�
	glutCreateMenu(main_menu_function);
	glutAddMenuEntry("Quit", 999); // 999�� quit�� ���̵�
	glutAddMenuEntry("Go!", 11); // 11�� go�� ���̵�
	glutAddSubMenu("What you wanna draw?", shape);
	glutAddSubMenu("Change Matrix Order", order);
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
	glClearColor(0.2f, 0.5f, 0.8f, 1.0f); // dark blue

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
	draw_string(GLUT_BITMAP_TIMES_ROMAN_10, "Graphics are cool!!", 250, 470);
	glColor3f(0.0f, 0.0f, 0.0f);
	draw_string(GLUT_BITMAP_TIMES_ROMAN_24, "12161719 Kim, Jin Ho", 20, 50);

	glColor3f(0.0f, 0.0f, 0.0f);
	char pivot_coord[50];
	sprintf(pivot_coord, "pivot : (%d, %d)", pivot_x, pivot_y);
	draw_string(GLUT_BITMAP_TIMES_ROMAN_10, pivot_coord, 20, 465);

	char angle[10];
	sprintf(angle, "%lf degree", spin);
	draw_string(GLUT_BITMAP_TIMES_ROMAN_10, angle, 20, 450);

	if (matrixOrder == 1)
	{
		/* pivot point rotating */
		glTranslatef(pivot_x, pivot_y, 0); // pivot ���� ���� ��ȯ ���
		glRotatef(spin, 0, 0, 1); // z �� ���� spin��ŭ ȸ��
		glTranslatef(-pivot_x, -pivot_y, 0); // pivot�� �������� �̵�
		/* ��������(�ؿ��� ����) ����� */
	}
	else if (matrixOrder == 2) // ȸ�� �����ϴ� // ��ǻ� ȸ���� �ϴ� �̻��� ��ȯ
	{
		/* pivot point rotating�� �Ǽ��� ������ (������ ��Ű�� ����) ��ȯ */
		glTranslatef(pivot_x, pivot_y, 0); // �̵� ������
		glTranslatef(-pivot_x, -pivot_y, 0); // �������� �̵�
		glRotatef(spin, 0, 0, 1); // z �� ���� spin��ŭ ȸ��
		/* ��������(�ؿ��� ����) ����� */
	}
	else
	{
		/* pivot point rotating�� �Ǽ��� ������ (������ ��Ű�� ����) ��ȯ */
		glTranslatef(-pivot_x, -pivot_y, 0); // �ٽ� ������ ��ǥ��ŭ -x -y ���� �̵�
		glRotatef(spin, 0, 0, 1); // z �� ���� spin��ŭ ȸ��
		glTranslatef(pivot_x, pivot_y, 0); // pivot�� �������� ���� �־���
		/* ��������(�ؿ��� ����) ����� */
	}

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
	glColor4f(1.0f, 1.0f, 0.0f, 0.5f);
	// GL_TRIANGLE_FAN	GL_QUADS	GL_QUAD_STRIP	GL_POLYGON
	glBegin(GL_TRIANGLES);
	glVertex2i(250, 100);
	glVertex2i(300, 350);
	glVertex2i(150, 250);
	glEnd();
	/*�� vertex ������ rgb ���� �ο��غ���*/
}

void draw_quads()
{
	glColor4f(1.0f, 1.0f, 0.0f, 0.5f);
	glBegin(GL_QUADS);
	glVertex2i(100, 100);
	glVertex2i(400, 100);
	glVertex2i(400, 400);
	glVertex2i(100, 400);
	glEnd();
}

void draw_polygon()
{
	glColor4f(1.0f, 1.0f, 0.0f, 0.5f);
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

void adjusting_angle_rate(double increment)
{
	spin_increment = increment;
}

void keyboard(unsigned char key, int x, int y)
{
	printf("You pressed %c\n", key);
	if (key == 's') isSpinning = !isSpinning; // spinning ȸ�� ����ġ

	if (key == 'a') // ��Ƽ�ٸ���� ����ġ // ���� ������ �Բ� �ϴ� ���� ����
	{
		antialiase_on = !antialiase_on;
		antialiasing();
		printf("%d\n", antialiase_on);
	}
	if (key == 'b') // ���� ���� ����ġ
	{
		alpha_blending_on = !alpha_blending_on;
		alpha_blending();
		printf("%d\n", alpha_blending_on);
	}
	/* ȸ�� �ӵ� ���� */
	if (key == 'q') adjusting_angle_rate(0.1);
	if (key == 'w') adjusting_angle_rate(0.2);
	if (key == 'e') adjusting_angle_rate(0.3);
	/* ȸ�� ���� ���� */
	if (key == 'r') adjusting_angle_rate(-spin_increment); 

	glutPostRedisplay(); // �ﰢ draw ȣ��
}

void keyboard_direction(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_UP:
		if (!(pivot_y - 5 < 100)) pivot_y -= 5;
		else printf("your pivot cannot go further\n");
		break;
	case GLUT_KEY_DOWN:
		if (!(pivot_y + 5 > 400)) pivot_y += 5;
		else printf("your pivot cannot go further\n");
		break;
	case GLUT_KEY_LEFT:
		if (!(pivot_x - 5 < 100)) pivot_x -= 5;
		else printf("your pivot cannot go further\n");
		break;
	case GLUT_KEY_RIGHT:
		if (!(pivot_x + 5 > 400)) pivot_x += 5;
		else printf("your pivot cannot go further\n");
		break;
	default:
		break;
	}
	glutPostRedisplay();
}

void idle()
{
	if (isSpinning)
	{
		spin = spin + spin_increment;
		if (spin > 360) spin -= 360;
		if (spin < -360) spin += 360; // ������ ȸ�� ��
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

void sub_menu_function_matrix_order(int option)
{
	matrixOrder = option;
	glutPostRedisplay();
}