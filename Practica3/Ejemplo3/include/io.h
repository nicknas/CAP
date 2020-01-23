#ifndef _IO_H_
#define _IO_H_

#ifdef __cplusplus
extern "C"
#endif
float *get_memory1D_float( int nx );
#ifdef __cplusplus
extern "C"
#endif
void delete_memory1D_float( float *buffer );

#ifdef __cplusplus
extern "C"
#endif
float **get_memory2D_float( int nx, int ny );
#ifdef __cplusplus
extern "C"
#endif
void delete_memory2D_float( float **buffer );

#ifdef __cplusplus
extern "C"
#endif
int read_movie_bin(float *frames, char *file_name, int nframes, int height, int width);
#ifdef __cplusplus
extern "C"
#endif
int read_movie_png(float *frames, char *file_name, int nframes, int height, int width);
#ifdef __cplusplus
extern "C"
#endif
void write_VelAng_bin(char* nameVel,char* nameAngle, int nframes, int height, int width, float *Vx,float *Vy);
#ifdef __cplusplus
extern "C"
#endif
void write_VelAng_png(char* nameVel,char* nameAngle, int nframes, int height, int width, float *Vx,float *Vy);
#ifdef __cplusplus
extern "C"
#endif
void write_vel_bin(char* nameVx,char* nameVy, int nframes, int height, int width, float *Vx,float *Vy);

#ifdef __cplusplus
extern "C"
#endif
void read_frame_bin(float **frame, char *file_name, int height, int width);

#ifdef __cplusplus
extern "C"
#endif
void write_frame_bin(float **frame, char *file_name, int height, int width);

#ifdef __cplusplus
extern "C"
#endif
double get_time();

#endif
