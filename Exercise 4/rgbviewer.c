/* to compile:	gcc -g -Wall $(pkg-config --libs --cflags opencv) -o rgbviewer.elf rgbviewer.c */
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
//
#include <cv.h>
#include <highgui.h>


/* Showing ErrorMessage + stop */
void errorMessage ( short exitNumber, char* message ) {

	fprintf( stderr, "\nERROR:\t\t(%i) - %s\n\nUSAGE:\t\trgbviewer.elf <infile> \n\nEXAMPLE:\trgbviewer.elf test.png \n\nDESCRIPTION:\trgbviewer is a small utility tool to show images.\n\t\tIn addition, it can normalize / invert / rotate them.\n\n\t\tThe options can be used with following keys:\n\n\t\t\tq - quit\n\t\t\tn - normalize\n\t\t\ti - invert\n\t\t\th - mirror horizontal\n\t\t\tl - rotate (clockwise 90°)\n\n", exitNumber, message );

	exit( exitNumber );
};


/* clean memory - free pixels data */
void freePixels ( int height, uchar *** pixels ) {

	int	y; // iterator

	for ( y = 0; y < height; y++ ) free( pixels[y] );

	free( pixels );
}


uchar *** getPixels( IplImage * img ) {

	// attributes
	int height    = img->height,
		width     = img->width,
		step      = img->widthStep,
		channels  = img->nChannels,
		x, y; // iterator

	uchar *data = NULL;
	data = (uchar *) img->imageData;

	uchar *** pixels = NULL;

	pixels = (uchar ***) malloc( height * sizeof(uchar **) );

	for ( y = 0; y < height; y++ ) {

		pixels[y] = (uchar **) malloc( width * sizeof(uchar *));

		for ( x = 0; x < width; x++ ) pixels[y][x] = &(data[y * step + x * channels]);
	}

	return pixels;
}


/* normalize image */
void normalize ( IplImage * img, uchar *** pixels ) {

    int width	= img->width,
    	height	= img->height,
    	channels= img->nChannels,
    	value,		// temp
    	x, y, c;	// iterator

	uchar	*max 	= NULL,
			*min 	= NULL,
			*range 	= NULL;

	max = (uchar *) malloc( channels * sizeof(uchar) );
	for ( c = 0; c < channels; c++ ) max[c] = 0;

	min = (uchar *) malloc( channels * sizeof(uchar) );
	for ( c = 0; c < channels; c++ ) min[c] = 255;


	// get min-max
    for ( y = 0; y < height; y++ ) {
    	for ( x = 0; x < width; x++ ) {

     		for ( c = 0; c < channels; c++ ) {

     			value = pixels[y][x][c];

     			if ( value < min[c] ) min[c] = value;
     			if ( value > max[c] ) max[c] = value;
     		}
		}
	}

	range = (uchar *) malloc( channels * sizeof(uchar) );
	for ( c = 0; c < channels; c++ ) range[c] = max[c] - min[c];

	// set normalized value
    for ( y = 0; y < height; y++ ) {
    	for ( x = 0; x < width; x++ ) {

     		for ( c = 0; c < channels; c++ ) {

     			value = pixels[y][x][c];
     			pixels[y][x][c] = ( value - min[c] ) * 255 / range[c];
     		}
		}
	}

	// clean memory
	free( min );
	free( max );
	free( range );
}


/* invert image */
void invert ( IplImage * img, uchar *** pixels ) {

    int width		= img->width,
    	height		= img->height,
    	channels	= img->nChannels,
    	x, y, c;	// iterator

    // set color difference
    for ( y = 0; y < height; y++ ) {
    	for ( x = 0; x < width; x++ ) {

     		for ( c = 0; c < channels; c++ ) pixels[y][x][c] = 255 - pixels[y][x][c];
		}
	}
};


/* rotate image by 90° */
IplImage * rotate ( IplImage * img, uchar *** pixels ) {

    int width		= img->width,
    	height		= img->height,
    	depth		= img->depth,
    	channels	= img->nChannels;

	IplImage *out = NULL;
	out = cvCreateImage( cvSize(height,width), depth, channels );

	uchar *** nPixels = NULL;
	nPixels = getPixels( out );

	int nY, nX, 	// temp
		x, y, c;	// iterator

	// set values
    for ( y = 0; y < height; y++ ) {
    	for ( x = 0; x < width; x++ ) {

 			nY = width - 1 - x;
 			nX = y;

     		for ( c = 0; c < channels; c++ ) nPixels[nY][nX][c] = pixels[y][x][c];
		}
	}

	/* using cv */
	// cvTranspose( img, out );
	// cvFlip( out, NULL, 0 );

	return out;
}


int main ( int argc, char * argv[] ) {

	if ( argc < 2 ) errorMessage( 1, "Missing Arguments" );
	if ( argc > 3 ) errorMessage( 2, "Too many Arguments" );

	IplImage *img = NULL;
	img = cvLoadImage( argv[1], 1 );

	// pixels [y][x][c]
	uchar *** pixels = NULL;
	pixels = getPixels( img );

	char key;

	do {

		key = (char) cvWaitKey(1);

		if ( key == 'n' ) normalize( img, pixels );
		if ( key == 'i' ) invert( img, pixels );
		if ( key == 'h' ) cvFlip( img, img, 1 );

		if ( key == 'l' ) {

			IplImage *tmp = NULL;
			tmp = rotate( img, pixels );

			freePixels( img->height, pixels );
			cvReleaseImage( &img );

			img = tmp;
			pixels = getPixels( img );
		}

		cvShowImage("rgbviewer", img );

	} while ( key != 'q' ); // quit

	// release the image & reference ( tmp )
	freePixels( img->height, pixels );
	cvReleaseImage( &img );

	return 0;
}
