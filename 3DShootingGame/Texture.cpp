#include "Texture.h"

#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>

Texture::Texture()
	:m_width(0)
	,m_height(0)
	,m_bitmapPixels(NULL)
	,m_pixels(NULL)
{
}

Texture::~Texture()
{
	if (m_pixels) {
		delete[] m_pixels;
	}
	if (m_bitmapPixels) {
		delete[] m_bitmapPixels;
	}
	m_pixels = NULL;
	m_bitmapPixels = NULL;
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
	createTexImage(_colorKey);

	return 0;
}

int Texture::loadBitmapFile(const char* _fileName,
	unsigned char _colorKeyR,
	unsigned char _colorKeyG,
	unsigned char _colorKeyB)
{
	unsigned char colorKey[] = {
		_colorKeyR,
		_colorKeyG,
		_colorKeyB
	};

	return loadBitmapFile(_fileName, colorKey);
}

void Texture::deleteTexImage()
{
	if (m_pixels) {
		delete[] m_pixels;
	}
	if (m_bitmapPixels) {
		delete[] m_bitmapPixels;
	}
	m_bitmapPixels = NULL;
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
		printf("The file was %s opened.\n", _fileName);

		fread(&bf, sizeof BITMAPFILEHEADER, 1, pFile);
		//printf("bfSize: %d\n", bf.bfSize);

		fread(&bi, sizeof BITMAPINFOHEADER, 1, pFile);
		//printf("biWidth:%d biHeight:%d\n", bi.biWidth, bi.biHeight);
		//printf("biBitCount:%d\n", bi.biBitCount);
		m_width = bi.biWidth;
		m_height = bi.biHeight;

		int padding = ((4 - (3 * m_width) % 4)) % 4;
		long imageSize = (3 * m_width + padding) * m_height;

		m_bitmapPixels = new unsigned char[imageSize];

		// 24bits
		if (bi.biBitCount == 24) {
			fseek(pFile, bf.bfOffBits, 0);
			fread(m_bitmapPixels, sizeof(unsigned char), imageSize, pFile);
		}
		else {
			fclose(pFile);
			return 1;
		}

		fclose(pFile);
	}
	else {
		printf("The file was %s open failed.\n", _fileName);
		return 1;
	}

	return 0;
}

int Texture::createTexImage(unsigned char* _colorKey)
{
	if (!m_bitmapPixels)
		return 1;

	m_pixels = new unsigned char[4 * m_width * m_height];
	if (!m_pixels)
		return 1;

	// RGBA
	for (int y = 0; y < m_height; y++) {
		for (int x = 0; x < m_width; x++) {
			int index = 4 * (m_width * y + x);
			int pixelIndex = 3 * (m_width * y + x);

			// BGR -> RBG
			m_pixels[index] = m_bitmapPixels[pixelIndex+2];
			m_pixels[index+1] = m_bitmapPixels[pixelIndex +1];
			m_pixels[index+2] = m_bitmapPixels[pixelIndex];

			// Add Alpha channel
			m_pixels[index+3] = ((_colorKey != nullptr)
				&& (m_pixels[index] == _colorKey[0])	// r
				&& (m_pixels[index+1] == _colorKey[1])	// g
				&& (m_pixels[index+2] == _colorKey[2]))	// b
				? 0x00
				: 0xff;
		}
	}

	// Swap
	for (int y = 0; y < m_height / 2; y++)
		for (int x = 0; x < m_width; x++) {
			int index0 = 4 * (m_width * y + x);
			int index1 = 4 * (m_width * (m_height - 1 - y) + x);

			unsigned char* pixel0 = &m_pixels[index0];
			unsigned char* pixel1 = &m_pixels[index1];
			unsigned char temp[4] = { *(pixel0), *(pixel0 + 1), *(pixel0 + 2), *(pixel0 + 3)};
			for (int i = 0; i < 4; i++) {
				*(pixel0 + i) = *(pixel1 + i);
				*(pixel1 + i) = temp[i];
			}
		}
	return 0;
}