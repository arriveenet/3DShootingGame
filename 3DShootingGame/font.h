#pragma once

typedef struct {
	char fase[64];
	unsigned int size;
	unsigned int bold;
	unsigned int italic;
	char charset[64];
	unsigned int unicode;
	unsigned int stretchH;
	unsigned int smooth;
	unsigned int aa;
	unsigned int padding[4];
	unsigned int spacing[2];
	unsigned int outline;
}FONTINFO;

typedef struct {
	unsigned int lineHeight;
	unsigned int base;
	unsigned int scaleW;
	unsigned int scaleH;
	unsigned int page;
	unsigned int packed;
	unsigned int alphaChnl;
	unsigned int redChnl;
	unsigned int greenChnl;
	unsigned int blueChnl;
}FONTCOMMON;

typedef struct {
	unsigned int id;
	char file[256];
}FONTPAGE;

typedef struct {
	unsigned int count;
}FONTCHARS;

typedef struct {
	unsigned int id;
	unsigned int x;
	unsigned int y;
	unsigned int width;
	unsigned int height;
	unsigned int xoffset;
	unsigned int yoffset;
	unsigned int xadvance;
	unsigned int page;
	unsigned int chnl;
}FONTCHAR;

int fontInit(const char* _fileName);
void fontRelease();