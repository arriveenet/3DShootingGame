#pragma once

typedef struct {
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;
}RGBA;

class Texture
{
	int m_width;
	int m_height;
	unsigned char* m_pixels;

public:
	Texture();
	~Texture();

	int getWidth() const;
	int getHeight() const;

	const unsigned char* getTexImage() const;
	void deleteTexImage();

	int loadBitmapFile(const char* _fileName, unsigned char* _colorKey = nullptr);
private:
	int openBitmapFile(const char* _fileName);
	int createTexImage();
};
