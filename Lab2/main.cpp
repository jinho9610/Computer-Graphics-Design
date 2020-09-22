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

int main(int argc, char** argv)
{
	int submenu1;

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

	// ���� �޴�
	glutCreateMenu(main_menu_function);
	glutAddMenuEntry("Quit", 999); // 999�� quit�� ���̵�
	glutAddMenuEntry("Go!", 11); // 11�� go�� ���̵�
	glutAddSubMenu("Sub Menu", submenu1);
	glutAttachMenu(GLUT_RIGHT_BUTTON); // � ��ư Ŭ������ �� �޴��� �߰� �� ������ ����

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
	glClearColor(0.0f, 0.0f, 0.5f, 1.0f); // dark blue
}

void draw()
{
	glClear(GL_COLOR_BUFFER_BIT); // ȭ�� Ŭ����
	glColor3f(1.0f, 1.0f, 1.0f); // ���
	glutWireTeapot(0.5f); // �����θ� �׷��� // ũ�� 0.5f
	glFlush(); // �����Ű��
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

void main_menu_function(int option)
{
	printf("Main menu %d has been selected\n", option);
	if (option == 999) exit(0); // "QUIT" Ŭ���ϸ� ���α׷� ����
}

