#pragma once
#include "texture.h"

class image_texture : public texture
{
public:
	unsigned char* data;
	int nx, ny;

	image_texture() {}
	image_texture(unsigned char* pixels, int A, int B) : data(pixels), nx(A), ny(B) {}

	virtual vec3 value(float u, float v, const vec3& p) const;
};

vec3 image_texture::value(float u, float v, const vec3& p) const
{
    int i = int(u)*nx;
    int j = int((1 - int(v)) * ny - 0.001f);
    if (i < 0) i = 0;
    if (j < 0) j = 0;
    if (i > nx - 1) i = nx - 1;
    if (j > ny - 1) j = ny - 1;
    float r = int(data[3 * i + 3 * nx * j]) / 255.0f;
    float g = int(data[3 * i + 3 * nx * j + 1]) / 255.0f;
    float b = int(data[3 * i + 3 * nx * j + 2]) / 255.0f;
    return vec3(r, g, b);
}