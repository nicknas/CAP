#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include "io.h"

/************************************/
/* Lucas-Kanade 1 Step              */
/************************************/
void luca_kanade_1step(float *Vx, float *Vy,
	float *Ix, float *Iy, float *It, 
	int spac_conv_size, int temp_conv_size, int window_size, int nx, int ny)
{
	float sumIx2 =0.0;
	float sumIxIy=0.0;
	float sumIy2 =0.0;
	float sumIxIt=0.0;
	float sumIyIt=0.0;
	int i, j, ii, jj;
	int pixel_id;

	int window_center = (window_size-1)/2;

	int spac_conv_center = (spac_conv_size-1)/2;

	for (i=window_center; i<ny-window_center; i++)
		for (j=window_center; j<nx-window_center; j++)
		{
			sumIx2 =0.0;
			sumIxIy=0.0;
			sumIy2 =0.0;
			sumIxIt=0.0;
			sumIyIt=0.0;

			for (ii=-window_center;ii<=window_center; ii++)
				for (jj=-window_center;jj<=window_center; jj++)
				{
					pixel_id = (i+ii)*nx+(j+jj);
					sumIx2  += Ix[pixel_id]*Ix[pixel_id];
					sumIxIy += Ix[pixel_id]*Iy[pixel_id];
					sumIy2  += Iy[pixel_id]*Iy[pixel_id];
					sumIxIt += Ix[pixel_id]*It[pixel_id];
					sumIyIt += Iy[pixel_id]*It[pixel_id];
				}
			float detA = (sumIx2*sumIy2-sumIxIy*sumIxIy);

			//Luca-Kanade desarrollado el producto vectorial con la inversa 2x2
			if (detA!=0.0){
				Vx[i*nx+j] = 1.0/detA*(sumIy2*(-sumIxIt)     + (-sumIxIy)*(-sumIyIt));
				Vy[i*nx+j] = 1.0/detA*((-sumIxIy)*(-sumIxIt) + sumIx2*(-sumIyIt));
				
			} else {
				Vx[i*nx+j] = 0.0;
				Vy[i*nx+j] = 0.0;
			}
		}
}


/************************************/
/* temporal convolution             */
/************************************/
void temp_convolution(float *im_conv, float *frame, float *frame_prev, int nx, int ny)
{
	int ii,jj;

	for(ii = 0; ii < ny; ++ii)
	{
		for(jj=0; jj < nx; ++jj) // number of columns
		{
			im_conv[ii*nx + jj] = frame[ii*nx + jj]-frame_prev[ii*nx + jj];
		}
	}
}


/***************************/
/* convolution 2D centered */
/***************************/
void spac_convolution2D_x(float *im_conv, float *frame_in, int nx, int ny, float *filter, int filter_size)
{
	int ii, jj, m;
	float pixel;

	float *ptmp;

	int filter_center = (filter_size-1)/2;

	for (ii=0; ii<ny; ii++)
	{
		for (jj=0; jj<nx; jj++)
		{
			im_conv[ii*nx + jj] = 0.0;

			pixel = 0.0;
			for(m =-filter_center; m<=filter_center; ++m)  // full kernel
			{
				if (jj+m>=0 && jj+m<nx)
					pixel += frame_in[ii*nx + jj+m] * filter[m+filter_center];
			}
			im_conv[ii*nx + jj] = pixel;

		}
	}
}

void spac_convolution2D_y(float *im_conv, float *frame_in, int nx, int ny, float *filter, int filter_size)
{
	int ii, jj, m;
	float pixel;

	float *ptmp;

	int filter_center = (filter_size-1)/2;

	for (ii=0; ii<ny; ii++)
	{
		for (jj=0; jj<nx; jj++)
		{
			im_conv[ii*nx + jj] = 0.0;

			pixel = 0.0;
			for(m =-filter_center; m<=filter_center; ++m)  // full kernel
			{
				if (ii+m>=0 && ii+m<ny)
					pixel += frame_in[(ii+m)*nx + jj] * filter[m+filter_center];
			}
			im_conv[ii*nx + jj] = pixel;

		}
	}
}


void getfilters(float *filt_x, float *filt_y, float *filt_t, int spac_filt_size, int temp_filt_size)
{
	float filt[9];
	int i;
	
	if (spac_filt_size==1){
		filt[0] = 1;
	} else if (spac_filt_size==3){
		filt[0] = -1.0/2.0;
		filt[1] =    0;
		filt[2] =  1.0/2.0;
	} else if (spac_filt_size==5){
		filt[0] =  1.0/12.0;
		filt[1] = -2.0/3.0;
		filt[2] =    0;
		filt[3] =  2.0/3.0;
		filt[4] = -1.0/12.0;
	} else if (spac_filt_size==7){
		filt[0] = -1.0/60.0;
		filt[1] =  3.0/20.0;
		filt[2] = -3.0/4.0;
		filt[3] =    0;
		filt[4] =  3.0/4.0;	
		filt[5] = -3.0/20.0;
		filt[6] =  1.0/60.0;
	} else if (spac_filt_size==9){
		filt[0] =  1.0/280.0;
		filt[1] = -4.0/105.0;
		filt[2] =  1.0/5.0;
		filt[3] = -4.0/5.0;
		filt[4] =    0;	
		filt[5] =  4.0/5.0;
		filt[6] = -1.0/5.0;
		filt[7] =  4.0/105.0;
		filt[8] = -1.0/280.0;
	} else 	{
		printf("Not developted!!!\n");
	}

	// Copy filt_x and filt_y
	for (i=0;i<spac_filt_size;i++)
		filt_x[i] = filt_y[i] = filt[i];

	if (temp_filt_size==1){
		filt[0] = 1;
	} else if (temp_filt_size==2){
		filt[0] = -1.0;
		filt[1] =  1.0;
	} else if (temp_filt_size==3){
		filt[0] = -1.0/2.0;
		filt[1] =    0;
		filt[2] =  1.0/2.0;
	} else if (temp_filt_size==5){
		filt[0] =  1.0/12.0;
		filt[1] = -2.0/3.0;
		filt[2] =    0;
		filt[3] =  2.0/3.0;
		filt[4] = -1.0/12.0;
	} else if (temp_filt_size==7){
		filt[0] = -1.0/60.0;
		filt[1] =  3.0/20.0;
		filt[2] = -3.0/4.0;
		filt[3] =    0;
		filt[4] =  3.0/4.0;	
		filt[5] = -3.0/20.0;
		filt[6] =  1.0/60.0;
	} else if (temp_filt_size==9){
		filt[0] =  1.0/280.0;
		filt[1] = -4.0/105.0;
		filt[2] =  1.0/5.0;
		filt[3] = -4.0/5.0;
		filt[4] =    0;	
		filt[5] =  4.0/5.0;
		filt[6] = -1.0/5.0;
		filt[7] =  4.0/105.0;
		filt[8] = -1.0/280.0;
	} else 	{
		printf("Not developted!!!\n");
	}
	// Copy filt_t
	for (i=0;i<temp_filt_size;i++)
		filt_t[i] = filt[i];
}

