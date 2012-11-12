/*
	A command line tool which shows information about acii/binary pgm-images.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>



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

void getType( FILE* fi, char * buffer, int bufferSize ) {

	char * type;

	nextString( fi, buffer, bufferSize, 0 );

	if ( strcmp( buffer, "P2") == 0 ) {

		type = "ASCII";

	} else if ( strcmp( buffer, "P5" ) == 0 ) {

		type = "BINARY";

	} else {

		fprintf(stderr, "\n%s\n", "\nERROR:\t(4) Invalid file format\n\nUSAGE:\tYou need to specify the filename of a valid acsii/binary pgm-image as the argument.\n\n\tFor more information use --help !" );
		exit(4);
	}

	fprintf(stdout, "\tType: \t\t%s\n", type );
	printf("\t___________________________\n\n");
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

	fprintf(stdout, "\tComments:\n\n\t%s", comments );
	printf("___________________________\n\n");
}


/* Show the width */

void getWidth( FILE* fi, char * buffer, int bufferSize, char last ) {

	nextString( fi, buffer, bufferSize, last );

	fprintf(stdout, "\tWidth: \t\t%s\n", buffer );
}


/* Show the height */

void getHeight( FILE* fi, char * buffer, int bufferSize ) {

	nextString( fi, buffer, bufferSize, 0 );

	fprintf(stdout, "\tHeight: \t%s\n", buffer );
}


/* Show the range */

void getRange( FILE* fi, char * buffer, int bufferSize ) {

	nextString( fi, buffer, bufferSize, 0 );

	fprintf(stdout, "\tRange: \t\t%s\n", buffer );

	printf("\t___________________________\n");
}


/* Show the stats */

void getStats( FILE* fi, char * type, int range ) {

	int min = range;
	int max = 0;
	int val;


	int histogram[range+1];
	int counter = 0;
	int i;

	for ( i = 0; i <= range; i++ ) histogram[i] = 0;


	char buffer[1024];

	int c = fgetc(fi);

	while ( c != EOF ) {

		if ( strcmp( type, "P2") == 0 ) {

			nextString( fi, buffer, sizeof(buffer), c );

			val = atoi(buffer);

		} else {

			val = c;
		}

		if ( val != '\0' ) {

			histogram[val]++;
			if ( val < min ) min = val;
			if ( val > max ) max = val;

			counter++;
		}

		c = fgetc(fi);
	}


	double arith = 0;
	int median = -1;
	int sum = 0;

	int ref[256];
	int amount = 0;
	int modal = -1;

	int current;

	char format = ' ';


	fprintf(stdout, "\n\tValues:\n\n");

	for ( i = 0; i <= range; i++ ) {

		current = histogram[i];

		sum += current;
		arith += (i * current);

		if ( current >= modal  ) {

			if ( current > modal ) amount = 0;

			modal = current;
			ref[amount++] = i;
		}

		if ( median == -1 && sum >= counter/2 ) median = i;

		if ( i < 10  ) { 		fprintf(stdout, "\t%c%c%i:	\t%i\n",	format, format, i, current ); }
		else if ( i < 100 ) { 	fprintf(stdout, "\t%c%i:	\t%i\n",	format, 		i, current ); }
		else { 					fprintf(stdout, "\t%i:		%i\n", 						i, current ); }

	}

	arith /= counter;


	printf("\t___________________________\n\n");

	fprintf(stdout, "\tMedian: \t%i\n", median );
	fprintf(stdout, "\tArith.: \t%f\n", arith );


	if ( amount == 1 ) {

		fprintf(stdout, "\tModal: \t\t%i\n", ref[0] );

	} else {

		fprintf(stdout, "\n\tModal:\n");

		for ( i = 0; i < amount; i++ ) {

			fprintf(stdout, "\t\t\t%i\n", ref[i] );
		}

		fprintf(stdout,"\n");
	}

	fprintf(stdout, "\tMax.: \t\t%i\n", max );
	fprintf(stdout, "\tMin.: \t\t%i\n", min );

	printf("\t___________________________\n\n");
}




int main ( int argc, char * argv[] ) {


	if ( argc < 2 ) {

		fprintf(stderr, "\n%s\n", "\nERROR:\t(1) Missing argument\n\nUSAGE:\tYou need to provide the filename of a valid asci/binary pgm-image as an argument.\n\n\tFor more information use --help !" );

		exit(1);
	}

	if ( argc > 2 ) {

		fprintf(stderr, "\n%s\n", "\nERROR:\t(2) Too many arguments\n\nUSAGE:\tYou can just use one argument (the filename of a valid asci/binary pgm-image).\n\n\tFor more information use --help !" );

		exit(2);
	}


	if ( strcmp( argv[1], "--help" ) == 0 || strcmp( argv[1], "-h" ) == 0 ) {

		fprintf(stderr, "\n%s\n", "\n\tThis programm can read an arbitary ascii or binary pgm-image and show it's statistics.\n\tIt takes the filename as the first argument and lists the results.");

		exit(0);
	}


	char * name = argv[1];

	FILE * fi = fopen( name , "r");

	if ( !fi ) {

		fprintf(stderr, "\n%s\n", "\nERROR:\t(3) The file couldn't be read\n\nUSAGE:\tEither the choosen file doesn't exist or you don't have the required permission for reading the file.\n\n\tIf you are not allowed to change something on the system by yourself - ask a trustworthy systemadmin for help !" );

		exit(3);
	}



	fprintf(stdout,"\n\n");

	fprintf(stdout, "\tFilename: \t%s\n\n", name );


	char type[10];
	getType( fi, type, sizeof(type) );

	char comments[2024];
	char last;

	getComments( fi, comments, sizeof(comments), &last );

	char width[100];
	getWidth( fi, width, sizeof(width), last );

	char height[100];
	getHeight( fi, height, sizeof(height) );

	char range[100];
	getRange( fi, range, sizeof(range) );


	getStats( fi, type, atoi(range) );


	fclose( fi );


	fprintf(stderr, "\t%s\n\n", "::\t SUCCESS\t ::" );

	return 0;
}
