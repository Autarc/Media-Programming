/*
	A c command line tool which reads PGM-images (binary or ascii).

	Use -help to learn more about the options.

	: comments
	: width
	: height
	: filename or -help

	: statistic values =>	arithemic mean, the median, the probability of the colors, modal value,
							maximum, minimum

	@params

	argv[2] => filename or -help
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>




/* Validation check */

int valid ( char c ) {

	if ( c != ' ' &&  c != '\n' && c != EOF ) return 1;

	return 0;
}


/* getting a new index, skipping ends */

char newIndex ( FILE* fi ) {

	char i = fgetc( fi );

	while( !valid(i) ) i = fgetc( fi );

	return i;
}



/* Retrieving a string */

void nextString ( FILE* fi, char * buffer, int bufferSize ) {

	char c = newIndex(fi);

	int pos = 0;

	while ( valid( c ) ) {

		buffer[pos++] = c;
		c = fgetc( fi );

		if ( pos+1 == bufferSize ) break;
	}

	buffer[pos] = '\0'; // end of string
}




void getType( FILE* fi, char * buffer, int bufferSize ) {

	char * type;

	nextString( fi, buffer, bufferSize );

	if ( strcmp( buffer, "P2") > -1 ) {

		type = "ASCII";

	} else if ( strcmp( buffer, "P5" ) > -1 ) {

		type = "BINARY";

	} else {

		fprintf(stderr, "\n%s\n", "Invalid file format - no ascii nor binary pgm got used." );

		exit(3);
	}

	fprintf(stdout, "Type: \t\t%s\n", type );
	printf("_____________________\n\n");
}



void getComments( FILE* fi, char * buffer, int bufferSize ) {

	// char * comments = "\0";
	// comments[0] = '\0';

	char c = fgetc(fi);

	int pos;

	while ( c == '#' ) {

		pos = 0;

		while ( c != '\n' ) {

			buffer[ pos++ ] = c;
			c = fgetc(fi);
		}

		// buffer[pos++] = '\n';
		buffer[pos++] = '\t';
		buffer[pos++] = '\t';
		buffer[pos++] = '\0';

		// https://en.wikibooks.org/wiki/C_Programming/Strings#The_strncat_function
		// strcat( comments, buffer );

		fprintf(stdout, "%s\n", buffer );

		c = fgetc(fi);
	}

	// fprintf(stdout, "\nComments: \t%s\n", comments );
	printf("\n");
	printf("_____________________\n\n");
}




/// ???? die eins wird noch verschluckt vom width...
void getWidth( FILE* fi, char * buffer, int bufferSize ) {

	nextString( fi, buffer, bufferSize );

	char * width = buffer;

	fprintf(stdout, "Width: \t\t%s\n", width );
}

void getHeight( FILE* fi, char * buffer, int bufferSize ) {

	nextString( fi, buffer, bufferSize );

	char * height = buffer;

	fprintf(stdout, "Height: \t%s\n", height );
}


void getRange( FILE* fi, char * buffer, int bufferSize ) {

	nextString( fi, buffer, bufferSize );

	char * range = buffer;

	fprintf(stdout, "Range: \t\t%s\n", range );
	printf("_____________________\n");
}


void getStats( FILE* fi, char * buffer, int bufferSize ) {

	// int range = atoi(buffer);

	// int min = range;
	// int max = 0;
	// int val; // current value

	// int histogram[range+1];

	// int i; // incrementor

	// for ( i = 0; i < range; i++ ) histogram[i] = 0;

	int counter = 0;

	char c = fgetc(fi);

	printf("\n");

	while ( c != EOF ) {

		nextString( fi, buffer, bufferSize );

		// val = atoi(buffer);

		counter++;

		fprintf(stdout, "%i: \t\t%s\n", counter, buffer );

		// histogram[val]++;
		// if ( val < min ) min = val;
		// if ( val > max ) max = val;

		c = fgetc(fi);
	}

	// int sum = 0;
	// int arith = 0;
	// int median = -1;

	// // modal ?
	// int modal = 0;

	// int current;

	// for ( i = 0; i < range; i++ ) {

	// 	current = histogram[i];

	// 	sum += current;
	// 	arith += i * current;

	// 	if ( median == -1 && sum >= counter/2 ) median = i;
	// }

	// fprintf(stdout, "Median: \t%i\n", median );
	// fprintf(stdout, "Arith.: \t%i\n", arith );
	// fprintf(stdout, "Max.: \t\t%i\n", max );
	// fprintf(stdout, "Min.: \t\t%i\n", min );

	// fprintf(stdout, "Modal: \t\t%i\n", modal );

	// Häufigkeit aller Grauwerte, arith. Mittel,
	// Median, Modalwert, Maximum, Minimum

	printf("_____________________\n\n");
}






int main ( int argc, char * argv[] ) {

	if ( argc != 2 ) {

		fprintf(stderr, "\n%s\n", "Missing arguments." );

		exit(1);
	}

	if ( strcmp( argv[1], "-help" ) == 0 ) {

		fprintf(stderr, "\n%s\n", "This programm can read an arbitary pgm ascii or binary image and show statistics.");

		exit(0);
	}



	char * name = argv[1];

	// input
	FILE * fi = fopen( name , "r");

	if ( !fi ) {

		fprintf(stderr, "\n%s\n", "Can't read the file." );

		exit(2);
	}

	// general memory temp buffer
	char buffer[1024];


	printf("\n");

	fprintf(stdout, "Filename: \t%s\n\n", name );

	getType( fi, buffer, sizeof(buffer) );
	getComments( fi, buffer, sizeof(buffer) );
	getWidth( fi, buffer, sizeof(buffer) );
	getHeight( fi, buffer, sizeof(buffer) );
	getRange( fi, buffer, sizeof(buffer) );

	getStats( fi, buffer, sizeof(buffer) );

	fclose( fi );

	return 0;
}
