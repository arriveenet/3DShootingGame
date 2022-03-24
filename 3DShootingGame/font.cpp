#include "font.h"

#include <stdio.h>
#include <string.h>

FONTINFO fontInfo;

int fontInit(const char* _fileName)
{
	FILE* pFile;
	errno_t err;
	err = fopen_s(&pFile, _fileName, "r");

	if (err != 0) {
		printf("font the file %s open failed!\n", _fileName);
		return 1;
	}
	printf("font the file %s opened!\n", _fileName);

	char str[256];
	char* blockType;
	char* context = NULL;

	while (fgets(str,256,pFile) != NULL){
		//printf("%s\n", str);
		blockType = strtok_s(str, " ", &context);
		if (strcmp(blockType, "info") == 0) {
			while (blockType != NULL) {
				blockType = strtok_s(NULL, " =,", &context);
				if (blockType != NULL) {
					puts(blockType);
				}
			}
		}
		else if (strcmp(blockType, "common") == 0) {

		}
		else if (strcmp(blockType, "page") == 0) {

		}
		else if (strcmp(blockType, "chars") == 0) {

		}
		else if (strcmp(blockType, "char") == 0) {

		}

	}

	fclose(pFile);
	return 0;
}

void fontTerm()
{

}