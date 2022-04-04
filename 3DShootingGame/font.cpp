#include "font.h"
#include "Texture.h"

#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <glm/glm.hpp>

using namespace glm;

// Font struct
static FONTINFO fontInfo;
static FONTCOMMON fontCommon;
static FONTPAGE fontPage;
static FONTCHAR* fontChars;
static int fontCharCount;

// Font texture
static Texture texture;
static GLuint texId;

// Font Internal variables
static GLint lastMatrixMode;
static ivec2 screenSize = { 640, 480 };
static vec2 size;
static float scale;
static vec2 position;
static vec2 origin;

// Function prototype declaration
static int fontLoadFntFile(const char* _fileName);
static int fontGetCharById(int _id);

int fontInit()
{
	scale = 0.6f;

	if (fontLoadFntFile("font/font.fnt") != 0)
		return 1;

	texture.loadBitmapFile("font/font_0.bmp", 0x00, 0x00, 0x00);
	glGenTextures(1, &texId);
	glBindTexture(GL_TEXTURE_2D, texId);
	glTexImage2D(
		GL_TEXTURE_2D,		// GLenum target
		0,					// GLint level
		GL_RGBA,			// GLint internalformat
		texture.getWidth(),	// GLsizei width
		texture.getHeight(),// GLsizei height
		0,					// GLint border
		GL_RGBA,			// GLenum format
		GL_UNSIGNED_BYTE,	// GLenum type
		texture.getTexImage());	 // const GLvoid * pixels
	glTexParameteri(
		GL_TEXTURE_2D,			// GLenum target
		GL_TEXTURE_MAG_FILTER,	// GLenum pname
		GL_NEAREST);			// GLint param
	glTexParameteri(
		GL_TEXTURE_2D,			// GLenum target
		GL_TEXTURE_MIN_FILTER,	// GLenum pname
		GL_NEAREST);			// GLint param

	return 0;
}

void fontRelease()
{
	if (fontChars != NULL)
		free(fontChars);
	texture.deleteTexImage();
}

int fontLoadFntFile(const char* _fileName)
{
	FILE* pFile;
	errno_t err;
	int result = 0;
	unsigned char header[4];
	char blockType;
	int blockSize;

	err = fopen_s(&pFile, _fileName, "rb");

	if (err != 0) {
		printf("The file was %s open failed!\n", _fileName);
		return 1;
	}
	printf("The file was %s opened!\n", _fileName);
	fpos_t fsize = 0;

	fseek(pFile, 0, SEEK_END);
	fgetpos(pFile, &fsize);

	fseek(pFile, 0, SEEK_SET);
	fread(header, 4, 1, pFile);
	if ((header[0] == 'B')
		&& (header[1] == 'M')
		&& (header[2] == 'F')
		) {
		// Read FONTINFO
		fread(&blockType, 1, 1, pFile);
		fread(&blockSize, 4, 1, pFile);
		if (blockSize > sizeof fontInfo)
			result = 1;
		else
		{
			fread(&fontInfo, blockSize, 1, pFile);
		}
		

		// Read FONTCOMMON
		fread(&blockType, 1, 1, pFile);
		fread(&blockSize, 4, 1, pFile);
		if (blockSize > sizeof fontInfo)
			result = 1;
		else
		{
			fread(&fontCommon, blockSize, 1, pFile);
		}

		// Read FONTPAGE
		fread(&blockType, 1, 1, pFile);
		fread(&blockSize, 4, 1, pFile);
		if (blockSize > sizeof fontPage)
			result =  1;
		else
		{
			fread(&fontPage, blockSize, 1, pFile);
		}
		
		// Read FONTCHAR
		fread(&blockType, 1, 1, pFile);
		fread(&blockSize, 4, 1, pFile);
		fontCharCount = blockSize / sizeof FONTCHAR;
		fontChars = (FONTCHAR*)malloc(blockSize);
		if (fontChars != NULL)
			fread(fontChars, sizeof FONTCHAR, fontCharCount, pFile);
		else
			result = 1;

	} else {
		result = 1;
	}

	fclose(pFile);

	return result;
}

