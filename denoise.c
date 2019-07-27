#include "denoise.h"

#include <stdio.h>

#include "config.h"
#ifdef USE_INTEL_OPEN_DENOISE
#include "OpenImageDenoise/oidn.h"

int denoiseImage(denoiseDesc denoiseDesc)
{
	OIDNDevice device = oidnNewDevice(OIDN_DEVICE_TYPE_DEFAULT);
	oidnCommitDevice(device);
	/*Create buffers*/
	OIDNBuffer colorBuffer = oidnNewSharedBuffer(device, denoiseDesc.baseImage, denoiseDesc.w * denoiseDesc.h * 3 * sizeof(float));
	OIDNBuffer normalBuffer = oidnNewSharedBuffer(device, denoiseDesc.normal, denoiseDesc.w * denoiseDesc.h * 3 * sizeof(float));
	OIDNBuffer albedoBuffer = oidnNewSharedBuffer(device, denoiseDesc.albedo, denoiseDesc.w * denoiseDesc.h * 3 * sizeof(float));
	OIDNBuffer outBuffer = oidnNewSharedBuffer(device, denoiseDesc.outputImage, denoiseDesc.w * denoiseDesc.h * 3 * sizeof(float));
	
	/*Setup filter*/
	OIDNFilter filter = oidnNewFilter(device, "RT");
	oidnSetFilterImage(filter, "color", colorBuffer, OIDN_FORMAT_FLOAT3, denoiseDesc.w, denoiseDesc.h, 0, 0, 0);
	oidnSetFilterImage(filter, "albedo", albedoBuffer, OIDN_FORMAT_FLOAT3, denoiseDesc.w, denoiseDesc.h, 0, 0, 0);
	oidnSetFilterImage(filter, "normal", normalBuffer, OIDN_FORMAT_FLOAT3, denoiseDesc.w, denoiseDesc.h, 0, 0, 0);
	oidnSetFilterImage(filter, "output", outBuffer, OIDN_FORMAT_FLOAT3, denoiseDesc.w, denoiseDesc.h, 0, 0, 0);
	oidnSetFilter1b(filter, "hdr", false); /*The HDR setting currently produces very upsetting results*/
	oidnCommitFilter(filter);

	/*Execute*/
	oidnExecuteFilter(filter);

	/*Errors*/
	const char* errorMessage;
	if (oidnGetDeviceError(device, &errorMessage) != OIDN_ERROR_NONE)
		printf("Denoise ERROR: %s\n", errorMessage);

	oidnReleaseFilter(filter);
	oidnReleaseDevice(device);
	return 0;
}
#else
#include <string.h>
int denoiseImage(denoiseDesc denoiseDesc)
{
	printf("Denoiser not available... raw will be copied to output\n");
	memcpy(denoiseDesc.outputImage, denoiseDesc.baseImage, denoiseDesc.w * denoiseDesc.h * 3 * sizeof(float));
}
#endif