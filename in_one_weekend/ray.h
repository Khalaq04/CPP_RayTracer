#pragma once
#include "vec3.h"

vec3 random_in_unit_sphere()
{
	vec3 p;
	do
	{
		p = 2.0f * vec3(drand48(), drand48(), drand48()) - vec3(1.0f, 1.0f, 1.0f);
	} while (dot(p, p) >= 1);

	return p;
}

class ray
{
public:
	vec3 A, B;
	float _time;

	ray(){}
	ray(const vec3& a, const vec3& b, float ti = 0.0f) { A = a; B = b; _time = ti; }

	vec3 origin() const { return A; }
	vec3 direction() const { return B; }
	float time() const { return _time; }
	vec3 point_at_parameter(float t) const { return (A + t * B); }
};