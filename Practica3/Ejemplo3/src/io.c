#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>
#include <sys/resource.h>
#include "pngio.h"

float *get_memory1D_float( int nx )
{
	int i,j;
	float *buffer;

	if( (buffer=(float *)malloc(nx*sizeof(float)))== NULL )
	{
		fprintf( stderr, "ERROR in memory allocation\n" );
		return( NULL );
	}

	for( i=0; i<nx; i++ )
		buffer[i] = 0.0;

	return( buffer );
}

void delete_memory1D_float( float *buffer )
{ 
	free(buffer);
}


float **get_memory2D_float( int nx, int ny )
{
	int i,j;
	float **buffer;

	if( (buffer=(float **)malloc(nx*sizeof(float *)))== NULL )
	{
		fprintf( stderr, "ERROR in memory allocation\n" );
		return( NULL );
	}

	if( (buffer[0]=(float *)malloc(nx*ny*sizeof(float)))==NULL )
	{
		fprintf( stderr, "ERROR in memory allocation\n" );
		free( buffer );
		return( NULL );
	}

	for( i=1; i<nx; i++ )
	{
		buffer[i] = buffer[i-1] + ny;
	}

	for( i=0; i<nx; i++ )
		for( j=0; j<ny; j++ )
		{
			buffer[i][j] = 0.0;
		}

	return( buffer );
}

void delete_memory2D_float( float **buffer )
{ 
	free(buffer[0]);
	free(buffer);
}


int read_movie_png(float *frames, char *file_name, int nframes, int height, int width)
{
    int i,j,fr;
    char file[100];
    int width_read, height_read;
    unsigned char *image;

	for (fr=0; fr<nframes; fr++)
    {
        sprintf(file,"%s%i.png", file_name, fr+1);
        image = readImagePNG(file, &width_read, &height_read);
        if (width_read!=width || height_read!=height)
            return(0);

        for (i=0; i<height; i++)
            for (j=0; j<width; j++)
                frames[fr*width*height+i*width+j] = (float)(image[i*width+j]);
    }
    free(image);
	return(1);
}

int read_movie_bin(float *frames, char *file_name, int nframes, int height, int width)
{
	FILE *in;
	size_t length=nframes*height*width;

	if ((in = fopen(file_name, "rb")) == NULL) {
		fprintf(stderr, "Couldn't open %s\n", file_name);
		return(0);
	}       

	if(fread(frames,sizeof(float), length, in) != length){
		fprintf(stderr,"Ran out of frames\n");
		return(0);
	}       

	fclose(in);
	return(1);
}

void write_vel_bin(char* nameVx,char* nameVy, int nframes, int height, int width, float *Vx,float *Vy)
{
	FILE *vx;
	FILE *vy;
	size_t length=nframes*height*width;

	if ((vx = fopen(nameVx, "wb")) == NULL) {
		fprintf(stderr, "Couldn't create %s\n", nameVx);
		exit(-1);
	}
	if ((vy = fopen(nameVy, "wb")) == NULL) {
		fprintf(stderr, "Couldn't create %s\n", nameVy);
		exit(-1);
	}
	if(fwrite(Vx,sizeof(float), length, vx) != length){
		fprintf(stderr,"Ran out of frames in %s\n",nameVx);
		exit(-1);
	}
	if(fwrite(Vy,sizeof(float), length, vy) != length){
		fprintf(stderr,"Ran out of frames in %s\n",nameVy);
		exit(-1);
	}
	if (fclose(vx)) 
	{
		printf("error closing file.");
		exit(-1); 
	}
	if (fclose(vy)) 
	{
		printf("error closing file.");
		exit(-1); 
	}
}

