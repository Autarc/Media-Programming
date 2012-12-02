
typedef unsigned char byte;

typedef struct image {

	char*	name;
	char	magic[2];
	char**	comments;

	// unsigned
	short	width;
	short	height;
	short	depth;

	byte**	data;

	short	min;
	short	max;

} Image;



unsigned short LoadPGM( FILE * fi, Image * in );
unsigned short SavePGM( FILE * fo, Image * out );
unsigned short NormPGM( Image * in, Image * out );
unsigned short InvertPGM( Image * in, Image * out );

/* own function declarations for reading etc. */

void errorMessage( short exitNumber, char* message );

void nextString( FILE* fi, char * buffer, int bufferSize, char start );

void freeImage( Image * img );
