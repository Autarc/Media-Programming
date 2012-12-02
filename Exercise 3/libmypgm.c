#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include <mypgm.h>


// showing error message + stop

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

	buffer[pos] = '\0'; // end of string
}









/* Receive the type */
void getMagic( FILE* fi, char * buffer, int bufferSize ) {


	nextString( fi, buffer, bufferSize, 0 );

	if ( strcmp( buffer, "P2") == 0 ) {

		// type = "ASCII";

	} else if ( strcmp( buffer, "P5" ) == 0 ) {

		// type = "BINARY";

	} else {

		errorMessage( 6, "Invalid file format" );
	}
}



/* Show the comments */

void getComments( FILE* fi, char * buffer, int bufferSize, char * last ) {


	char comments[1024];
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



void getData( FILE* fi, char * magic,  Image * img ) {

	int width 	= img->width,
		height	= img->height;

	// dclaring space

	img->data = (byte**) malloc( height * sizeof(byte*) );

	int i; // not requiring std=c99

	for ( i = 0; i < height; i++ ) {

		img->data[i] = (byte*) malloc( width * sizeof(byte) );
	}




	int min		= img->depth,
		max		= 0;


	// data instead of hist, using a values array
	// reading all the values in here.., simple array access
	int values[ width * height ],
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



	int y; // not requiring std=c99
	int x; // not requiring std=c99

	int pos;


	// could check for wrong information, manipulated header
	for ( y = 0; y < height; y++ ) {

		for ( x = 0; x < width; x++ ) {

			pos = ( y * width + x );

			img->data[y][x] = values[pos];
		}
	}

}






// Implementation of the in 'mypgm.h' declared functions

unsigned short LoadPGM( FILE * fi, Image * img ) {


	char magic[3]; // 3 => P2 \0 , as it was just 2 -> P \0

	getMagic( fi, magic, sizeof(magic) );

	if ( !magic[0] ) return 0;

	img->magic[0] = magic[0];
	img->magic[1] = magic[1];


	char comments[2024];
	char last;

	getComments( fi, comments, sizeof(comments), &last );
	// if ( !comments ) return 0;


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


	// read data
	getData( fi, magic, img );


	return 1; // success
};










	// char	magic[2];
	// char**	comments;



// copy meta information and set defaults

void setStats( Image * in, Image * out ) {

	// strcpy( in->magic, out->magic );
	// out->magic[0] = in->magic[0];
	// out->magic[1] = in->magic[1];

	// char magic[3]; // 3 => P2 \0 , as it was just 2 -> P \0

	// magic[0] = "P";
	// magic[1] = "2";
	// magic[2] = "\0";

	out->magic[0] = 80;//magic[0];//80;
	out->magic[1] = 50;//magic[1];//50;

	//out->comments = in->comments;

	int width = in->width,
		height = in->height;

	out->width = width;
	out->height = height;

	out->depth = in->depth;


	out->data = (byte**) malloc( height * sizeof(byte*) );

	int i;

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

	// magic
	fprintf( fo, "%s\n", out->magic );

	// comments
	// fprintf( fo, "%s\n", out.magic );


	// width & height
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



	// success
	return 1;
};



unsigned short NormPGM( Image * in, Image * out ) {

    int height	= in->height,
    	width	= in->width,
    	min		= in->min,
    	max		= in->max,
    	range	= max - min;

    int x, y;
    byte color; // using byte to normalise, more efficienty unsigned short.

    for ( y = 0; y < height; y++ ) {
    	for ( x = 0; x < width; x++ ) {

			color = in->data[y][x];
			out->data[y][x] = ( color - min ) * 255 / range;
    	}
    }


    // return 0; // fail

	// succerss
	return 1;
};



unsigned short InvertPGM( Image * in, Image * out ) {

    int height	= in->height;
    int width	= in->width;

    int x;
    int y;
    int color;

    for ( y = 0; y < height; y++ ) {
    	for ( x = 0; x < width; x++ ) {

			color = in->data[y][x];
			out->data[y][x] = 255 - color;
    	}
    }

    // return 0; fail

	// success
	return 1;
};










// clean memory usage for image. free the image data
void freeImage( Image * img ) {

	int height = img->height;
	int i;

	for ( i = 0; i < height; i++ ) {

		free( img->data[i] );
	}

	free( img->data );
}










