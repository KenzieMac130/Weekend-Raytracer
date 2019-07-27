#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <float.h>

#ifdef _OPENMP
#include <omp.h>
#endif

#define STB_IMAGE_WRITE_IMPLEMENTATION
#ifdef WIN32
#define STBI_MSC_SECURE_CRT
#endif
#include "stb_image_write.h"

#include "platformUtil.h"

#include "vec3.h"
#include "extraMath.h"
#include "ray.h"
#include "camera.h"
#include "material.h"

#include "shapes/shapeList.h"
#include "shapes/sphere.h"
#include "shapes/plane.h"

#include "shaders/lambert.h"
#include "shaders/simpleMetal.h"
#include "shaders/glass.h"
#include "shaders/emission.h"

#include "denoise.h"

#define MAX_RECURSUIVE 5

#define DENOISE
#define HDR
#define WRITE_MULTI_OUTPUT

/*Denoiser dependencies*/
#ifdef DENOISE
#ifndef HDR
#define HDR
#endif
#ifndef MULTI_OUTPUT
#define MULTI_OUTPUT
#endif
#endif

#ifndef HDR
#define COLOR_FORMAT uint8_t
#define COLOR_CONVERSION(raw) (COLOR_FORMAT)(255.99*sqrt(raw))
#else
#define COLOR_FORMAT float
#define COLOR_CONVERSION(raw) (COLOR_FORMAT)(raw)
#endif

//#define MULTI_OUTPUT

typedef struct
{
	vec3 color;
#ifdef MULTI_OUTPUT
	vec3 normal;
	vec3 albedo;
	float depth;
#endif
} renderOutput;

renderOutput calcOutput(ray r, shapeBase* shape, int reccussion)
{
	hit_record record;
	if (shape->fpHit(shape, r, 0.0001f, FLT_MAX, &record))
	{
		rayOutput rayOutput;
		rayOutput.emission = (vec3) { 0, 0, 0 };
		rayOutput.albedo = (vec3) { 0, 0, 0 };
		if (reccussion < MAX_RECURSUIVE && record.pMat->fpScatter((void*)record.pMat, r, (hit_record*)&record, &rayOutput))
		{
			return (renderOutput) {
				.color = v3Add(v3Mult(rayOutput.attenuation, calcOutput(rayOutput.scattered, shape, ++reccussion).color), rayOutput.emission),
#ifdef MULTI_OUTPUT
				.albedo = rayOutput.albedo,
				.normal = record.normal,
				.depth = record.t
#endif
			};
		}
		else
		{
			return (renderOutput) {
				.color = rayOutput.emission,
#ifdef MULTI_OUTPUT
				.albedo = rayOutput.albedo,
				.normal = record.normal,
				.depth = record.t
#endif
			};
		}
	}

	/*Calculate a background gradient*/
	vec3 v = v3Addf(v3Multf(v3Unit(r.dir),0.5f),0.5f);
	vec3 floor = { 0.0f, 0.0f, 0.0f };
	vec3 sky = { 5.0f, 7.0f, 10.0f };
	vec3 background = v3Lerp(floor, sky, v.y);
	return (renderOutput) {
		.color = background,
#ifdef MULTI_OUTPUT
		.albedo = background,
		.normal = v3Neg(r.dir),
		.depth = 10000.0f
#endif
	};
}

/*Overrides*/
#define dimX DimX
#define dimY DimY
#define AA samples

