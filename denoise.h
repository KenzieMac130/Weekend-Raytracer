#pragma once

typedef struct
{
	int w, h;
	float* baseImage;
	float* outputImage;
	float* albedo;
	float* normal;
	int isHdrInput;
} denoiseDesc;

int denoiseImage(denoiseDesc denoiseDesc);