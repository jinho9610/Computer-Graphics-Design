#ifndef CALLBACKFUNC_H
#define CALLBACKFUNC_H

#include "global.h"

using namespace std;

void mouse(int, int, int, int);
void motion(int, int);
void keyboard(unsigned char, int, int);
void specialKeyboard(int, int, int);
void idle();
void resize(int, int);
void Picking(GLint, GLint);

const float PI = 3.141592;

GLint pos;
GLfloat theta = 90, phi = 0;
GLfloat r = 7;
GLfloat x, y, z;
GLfloat cam_uv = 1; // camera up vector (0, cam_uv, 0)을 결정
GLfloat g_nGLWidth = 800, g_nGLHeight = 600;
GLint g_nX, g_nY; // 마우스 클릭 위치
GLint g_nSelect; // 선택된 개체 id
GLboolean isBS = false;
GLuint bs_increment;

// w a s d 관련
GLfloat Heli_Pos[3] = { 0.0f, 0.0f, 0.0f };
GLfloat HeliHead_v[3] = { 1.0f, 0.0f, 0.0f };
GLfloat body_angle = 0.0f;

Matrix Trans1, Trans2, Trans3, Left, cur_direction, new_direction;

extern GLfloat Behind_pos[3], Blade_pos[3];

//GLfloat g_pPosition[4] = { Blade_pos[0], Blade_pos[1], Behind_pos[0], Behind_pos[1] };
GLfloat* g_pPosition[4] = { &Behind_pos[0], &Behind_pos[1], &Blade_pos[0], &Blade_pos[1] };
GLfloat tail = 1.0f;

GLfloat blade_speed = 1.0f;

extern void draw();

void mouse(int button, int state, int x, int y)
{
	printf("x:%d\ty:%d\n", x, y);
	y = g_nGLHeight - y;
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		Picking(x, y);
		g_nX = x;
		g_nY = y;
	}
	else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		g_nSelect = 0;
	}
}

void motion(int x, int y)
{
	y = g_nGLHeight - y;
	if (g_nSelect > 0)
	{
		*g_pPosition[(g_nSelect - 1) * 2 + 0] += x - g_nX;
		*g_pPosition[(g_nSelect - 1) * 2 + 1] += y - g_nY;
		g_nX = x;
		g_nY = y;
		glutPostRedisplay();
	}
}

void resize(int width, int height)
{
	g_nGLWidth = width;
	g_nGLHeight = height;
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//glOrtho(-g_nGLWidth / 2.0f, g_nGLWidth, -g_nGLHeight / 2.0f, g_nGLHeight / 2.0f, -10, 10);
	gluPerspective(45, g_nGLWidth / g_nGLHeight, 1, 500);
	glMatrixMode(GL_MODELVIEW);
}

void keyboard(unsigned char key, int x, int y)
{
	printf("You pressed %c\n", key);
	if (key == '0') pos = 0;
	else if (key == '1') pos = 1;
	else if (key == '2') pos = 2;
	else if (key == '3') pos = 3;
	else if (key == '4') pos = 4;

	else if (key == 'y') isBS = !isBS;
	// if (isBS) PlaySound(TEXT("./sound/Heli_Flying.wav"), 0, SND_FILENAME | SND_ASYNC | SND_LOOP);
	else if (key == 'u') bs_increment = 1;
	else if (key == 'i') bs_increment = 2;

	else if (key == 'w')
	{
		Heli_Pos[0] += HeliHead_v[0] * 0.3;
		Heli_Pos[1] += HeliHead_v[1] * 0.3;
		Heli_Pos[2] += HeliHead_v[2] * 0.3;
		cout << Heli_Pos[0] << " " << Heli_Pos[1] << " " << Heli_Pos[2] << endl;
	}
	else if (key == 'a')
	{
		//double trans1[16] = { 1, 0, 0, -Heli_Pos[0], 0,1,0,-Heli_Pos[1], 0, 0, 1,-Heli_Pos[2], 0, 0, 0, 1 };
		//double trans3[16] = { 1, 0, 0, Heli_Pos[0], 0,1,0,Heli_Pos[1], 0, 0, 1,Heli_Pos[2], 0, 0, 0, 1 };
		double trans2[16] = { cos(2.1 * PI / 180), 0, sin(2.1 * PI / 180), 0, 0, 1, 0, 0, -sin(2.1 * PI / 180), 0, cos(2.1 * PI / 180), 0, 0, 0, 0, 1 };
		double cur_dir[4] = { HeliHead_v[0], HeliHead_v[1], HeliHead_v[2], 1 };
		//Trans1.SetValue(trans1, 4, 4);
		Trans2.SetValue(trans2, 4, 4);
		//Trans3.SetValue(trans3, 4, 4);
		//Left = Trans3 * Trans2 * Trans1;
		cur_direction.SetValue(cur_dir, 4, 1);
		new_direction = Trans2 * cur_direction;
		//float size = sqrt(pow(new_direction.GetValue(1, 1), 2) + pow(new_direction.GetValue(1, 2), 2) + pow(new_direction.GetValue(1, 3), 2));
		HeliHead_v[0] = new_direction.GetValue(1, 1);
		HeliHead_v[1] = new_direction.GetValue(1, 2);
		HeliHead_v[2] = new_direction.GetValue(1, 3); // 방향 벡터 변경
		cout << HeliHead_v[0] << " " << HeliHead_v[1] << " " << HeliHead_v[2] << endl;
		//Heli_Pos[0] += HeliHead_v[0] * 0.05;
		//Heli_Pos[1] += HeliHead_v[1] * 0.05;
		//Heli_Pos[2] += HeliHead_v[2] * 0.05;
		body_angle += 2.1;
	}

	else if (key == 'd')
	{
		//double trans1[16] = { 1, 0, 0, -Heli_Pos[0], 0,1,0,-Heli_Pos[1], 0, 0, 1,-Heli_Pos[2], 0, 0, 0, 1 };
		//double trans3[16] = { 1, 0, 0, Heli_Pos[0], 0,1,0,Heli_Pos[1], 0, 0, 1,Heli_Pos[2], 0, 0, 0, 1 };
		double trans2[16] = { cos(-2.1 * PI / 180), 0, sin(-2.1 * PI / 180), 0, 0, 1, 0, 0, -sin(-2.1 * PI / 180), 0, cos(-2.1 * PI / 180), 0, 0, 0, 0, 1 };
		double cur_dir[4] = { HeliHead_v[0], HeliHead_v[1], HeliHead_v[2], 1 };
		//Trans1.SetValue(trans1, 4, 4);
		Trans2.SetValue(trans2, 4, 4);
		//Trans3.SetValue(trans3, 4, 4);
		//Left = Trans3 * Trans2 * Trans1;
		cur_direction.SetValue(cur_dir, 4, 1);
		new_direction = Trans2 * cur_direction;
		//float size = sqrt(pow(new_direction.GetValue(1, 1), 2) + pow(new_direction.GetValue(1, 2), 2) + pow(new_direction.GetValue(1, 3), 2));
		HeliHead_v[0] = new_direction.GetValue(1, 1);
		HeliHead_v[1] = new_direction.GetValue(1, 2);
		HeliHead_v[2] = new_direction.GetValue(1, 3); // 방향 벡터 변경
		cout << HeliHead_v[0] << " " << HeliHead_v[1] << " " << HeliHead_v[2] << endl;
		//Heli_Pos[0] += HeliHead_v[0] * 0.05;
		//Heli_Pos[1] += HeliHead_v[1] * 0.05;
		//Heli_Pos[2] += HeliHead_v[2] * 0.05;
		body_angle -= 2.1;
	}

	if (key == 'r') r++; // farther
	if (key == 'c') r--; // closer
	cout << "r:" << r << endl;
	cout << "pos:" << pos << endl;
	glutPostRedisplay();
}

