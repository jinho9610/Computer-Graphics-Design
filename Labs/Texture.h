#ifndef TEXTURE_H
#define TEXTURE_H

#include "global.h"
#include "bmpfuncs.h"

GLuint textureMonkey;
GLuint g_nCubeTex;
GLuint tex_cube[6];

void setTextureMapping();
void Cube_Texture(); 

void setTextureMapping() {
	int imgWidth, imgHeight, channels;
	uchar* img = readImageData("./img/FrontTex.bmp", &imgWidth, &imgHeight, &channels);

	int texNum = 1;
	glGenTextures(texNum, &textureMonkey);
	glBindTexture(GL_TEXTURE_2D, textureMonkey);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, imgWidth, imgHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, img);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	//GL_REPEAT ���� �ϳ� ����
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//glTexParameterf(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, TRUE);
	//gluBuild2DMipmaps(GL_TEXTURE_2D, 3, imgWidth, imgHeight, GL_RGB, GL_UNSIGNED_BYTE, img);
}

void Cube_Texture()
{
	uchar* data[6] = { NULL, };
	int width[6] = { 0, }, height[6] = { 0 }, channels[6] = { 0, };
	data[0] = readImageData("./img/right.bmp", &width[0], &height[0], &channels[0]);
	data[1] = readImageData("./img/left.bmp", &width[1], &height[1], &channels[1]);
	data[2] = readImageData("./img/top.bmp", &width[2], &height[2], &channels[2]);
	data[3] = readImageData("./img/bottom.bmp", &width[3], &height[3], &channels[3]);
	data[4] = readImageData("./img/back.bmp", &width[4], &height[4], &channels[4]);
	data[5] = readImageData("./img/front.bmp", &width[5], &height[5], &channels[5]);

	glGenTextures(1, &g_nCubeTex);
	glBindTexture(GL_TEXTURE_CUBE_MAP, g_nCubeTex);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA, width[0], height[0], 0, GL_RGB, GL_UNSIGNED_BYTE, data[0]);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA, width[1], height[1], 0, GL_RGB, GL_UNSIGNED_BYTE, data[1]);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA, width[2], height[2], 0, GL_RGB, GL_UNSIGNED_BYTE, data[2]);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA, width[3], height[3], 0, GL_RGB, GL_UNSIGNED_BYTE, data[3]);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA, width[4], height[4], 0, GL_RGB, GL_UNSIGNED_BYTE, data[4]);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA, width[5], height[5], 0, GL_RGB, GL_UNSIGNED_BYTE, data[5]);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
	glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
}

void TextureMapping_cube()
{
	glGenTextures(6, tex_cube);
	int width = 0, height = 0, channels = 0;
	unsigned char* data = NULL;
	for (int i = 0; i < 6; i++)
	{
		glBindTexture(GL_TEXTURE_2D, tex_cube[i]);
		switch (i)
		{
		case 0:
			data = readImageData("./img/right.bmp", &width, &height, &channels);
			break;
		case 1:
			data = readImageData("./img/right.bmp", &width, &height, &channels);
			break;
		case 2:
			data = readImageData("./img/bottom.bmp", &width, &height, &channels);
			break;
		case 3:
			data = readImageData("./img/TexImage3.bmp", &width, &height, &channels);
			break;
		case 4:
			data = readImageData("./img/front.bmp", &width, &height, &channels);
			break;
		case 5:
			data = readImageData("./img/back.bmp", &width, &height, &channels);
			break;
		}
		glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	}
}
#endif
