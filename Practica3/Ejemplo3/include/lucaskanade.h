#ifndef _LUCASKANADE_H_
#define _LUCASKANADE_H_

#ifdef __cplusplus
extern "C"
#endif
void getfilters(float *filt_x, float *filt_y, float *filt_t, int spac_filt_size, int temp_filt_size);

#ifdef __cplusplus
extern "C"
#endif
void lucas_kanade(float *Vx, float  *Vy, 
	float *frame, int nx, int ny, int fr, float *Ix, float *Iy, float *It, float *tmp,
	float *filt_x, float *filt_y, int spac_conv_size, int spac_window_sizeX, int spac_window_sizeY,
	float *filt_t, int temp_conv_size, int temp_window_sizeX, int temp_window_sizeY);

#endif
