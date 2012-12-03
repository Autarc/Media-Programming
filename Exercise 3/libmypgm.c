#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <mypgm.h>


/* Showing ErrorMessage + stop */

void errorMessage ( short exitNumber, char* message ) {

	fprintf( stderr, "\nERROR:\t\t(%i) - %s\n\nUSAGE:\t\tpgmop.elf <infile> [-o <outfile>] [ -norm | -invert ]\n\nEXAMPLE:\tpgmop.elf logo.bin.pgm -o normalized.bin.pgm -norm\n\t\tpgmop.elf logo.bin.pgm -invert | display\n\nDESCRIPTION:\tpgmop is a small utility tool, which reads PGM images (ascii or binary)\n\t\tand can normalize or invert them. The result can be saved into a file\n\t\tor shown at the commandline.\n\n", exitNumber, message );

	exit( exitNumber );
};



/* Validation */

int valid ( char c ) {

	if ( c != ' ' &&  c != '\n' && c!= '\t' && c!= '\r' && c != EOF ) return 1;

	return 0;
}


/* Retrieving a string */

void nextString ( FILE* fi, char * buffer, int bufferSize, char start ) {

	char c = start ? start : fgetc(fi);

	int pos = 0;

	while ( valid( c ) ) {

		buffer[pos++] = c;
		c = fgetc( fi );
		if ( pos+1 == bufferSize ) break;
	}

	buffer[pos] = '\0';
}



/* Receive the type */

void getMagic( FILE* fi, char * buffer, int bufferSize ) {

	nextString( fi, buffer, bufferSize, 0 );

	if ( strcmp( buffer, "P2") != 0 && strcmp( buffer, "P5" ) != 0 ) {

		errorMessage( 9, "Invalid file format" );
	}
}



/* Show the comments */

void getComments( FILE* fi, Image * img, char * last ) {

	char buffer[1024],
		comments[2024];

	comments[0] = '\0';

	char c = fgetc(fi);

	int pos;

	while ( c == '#' ) {

		pos = 0;

		while ( c != '\n' ) {

			buffer[ pos++ ] = c;
			c = fgetc(fi);
		}

		buffer[pos++] = '\n';
		buffer[pos++] = '\t';
		buffer[pos++] = '\0';

		strcat( comments, buffer );

		c = fgetc(fi);
	}

	*last = c;
}


/* Show the width */
void getWidth( FILE* fi, char * buffer, int bufferSize, char last ) {

	nextString( fi, buffer, bufferSize, last );
}


/* Show the height */
void getHeight( FILE* fi, char * buffer, int bufferSize ) {

	nextString( fi, buffer, bufferSize, 0 );
}


/* Show the range */
void getDepth( FILE* fi, char * buffer, int bufferSize ) {

	nextString( fi, buffer, bufferSize, 0 );
}


/* Get the data */
void getData( FILE* fi, char * magic,  Image * img ) {

	int width 	= img->width,
		height	= img->height,

		i;

	img->data = (byte**) malloc( height * sizeof(byte*) );

	for ( i = 0; i < height; i++ ) {

		img->data[i] = (byte*) malloc( width * sizeof(byte) );
	}

	int min		= img->depth,
		max		= 0,

		values[ width * height ],
		counter = 0,
		val;


 	char buffer[1024];

 	int c = fgetc(fi);

	while ( c != EOF ) {

		if ( strcmp( magic, "P2" ) == 0 ) {

			nextString( fi, buffer, sizeof(buffer), c );

			val = atoi(buffer);

		} else {

			val = c;
		}

		if ( val != '\0' ) {

			values[counter] = val;

			if ( val < min ) min = val;
			if ( val > max ) max = val;

			counter++;
		}

		c = fgetc(fi);
	}


	img->min = min;
	img->max = max;


	int y, x, pos;

	for ( y = 0; y < height; y++ ) {

		for ( x = 0; x < width; x++ ) {

			pos = ( y * width + x );

			img->data[y][x] = values[pos];
		}
	}

}




/* mylibpgm - functions */

unsigned short LoadPGM( FILE * fi, Image * img ) {

	char magic[3];

	getMagic( fi, magic, sizeof(magic) );

	if ( !magic[0] ) return 0;

	img->magic[0] = magic[0];
	img->magic[1] = magic[1];


	char last;

	getComments( fi, img, &last );

	char width[100];
	getWidth( fi, width, sizeof(width), last );
	if ( !atoi(width) ) return 0;
	img->width = atoi(width);

	char height[100];
	getHeight( fi, height, sizeof(height) );
	if ( !atoi(height) ) return 0;
	img->height = atoi(height);

	char depth[100];
	getDepth( fi, depth, sizeof(depth) );
	if ( !atoi(depth) ) return 0;
	img->depth = atoi(depth);

	getData( fi, magic, img );

	return 1;
};



/* copy meta information - defaults */
void setStats( Image * in, Image * out ) {

	out->magic[0] = 'P';
	out->magic[1] = '2';

	int width = in->width,
		height = in->height,
		i;

	out->width = width;
	out->height = height;

	out->depth = in->depth;

	out->data = (byte**) malloc( height * sizeof(byte*) );

	for ( i = 0; i < height; i++ ) {

		out->data[i] = (byte*) malloc( width * sizeof(byte) );
	}


	int x, y;

	for ( y = 0; y < height; y++ ) {
		for ( x = 0; x < width; x++ ) {

			out->data[y][x] = in->data[y][x];
		}
	}

}



unsigned short SavePGM( FILE * fo, Image * out ) {


	fprintf(fo, "%c%c\n", out->magic[0], out->magic[1]);


	// fprintf( fo, "%s\n", out.magic );

	fprintf( fo, "%i %i\n", out->width, out->height );

	fprintf( fo, "%i\n", out->depth );


	int width = out->width,
		height = out->height,

		x, y;

	for ( y = 0; y < height; y++ ) {
		for ( x = 0; x < width; x++ ) {

			fprintf( fo, "%i ", out->data[y][x] );
		}
	}

	// fail
	// return 0;

	return 1;
};



unsigned short NormPGM( Image * in, Image * out ) {

    int height	= in->height,
    	width	= in->width,
    	min		= in->min,
    	max		= in->max,
    	range	= max - min;

    int x, y;
    byte color;

    for ( y = 0; y < height; y++ ) {
    	for ( x = 0; x < width; x++ ) {

			color = in->data[y][x];
			out->data[y][x] = ( color - min ) * 255 / range;
    	}
    }

    // fail
    // return 0;

	return 1;
};



unsigned short InvertPGM( Image * in, Image * out ) {

    int height	= in->height,
    	width	= in->width,

    	x, y, color;

    for ( y = 0; y < height; y++ ) {
    	for ( x = 0; x < width; x++ ) {

			color = in->data[y][x];
			out->data[y][x] = 255 - color;
    	}
    }

 	// fail
    // return 0;

	return 1;
};


/* clean memory - free image data */
void freeImage( Image * img ) {

	int height = img->height,
		i;

	for ( i = 0; i < height; i++ ) {

		free( img->data[i] );
	}

	free( img->data );
}
