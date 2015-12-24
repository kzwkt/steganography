#include "stego.h"

// reads the PGM file and fills in the values for image
void
readImage( char fileName[], Image *image )
{
	FILE *in = fopen( fileName, "rb" );
	if( in == NULL )
		error( "File was not opened correctly!");
	char buffer[100];
	if( fgets( buffer, 100, in ) == NULL )
		error( "Error reading magic number!" );
	if( strcmp( "P5\n", buffer) != 0 )
		error( "File's magic number must be P5!" );
	strcpy( image->name, fileName );
	// iterate through all comments to find height & width of image
	while( 1 )
	{
		if( fgets( buffer, 50, in ) == NULL )
			error( "End of file reached when looking for image height & width!");
		if( buffer[0] == '#' )
			continue;
		sscanf(buffer,"%d %d",&(image->width),&(image->height));
		break;
	}
	// iterate through all comments to find max grayscale value
	while( 1 )
	{
		if( fgets( buffer, 100, in ) == NULL )
			error( "End of file reached when looking for max grayscale value!");
		if( buffer[0] == '#' )
			continue;
		sscanf( buffer, "%d", &(image->maxVal));
		break;
	}
	image->data = (unsigned char *) malloc( sizeof( unsigned char ) * image->width * image->height );
	if( image->data == NULL )
		error( "Not enough memory!" );
	int i, j, k;
	k = 0;
	for( i = 0; i < image->height; i++ ){
		for( j = 0; j < image->width; j++ ){
			fscanf( in, "%c", &(image->data[k++]) );
		}
	}
	fclose( in );
}

void
writeImage( char fileName[], Image *image )
{
	FILE *out = fopen( fileName, "wb" );
	if( out == NULL )
		error( "Error when writing a new PGM file!" );
	fprintf( out, "%s\n%d %d\n%d\n", "P5", image->width, image->height, 255 );
	int i, j, k;
	k = 0;
	for( i = 0; i < image->height; i++ ){
		for( j = 0; j < image->width; j++ ){
			fwrite( &(image->data[k++]), sizeof( unsigned char ), 1, out );
		}
	}
	fclose( out );
	free( image->data );
	free( image );
}

void
readPayload( char *fileName, Payload *payload )
{
	FILE *pf = fopen( fileName, "rb" );
	if( pf == NULL )
		error( "Could not open payload file!" );
	unsigned char storage[4096]; // max amount of storage
	payload->size = fread( storage, sizeof(unsigned char), 4096, pf );
	payload->data = malloc( sizeof( unsigned char ) * payload->size );
	int i;
	for( i = 0; i < payload->size; i++ )
		payload->data[i] = storage[i];
}

void
setlsbs( unsigned char p[], unsigned char b0, int total, Image *img )
{
	int i = 0;
	int iter;
	for( iter = total - 8; iter < total; iter++ )
	{
		img->data[iter] |= (b0 >> i) & 0x01;
		if( (img->data[iter] % 2) && (b0 >> i) % 2 == 0 ){
			img->data[iter]--;
		} 
		i++;
	}
}

void
printBinary( unsigned char byte )
{
	int rtnByte[8];
	rtnByte[7] = (byte & 0x80) > 0 ? 1 : 0;
	rtnByte[6] = (byte & 0x40) > 0 ? 1 : 0;
	rtnByte[5] = (byte & 0x20) > 0 ? 1 : 0;
	rtnByte[4] = (byte & 0x10) > 0 ? 1 : 0;
	rtnByte[3] = (byte & 0x08) > 0 ? 1 : 0;
	rtnByte[2] = (byte & 0x04) > 0 ? 1 : 0;
	rtnByte[1] = (byte & 0x02) > 0 ? 1 : 0;
	rtnByte[0] = (byte & 0x01) > 0 ? 1 : 0;
	int i;
	for( i = 7; i >= 0; i-- )
	{
		printf("%d", rtnByte[i]);
	}
	printf("\n");
}

void
error( char *msg )
{
	printf("%s", msg );
	exit( 1 );
}