int main(int argc, char* argv[])
{
	/*Get Settings*/
	int DimX = 1920;
	int DimY = 1080;
	int samples = 25;

	printf("Starting Raytracer...\n");
	/*Create an RGB Image*/
	COLOR_FORMAT* image = malloc(dimX * dimY * 3 * sizeof(COLOR_FORMAT));
#ifdef MULTI_OUTPUT
	float* albedoBuffer = malloc(dimX * dimY * 3 * sizeof(float));
	float* normalBuffer = malloc(dimX * dimY * 3 * sizeof(float));
	float* depthBuffer = malloc(dimX * dimY * 1 * sizeof(float));
#endif

	/*CameraStuff*/
	camera cam = camera_init((vec3) { 0, 0, 0 }, (vec3) { 0, 0, -1 }, 45.0f, dimX, dimY);

	/*Create materials*/
	mat_lambert whiteDiffuseMat = mat_lambert_init((vec3) { 0.9f, 0.9f, 0.9f });
	mat_lambert redDiffuseMat = mat_lambert_init((vec3) { 0.9f, 0.0f, 0.0f });
	mat_lambert greenDiffuseMat = mat_lambert_init((vec3) { 0.0f, 0.9f, 0.0f });
	mat_lambert blueDiffuseMat = mat_lambert_init((vec3) { 0.0f, 0.0f, 0.9f });
	mat_lambert blackDiffuseMat = mat_lambert_init((vec3) { 0.0f, 0.0f, 0.0f });
	mat_metalSimple goldMetal = mat_metalSimple_init((vec3) { 1.0f, 0.765557f, 0.336057f }, 0.2f);
	mat_glass glass = mat_glass_init((vec3) { 1.0f, 0.0f, 0.0f }, 0.0f, 1.33f);
	mat_emit bulbMat = mat_emit_init((vec3) { 500, 500, 500 });

	/*Setup Scene*/
#define SPHERE_COUNT 5
	sphere spheres[SPHERE_COUNT];
	/*
	float R = cosf(PI / 4);
	spheres[0] = sphere_init((vec3) { -R, 0, -1 }, R, &blueDiffuseMat);
	spheres[1] = sphere_init((vec3) { R, 0, -1 }, R, &redDiffuseMat);
	spheres[2] = sphere_init((vec3) { 0, 0, -1 }, R, &whiteDiffuseMat);
	spheres[3] = sphere_init((vec3) { -2, 3, -1 }, 2, &bulbMat);
	*/

	spheres[0] = sphere_init((vec3) { 0, 0, -2 }, 0.5f, &goldMetal);
	spheres[1] = sphere_init((vec3) { 1, 0, -2 }, 0.5f, &whiteDiffuseMat);
	spheres[2] = sphere_init((vec3) { -1, 1, -1.5 }, 0.5f, &greenDiffuseMat);
	spheres[3] = sphere_init((vec3) { -1, 200, -1000 }, 500.0f, &redDiffuseMat);
	spheres[4] = sphere_init((vec3) { -1, 0, -2 }, 0.5f, &glass);

	/*plane groundPlane = plane_init((vec3) { 0, -1, -2 }, (vec3) { 0, 1, 0 }, 0.5f, &blueDiffuseMat);*/

	/*Link together all objects in scene*/
	void* allShapes[SPHERE_COUNT];
	int allShapeCount = 0;
	for (int i = 0; i < SPHERE_COUNT; i++)
	{
		allShapes[allShapeCount] = (void*)&spheres[i];
		allShapeCount++;
	}
	/*allShapes[allShapeCount] = (void*)&groundPlane;
	allShapeCount++;*/
	shapeList world = shapeList_init(allShapes, allShapeCount);

	/*Multithreading stuff*/
#ifdef _OPENMP
#pragma omp parallel
{
	if(!omp_get_thread_num())
		printf("Running %d Threads...\n", omp_get_num_threads());
#endif
	int j; int i;
#ifdef _OPENMP
#if _OPENMP >= 201107 /*Modern OpenMP*/
#pragma omp for collapse(2)
#else /*MSVC's crappy, less efficent, old OpenMP*/
#pragma omp for
#endif
#endif
	for (j = 0; j < dimY; j++)
	{
		for (i = 0; i < dimX; i++)
		{
			/*Raytrace*/
			vec3 color = (vec3) { 0, 0, 0 };
#ifdef MULTI_OUTPUT
			vec3 normal = (vec3) { 0, 0, 0 };
			vec3 albedo = (vec3) { 0, 0, 0 };
			float depth = 0.0f;
#endif
			for (int aa = 0; aa < AA; aa++)
			{
				float u = ((float)i + antiAliasJitter(0.99f)) / (float)dimX;
				float v = 1.0f-((float)j + antiAliasJitter(0.99f)) / (float)dimY;
				ray r = camera_getRay(cam, u, v);
				renderOutput render = calcOutput(r, (shapeBase*)&world, 0);
				color = v3Add(color, v3Divf(render.color,AA));
#ifdef MULTI_OUTPUT
				normal = v3Add(normal, v3Divf(render.normal, AA));
				albedo = v3Add(albedo, v3Divf(render.albedo, AA));
				depth += render.depth / AA;
#endif
			}
			
			/*Write Color*/
			image[(j*dimX * 3) + (i * 3) + 0] = COLOR_CONVERSION(unchartedTonemap(color.r));
			image[(j*dimX * 3) + (i * 3) + 1] = COLOR_CONVERSION(unchartedTonemap(color.g));
			image[(j*dimX * 3) + (i * 3) + 2] = COLOR_CONVERSION(unchartedTonemap(color.b));

#ifdef MULTI_OUTPUT
			/*Write Albedo*/
			albedoBuffer[(j*dimX * 3) + (i * 3) + 0] = albedo.r;
			albedoBuffer[(j*dimX * 3) + (i * 3) + 1] = albedo.g;
			albedoBuffer[(j*dimX * 3) + (i * 3) + 2] = albedo.b;

			/*Write Normal*/
			normalBuffer[(j*dimX * 3) + (i * 3) + 0] = normal.x;
			normalBuffer[(j*dimX * 3) + (i * 3) + 1] = normal.y;
			normalBuffer[(j*dimX * 3) + (i * 3) + 2] = normal.z;

			/*Write Depth*/
			depthBuffer[(j*dimX) + (i)+0] = depth;
#endif
		}
	}
#ifdef _OPENMP
}
#endif
	printf("Finished Rendering...\n");
	COLOR_FORMAT* finalImage;
#ifdef DENOISE
	printf("Attempting Denoise...\n");

	/*Denoise Image*/
	finalImage = malloc(dimX * dimY * 3 * sizeof(COLOR_FORMAT));
	denoiseDesc denoise;
	denoise.h = dimY;
	denoise.w = dimX;
	denoise.baseImage = image;
	denoise.normal = normalBuffer;
	denoise.albedo = albedoBuffer;
	denoise.outputImage = finalImage;
	denoiseImage(denoise);
	free(image);
#else
	finalImage = image;
#endif

	/*Tonemap*/
#ifdef _OPENMP
#pragma omp parallel
{
#endif
	int i;
#ifdef _OPENMP
#pragma omp for
#endif
	for (i = 0; i < dimX * dimY * 3; i++)
	{
		normalBuffer[i] = (normalBuffer[i] * 0.5f + 0.5f);
		//finalImage[i] = unchartedTonemap(finalImage[i]);
	}
#ifdef _OPENMP
}
#endif

	/*Save to disk*/
#ifndef HDR
	const char* filename = "Test.bmp";
	stbi_write_bmp(filename, dimX, dimY, 3, finalImage);
#else
	const char* filename = "Test.hdr";
	stbi_write_hdr(filename, dimX, dimY, 3, finalImage);
#endif
	free(finalImage);

#ifdef MULTI_OUTPUT
#ifdef WRITE_MULTI_OUTPUT
	stbi_write_hdr("Test_Albedo.hdr", dimX, dimY, 3, albedoBuffer);
	stbi_write_hdr("Test_Normal.hdr", dimX, dimY, 3, normalBuffer);
	stbi_write_hdr("Test_Depth.hdr", dimX, dimY, 1, depthBuffer);
#endif

	free(normalBuffer);
	free(albedoBuffer);
	free(depthBuffer);
#endif

	/*Present to user*/
	showFile(filename);
	return 0;
}