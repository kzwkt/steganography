#include "stego.h"

void stegoExtract( Image *image, char *out );

void
main( int argc, char *argv[] )
{
	if( argc != 3 )
		error( "Must provide an image with imbedded information and an output file!" );
	Image *image = malloc( sizeof( Image ) );
	if( image == NULL )
		error( "Ran out of memory when allocating memory for the image!" );
	readImage( argv[1], image );
	stegoExtract( image, argv[2] );
	free( image->data );
	free( image );
}

void
stegoExtract( Image *image, char *out )
{
	// extracts the size of the payload
	int bSize = 0;
	int i, j;
	for( i = 31; i >= 0; i-- )
	{ 
    	if( image->data[i] % 2 != 0 )
        	bSize++;
    	if( i != 0 )
        	bSize = bSize << 1;
    }
    // extracts the payload
    FILE *outfile;
    outfile = fopen( out, "wb" );
    unsigned char allChars[bSize];
    int currIter = 32;
    int iter = 0;
    for( i = 32; i < bSize + 32; i++ )
    {
    	// extract information from the image one byte at the time
    	unsigned char theChar = 0;
    	for( j = 32 + (iter * 8); j < 32 + (iter * 8) + 8; j++)
    	{
        	if( image->data[j] % 2 != 0 )
          		theChar++;
        	if( j != currIter + 7 )
          		theChar = theChar << 1;
    	}
    	currIter += 8;
    	// the bits are backwards so we need to reverse them
    	theChar = ( theChar & 0xF0 ) >> 4 | ( theChar & 0x0F ) << 4; // swaps the two nibbles
    	theChar = ( theChar & 0xCC ) >> 2 | ( theChar & 0x33 ) << 2; // swaps the two pairs within the nibbles
    	theChar = ( theChar & 0xAA ) >> 1 | ( theChar & 0x55 ) << 1; // swaps the bits sitting adjacent to each other
    	allChars[iter] = theChar;
    	iter++;
    }
    fwrite( allChars, sizeof( allChars[0] ), sizeof( allChars ), outfile );
    fclose( outfile );
}