#pragma once
#include "perlin.h"

class texture
{
public:
	virtual vec3 value(float u, float v, const vec3& p) const = 0;
};

class constant_texture : public texture
{
public:
	vec3 color;

	constant_texture() {}
	constant_texture(vec3 c) : color(c) {}

	virtual vec3 value(float u, float v, const vec3& p) const
	{
		return color;
	}
};

class checker_texture : public texture
{
public:
	texture* even;
	texture* odd;

	checker_texture() {}
	checker_texture(texture* t0, texture* t1) : even(t0), odd(t1) {}

	virtual vec3 value(float u, float v, const vec3& p) const
	{
		float sines = float(sin(10 * p.e[0]) * sin(10 * p.e[1]) * sin(10 * p.e[2]));
		if (sines < 0)
			return odd->value(u, v, p);
		else
			return even->value(u, v, p);
	}
};

class noise_texture : public texture
{
public:
	perlin noise;
	float scale;

	noise_texture(float sc) : scale(sc) {}
	virtual vec3 value(float u, float v, const vec3& p) const
	{
		return vec3(1.0f, 1.0f, 1.0f) * 0.5f * (1 + float(sin(scale * p.z() + 10*noise.turb(p))));
	}
};