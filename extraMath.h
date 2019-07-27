#pragma once

#define PI 3.1415926535897932

float lerpf(float a, float b, float t);

float floatRandom();

float fresnel_schlick(float cosine, float ior);

float unchartedTonemap(float x);