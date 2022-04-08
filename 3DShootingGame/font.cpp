#include "font.h"
#include "Texture.h"
#include "Header.h"

#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <glm/glm.hpp>
#include <time.h>

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
static ivec2 screenSize;
static vec2 size;
static float scale;
static vec2 position;
static vec2 origin;

// Function prototype declaration
static int fontLoadFntFile(const char* _fileName);
static int fontGetCharById(unsigned int _id);
static inline int binarySearch(unsigned int _key, int _min, int _max);

int fontInit()
{
	screenSize = g_windowSize;
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
	glBindTexture(GL_TEXTURE_2D, 0);

	return 0;
}

void fontRelease()
{
	if (fontChars != NULL)
		free(fontChars);
	texture.deleteTexImage();
	glDeleteTextures(1, &texId); // GLsizei n, const GLuint *textures
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

	glBindTexture(
		GL_TEXTURE_2D,	// GLenum target
		texId);			// GLuint texture
}

void fontEnd()
{
	glBindTexture(
		GL_TEXTURE_2D,	// GLenum target
		0);				// GLuint texture

	glMatrixMode(GL_MODELVIEW);
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

unsigned short fontGetLineHeight()
{
	return (unsigned short)(fontCommon.lineHeight * scale);
}

void fontDraw(const char* format, ...)
{
	va_list ap;
	char str[256];
	char* p;
	vec2 pos = position;

	va_start(ap, format);
	vsprintf_s(str, format, ap);
	va_end(ap);
	
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


			// Upper left
			glTexCoord2f(leftX, leftY);
			glVertex2f(pos.x + (fontChars[index].xoffset * scale), pos.y + (fontChars[index].yoffset * scale));

			// Lower left
			glTexCoord2f(leftX, rightY);
			glVertex2f(pos.x + (fontChars[index].xoffset * scale), pos.y + (size.y + fontChars[index].yoffset) * scale);

			// Lower right
			glTexCoord2f(rightX, rightY);
			glVertex2f(pos.x + (size.x + fontChars[index].xoffset) * scale, pos.y + (size.y + fontChars[index].yoffset) * scale);

			// Upper right
			glTexCoord2f(rightX, leftY);
			glVertex2f(pos.x + (size.x + fontChars[index].xoffset) * scale, pos.y + (fontChars[index].yoffset * scale));
		}
		glEnd();

		pos.x += fontChars[index].xadvance * scale;
	}

	if (*p == '\n') {
		position.x = origin.x;
		position.y += fontGetLineHeight();
		fontDraw(++p);
	}

}

int fontGetCharById(unsigned int _id)
{
	int result = -1;

	/*
	for (int i = 0; i < fontCharCount; i++) {
		if (fontChars[i].id == _id)
			result = i;
	}
	*/
	result = binarySearch(_id, 1, fontCharCount);

	return result;
	
}

static inline int binarySearch(unsigned int _key, int _min, int _max)
{
	if (_max < _min) {
		return -1;
	} else {
		int mid = _min + (_max - _min) / 2;
		if (fontChars[mid].id > _key) {
			return binarySearch(_key, _min, mid - 1);
		} else if (fontChars[mid].id < _key) {
			return binarySearch(_key, mid+1, _max);
		} else {
			return mid;
		}
	}
}