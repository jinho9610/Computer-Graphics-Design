#include <gl/glut.h>
#include <stdio.h>
#include <windows.h> // idle 함수 처리를 위한 헤더

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
	glutCreateWindow("12161719 김진호"); // window 이름

	init();

	/*popup menu 생성 및 추가*/
	submenu1 = glutCreateMenu(sub_menu_function);
	glutAddMenuEntry("Sub1", 1); // 1은 submenu의 아이디
	glutAddMenuEntry("Sub2", 2); // 2은 submenu의 아이디
	glutAddMenuEntry("Sub3", 3); // 3은 submenu의 아이디

	// 메인 메뉴
	glutCreateMenu(main_menu_function);
	glutAddMenuEntry("Quit", 999); // 999는 quit의 아이디
	glutAddMenuEntry("Go!", 11); // 11은 go의 아이디
	glutAddSubMenu("Sub Menu", submenu1);
	glutAttachMenu(GLUT_RIGHT_BUTTON); // 어떤 버튼 클릭했을 때 메뉴가 뜨게 할 것인지 설정

	/*Callback 함수 정의*/
	glutDisplayFunc(draw);
	glutMouseFunc(mouse); // 마우스 클릭 이벤트 처리
	glutMotionFunc(motion); // "클릭된 상태"에서 마우스 움직임이 있을 때 호출되는 것으로 보임
	glutKeyboardFunc(keyboard); // 키보드 입력 처리
	//glutIdleFunc(idle); // 컴퓨터의 유휴시간에 호출됨

	/*Looping 시작*/
	glutMainLoop();
		
	return 0;
}

/*초기화 함수*/
void init()
{	// 배경색 설정
	glClearColor(0.0f, 0.0f, 0.5f, 1.0f); // dark blue
}

void draw()
{
	glClear(GL_COLOR_BUFFER_BIT); // 화면 클리어
	glColor3f(1.0f, 1.0f, 1.0f); // 흰색
	glutWireTeapot(0.5f); // 선으로만 그려짐 // 크기 0.5f
	glFlush(); // 실행시키기
}

void mouse(int button, int state, int x, int y)
{
	// 버튼은 숫자로 어떤 버튼 클릭인지 인식함
	// GLUT_LEFT_BUTTON = 0, GLUT_MIDDLE_BUTTON = 1, GLUT_RIGHT_BUTTON = 2
	// state는 클릭 상태 // 마우스 클릭하면 GLUT_UP = 0, 마우스가 클릭 해제하면 GLUT_DOWN = 1
	// x, y는 좌표
	printf("Mouse button is clicked! (%d, %d, %d, %d)\n", button, state, x, y);
}

void motion(int x, int y)
{
	printf("Mouse is moving! (%d, %d)\n", x, y);
}

void keyboard(unsigned char key, int x, int y)
{	// x, y 는 키보드 입력 시점의 마우스 위치
	printf("You pressed %c\n", key);
}

void idle()
{	// 추후 물체의 animation 구현에 이용될 것
	printf("%d second has elapsed since the system was started\n", GetTickCount() / 1000);
}

void sub_menu_function(int option)
{
	printf("Submenu %d has been selected\n", option);
}

void main_menu_function(int option)
{
	printf("Main menu %d has been selected\n", option);
	if (option == 999) exit(0); // "QUIT" 클릭하면 프로그램 종료
}

