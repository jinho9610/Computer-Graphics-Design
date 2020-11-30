#define _USE_MATH_DEFINES
#define _CRT_SECURE_NO_WARNINGS

#include "global.h"
#include "ObjParser.h"
#include "bmpfuncs.h"
#include "callbackFunc.h"
#include "Texture.h"

using namespace std;

void init();
void setting_light();
void setting_quadric();

void draw();
void draw_obj(ObjParser*);
void draw_obj_with_texture(ObjParser*);
void draw_string(void*, const char*, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat);
void DrawSkybox();
void draw_axis();

void getEyePosition(double, double);
double getRadian(double);

void printMatrix(GLfloat*);

void main_menu(int);
void print_manual();
void InverseMat();

ObjParser* monkey;
ObjParser* Blades, *Front, *Behind, *BladeAxis, *LandingBar, *TailBlade, *Heli;

extern GLfloat theta, phi;
extern GLfloat x, y, x;
extern GLfloat r;
extern GLfloat cam_uv;
extern GLuint textureMonkey;
extern GLuint g_nCubeTex;
extern GLfloat g_nGLWidth, g_nGLHeight;
extern GLfloat tail;
extern GLfloat blade_speed;
extern GLfloat Heli_Pos[3];
extern GLfloat body_angle;

GLfloat Behind_pos[3] = { 0.0f, 0.0f, 0.0f };
GLfloat Blade_pos[3] = { 0.722443f, 2.09993f, 0.00572f };


MCI_OPEN_PARMS m_mciOPenParam;
MCI_PLAY_PARMS m_mciPlayParms;
DWORD m_dwDeviceID;
MCI_OPEN_PARMS mciOpen;
MCI_PLAY_PARMS mciPlay;
int dwID;

int main(int argc, char** argv)
{
	/*mciOpen.lpstrElementName = TEXT("./sound/Heli_Flying.MP3");
	mciOpen.lpstrDeviceType = TEXT("mpegVideo");
	mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE,
		(DWORD)(LPVOID)&mciOpen);
	dwID = mciOpen.wDeviceID;
	mciSendCommand(dwID, MCI_PLAY, MCI_DGV_PLAY_REPEAT, (DWORD)(LPVOID)&m_mciPlayParms);*/

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(g_nGLWidth, g_nGLHeight);
	glutInitWindowPosition(100, 100); // x, y
	glutCreateWindow("12161719 김진호 Mini Project"); // window 이름

	init();

	/*Callback 함수 정의*/
	glutDisplayFunc(draw);
	glutMouseFunc(mouse); // 마우스 클릭 이벤트 처리
	glutMotionFunc(motion); // "클릭된 상태"에서 마우스 움직임이 있을 때 호출되는 것으로 보임
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(specialKeyboard);
	glutReshapeFunc(resize);
	glutIdleFunc(idle);

	/*Looping 시작*/
	glutMainLoop();

	return 0;
}

/*초기화 함수*/
void init()
{
	glClearColor(0.2f, 0.5f, 0.8f, 1.0f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);

	setting_light();
	Cube_Texture();
	setTextureMapping();
	monkey = new ObjParser("monkey.obj");
	Blades = new ObjParser("./obj/Blade2.obj");
	Behind = new ObjParser("./obj/Behind2.obj");
	Front = new ObjParser("./obj/Front2.obj");
	BladeAxis = new ObjParser("./obj/BladeAxis2.obj");
	LandingBar = new ObjParser("./obj/LandingBar2.obj");
	TailBlade = new ObjParser("./obj/TailBlade2.obj");
	Heli = new ObjParser("./obj/b.obj");
}

void draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	getEyePosition(theta, phi);
	gluLookAt(y, z, x, 0, 0, 0, 0, cam_uv, 0);

	DrawSkybox();

	glPushMatrix();
	draw_axis();
	glPopMatrix();

	glColor3f(1.0f, 1.0f, 1.0f);

	glTranslatef(Heli_Pos[0], Heli_Pos[1], Heli_Pos[2]);
	glRotatef(body_angle, 0, 1, 0);

	glLoadName(1);
	glPushMatrix();
	glTranslatef(Blade_pos[0], Blade_pos[1], 0);
	glRotatef(blade_speed, 0, 1, 0);
	glTranslatef(-Blade_pos[0], -Blade_pos[1], 0);
	draw_obj_with_texture(Blades);
	glPopMatrix();

	glLoadName(2);
	glPushMatrix();
	draw_obj_with_texture(BladeAxis);
	//draw_obj(BladeAxis);
	glPopMatrix();

	glLoadName(3);
	glPushMatrix();
	draw_obj_with_texture(Front);
	//draw_obj(Front);
	glPopMatrix();

	glLoadName(4);
	glPushMatrix();
	draw_obj_with_texture(Behind);
	glPopMatrix();

	glLoadName(5);
	glPushMatrix();
	draw_obj_with_texture(TailBlade);
	//draw_obj(TailBlade);
	glPopMatrix();

	glLoadName(6);
	glPushMatrix();
	draw_obj_with_texture(LandingBar);
	//draw_obj(LandingBar);
	glPopMatrix();

	glFlush();
	glutSwapBuffers();
}

void draw_obj_with_texture(ObjParser* objParser)
{
	//glDisable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);	// texture 색 보존을 위한 enable
	glBindTexture(GL_TEXTURE_2D, textureMonkey);
	glBegin(GL_TRIANGLES);
	for (unsigned int n = 0; n < objParser->getFaceSize(); n += 3) {
		glTexCoord2f(objParser->textures[objParser->textureIdx[n] - 1].x,
			objParser->textures[objParser->textureIdx[n] - 1].y);
		glNormal3f(objParser->normal[objParser->normalIdx[n] - 1].x,
			objParser->normal[objParser->normalIdx[n] - 1].y,
			objParser->normal[objParser->normalIdx[n] - 1].z);
		glVertex3f(objParser->vertices[objParser->vertexIdx[n] - 1].x,
			objParser->vertices[objParser->vertexIdx[n] - 1].y,
			objParser->vertices[objParser->vertexIdx[n] - 1].z);

		glTexCoord2f(objParser->textures[objParser->textureIdx[n + 1] - 1].x,
			objParser->textures[objParser->textureIdx[n + 1] - 1].y);
		glNormal3f(objParser->normal[objParser->normalIdx[n + 1] - 1].x,
			objParser->normal[objParser->normalIdx[n + 1] - 1].y,
			objParser->normal[objParser->normalIdx[n + 1] - 1].z);
		glVertex3f(objParser->vertices[objParser->vertexIdx[n + 1] - 1].x,
			objParser->vertices[objParser->vertexIdx[n + 1] - 1].y,
			objParser->vertices[objParser->vertexIdx[n + 1] - 1].z);

		glTexCoord2f(objParser->textures[objParser->textureIdx[n + 2] - 1].x,
			objParser->textures[objParser->textureIdx[n + 2] - 1].y);
		glNormal3f(objParser->normal[objParser->normalIdx[n + 2] - 1].x,
			objParser->normal[objParser->normalIdx[n + 2] - 1].y,
			objParser->normal[objParser->normalIdx[n + 2] - 1].z);
		glVertex3f(objParser->vertices[objParser->vertexIdx[n + 2] - 1].x,
			objParser->vertices[objParser->vertexIdx[n + 2] - 1].y,
			objParser->vertices[objParser->vertexIdx[n + 2] - 1].z);
	}
	glEnd();
	glEnable(GL_BLEND);
}

void draw_obj(ObjParser* objParser)
{
	glBegin(GL_TRIANGLES);
	for (unsigned int n = 0; n < objParser->getFaceSize(); n += 3)
	{
		glNormal3f(objParser->normal[objParser->normalIdx[n] - 1].x,
			objParser->normal[objParser->normalIdx[n] - 1].y,
			objParser->normal[objParser->normalIdx[n] - 1].z);
		glVertex3f(objParser->vertices[objParser->vertexIdx[n] - 1].x,
			objParser->vertices[objParser->vertexIdx[n] - 1].y,
			objParser->vertices[objParser->vertexIdx[n] - 1].z);

		glNormal3f(objParser->normal[objParser->normalIdx[n + 1] - 1].x,
			objParser->normal[objParser->normalIdx[n + 1] - 1].y,
			objParser->normal[objParser->normalIdx[n + 1] - 1].z);
		glVertex3f(objParser->vertices[objParser->vertexIdx[n + 1] - 1].x,
			objParser->vertices[objParser->vertexIdx[n + 1] - 1].y,
			objParser->vertices[objParser->vertexIdx[n + 1] - 1].z);

		glNormal3f(objParser->normal[objParser->normalIdx[n + 2] - 1].x,
			objParser->normal[objParser->normalIdx[n + 2] - 1].y,
			objParser->normal[objParser->normalIdx[n + 2] - 1].z);
		glVertex3f(objParser->vertices[objParser->vertexIdx[n + 2] - 1].x,
			objParser->vertices[objParser->vertexIdx[n + 2] - 1].y,
			objParser->vertices[objParser->vertexIdx[n + 2] - 1].z);
	}
	glEnd();
}