void specialKeyboard(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_LEFT:
		phi -= 5.3;
		break;
	case GLUT_KEY_RIGHT:
		phi += 5.3;
		break;
	case GLUT_KEY_UP:
		theta -= 5.3;
		break;
	case GLUT_KEY_DOWN:
		theta += 5.3;
		break;
	}

	if (phi > 360) phi -= 360;
	else if (phi < -360) phi += 360;
	if (theta > 360) theta -= 360;
	else if (theta < -360) theta += 360;

	glutPostRedisplay();
}

void idle()
{
	tail += 0.3;
	if(isBS) blade_speed += bs_increment;
	//cout << "tail: " << tail << endl;
	//cout << "blade_speed: " << blade_speed << endl;
	glutPostRedisplay();
}

void Picking(GLint x, GLint y)
{
	printf("\n\nPICKING...\n");
	GLuint selectBuf[100] = { 0, };

	glSelectBuffer(100, selectBuf); // picking한 개체에 대한 정보 저장
	glRenderMode(GL_SELECT); // GL_SELECT를 사용하여 선택 모드 설정
	glMatrixMode(GL_PROJECTION); // 선택의 처리는 시점 좌표계에서 실시하므로 투영 변환 행렬 설정
	glInitNames(); // 
	glPushName(-1); // 개체의 이름 // 임시 -1 push

	glPushMatrix();
	glLoadIdentity();
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport); // 현재 viewport 영역을 viewport 배열에 저장
	gluPickMatrix(x, y, 0.1, 0.1, viewport);

	//glOrtho(-g_nGLWidth / 2.0f, g_nGLWidth / 2.0f, -g_nGLHeight / 2.0f, g_nGLHeight / 2.0f, -100, 100);
	gluPerspective(45, g_nGLWidth / g_nGLHeight, 1, 500);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	draw();

	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glFlush();

	GLint hits = glRenderMode(GL_RENDER); // 마우스 좌클릭으로 선택된 객체의 갯수

	if (hits <= 0) return; // 아무것도 선택되지 않은 경우

	int stack = selectBuf[0]; // name stack에 있는 name의 개수
	GLuint zMin = selectBuf[1]; // 선택된 객체의 최소 z값
	GLuint zMax = selectBuf[2]; // 선택된 객체의 최대 z값
	g_nSelect = selectBuf[3]; // 선택된 객체의 id

	int index = 3 + stack;
	for (int i = 1; i < hits; i++)
	{
		stack = selectBuf[index];

		/* zMin을 이용해서 가장 가까운 것 pick 하기 */
		if (zMin > selectBuf[index + 1])
		{
			zMin = selectBuf[index + 1];
			g_nSelect = selectBuf[index + 3];
		}
		index += 3 + stack;
	}

	printf("hits:%d   name id: %d   zMin: %u   zMax: %u   선택된 개체: %d", hits, stack, zMin, zMax, g_nSelect);
}

#endif