void init(float **movie, char *file_name, float **filt_x, float **filt_y, float **filt_t, 
	float **Ix, float**Iy, float **It,
	int nx, int ny, int nframes, int temp_filt_size, int spac_filt_size, int window_size,
	float **Vx, float **Vy)
{
	*movie  = get_memory1D_float( nframes*nx*ny );
	*filt_x = get_memory1D_float( spac_filt_size );
	*filt_y = get_memory1D_float( spac_filt_size );
	*filt_t = get_memory1D_float( temp_filt_size );

	*Ix     = get_memory1D_float( nx*ny );
	*Iy     = get_memory1D_float( nx*ny );
	*It     = get_memory1D_float( nx*ny );

	*Vx     = get_memory1D_float( nframes*nx*ny );
	*Vy     = get_memory1D_float( nframes*nx*ny );

//	read_movie_bin(*movie, file_name, nframes, ny, nx);
	read_movie_png(*movie, file_name, nframes, ny, nx);
	getfilters(*filt_x, *filt_y, *filt_t, spac_filt_size, temp_filt_size);
} 

void lucas_kanade(float *Vx, float  *Vy, 
	float *frame, float *frame_prev, int nx, int ny, int fr, float *Ix, float *Iy, float *It,
	float *filt_x, float *filt_y, int spac_conv_size, 
	float *filt_t, int temp_conv_size,
	int window_size)
{
	// It
	temp_convolution(It, frame, frame_prev, nx, ny);

	// Ix = imfilter(frame, filter_x,'left-top');
	spac_convolution2D_x(Ix, frame, nx, ny,
		filt_x, spac_conv_size);

	// Iy = imfilter(frame, filter_y,'left-top');
	spac_convolution2D_y(Iy, frame, nx, ny,
		filt_y, spac_conv_size);

	luca_kanade_1step(Vx, Vy, Ix, Iy, It,
		spac_conv_size, temp_conv_size, window_size, nx, ny);
}


void main(int argc, void **argv)
{
	float *movie, *filt_x, *filt_y, *filt_t;
	float *frame_in, *frame_prev;
	float *Ix, *Iy, *It;
	float *Vx, *Vy;
	int window_size, spac_conv_size, temp_conv_size, nx, ny, nframes;
	char file_name[100];
	int fr,i,j;
	double t0, t1;

	setbuf(stdin, NULL);
	setbuf(stdout, NULL);

	if (argc==8){
		strcpy(file_name, argv[1]);
		nframes        = atoi(argv[2]);
		nx             = atoi(argv[3]);
		ny             = atoi(argv[4]);
		window_size    = atoi(argv[5]);
		spac_conv_size = atoi(argv[6]);
		temp_conv_size = atoi(argv[7]);
	} else {
		printf("Error in input parameter ./exec file_name nframes nx ny window_size spac_conv_size temp_conv_size\n");
		exit(-1);
	}

	
	/* Init memory, film and filters */
	init(&movie, file_name, &filt_x, &filt_y, &filt_t, 
		&Ix, &Iy, &It, 
		nx, ny, nframes, temp_conv_size, spac_conv_size, window_size,
		&Vx, &Vy);

	t0 = get_time();
	for (fr=1; fr<nframes; fr++)
	{
		frame_in   = movie+(fr*nx*ny);
		frame_prev = movie+((fr-1)*nx*ny);

		lucas_kanade(Vx+(fr*nx*ny), Vy+(fr*nx*ny),
			frame_in, frame_prev, nx, ny, fr, Ix, Iy, It, 
			filt_x, filt_y, spac_conv_size, 
			filt_t, temp_conv_size, 
			window_size); 

        // printf("frame=%i performed\n",fr);
	}
	t1 = get_time();
	printf("Perfomed %i frames in %f (ms)\n", nframes-temp_conv_size+1, t1-t0);


	//Results
	write_VelAng_png("out/vel", "out/ang", nframes, ny, nx, Vx, Vy);

	free(movie);
	free(filt_x);
	free(filt_y);
	free(filt_t);
	free(Ix);
	free(Iy);
	free(It);
	free(Vx);
	free(Vy);
}

