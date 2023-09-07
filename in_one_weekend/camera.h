#pragma once
#include "ray.h"

#define _USE_MATH_DEFINES
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

class camera
{
public:
	vec3 origin, lower_left_corner, horizontal, vertical;
	vec3 u, v, w;
	float lens_radius;
	float time0, time1;

	camera(vec3 lookfrom, vec3 lookat, vec3 vup, float vfov, float aspect, float aperture, float focus_dist, float t0, float t1)
	{
		time0 = t0;
		time1 = t1;

		lens_radius = aperture / 2;

		float theta = vfov * float(M_PI) / 180.0f;
		float half_height = tan(theta / 2);
		float half_width = aspect * half_height;
		origin = lookfrom;

		w = unit_vector(lookfrom - lookat);
		u = unit_vector(cross(vup, w));
		v = cross(w, u);

		lower_left_corner = origin - half_width * focus_dist * u - half_height * focus_dist * v - focus_dist * w;
		horizontal = 2 * half_width * focus_dist * u;
		vertical = 2 * half_height * focus_dist * v;
	}

	ray get_ray(float s, float t)
	{
		vec3 rd = lens_radius * random_in_unit_sphere();
		vec3 offset = u * rd.x() + v * rd.y();
		float time = time0 + drand48() * (time1 - time0);
		return ray(origin + offset, lower_left_corner + s * horizontal + t * vertical - origin - offset, time);
	}
};