void write_VelAng_bin(char* nameVel,char* nameAngle, int nframes, int height, int width, float *Vx,float *Vy)
{
	FILE *ang;
	FILE *vel;
	size_t length=nframes*height*width;
	int fr, i, j;
	float vx, vy;

	float *Vel  = NULL;
	float *Ang  = NULL;

	Vel = get_memory1D_float( nframes*height*width );
	Ang = get_memory1D_float( nframes*height*width );

	for (fr=0; fr<nframes; fr++)
		for (i=0; i<height; i++)
			for (j=0; j<width; j++)
			{
				vx = Vx[fr*height*width+ i*width + j];
				vy = Vy[fr*height*width+ i*width + j];

				if (vx!=0.0 && vy!=0.0) {
					Vel[fr*height*width+ i*width + j] = sqrtf(vx*vx+vy*vy);
					Ang[fr*height*width+ i*width + j] = atan2f(vy,vx)*180/3.14159;
				} else {
					Vel[fr*height*width+ i*width + j] = 0.0;
					Ang[fr*height*width+ i*width + j] = 0.0;
				}	
			}
			

	if ((vel = fopen(nameVel, "wb")) == NULL) {
		fprintf(stderr, "Couldn't create %s\n", nameVel);
		exit(-1);
	}
	if ((ang = fopen(nameAngle, "wb")) == NULL) {
		fprintf(stderr, "Couldn't create %s\n", nameAngle);
		exit(-1);
	}
	if(fwrite(Vel,sizeof(float), length, vel) != length){
		fprintf(stderr,"Ran out of frames in %s\n",nameVel);
		exit(-1);
	}
	if(fwrite(Ang,sizeof(float), length, ang) != length){
		fprintf(stderr,"Ran out of frames in %s\n",nameAngle);
		exit(-1);
	}
	fclose(vel); 
	fclose(ang);

	free(Vel);
	free(Ang);
}

void write_VelAng_png(char* nameVel,char* nameAngle, int nframes, int height, int width, float *Vx,float *Vy)
{
	FILE *ang;
	FILE *vel;
	size_t length=nframes*height*width;
	int fr, i, j;
	float vx, vy;
    char filenameVel[100];
    char filenameAng[100];

    float Vel, Ang;
	unsigned char *uVel  = NULL;
	unsigned char *uAng  = NULL;

	uVel = malloc( height*width*sizeof(unsigned char) );
	uAng = malloc( height*width*sizeof(unsigned char) );

	for (fr=0; fr<nframes; fr++){
		for (i=0; i<height; i++)
        {
			for (j=0; j<width; j++)
			{
				vx = Vx[fr*height*width+ i*width + j];
				vy = Vy[fr*height*width+ i*width + j];

				if (vx!=0.0 && vy!=0.0) {
					Vel = sqrtf(vx*vx+vy*vy);
					Ang = atan2f(vy,vx)*180/3.14159;
				} else {
					Vel = 0.0;
					Ang = 0.0;
				}

                // Scaling Vel
                if (Vel>4.0)
                    Vel=4.0;
                Vel=Vel*63.75;

                uVel[i*width + j] = (unsigned char)(Vel);
                uAng[i*width + j] = (unsigned char)(Ang);
			}
        }
        sprintf(filenameVel,"%s%i.png", nameVel, fr+1);
        writeImagePNG(filenameVel, uVel, width, height);
        sprintf(filenameAng,"%s%i.png", nameAngle, fr+1);
        writeImagePNG(filenameAng, uAng, width, height);
    }

	free(uVel);
	free(uAng);
}



void read_frame_bin(float **frame, char *file_name, int height, int width)
{
	int i, j;

	FILE *fd;

	fd = fopen(file_name,"r+");
	for (i=0; i<height; i++)
		for(j=0; j<width; j++)
			fscanf(fd,"%e ",&frame[i][j]);
	fclose(fd);
}

void write_frame_bin(float **frame, char *file_name, int height, int width)
{
	int i, j;

	FILE *fd;

	fd = fopen(file_name,"w+");
	for (i=0; i<height; i++)
	{
		for(j=0; j<width; j++)
			fprintf(fd, "%e ", frame[i][j]);
		fprintf(fd,"\n");
	}

	fclose(fd);
}

static struct timeval time;
double get_time()
{
	double t;
	gettimeofday(&time, (struct timezone*)0);
	t = ((time.tv_usec) + (time.tv_sec)*1000000);

	return (t);
}

