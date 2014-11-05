/*
   This file tests all the edge detection algorithms implemented in
   edge_detectors.c. It also leads with input/output and with the
   parameters handling.

   Copyright (c) 2011-2013, Haldo Sponton <haldos@fing.edu.uy>
   Copyright (c) 2011-2013, Juan Cardelino <juanc@fing.edu.uy>

   This program is free software: you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation, either version 3 of
   the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <stdlib.h>
#include "classic_edge_detectors.h"   // edge detection algorithms
#include "io_png.h"                   // image input/output

/* Display help function */
void displayhelp(void) {
    printf("Usage: edges [options] input.png\n"
           "  -r <th>                Roberts' algorithm\n"
           "                            th      threshold (float in [0,1])\n"
           "  -p <th>                Prewitt's algorithm\n"
           "                            th      threshold (float in [0,1])\n"
           "  -s <th>                Sobel's algorithm\n"
           "                            th      threshold (float in [0,1])\n"
           "  -m <sigma> <n> <th>    Marr-Hildreth algorithm (Gaussian)\n"
           "                            sigma   std dev of Gaussian kernel\n"
           "                            n       size of Gaussian kernel (int)\n"
           "                            th      threshold (float in [0,1])\n"
           "  -l <sigma> <n> <th>    Marr-Hildreth algorithm (LoG)\n"
           "                            sigma   std dev of LoG kernel\n"
           "                            n       size of LoG kernel (int)\n"
           "                            th      threshold (float in [0,1])\n"
           "  -h <th>                Haralick's algorithm\n"
           "                            th      threshold (float in [0,1])\n"
           "  -z <pd>                Boundary condition used\n"
           "                            pd      padding method (0 zero padding, 1 periodic)\n"
           "  -o                     output filename prefix\n"
			  "                            bname   basename of the file (Default value=\"out\")\n"
			  "  -H                     Display this help\n"
           "\nMore than one option can be used together. "
           "\nOutput is written to image files. Output filename has the form <output_prefix>_<algorithm>.png."
				"\nFor example if output_prefix is \"oranges\" and you run Sobel, the output filename will be \"oranges_sobel.png\"\n");
}

