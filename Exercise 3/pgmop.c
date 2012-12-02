/*
	....
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

// #include <mypgmstd.h>
#include <mypgm.h>


// pgmop.elf <infile> [-o <outfile>] [ -norm | -invert ]

// need to regard  the dependency - of -o, then output file required !


int main ( int argc, char * argv[] ) {


	if ( argc < 2 ) {

		errorMessage( 1, "Missing Arguments" );
	}

	if ( argc > 5 ) {

		errorMessage( 2, "Too many Arguments" );
	}


	char* inName = argv[1];

	FILE * fi = fopen( inName , "r" );

	if ( !fi ) {

		errorMessage( 3, "The file couldn't be read" );
	}


	Image in; // doesnt require manuel free( imgIn ), automatic

	in.name = inName;

	short loaded = LoadPGM( fi, &in );

	if ( !loaded ) {

		errorMessage( 4, "Couldn't load the Image data" );
	}


	// // Aufruf: (wahlweise)
	// // pgmop.elf <infile> [-o <outfile>] [ -norm | -invert ]
	// // Beispiele für Aufruf in der Shell mit/ohne Pipe:
	// // pgmop.elf logo.bin.pgm -o normalized.bin.pgm -norm 	// 5
	// // pgmop.elf logo.bin.pgm -invert | display				// 3

	// work, logic
	if ( argc >= 3 ) {

		Image out;
		FILE* fo = stdout;


		// copy basics
		setStats( &in, &out );


		// currently : all second argument

		if ( strcmp( argv[2], "-o" ) == 0  || strcmp( argv[2], "--output" ) == 0 ) { // 3

			if ( !argv[3] ) { // expect ouputfile

				errorMessage( 5, "Missing output filename" );
			}

			char* outName = argv[3];

			fo = fopen( outName, "w" );

			if ( !fo ) {

				errorMessage( 6, "The file couldn't be created" );
			}

			out.name = outName;
		}


		if ( strcmp( argv[2], "-norm" ) == 0 || strcmp( argv[2], "-n" ) == 0 || // 3 || 5
			 ( argv[4] && strcmp( argv[4], "-norm" ) == 0 ) ||
			 ( argv[4] && strcmp( argv[4], "-n" ) == 0 ) ) {

			byte normalized = NormPGM( &in, &out );

			if ( !normalized ) {

				errorMessage( 7, "The image couldn't be normalized" );
			}
		}


		if ( strcmp( argv[2], "-invert" ) == 0 || strcmp( argv[2], "-i" ) == 0 || // 3 || 5
			 ( argv[4] && strcmp( argv[4], "-invert" ) == 0 ) ||
			 ( argv[4] && strcmp( argv[4], "-i" ) == 0 ) ) {

			byte inverted = InvertPGM( &in, &out );

			if ( !inverted ) {

				errorMessage( 8, "The image couldn't be inverted" );
			}
		}


		SavePGM( fo, &out ); // showing - creating output from the data

		freeImage( &out );

		fclose( fo );
	}



	// clean up memory / connections
	freeImage( &in );

	fclose( fi );

	fprintf( stdout, "\t%s\n\n", "::\t SUCCESS\t ::" );

	return 0;
}
