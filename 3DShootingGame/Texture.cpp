#include "Texture.h"

#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>

Texture::Texture()
	:m_width(0)
	,m_height(0)
	,m_pixels(NULL)
{
}

Texture::~Texture()
{
	if (m_pixels) {
		delete[] m_pixels;
	}
	m_pixels = NULL;
	m_width = 0;
	m_height = 0;
}

int Texture::getWidth() const
{
	return m_width;
}

int Texture::getHeight() const
{
	return m_height;
}

const unsigned char* Texture::getTexImage() const
{
	return m_pixels;
}

int Texture::loadBitmapFile(const char* _fileName, unsigned char* _colorKey)
{
	openBitmapFile(_fileName);
	createTexImage();

	return 0;
}

void Texture::deleteTexImage()
{
	if (m_pixels) {
		delete[] m_pixels;
	}
	m_pixels = NULL;
	m_width = 0;
	m_height = 0;
}

int Texture::openBitmapFile(const char* _fileName)
{
	FILE* pFile;
	BITMAPFILEHEADER bf;
	BITMAPINFOHEADER bi;

	if (fopen_s(&pFile, _fileName, "rb") == 0) {
		printf("The file was %s open.\n", _fileName);

		fread(&bf, sizeof BITMAPFILEHEADER, 1, pFile);
		printf("bfSize: %d\n", bf.bfSize);

		fread(&bi, sizeof BITMAPINFOHEADER, 1, pFile);
		printf("biWidth:%d biHeight:%d\n", bi.biWidth, bi.biHeight);
		printf("biBitCount:%d\n", bi.biBitCount);
		m_width = bi.biWidth;
		m_height = bi.biHeight;
		long size = 3 * m_width * m_height;

		m_pixels = new unsigned char[size];

		// 24bits
		if (bi.biBitCount == 24) {
			fseek(pFile, bf.bfOffBits, 0);
			fread(m_pixels, sizeof(unsigned char), size, pFile);
		}

		fclose(pFile);
	}
	else {
		printf("The file was %s open failed.\n", _fileName);
		return 1;
	}

	return 0;
}

int Texture::createTexImage()
{
	return 0;
}