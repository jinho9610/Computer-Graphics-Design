#include <gl/glut.h>
#include <stdio.h>
#include <windows.h> // idle �Լ� ó���� ���� ���

void init();
void draw();
void mouse(int, int, int, int);
void motion(int, int);
void keyboard(unsigned char, int, int);
void idle();
void sub_menu_function(int);
void main_menu_function(int);
void sub_menu_color_function(int);
void sub_menu_figure_function(int);

BOOLEAN sw_cube = false;
BOOLEAN sw_sphere = false;

int main(int argc, char** argv)
{
	int submenu1;
	int submenu_color;
	int submenu_figure;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(300, 300); // x, y
	glutCreateWindow("12161719 ����ȣ"); // window �̸�

	init();

	/*popup menu ���� �� �߰�*/
	submenu1 = glutCreateMenu(sub_menu_function);
	glutAddMenuEntry("Sub1", 1); // 1�� submenu�� ���̵�
	glutAddMenuEntry("Sub2", 2); // 2�� submenu�� ���̵�
	glutAddMenuEntry("Sub3", 3); // 3�� submenu�� ���̵�

	// ��� �� ���� ���� �޴�
	submenu_color = glutCreateMenu(sub_menu_color_function);
	glutAddMenuEntry("white", 1);
	glutAddMenuEntry("Red", 2);
	glutAddMenuEntry("Green", 3);
	glutAddMenuEntry("Blue", 4);
	glutAddMenuEntry("magenta", 5);

	// ���� ���� ���� �޴�
	submenu_figure = glutCreateMenu(sub_menu_figure_function);
	glutAddMenuEntry("cube", 1);
	glutAddMenuEntry("sphere", 2);

	// ���� �޴�
	glutCreateMenu(main_menu_function);
	glutAddMenuEntry("Quit", 999); // 999�� quit�� ���̵�
	glutAddMenuEntry("Go!", 11); // 11�� go�� ���̵�
	glutAddSubMenu("Sub Menu", submenu1);
	glutAddSubMenu("Background", submenu_color);
	glutAddSubMenu("Figure", submenu_figure);
	glutAttachMenu(GLUT_RIGHT_BUTTON); // � ��ư Ŭ������ �� �޴��� �߰� �� ������ ����

	/*Callback �Լ� ����*/
	glutDisplayFunc(draw);
	glutMouseFunc(mouse); // ���콺 Ŭ�� �̺�Ʈ ó��
	glutMotionFunc(motion); // "Ŭ���� ����"���� ���콺 �������� ���� �� ȣ��Ǵ� ������ ����
	glutKeyboardFunc(keyboard); // Ű���� �Է� ó��
	// glutIdleFunc(idle); // ��ǻ���� ���޽ð��� ȣ���

	/*Looping ����*/
	glutMainLoop();

	return 0;
}

/*�ʱ�ȭ �Լ�*/
void init()
{	// ���� ����
	glClearColor(0.0f, 0.0f, 0.5f, 1.0f); // dark blue
}

void draw()
{
	if (sw_cube)
	{
		glClear(GL_COLOR_BUFFER_BIT); // ȭ�� Ŭ����
		glColor3f(1.0f, 1.0f, 1.0f); // ���
		glutWireCube(0.5f); // �����θ� �׷��� // ũ�� 0.5f
		glFlush(); // �����Ű��
		glutPostRedisplay(); // �ٷ� draw �Լ� ��ȣ��
	}
	if (sw_sphere)
	{
		glClear(GL_COLOR_BUFFER_BIT); // ȭ�� Ŭ����
		glColor3f(1.0f, 1.0f, 1.0f); // ���
		glutWireSphere(0.5f, 50, 60); // �����θ� �׷��� // ũ�� 0.5f
		glFlush(); // �����Ű��
		glutPostRedisplay();
	}
	if (!sw_cube && !sw_sphere)
	{
		glClear(GL_COLOR_BUFFER_BIT); // ȭ�� Ŭ����
		glColor3f(1.0f, 1.0f, 1.0f); // ���
		glutWireTeapot(0.5f); // �����θ� �׷��� // ũ�� 0.5f
		glFlush(); // �����Ű��
		glutPostRedisplay();
	}
}

void mouse(int button, int state, int x, int y)
{
	// ��ư�� ���ڷ� � ��ư Ŭ������ �ν���
	// GLUT_LEFT_BUTTON = 0, GLUT_MIDDLE_BUTTON = 1, GLUT_RIGHT_BUTTON = 2
	// state�� Ŭ�� ���� // ���콺 Ŭ���ϸ� GLUT_UP = 0, ���콺�� Ŭ�� �����ϸ� GLUT_DOWN = 1
	// x, y�� ��ǥ
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

void idle()
{	// ���� ��ü�� animation ������ �̿�� ��
	printf("%d second has elapsed since the system was started\n", GetTickCount() / 1000);
}

void sub_menu_function(int option)
{
	printf("Submenu %d has been selected\n", option);
}

void sub_menu_color_function(int option)
{
	if (option == 1)
	{
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glFlush();
		glutPostRedisplay();
	}
	else if (option == 2)
	{
		glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
		glFlush();
		glutPostRedisplay();
	}
	else if (option == 3)
	{
		glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
		glFlush();
		glutPostRedisplay();
	}
	else if (option == 4)
	{
		glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
		glFlush();
		glutPostRedisplay();
	}
	else
	{
		glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
		glFlush();
		glutPostRedisplay();
	}
}

void sub_menu_figure_function(int option)
{
	if (option == 1)
	{
		sw_cube = true;
		sw_sphere = false;
	}
	else if (option == 2)
	{
		sw_cube = false;
		sw_sphere = true;
	}
}

void main_menu_function(int option)
{
	printf("Main menu %d has been selected\n", option);
	if (option == 999) exit(0); // "QUIT" Ŭ���ϸ� ���α׷� ����
}

