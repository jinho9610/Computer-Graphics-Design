#define _CRT_SECURE_NO_WARNINGS

#include <gl/glut.h>
#include <stdio.h>
#include <windows.h> // idle 함수 처리를 위한 헤더
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

int shape_type; // 어떤 도형 그릴 것인지
int matrixOrder = 1; // 매트릭스 순서 // 1이 pivot rotating을 위한 정상 순서 // 2는 반대

double spin;
double spin_increment = 0.1; // 각도가 증가하는 정도 // adjusting_angle_rate로 컨트롤
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

	int order = glutCreateMenu(sub_menu_function_matrix_order);
	glutAddMenuEntry("Appropriate Pivot-rotating Matrix", 1);
	glutAddMenuEntry("Wrong Pivot-rotating Matrix", 2);
	glutAddMenuEntry("Rotating-first Matrix", 3); // 회전 먼저 하는 이상한 변환

	// 메인 메뉴
	glutCreateMenu(main_menu_function);
	glutAddMenuEntry("Quit", 999); // 999는 quit의 아이디
	glutAddMenuEntry("Go!", 11); // 11은 go의 아이디
	glutAddSubMenu("What you wanna draw?", shape);
	glutAddSubMenu("Change Matrix Order", order);
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
	glClearColor(0.2f, 0.5f, 0.8f, 1.0f); // dark blue

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
		glTranslatef(pivot_x, pivot_y, 0); // pivot 원상 복구 변환 행렬
		glRotatef(spin, 0, 0, 1); // z 축 기준 spin만큼 회전
		glTranslatef(-pivot_x, -pivot_y, 0); // pivot을 원점으로 이동
		/* 역순으로(밑에서 부터) 실행됨 */
	}
	else if (matrixOrder == 2) // 회전 먼저하는 // 사실상 회전만 하는 이상한 변환
	{
		/* pivot point rotating을 실수로 구현한 (역순을 지키지 못한) 변환 */
		glTranslatef(pivot_x, pivot_y, 0); // 이동 복구ㄴ
		glTranslatef(-pivot_x, -pivot_y, 0); // 원점으로 이동
		glRotatef(spin, 0, 0, 1); // z 축 기준 spin만큼 회전
		/* 역순으로(밑에서 부터) 실행됨 */
	}
	else
	{
		/* pivot point rotating을 실수로 구현한 (역순을 지키지 못한) 변환 */
		glTranslatef(-pivot_x, -pivot_y, 0); // 다시 원래의 좌표만큼 -x -y 방향 이동
		glRotatef(spin, 0, 0, 1); // z 축 기준 spin만큼 회전
		glTranslatef(pivot_x, pivot_y, 0); // pivot이 원점에서 더욱 멀어짐
		/* 역순으로(밑에서 부터) 실행됨 */
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
	glColor4f(1.0f, 1.0f, 0.0f, 0.5f);
	// GL_TRIANGLE_FAN	GL_QUADS	GL_QUAD_STRIP	GL_POLYGON
	glBegin(GL_TRIANGLES);
	glVertex2i(250, 100);
	glVertex2i(300, 350);
	glVertex2i(150, 250);
	glEnd();
	/*각 vertex 색상을 rgb 각각 부여해볼것*/
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
	if (key == 's') isSpinning = !isSpinning; // spinning 회전 스위치

	if (key == 'a') // 안티앨리어싱 스위치 // 알파 블렌딩과 함께 하는 것이 좋음
	{
		antialiase_on = !antialiase_on;
		antialiasing();
		printf("%d\n", antialiase_on);
	}
	if (key == 'b') // 알파 블렌딩 스위치
	{
		alpha_blending_on = !alpha_blending_on;
		alpha_blending();
		printf("%d\n", alpha_blending_on);
	}
	/* 회전 속도 증감 */
	if (key == 'q') adjusting_angle_rate(0.1);
	if (key == 'w') adjusting_angle_rate(0.2);
	if (key == 'e') adjusting_angle_rate(0.3);
	/* 회전 방향 변경 */
	if (key == 'r') adjusting_angle_rate(-spin_increment); 

	glutPostRedisplay(); // 즉각 draw 호출
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
		if (spin < -360) spin += 360; // 역방향 회전 시
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

void sub_menu_function_matrix_order(int option)
{
	matrixOrder = option;
	glutPostRedisplay();
}