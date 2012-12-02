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



	char* name = argv[1];

	FILE * fi = fopen( name , "r");

	if ( !fi ) {

		errorMessage( 3, "The file couldn't be read" );
	}


	Image in; // doesnt require manuel free( imgIn ), automatic

	in.name = name;

	short loaded = LoadPGM( fi, &in );

	if ( !loaded ) {

		errorMessage( 4, "Couldn't load the Image data" );
	}


	Image out;

	// if ( argc === 3 ) { // -o

	// 	if ( strcmp( argv[2], "-o" ) == 0  || strcmp( argv[2], "--output" ) == 0 ) {

	// 		// expect ouputfile
	// 	// 	// if ( !argv[3] ) {

	// 	// 		errorMessage( 5, "Missing outputfilename" );
	// 	// 	// }
	// 		fprintf(stdout, "%s\n", "needs output");
	// 	// 	// char* outName = argv[3];
	// 	}

	// // Aufruf: (wahlweise)
	// // pgmop.elf <infile> [-o <outfile>] [ -norm | -invert ]
	// // Beispiele für Aufruf in der Shell mit/ohne Pipe:
	// // pgmop.elf logo.bin.pgm -o normalized.bin.pgm -norm
	// // pgmop.elf logo.bin.pgm -invert | display

	// }





	// // receives the informations..
	// fprintf(stdout, "Name:\t%s\n", imgIn.name );
	// fprintf(stdout, "Magic:\t%s\n", imgIn.magic );
	// // comments
	// fprintf(stdout, "Width:\t%i\n", imgIn.width );
	// fprintf(stdout, "Height:\t%i\n", imgIn.height );
	// fprintf(stdout, "Depth:\t%i\n", imgIn.depth );
	// // data






	// if ( argc > 2 ) {

	// 	fprintf(stderr, "\n%s\n", "\nERROR:\t(2) Too many arguments\n\nUSAGE:\tYou can just use one argument (the filename of a valid asci/binary pgm-image).\n\n\tFor more information use --help !" );

	// 	exit(2);
	// }


	// if ( strcmp( argv[1], "--help" ) == 0 || strcmp( argv[1], "-h" ) == 0 ) {

	// 	fprintf(stderr, "\n%s\n", "\n\tThis programm can read an arbitary ascii or binary pgm-image and show it's statistics.\n\tIt takes the filename as the first argument and lists the results.");

	// 	exit(0);
	// }


	// char * name = argv[1];

	// FILE * fi = fopen( name , "r");

	// if ( !fi ) {

	// 	fprintf(stderr, "\n%s\n", "\nERROR:\t(3) The file couldn't be read\n\nUSAGE:\tEither the choosen file doesn't exist or you don't have the required permission for reading the file.\n\n\tIf you are not allowed to change something on the system by yourself - ask a trustworthy systemadmin for help !" );

	// 	exit(3);
	// }



	// fprintf(stdout,"\n\n");

	// fprintf(stdout, "\tFilename: \t%s\n\n", name );







	// clean up memory / connections
	freeImage( &in );

	fclose( fi );



	fprintf( stdout, "\t%s\n\n", "::\t SUCCESS\t ::" );

	return 0;
}

// main() {

//     Image* img; // 4 byte
//     img = (Image*) malloc( sizeof(Image) ); // 12 byte, heap: wird erst bei free() gelöscht (oder bei programm-ende)
//                             // void*
//     LoadPGM(img);


//     Image imgV; // 12 byte, stack: wird gelöschte wenn block {} verlassen wird

//     imgV = LoadPGMValue(imgV);
//     LoadPGM(&imgV);

//     // normalisieren:

//     byte color = img->data[y][x]; // wertebereich: min bis max
//     byte min, max;
//     //1) auf null schieben
//     color = (color - min) // wertebereich: 0 bis max-min
//     //2) skalieren auf 0 bis 1 - achtung integer-devision ->  reihenfolge: 1) 3) 2)
//     color = color / (max - min) // wertebereich: 0 bis 1
//     //3) skalieren auf 255
//     color = color * 255 // wertebereich: 0 bis 255 :)

//     // zusammen:
//     color = (color - min) * 255 / (max - min);



//     FreeImage(img);

//     free(img);
// }
