#ifndef PNGIO_H
#define PNGIO_H

unsigned char* readImagePNG(char const * file_name, int *width, int *height);
void writeImagePNG(char const * file_name, unsigned char *pixel, int width, int height);

#endif
