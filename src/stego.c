#include "stego.h"

void stego( Image *image, Payload *payload );

void
main( int argc, char *argv[] )
{
	if( argc != 4 )
		error( "Must provide an input PGM, output PGM, and payload!" );
	Image *image = malloc( sizeof( Image ) );
	if( image == NULL )
		error( "Ran out of memory when allocating memory for the image!" );
	Payload *payload = malloc( sizeof( Payload ) );
	if( payload == NULL )
		error( "Ran out of memory when allocating memory for the payload!" );
	readImage( argv[1], image );
	readPayload( argv[3], payload );
	if( (image->width * image->height) < (payload->size + 8) * 8 )
		error( "The file is not large enough to embed the payload!" );
	stego( image, payload );
	writeImage( argv[2], image );
}

void
stego( Image *image, Payload *payload )
{
	// embedding the size of the payload into the image
	unsigned char bSize1 = (payload->size >> (8*0)) & 0xff;
    unsigned char bSize2 = (payload->size >> (8*1)) & 0xff;
    unsigned char bSize3 = (payload->size >> (8*2)) & 0xff;
    unsigned char bSize4 = (payload->size >> (8*3)) & 0xff;
    int totalcounter = 0;
    int iter;
    unsigned char currByte;
    for( iter = 0; iter < 4; iter++ )
    {
    	// embeds the size of the payload
    	unsigned char theBytes[8];
    	int z;
    	for(z = 0; z < 8; z++)
    	{
        	theBytes[z] = image->data[totalcounter];
        	totalcounter++;
    	}
    	switch(iter)
    	{
    		case 0:
    			currByte = bSize1;
    			break;
    		case 1:
    			currByte = bSize2;
    			break;
    		case 2:
    			currByte = bSize3;
    			break;
    		case 3:
    			currByte = bSize4;
    			break;
    	}
      setlsbs( theBytes, currByte, totalcounter, image );
    }
    // embeds the payload into the image
    for ( iter = 0; iter < payload->size; iter++)
    {
    	unsigned char theBytes[1];
    	currByte = payload->data[iter];
    	totalcounter += 8;
    	setlsbs( theBytes, currByte, totalcounter, image );
    }
}