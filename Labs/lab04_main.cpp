#include <gl/glut.h>
#include <stdio.h>
#include <windows.h> // idle 함수 처리를 위한 헤더
#include <string.h>
#include <vector>
#include <algorithm>

using namespace std;

void init();
void draw();
void draw_string(void*, const char*, int, int);
void draw_point();
void draw_line();
void draw_triangle();
void mouse(int, int, int, int);
void motion(int, int);
void keyboard(unsigned char, int, int);
void main_menu_function(int);
void antialiasing();

int type = 1;
int point_color; // 그려진 점의 색을 기억하는 변수
int line_state; // 그려진 선의 타입을 기억하는 변수
int polygon_state; // 그려진 도형의 타입을 기억하는 변수
bool antialiase_on = false;
bool alpha_blending_on = false;

vector<pair<int, int> > point_coordinates;
vector<pair<int, int> > line_coordinates;
vector<pair<int, int> > triangle_vertex;

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(300, 300); // x, y
	glutCreateWindow("12161719 김진호"); // window 이름

	init();

	glutAddMenuEntry("Line", 1);
	glutAddMenuEntry("Strip", 2);
	glutAddMenuEntry("Loop Line", 3);
	glutAddMenuEntry("Stipple", 4);


	// 메인 메뉴
	glutCreateMenu(main_menu_function);
	glutAddMenuEntry("Quit", 999); // 999는 quit의 아이디
	glutAddMenuEntry("Go!", 11); // 11은 go의 아이디
	glutAddMenuEntry("Point", 1);
	glutAddMenuEntry("Line", 2);
	glutAddMenuEntry("Triangle", 3);
	glutAttachMenu(GLUT_RIGHT_BUTTON); // 우클릭 시 메뉴 나타남

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
{
	// 배경색 설정
	glClearColor(0.57f, 0.5f, 0.5f, 1.0f);
	//glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glMatrixMode(GL_PROJECTION); // 투영 행렬로 설정
	glLoadIdentity(); // 투영 행렬을 항등 행렬로 만듦
	gluOrtho2D(0.0f, 500.0f, 500.0f, 0.0f); // 2차원 viewport 생성 // left right bottom top 순서
	// 앞으로 그리게 되는 2차원 도형은 이 viewport 영역 내에서 정의 

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
}

void draw()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(1.0f, 1.0f, 0.0f);
	draw_string(GLUT_BITMAP_TIMES_ROMAN_10, "Graphics are cool!!", 20, 50);
	glColor3f(0.0f, 0.0f, 0.0f);
	draw_string(GLUT_BITMAP_TIMES_ROMAN_24, "12161719 Kim, Jin Ho", 250, 470);

	if (point_coordinates.size() != 0) draw_point();
	if (line_coordinates.size() >= 2) draw_line();
	if (triangle_vertex.size() >= 3) draw_triangle();

	glFlush(); // 실행시키기
}

void draw_string(void* font, const char* str, int x, int y)
{
	unsigned int i;
	glRasterPos2i(x, y);
	for (i = 0; i < strlen(str); i++)
		glutBitmapCharacter(font, str[i]);
}

void draw_point()
{
	glColor3f(0.0f, 0.0f, 1.0f);
	glPointSize(5.0f);
	for (int i = 0; i <= point_coordinates.size(); i++)
	{
		glBegin(GL_POINTS);
		glVertex2i(point_coordinates[i].first, point_coordinates[i].second);
		glEnd();
	}
}

void draw_line()
{
	glColor3f(1.0f, 0.0f, 0.0f);
	glLineWidth(2.5f);
	for (int i = 0; i < line_coordinates.size() - line_coordinates.size() % 2; i += 2)
	{
		glBegin(GL_LINES);
		glVertex2i(line_coordinates[i].first, line_coordinates[i].second);
		glVertex2i(line_coordinates[i + 1].first, line_coordinates[i + 1].second);
		glEnd();
	}
}

void draw_triangle()
{
	/*면의 색상을 노란색으로*/
	glColor4f(1.0f, 1.0f, 0.0f, 0.5f);
	for (int i = 0; i < triangle_vertex.size() - triangle_vertex.size() % 3; i += 3)
	{
		glBegin(GL_TRIANGLES);
		glVertex2i(triangle_vertex[i].first, triangle_vertex[i].second);
		glVertex2i(triangle_vertex[i + 1].first, triangle_vertex[i + 1].second);
		glVertex2i(triangle_vertex[i + 2].first, triangle_vertex[i + 2].second);
		glEnd();
	}
}

void mouse(int button, int state, int x, int y)
{
	printf("Mouse button is clicked! (%d, %d, %d, %d)\n", button, state, x, y);
	if (type == 1 && state == 0)
	{
		point_coordinates.push_back(make_pair(x, y));
		printf("점 좌표 개수 : %d\n", point_coordinates.size());
	}
	else if (type == 2 && state == 0)
	{
		line_coordinates.push_back(make_pair(x, y));
		printf("선분 좌표 개수 : %d\n", line_coordinates.size());
	}
	else if (type == 3 && state == 0)
	{
		triangle_vertex.push_back(make_pair(x, y));
		printf("삼각형 좌표 개수 : %d\n", triangle_vertex.size());
	}
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
{	// x, y 는 키보드 입력 시점의 마우스 위치
	printf("You pressed %c\n", key);
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

void main_menu_function(int option)
{
	printf("Main menu %d has been selected\n", option);
	if (option == 999) exit(0); // "QUIT" 클릭하면 프로그램 종료
	else if (option == 1)
	{
		type = 1;
	}
	else if (option == 2)
	{
		type = 2;
	}
	else if (option == 3)
	{
		type = 3;
	}
}