/* Main function */
int main(int argc, char *argv[]) {

    /* Start */
    printf("A review of classic edge detection algorithms\n");
    printf("Haldo Sponton & Juan Cardelino, IPOL 2014\n\n");

    /* Options and parameters handling */
    int argc_sobel = 0;         //
    int argc_prewitt = 0;       //
    int argc_roberts = 0;       // indexes in argv[]
    int argc_mh = 0;            //
    int argc_mhl = 0;           //
    int argc_haralick = 0;      //
    int argc_padding = 0;      //
	 int argc_output = 0;      //
    int padding_method = 1;     // Reflection of image boundary. Hard-coded.
                                // Can be changed to 0 (zero-padding)

    for(int n = 1; n<argc; n++ ) {           // scan through args
        switch( (int)argv[n][0] ) {          // check for option character
            case '-':
                switch( (int)argv[n][1] ) {
                    case 'r':  argc_roberts = n;
                               break;
                    case 'p':  argc_prewitt = n;
                               break;
                    case 's':  argc_sobel = n;
                               break;
                    case 'm':  argc_mh = n;
                               break;
                    case 'l':  argc_mhl = n;
                               break;
                    case 'h':  argc_haralick = n;
                               break;
                    case 'z':  argc_padding = n;
                               break;
						  case 'o':  argc_output = n;
                               break;
                    case 'H':  /* Display help! */
                               displayhelp();
                               exit(1);
                               break;
                    default:   printf("Error: Invalid option -> %s. Valid"
                                      " options are -r, -p, -s, -h, -m, -l, -z"
                                      " (-H for help).\n", argv[n]);
                               exit(1);
                               break;
                }

                /* Error message if an option is the last input parameter
                   or if the next parameter after an option is another option */
                if( n==argc-1 ) {
                    printf("Error: Missing parameter(s) for %s option.\n",
                           argv[n]);
                    exit(1);
                    break;
                } else if( argv[n+1][0]=='-' ) {
                    printf("Error: Missing parameter(s) for %s option.\n",
                            argv[n]);
                    exit(1);
                    break;
                }
                /* In the case of -m and -l, three parameters are needed */
                if( (argv[n][1]=='m') || (argv[n][1]=='l') ) {
                    if( n+3>argc-1 ) {
                        printf("Error: Missing parameter(s) for %s option.\n",
                               argv[n]);
                        exit(1);
                        break;
                    } else if( (argv[n+2][0]=='-') || (argv[n+3][0]=='-') ) {
                        printf("Error: Missing parameter(s) for %s option.\n",
                               argv[n]);
                        exit(1);
                        break;
                    }
                }
            default:
                break;
        }
    }

    /* Check for total number of parameters */
    int nparam = 2;
    if(argc_roberts!=0)  nparam+=2;
    if(argc_prewitt!=0)  nparam+=2;
    if(argc_sobel!=0)    nparam+=2;
    if(argc_mh!=0)       nparam+=4;
    if(argc_mhl!=0)      nparam+=4;
    if(argc_haralick!=0) nparam+=2;
    if(argc_padding!=0)  nparam+=2;
	 if(argc_output!=0)  nparam+=2;
    if(nparam!=argc) {
        printf("Error: Wrong number of arguments (%d instead of %d)\n"
               "Usage: %s [options] input.png\n"
               "For help type %s -H.\n",argc-1,nparam-1,argv[0],argv[0]);
        exit(1);
    }

    /* Load input image */
    size_t w, h;
    float *im = io_png_read_f32_gray(argv[argc-1], &w, &h);
    printf("Input image: %s\n",argv[argc-1]);
    printf("Size: %zd x %zd\n\n",w,h);

	 /* pick padding method */
    if(argc_padding!=0) {
		 padding_method=atoi(argv[argc_padding+1]);
	 }

	 const char fname_output[256];
	 const char basename_output[256];
	 sprintf(basename_output,"%s","out");
	 if(argc_output!=0)
	 {
		 sprintf(basename_output,"%s",argv[argc_output+1]);
		 printf("basename_output: %s\n",basename_output);
	 }

	 /* Roberts edge detection algorithm */
    if(argc_roberts!=0) {
        float th_roberts = atof(argv[argc_roberts+1]); // threshold
        printf("Running Roberts, threshold=%.2f",th_roberts);
        float *im_roberts = edges_roberts(im, w, h, th_roberts, padding_method);
		  sprintf(fname_output,"%s_%s",basename_output,"roberts.png");
        io_png_write_f32(fname_output, im_roberts, w, h, 1);
        free(im_roberts);
    }

    /* Prewitt edge detection algorithm */
    if(argc_prewitt!=0) {
        float th_prewitt = atof(argv[argc_prewitt+1]); // threshold
        printf("Running Prewitt, threshold=%.2f",th_prewitt);
        float *im_prewitt = edges_prewitt(im, w, h, th_prewitt, padding_method);
		  sprintf(fname_output,"%s_%s",basename_output,"prewitt.png");
        io_png_write_f32(fname_output, im_prewitt, w, h, 1);
        printf(" output: %s\n",fname_output);
        free(im_prewitt);
    }

    /* Sobel edge detection algorithm */
    if(argc_sobel!=0) {
        float th_sobel = atof(argv[argc_sobel+1]); // threshold
        printf("Running Sobel, threshold=%.2f",th_sobel);
        float *im_sobel = edges_sobel(im, w, h, th_sobel, padding_method);
		  sprintf(fname_output,"%s_%s",basename_output,"sobel.png");
        io_png_write_f32(fname_output, im_sobel, w, h, 1);
        free(im_sobel);
    }

    /* Marr-Hildreth (Gaussian) edge detection algorithm */
    if(argc_mh!=0) {
        float sigma_m = atof(argv[argc_mh+1]); // Gaussian kernel's std dev.
        int   n_m     = atoi(argv[argc_mh+2]); // kernel size
        float tzc_m   = atof(argv[argc_mh+3]); // threshold for zero-crossing
        printf("Running Marr-Hildreth (Gaussian), sigma=%.2f, N=%d, TZC=%.2f",
               sigma_m, n_m, tzc_m);
        float *im_mh = edges_mh(im, w, h, sigma_m, n_m, tzc_m, padding_method);
		  sprintf(fname_output,"%s_%s",basename_output,"mh.png");
        io_png_write_f32(fname_output, im_mh, w, h, 1);
        free(im_mh);
    }

    /* Marr-Hildreth (LoG) edge detection algorithm */
    if(argc_mhl!=0) {
        float sigma_l = atof(argv[argc_mhl+1]); // Gaussian kernel's std dev.
        int   n_l     = atoi(argv[argc_mhl+2]); // kernel size
        float tzc_l   = atof(argv[argc_mhl+3]); // threshold for zero-crossing
        printf("Running Marr-Hildreth (LoG), sigma=%.2f, N=%d, TZC=%.2f",
               sigma_l, n_l, tzc_l);
        float *im_mhl = edges_mh_log(im, w, h,
                                     sigma_l, n_l, tzc_l, padding_method);
		  sprintf(fname_output,"%s_%s",basename_output,"mhl.png");
        io_png_write_f32(fname_output, im_mhl, w, h, 1);
        free(im_mhl);
    }

    /* Haralick edge detection algorithm */
    if(argc_haralick!=0) {
        float rhozero = atof(argv[argc_haralick+1]); // threshold
        printf("Running Haralick, rhozero=%.2f",rhozero);
        float *im_haralick = edges_haralick(im, w, h, rhozero, padding_method);
		  sprintf(fname_output,"%s_%s",basename_output,"haralick.png");
        io_png_write_f32(fname_output, im_haralick, w, h, 1);
        free(im_haralick);
    }

	 printf(" output: %s\n",fname_output);
    /* Memory free */
    free(im);

    exit(EXIT_SUCCESS);
}