void draw_axis()
{
	glLineWidth(3);
	glBegin(GL_LINES);
	glColor3f(1, 0, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(4, 0, 0);

	glColor3f(0, 1, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 4, 0);

	glColor3f(0, 0, 1);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, 4);
	glEnd();
	glLineWidth(1);
}


void DrawSkybox()
{
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_CUBE_MAP);
	glBindTexture(GL_TEXTURE_CUBE_MAP, g_nCubeTex);
	GLfloat g_nSkySize = 50.0f;

	glBegin(GL_QUADS);
	// +x
	glTexCoord3f(1.0, -1.0, -1.0); glVertex3f(g_nSkySize, -g_nSkySize, -g_nSkySize);
	glTexCoord3f(1.0, -1.0, 1.0); glVertex3f(g_nSkySize, -g_nSkySize, g_nSkySize);
	glTexCoord3f(1.0, 1.0, 1.0); glVertex3f(g_nSkySize, g_nSkySize, g_nSkySize);
	glTexCoord3f(1.0, 1.0, -1.0); glVertex3f(g_nSkySize, g_nSkySize, -g_nSkySize);

	// -x
	glTexCoord3f(-1.0, -1.0, -1.0); glVertex3f(-g_nSkySize, -g_nSkySize, -g_nSkySize);
	glTexCoord3f(-1.0, -1.0, 1.0); glVertex3f(-g_nSkySize, -g_nSkySize, g_nSkySize);
	glTexCoord3f(-1.0, 1.0, 1.0); glVertex3f(-g_nSkySize, g_nSkySize, g_nSkySize);
	glTexCoord3f(-1.0, 1.0, -1.0); glVertex3f(-g_nSkySize, g_nSkySize, -g_nSkySize);

	// +y
	glTexCoord3f(1.0, 1.0, 1.0); glVertex3f(g_nSkySize, g_nSkySize, g_nSkySize);
	glTexCoord3f(-1.0, 1.0, 1.0); glVertex3f(-g_nSkySize, g_nSkySize, g_nSkySize);
	glTexCoord3f(-1.0, 1.0, -1.0); glVertex3f(-g_nSkySize, g_nSkySize, -g_nSkySize);
	glTexCoord3f(1.0, 1.0, -1.0); glVertex3f(g_nSkySize, g_nSkySize, -g_nSkySize);

	// -y
	glTexCoord3f(1.0, -1.0, 1.0); glVertex3f(g_nSkySize, -g_nSkySize, g_nSkySize);
	glTexCoord3f(-1.0, -1.0, 1.0); glVertex3f(-g_nSkySize, -g_nSkySize, g_nSkySize);
	glTexCoord3f(-1.0, -1.0, -1.0); glVertex3f(-g_nSkySize, -g_nSkySize, -g_nSkySize);
	glTexCoord3f(1.0, -1.0, -1.0); glVertex3f(g_nSkySize, -g_nSkySize, -g_nSkySize);

	// +z
	glTexCoord3f(1.0, -1.0, 1.0); glVertex3f(g_nSkySize, -g_nSkySize, g_nSkySize);
	glTexCoord3f(-1.0, -1.0, 1.0); glVertex3f(-g_nSkySize, -g_nSkySize, g_nSkySize);
	glTexCoord3f(-1.0, 1.0, 1.0); glVertex3f(-g_nSkySize, g_nSkySize, g_nSkySize);
	glTexCoord3f(1.0, 1.0, 1.0); glVertex3f(g_nSkySize, g_nSkySize, g_nSkySize);

	// -z
	glTexCoord3f(1.0, -1.0, -1.0); glVertex3f(g_nSkySize, -g_nSkySize, -g_nSkySize);
	glTexCoord3f(-1.0, -1.0, -1.0); glVertex3f(-g_nSkySize, -g_nSkySize, -g_nSkySize);
	glTexCoord3f(-1.0, 1.0, -1.0); glVertex3f(-g_nSkySize, g_nSkySize, -g_nSkySize);
	glTexCoord3f(1.0, 1.0, -1.0); glVertex3f(g_nSkySize, g_nSkySize, -g_nSkySize);

	glEnd();

	glEnable(GL_LIGHTING);
}

