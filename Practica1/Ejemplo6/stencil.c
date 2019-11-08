#include "stencil.h"
#include <immintrin.h>

void ApplyStencil(unsigned char *img_in, unsigned char *img_out, int width, int height) {
  
	short val;
	unsigned char val_out;

	for (int i = 1; i < height-1; i++) {
		#pragma ivdep
		#pragma vector aligned
		for (int j = 1; j < width-1; j++) {
			val = img_in[(i  )*width + j];
			__m256i lf, mf, rf, lm, mm, rm, ll, ml, rl, val_final;
			lf = _mm256_load_si256((__m256i*)(img_in + ((i-1)*width + j - 1)));
			mf = _mm256_load_si256((__m256i*)(img_in + ((i-1)*width + j)));
			rf = _mm256_load_si256((__m256i*)(img_in + ((i-1)*width + j + 1)));
			lm = _mm256_load_si256((__m256i*)(img_in + ((i )*width + j - 1)));
			mm = _mm256_load_si256((__m256i*)(img_in + ((i )*width + j)));
			rm = _mm256_load_si256((__m256i*)(img_in + ((i )*width + j + 1)));
			ll = _mm256_load_si256((__m256i*)(img_in + ((i + 1)*width + j - 1)));
			ml = _mm256_load_si256((__m256i*)(img_in + ((i + 1)*width + j)));
			rl = _mm256_load_si256((__m256i*)(img_in + ((i + 1)*width + j + 1)));
			val_final = mm;
			val_final = _mm256_subs_epu8(val_final, lf);
			val_final = _mm256_adds_epu8(val_final, mm);
			val_final = _mm256_subs_epu8(val_final, mf);
			val_final = _mm256_adds_epu8(val_final, mm);
			val_final = _mm256_subs_epu8(val_final, rf);
			val_final = _mm256_adds_epu8(val_final, mm);
			val_final = _mm256_subs_epu8(val_final, lm);
			val_final = _mm256_adds_epu8(val_final, mm);
			val_final = _mm256_subs_epu8(val_final, rm);
			val_final = _mm256_adds_epu8(val_final, mm);
			val_final = _mm256_subs_epu8(val_final, ll);
			val_final = _mm256_adds_epu8(val_final, mm);
			val_final = _mm256_subs_epu8(val_final, ml);
			val_final = _mm256_adds_epu8(val_final, mm);
			val_final = _mm256_subs_epu8(val_final, rl);
			_mm256_store_si256((__m256i*)(img_out + ((i )*width + j)), val_final);
		}
	}
}

void CopyImage(unsigned char *img_in, unsigned char *img_out, int width, int height) {

	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
			img_in[i*width + j] = img_out[i*width + j];
}
