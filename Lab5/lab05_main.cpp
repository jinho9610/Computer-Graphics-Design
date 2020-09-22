#include <gl/glut.h>
#include <stdio.h>
#include <windows.h> // idle 함수 처리를 위한 헤더
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
int point_color; // 그려진 점의 색을 기억하는 변수
int line_state; // 그려진 선의 타입을 기억하는 변수
int polygon_state; // 그려진 도형의 타입을 기억하는 변수
int shape_type;
double spin;
bool isSpinning = false; // 회전 플래그
bool antialiase_on = false; // 안티앨리어싱 플래그
bool alpha_blending_on = false; // 알파 블렌딩 플래그

int pivot_x = 500 / 2;
int pivot_y = 500 / 2; // 뷰 포트의 정중앙에 초기 pivot

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB); // 애니메이션을 위한 더블 버퍼 사용
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(300, 300); // x, y
	glutCreateWindow("12161719 김진호"); // window 이름

	init();

	int shape = glutCreateMenu(sub_menu_function_shape);
	glutAddMenuEntry("triangle", 1);
	glutAddMenuEntry("quads", 2);
	glutAddMenuEntry("polygon", 3);

	// 메인 메뉴
	glutCreateMenu(main_menu_function);
	glutAddMenuEntry("Quit", 999); // 999는 quit의 아이디
	glutAddMenuEntry("Go!", 11); // 11은 go의 아이디
	glutAddSubMenu("What you wanna draw?", shape);
	glutAttachMenu(GLUT_RIGHT_BUTTON); // 우클릭 시 메뉴 나타남

	/*Callback 함수 정의*/
	glutDisplayFunc(draw);
	glutMouseFunc(mouse); // 마우스 클릭 이벤트 처리
	glutMotionFunc(motion); // "클릭된 상태"에서 마우스 움직임이 있을 때 호출되는 것으로 보임
	glutIdleFunc(idle);
	glutKeyboardFunc(keyboard); // 키보드 입력 처리
	glutIdleFunc(idle); // 컴퓨터의 유휴시간에 호출됨
	glutSpecialFunc(keyboard_direction);

	/*Looping 시작*/
	glutMainLoop();

	return EXIT_SUCCESS;
}

/*초기화 함수*/
void init()
{	// 배경색 설정
	glClearColor(0.57f, 0.5f, 0.5f, 1.0f); // dark blue

	glMatrixMode(GL_PROJECTION); // 투영 행렬로 설정
	glLoadIdentity(); // 투영 행렬을 항등 행렬로 만듦
	gluOrtho2D(0.0f, 500.0f, 500.0f, 0.0f); // 2차원 viewport 생성 // left right bottom top 순서
	// 앞으로 그리게 되는 2차원 도형은 이 viewport 영역 내에서 정의

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_DST_ALPHA);
	glHint(GL_POINT_SMOOTH, GL_NICEST);
	glHint(GL_LINE_SMOOTH, GL_NICEST);
	glHint(GL_POLYGON_SMOOTH, GL_NICEST);
}

void draw()
{
	glClear(GL_COLOR_BUFFER_BIT); // 색상 버퍼 클리어

	glMatrixMode(GL_MODELVIEW); // 모델 뷰 매트릭스 활성화
	glLoadIdentity(); // M 초기화

	glColor3f(1.0f, 1.0f, 0);
	draw_string(GLUT_BITMAP_TIMES_ROMAN_24, "Graphics are cool!!", 20, 50);
	glColor3f(0.0f, 0.0f, 0.0f);
	draw_string(GLUT_BITMAP_TIMES_ROMAN_24, "12161719 Kim, Jin Ho", 250, 470);

	/* (250,250) pivot point translation */
	glTranslatef(pivot_x, pivot_y, 0); // x y 방향으로 250이동
	glRotatef(spin, 0, 0, 1); // z 축 기준 60도 회전
	glTranslatef(-250, -250, 0); // x y 방향으로 -250 이동
	/* 역순으로(밑에서 부터) 실행됨 */

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

	glFlush(); // 실행시키기

	glutSwapBuffers(); // back buffer를 front buffer랑 교체하는 것
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
	glColor3f(r, g, b); // 점 색상
	glPointSize(4.0f); // 점 크기

	glBegin(GL_POINTS); // 점 그릴 것이라고 선언
	glVertex2i(100, 200); // 점의 x y 좌표
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
	/*면의 색상을 노란색으로*/
	glColor4f(1.0f, 1.0f, 0.0f, 0.25f);
	// GL_TRIANGLE_FAN	GL_QUADS	GL_QUAD_STRIP	GL_POLYGON
	glBegin(GL_TRIANGLES);
	glVertex2i(250, 100);
	glVertex2i(300, 350);
	glVertex2i(150, 250);
	glEnd();
	/*각 vertex 색상을 rgb 각각 부여해볼것*/
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
	if (option == 999) exit(0); // "QUIT" 클릭하면 프로그램 종료
}

void sub_menu_function_shape(int option)
{
	if (option == 1) shape_type = 1;
	else if (option == 2) shape_type = 2;
	else shape_type = 3;
		
	glFlush();
	glutPostRedisplay();
}