void fontBegin()
{
	glGetIntegerv(
		GL_MATRIX_MODE,		// GLenum pname
		&lastMatrixMode);	// GLint *params

	glPushAttrib(GL_ALL_ATTRIB_BITS);	//GLbitfield mask

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	gluOrtho2D(
		0, screenSize.x,
		screenSize.y, 0);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glDisable(GL_DEPTH_TEST);	//GLenum cap
	glDisable(GL_LIGHTING);		//GLenum cap

	// Enable texture
	glEnable(GL_TEXTURE_2D);	// GLenum cap
	glEnable(GL_BLEND);			// GLenum cap
	glBlendFunc(
		GL_SRC_ALPHA,			//GLenum sfactor
		GL_ONE_MINUS_SRC_ALPHA);//GLenum dfactor
}

void fontEnd()
{
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glMatrixMode(lastMatrixMode);

	glPopAttrib();
}

void fontPosition(float _x, float _y)
{
	origin = position = { _x, _y };
}

void fontScale(float _scale)
{
	scale = _scale;
}

short fontGetLineHeight()
{
	return fontCommon.lineHeight;
}

void fontDraw(const char* format, ...)
{
	/*
	glColor3ub(0xff, 0xff, 0xff);
	glBegin(GL_QUADS);
	{
		glTexCoord2f(0, 0);
		glVertex2f(position.x, position.y);

		glTexCoord2f(0, 1);
		glVertex2f(position.x, position.y + size.y);

		glTexCoord2f(1, 1);
		glVertex2f(position.x + size.x, position.y + size.y);

		glTexCoord2f(1, 0);
		glVertex2f(position.x + size.x, position.y);
	}
	glEnd();
		*/
	va_list ap;
	char str[256];
	char* p;

	va_start(ap, format);
	vsprintf_s(str, format, ap);
	va_end(ap);

	glBindTexture(
		GL_TEXTURE_2D,	// GLenum target
		texId);		// GLuint texture

	position = origin;

	for (p = str; (*p != '\0') && (*p != '\n'); p++) {
		int index = fontGetCharById(*p);
		if (index == -1)
			index = fontGetCharById(0x3f);// ?

		glBegin(GL_QUADS);
		{
			size = { fontChars[index].width ,fontChars[index].height};

			float leftX = (float)fontChars[index].x / (float)texture.getWidth();
			float leftY = (float)fontChars[index].y / (float)texture.getHeight();
			float rightX = (float)(fontChars[index].x + fontChars[index].width) / (float)texture.getWidth();
			float rightY = (float)(fontChars[index].y + fontChars[index].height) / (float)texture.getHeight();


			// ç∂è„
			glTexCoord2f(leftX, leftY);
			glVertex2f(position.x + (fontChars[index].xoffset * scale), position.y + (fontChars[index].yoffset * scale));

			// ç∂â∫
			glTexCoord2f(leftX, rightY);
			glVertex2f(position.x + (fontChars[index].xoffset * scale), position.y + (size.y + fontChars[index].yoffset) * scale);

			// âEâ∫
			glTexCoord2f(rightX, rightY);
			glVertex2f(position.x + (size.x + fontChars[index].xoffset) * scale, position.y + (size.y + fontChars[index].yoffset) * scale);

			// âEè„
			glTexCoord2f(rightX, leftY);
			glVertex2f(position.x + (size.x + fontChars[index].xoffset) * scale, position.y + (fontChars[index].yoffset * scale));
		}
		glEnd();

		position.x += fontChars[index].xadvance * scale;
	}

	if (*p == '\n') {
		origin.y = position.y;
		fontDraw(++p);
	}

}

int fontGetCharById(int _id)
{
	int result = -1;

	for (int i = 0; i < fontCharCount; i++) {
		if (fontChars[i].id == _id)
			result = i;
	}

	return result;
	
}