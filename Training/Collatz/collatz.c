/*
	A small c command line utility to follow the Collatz conjecture.

	@params

	argv[2] => seed
	argv[3] => logging file (* optional)
*/

#include <stdio.h>
#include <string.h>


int collatz ( long int num, char file[] ) {

	int i = 0;

	// char steps[1000] = "\0";

	// if ( file ) {

	// 	FILE* fo = 0;

	// 	fo = fopen( file, "w" );

	// 	if ( !fo ) {
	// 		printf("Can't write to %s\n", file );
	// 	}
	// }

	do {

		if ( num%2 == 0 ) {

			num /= 2;

		} else {

			num = num * 3 + 1;
		}

		i++;

		// if ( fo ) {

		// 	fprintf(fo, "%d: %ld\n", i, num);
		// 	fflush(fo);
		// }

	} while ( num != 1 );

	return i;
};





int main ( int argc, char * argv[] ) {


	if ( argc != 2 && argc != 3 ) {

		printf("\n%s\n", "Invalid/missing input.");

		return 1;
	}


	long int num = atol(argv[1]);

	if ( num < 0 ) {

		printf("\n%s\n", "No natural number.");

		return 2;
	}

	int steps = 0;

	if ( argc == 2 ) {

		steps = collatz( num, "\0" );

	} else {

		// char file[] = argv[3];
		// steps = collatz( num, file );
	}

	printf("\nsteps: %i\n", steps );


	return 0;
}