void setting_light()
{
	// 조명 설정
	GLfloat light_ambient[] = { 1.0f,1.0f,1.0f,1.0f };
	GLfloat light_diffuse[] = { 1.0f,1.0f,1.0f,1.0f };
	GLfloat light_speuclar[] = { 1.0f,1.0f,1.0f,1.0f };
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_speuclar);

	GLfloat light_position[] = { 10.0f,10.0f,10.0f,0.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	GLfloat mat_speuclar[] = { 1.0f,1.0f,1.0f,1.0f };
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_speuclar);
	glMaterialf(GL_FRONT, GL_SHININESS, 64);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);
}

void draw_string(void* font, const char* str, GLfloat x, GLfloat y, GLfloat r, GLfloat g, GLfloat b)
{
	glPushAttrib(GL_LIGHTING_BIT);
	glDisable(GL_LIGHTING);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(-5, 5, -5, 5);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glColor3f(r, g, b);
	glRasterPos3f(x, y, 0);
	for (unsigned int i = 0; i < strlen(str); i++)
		glutBitmapCharacter(font, str[i]);
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopAttrib();
}

void getEyePosition(double a, double b)
{
	if (a < 0)
	{
		if (a > -180) cam_uv = -1;
		else cam_uv = 1; //a < -180

		a = -a;
		b += 180;
	}
	else
	{
		if (a > 180)
		{
			cam_uv = -1;
			a = 360 - a;
			b += 180;
			//cout << b << endl;
		}
		else cam_uv = 1;
	}
	//cout << "cam_uv: " << cam_uv << endl;
	if (b > 360) b -= 360;
	else if (b < -360) b += 360;

	x = r * sin(getRadian(a)) * cos(getRadian(b));
	y = r * sin(getRadian(a)) * sin(getRadian(b));
	z = r * cos(getRadian(a));
}

double getRadian(double angle)
{
	return angle * M_PI / 180;
}

void sub_menu_function(int option)
{
	printf("Submenu %d has been selected\n", option);
}

void main_menu(int option)
{
	if (option == 999) exit(0);

	glutPostRedisplay();
}

void print_manual()
{
	cout << "++++++keyboard++++++\n0: 광원이 시점에 위치\n1 : 광원이 큐브에 위치\n2: 광원이 사이다에 위치\n3: 광원이 지구에 위치\n4: 광원 (10, 5, 3)에 위치\n\nr: Zoom Out\tc: Zoom-in\n";
	cout << "++++++++++++++++++++\n";
}

void printMatrix(GLfloat* m)
{
	cout << m[0] << ' ' << m[4] << ' ' << m[8] << ' ' << m[12] << endl;
	cout << m[1] << ' ' << m[5] << ' ' << m[9] << ' ' << m[13] << endl;
	cout << m[2] << ' ' << m[6] << ' ' << m[10] << ' ' << m[14] << endl;
	cout << m[3] << ' ' << m[7] << ' ' << m[11] << ' ' << m[15] << endl;
}

