typedef unsigned char byte;

typedef struct image {

	char*	name;
	char	magic[2];
	char**	comments;

	unsigned short	width;
	unsigned short	height;
	unsigned short	depth;

	byte**	data;

	unsigned short	min;
	unsigned short	max;

} Image;


unsigned short LoadPGM( FILE * fi, Image * in );
unsigned short SavePGM( FILE * fo, Image * out );
unsigned short NormPGM( Image * in, Image * out );
unsigned short InvertPGM( Image * in, Image * out );

/* own declarations */

void errorMessage( short exitNumber, char* message );
void setStats( Image * in, Image * out );
void freeImage( Image * img );
