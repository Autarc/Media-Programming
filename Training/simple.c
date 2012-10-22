#include <stdio.h>
#include <string.h>

int main ( int argc, char * argv[] ) {

	printf("%s\n\n", "Types:" );

 	printf("%s", "int:" ); printf("\t%lui\n", sizeof(int) );
 	printf("%s", "char:" ); printf("\t%lui\n", sizeof(char) );
 	printf("%s", "long:" ); printf("\t%lui\n", sizeof(long) );
 	printf("%s", "double:" ); printf("\t%lui\n", sizeof(double) );

	int l = 0;

	printf("\n%s\n", "List:" );

 	while ( ++l < argc ) {

		printf("-%s\n", argv[l] );
	}

	printf("\n%s\n", "End" );

	return 0;
}