void InverseMat() {
	GLfloat m[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, m);

	GLfloat inv[16];

	inv[0] =
		m[5] * m[10] * m[15]
		- m[5] * m[11] * m[14]
		- m[9] * m[6] * m[15]
		+ m[9] * m[7] * m[14]
		+ m[13] * m[6] * m[11]
		- m[13] * m[7] * m[10];

	inv[1] =
		-m[1] * m[10] * m[15]
		+ m[1] * m[11] * m[14]
		+ m[9] * m[2] * m[15]
		- m[9] * m[3] * m[14]
		- m[13] * m[2] * m[11]
		+ m[13] * m[3] * m[10];

	inv[2] =
		m[1] * m[6] * m[15]
		- m[1] * m[7] * m[14]
		- m[5] * m[2] * m[15]
		+ m[5] * m[3] * m[14]
		+ m[13] * m[2] * m[7]
		- m[13] * m[3] * m[6];

	inv[3] =
		-m[1] * m[6] * m[11]
		+ m[1] * m[7] * m[10]
		+ m[5] * m[2] * m[11]
		- m[5] * m[3] * m[10]
		- m[9] * m[2] * m[7]
		+ m[9] * m[3] * m[6];

	inv[4] =
		-m[4] * m[10] * m[15]
		+ m[4] * m[11] * m[14]
		+ m[8] * m[6] * m[15]
		- m[8] * m[7] * m[14]
		- m[12] * m[6] * m[11]
		+ m[12] * m[7] * m[10];

	inv[5] =
		m[0] * m[10] * m[15]
		- m[0] * m[11] * m[14]
		- m[8] * m[2] * m[15]
		+ m[8] * m[3] * m[14]
		+ m[12] * m[2] * m[11]
		- m[12] * m[3] * m[10];

	inv[6] =
		-m[0] * m[6] * m[15]
		+ m[0] * m[7] * m[14]
		+ m[4] * m[2] * m[15]
		- m[4] * m[3] * m[14]
		- m[12] * m[2] * m[7]
		+ m[12] * m[3] * m[6];

	inv[7] =
		m[0] * m[6] * m[11]
		- m[0] * m[7] * m[10]
		- m[4] * m[2] * m[11]
		+ m[4] * m[3] * m[10]
		+ m[8] * m[2] * m[7]
		- m[8] * m[3] * m[6];

	inv[8] =
		m[4] * m[9] * m[15]
		- m[4] * m[11] * m[13]
		- m[8] * m[5] * m[15]
		+ m[8] * m[7] * m[13]
		+ m[12] * m[5] * m[11]
		- m[12] * m[7] * m[9];

	inv[9] =
		-m[0] * m[9] * m[15]
		+ m[0] * m[11] * m[13]
		+ m[8] * m[1] * m[15]
		- m[8] * m[3] * m[13]
		- m[12] * m[1] * m[11]
		+ m[12] * m[3] * m[9];

	inv[10] =
		m[0] * m[5] * m[15]
		- m[0] * m[7] * m[13]
		- m[4] * m[1] * m[15]
		+ m[4] * m[3] * m[13]
		+ m[12] * m[1] * m[7]
		- m[12] * m[3] * m[5];

	inv[11] =
		-m[0] * m[5] * m[11]
		+ m[0] * m[7] * m[9]
		+ m[4] * m[1] * m[11]
		- m[4] * m[3] * m[9]
		- m[8] * m[1] * m[7]
		+ m[8] * m[3] * m[5];

	inv[12] =
		-m[4] * m[9] * m[14]
		+ m[4] * m[10] * m[13]
		+ m[8] * m[5] * m[14]
		- m[8] * m[6] * m[13]
		- m[12] * m[5] * m[10]
		+ m[12] * m[6] * m[9];

	inv[13] =
		m[0] * m[9] * m[14]
		- m[0] * m[10] * m[13]
		- m[8] * m[1] * m[14]
		+ m[8] * m[2] * m[13]
		+ m[12] * m[1] * m[10]
		- m[12] * m[2] * m[9];

	inv[14] =
		-m[0] * m[5] * m[14]
		+ m[0] * m[6] * m[13]
		+ m[4] * m[1] * m[14]
		- m[4] * m[2] * m[13]
		- m[12] * m[1] * m[6]
		+ m[12] * m[2] * m[5];

	inv[15] =
		m[0] * m[5] * m[10]
		- m[0] * m[6] * m[9]
		- m[4] * m[1] * m[10]
		+ m[4] * m[2] * m[9]
		+ m[8] * m[1] * m[6]
		- m[8] * m[2] * m[5];

	glMultMatrixf(inv);
}