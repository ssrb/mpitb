/* Sequential Mandelbrot program */
/* from: http://www.cs.uncc.edu/par_prog */
/* from: http://www.cs.uncc.edu/~abw/ITCS3145F05/index.html */

/*
 * Compiling: To compile mandelbrot.c to create the executable mandelbrot:
 * cc -I/usr/openwin/include -o mandelbrot mandelbrot.c
 *    -L/usr/openwin/lib -L/usr/X11R6/lib -lX11 -lm
 *		   where -L/usr/X11R6/lib links the Xlib library.
 * Parallel code: use mpicc with same flags/libraries.
 */


#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <stdio.h>
//#include <string.h>
//#include <math.h>

#define		REAL_MIN -2.0	/* abscissa from -2 to 2 */
#define		REAL_MAX  2.0
#define		IMAG_MIN -2.0	/* ordinate from -2 to 2 */
#define		IMAG_MAX  2.0
#define		X_SIZE	512	/* x size in pixels (wide) */
#define		Y_SIZE (X_SIZE * (IMAG_MAX-IMAG_MIN) / (REAL_MAX-REAL_MIN))
				/* y size in pixels (tall) */
#define		MAX_ITER 1024

typedef struct complextype
	{
        float real, imag;
	} Compl;


//void main ()
   int main ()
{
	Window		win;		/* initialization for a window */
	unsigned
	int             width, height,	/* window size */
                        x, y,		/* window position */
                        border_width,	/*border width in pixels */
                        display_width, display_height,	/* size of screen */
                        screen;				/* which screen */

	char            *window_name = "Mandelbrot Set", *display_name = NULL;
	GC              gc;
	unsigned
	long		valuemask = 0;
	XGCValues	values;
	Display		*display;
	XSizeHints	size_hints;
//	Pixmap		bitmap;
//	XPoint		points[800];
//	FILE		*fp, *fopen ();
//	char		str[100];
	
	XSetWindowAttributes attr[1];

       /* Mandlebrot variables */

	int i, j, k;
        Compl	z, c;
        float	lengthsq, temp;
        float	scale_real, scale_imag;
	int	l, m, r,g,b;		/* light, modulus, red/green/blue */
	int	iter;
       
	/* connect to Xserver */

	if (  (display = XOpenDisplay (display_name)) == NULL ) {
	   fprintf (stderr, "drawon: cannot connect to X server %s\n",
				XDisplayName (display_name) );
	exit (-1);
	}
	
	/* get screen size */

	screen = DefaultScreen (display);
	display_width = DisplayWidth (display, screen);
	display_height = DisplayHeight (display, screen);

	/* set window size */

	width = X_SIZE;
	height = Y_SIZE;

	/* set window position */

	x = 0;
	y = 0;

        /* create opaque window */

	border_width = 4;
	win = XCreateSimpleWindow (display, RootWindow (display, screen),
				x, y, width, height, border_width, 
					    BlackPixel (display, screen),
					    WhitePixel (display, screen));

	size_hints.flags = USPosition|USSize;
	size_hints.x = x;
	size_hints.y = y;
	size_hints.width = width;
	size_hints.height = height;
	size_hints.min_width = 300;
	size_hints.min_height = 300;
	
	XSetNormalHints (display, win, &size_hints);
	XStoreName(display, win, window_name);

        /* create graphics context */

	gc = XCreateGC (display, win, valuemask, &values);

	XSetBackground (display, gc, WhitePixel (display, screen));
	XSetForeground (display, gc, BlackPixel (display, screen));
//	XSetLineAttributes (display, gc, 1, LineSolid, CapRound, JoinRound);
/*
	attr[0].backing_store = Always;
	attr[0].backing_planes = 1;
	attr[0].backing_pixel = BlackPixel(display, screen);

	XChangeWindowAttributes(display, win,
			CWBackingStore | CWBackingPlanes | CWBackingPixel,
				attr);
*/
	XMapWindow (display, win);
	XSync(display, 0);
      	 
        /* Calculate and draw points */
	scale_real = (REAL_MAX-REAL_MIN)/(X_SIZE-1);	// j=0..X_SIZE-1
	scale_imag = (IMAG_MAX-IMAG_MIN)/(Y_SIZE-1);	// i=0..Y_SIZE-1

for (iter=0; iter<=MAX_ITER; iter++){

        for(i=0; i < Y_SIZE; i++) 
        for(j=0; j < X_SIZE; j++) {

          z.real = z.imag = 0.0;
	  c.real = REAL_MIN + ((float) j * scale_real);
	  c.imag = IMAG_MIN + ((float) i * scale_imag);
          k = 0;

          do  {					/* iterate for pixel color */

            temp = z.real*z.real - z.imag*z.imag + c.real;
            z.imag = 2.0*z.real*z.imag + c.imag;
            z.real = temp;
            lengthsq = z.real*z.real+z.imag*z.imag;
            k++;

          } while (lengthsq < 9.0 && k < iter);	/* mod(2+2i)=2.8284 */

	m=k-1;
	l=m&0x3;		/* light: 0..7, 3 LSB's */
	k=m>>2; k&=0x3;		/* kolor:grey,R,G,B, cyclic, 2 next LSB's */

	l<<=6; r=g=b=0xFF;	/* lighten up */
	switch(k){
	case 0:	r=g=b=l;break;
	case 1:	r=    l;break;
	case 2:	  g  =l;break;
	case 3:     b=l;break;
	};
	l =(r<<8|g) <<8|b;
	l^=0xFFFFFF;		/* reverse video */
	XSetForeground(display,gc,l);
	XDrawPoint (display, win, gc, j, i);
        }
	 
	XFlush (display);
//	sleep (30);
//	printf("iter=%d\n",iter);
//	getchar();
}
	/* Program Finished */
	return(0);
}

