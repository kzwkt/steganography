#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _img
{
	char name[100];
	int height, width, maxVal;
	unsigned char *data;
} Image;

typedef struct _payload
{
	int size;
	unsigned char *data;
} Payload;

void error( char *msg );
void readImage( char fileName[], Image *image );
void writeImage( char fileName[], Image *image );
void printBinary( unsigned char byte ); // used for debugging
void readPayload( char *fileName, Payload *payload );
void setlsbs( unsigned char p[], unsigned char b0, int total, Image *img );