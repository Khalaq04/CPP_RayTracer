#pragma once
#define _CRT_SECURE_NO_WARNINGS

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "hitable_list.h"
#include "sphere.h"
#include "camera.h"
#include "material.h"
#include "random.h"
#include "moving_sphere.h"
#include "surface_texture.h"
#include "aarect.h"
#include "box.h"
#include "constant_medium.h"
#include "bvh.h"

#include <float.h>
#include <iostream>
#include <stdlib.h>
#include <future>

#define drand48() ((float)rand()/RAND_MAX)

hitable* random_scene()
{
	int n = 500;
	hitable** list = new hitable * [n + 1];

	texture* checker = new checker_texture(new constant_texture(vec3(0.2f, 0.3f, 0.1f)), new constant_texture(vec3(0.9f, 0.9f, 0.9f)));
	list[0] = new sphere(vec3(0.0f, -1000.0f, 0.0f), 1000.0f, new lambertian(checker));
	int i = 1;
	for (int a = -11; a < 11; a++)
	{
		for (int b = -11; b < 11; b++)
		{
			float choose_mat = float(random_double());
			vec3 center = vec3(a + 0.9f * float(random_double()), 0.2f, b + 0.9f * float(random_double()));

			if ((center - vec3(4.0f, 0.2f, 0.0f)).length() > 0.85f)
			{
				if (choose_mat < 0.7)
				{
					list[i++] = new moving_sphere(center, center + vec3(0.0f, 0.5f * drand48(), 0.0f), 0.0f, 1.0f, 0.2f, new lambertian(new constant_texture(vec3(float(random_double() * random_double()), float(random_double() * random_double()), float(random_double() * random_double())))));
				}
				else if (choose_mat < 0.95)
				{
					list[i++] = new sphere(center, 0.2f, new metal(vec3(0.5f * (1 + float(random_double())), 0.5f * (1 + float(random_double())), 0.5f * (1 + float(random_double()))), 0.5f * float(random_double())));
				}
				else
				{
					list[i++] = new sphere(center, 0.2f, new dielectric(1.5f));
				}
			}
		}
	}

	list[i++] = new sphere(vec3(0.0f, 1.0f, 0.0f), 1.0f, new dielectric(1.5f));
	list[i++] = new sphere(vec3(-4.0f, 1.0f, 0.0f), 1.0f, new lambertian(new constant_texture(vec3(0.5f, 0.1f, 0.1f))));
	list[i++] = new sphere(vec3(4.0f, 1.0f, 0.0f), 1.0f, new metal(vec3(0.1f, 0.1f, 0.2f), 0.0f));

	return new hitable_list(list, i);
}

hitable* two_spheres()
{
	texture* checker = new checker_texture(new constant_texture(vec3(0.2f, 0.3f, 0.1f)), new constant_texture(vec3(0.9f, 0.9f, 0.9f)));
	hitable** list = new hitable*[2];
	list[0] = new sphere(vec3(0.0f, 10.0f, 0.0f), 10.0f, new lambertian(checker));
	list[1] = new sphere(vec3(0.0f, -10.0f, 0.0f), 10.0f, new lambertian(checker));

	return new hitable_list(list, 2);

}

hitable* two_perlin_spheres()
{
	texture* pertext = new noise_texture(4);
	hitable** list = new hitable * [2];
	list[0] = new sphere(vec3(0.0f, -1000.0f, 0.0f), 1000.0f, new lambertian(pertext));
	list[1] = new sphere(vec3(0.0f, 2.0f, 0.0f), 2.0f, new lambertian(pertext));

	return new hitable_list(list, 2);
}

hitable* img_sphere()
{
	int nx, ny, nn;
	unsigned char* data = stbi_load("earthmap.jpeg", &nx, &ny, &nn, 0);
	material* mat = new lambertian(new image_texture(data, nx, ny));
	return new sphere(vec3(0.0f, 0.0f, 0.0f), 2.0f, mat);
}

hitable* simple_light()
{
	texture* pertext = new noise_texture(4);
	hitable** list = new hitable * [4];
	list[0] = new sphere(vec3(0, -1000, 0), 1000, new lambertian(pertext));
	list[1] = new sphere(vec3(0, 2, 0), 2, new lambertian(pertext));
	list[2] = new sphere(vec3(0, 13, 0), 2, new diffuse_light(new constant_texture(vec3(4, 4, 4))));
	list[3] = new xy_rect(8, 10, 1, 3, -2, new diffuse_light(new constant_texture(vec3(4, 4, 4))));
	return new hitable_list(list, 4);
}

hitable* cornell_box() {
	hitable** list = new hitable * [8];
	int i = 0;
	material* red = new lambertian(new constant_texture(vec3(0.65f, 0.05f, 0.05f)));
	material* white = new lambertian(new constant_texture(vec3(0.73f, 0.73f, 0.73f)));
	material* green = new lambertian(new constant_texture(vec3(0.12f, 0.45f, 0.15f)));
	material* light = new diffuse_light(new constant_texture(vec3(15, 15, 15)));
	list[i++] = new flip_normals(new yz_rect(0, 555, 0, 555, 555, green));
	list[i++] = new yz_rect(0, 555, 0, 555, 0, red);
	list[i++] = new xz_rect(213, 343, 227, 332, 554, light);
	list[i++] = new flip_normals(new xz_rect(0, 555, 0, 555, 555, white));
	list[i++] = new xz_rect(0, 555, 0, 555, 0, white);
	list[i++] = new flip_normals(new xy_rect(0, 555, 0, 555, 555, white));
	list[i++] = new translate(new rotate_y(new box(vec3(0, 0, 0), vec3(165, 165, 165), white), -18), vec3(130, 0, 65));
	list[i++] = new translate(new rotate_y(new box(vec3(0, 0, 0), vec3(165, 330, 165), white), 15), vec3(265, 0, 295));
	return new hitable_list(list, i);
}

hitable* cornell_smoke() {
	hitable** list = new hitable * [8];
	int i = 0;
	material* red = new lambertian(new constant_texture(vec3(0.65f, 0.05f, 0.05f)));
	material* white = new lambertian(new constant_texture(vec3(0.73f, 0.73f, 0.73f)));
	material* green = new lambertian(new constant_texture(vec3(0.12f, 0.45f, 0.15f)));
	material* light = new diffuse_light(new constant_texture(vec3(7, 7, 7)));
	list[i++] = new flip_normals(new yz_rect(0, 555, 0, 555, 555, green));
	list[i++] = new yz_rect(0, 555, 0, 555, 0, red);
	list[i++] = new xz_rect(113, 443, 127, 432, 554, light);
	list[i++] = new flip_normals(new xz_rect(0, 555, 0, 555, 555, white));
	list[i++] = new xz_rect(0, 555, 0, 555, 0, white);
	list[i++] = new flip_normals(new xy_rect(0, 555, 0, 555, 555, white));
	hitable* b1 = new translate(new rotate_y(new box(vec3(0, 0, 0), vec3(165, 165, 165), white), -18), vec3(130, 0, 65));
	hitable* b2 = new translate(new rotate_y(new box(vec3(0, 0, 0), vec3(165, 330, 165), white), 15), vec3(265, 0, 295));
	list[i++] = new constant_medium(b1, 0.01f, new constant_texture(vec3(1.0f, 1.0f, 1.0f)));
	list[i++] = new constant_medium(b2, 0.01f, new constant_texture(vec3(0.0f, 0.0f, 0.0f)));
	return new hitable_list(list, i);
}

hitable* final_world() {
	int nb = 20;
	hitable** list = new hitable * [30];
	hitable** boxlist = new hitable * [10000];
	hitable** boxlist2 = new hitable * [10000];
	material* white = new lambertian(new constant_texture(vec3(0.73f, 0.73f, 0.73f)));
	material* ground = new lambertian(new constant_texture(vec3(0.48f, 0.83f, 0.53f)));
	int b = 0;
	for (int i = 0; i < nb; i++) {
		for (int j = 0; j < nb; j++) {
			float w = 100;
			float x0 = -1000 + i * w;
			float z0 = -1000 + j * w;
			float y0 = 0;
			float x1 = x0 + w;
			float y1 = 100 * (drand48() + 0.01f);
			float z1 = z0 + w;
			boxlist[b++] = new box(vec3(x0, y0, z0), vec3(x1, y1, z1), ground);
		}
	}
	int l = 0;
	list[l++] = new bvh_node(boxlist, b, 0, 1);
	material* light = new diffuse_light(new constant_texture(vec3(7, 7, 7)));
	list[l++] = new xz_rect(123, 423, 147, 412, 554, light);
	vec3 center(400, 400, 200);
	list[l++] = new moving_sphere(center, center + vec3(30, 0, 0), 0, 1, 50, new lambertian(new constant_texture(vec3(0.7f, 0.3f, 0.1f))));
	list[l++] = new sphere(vec3(260, 150, 45), 50, new dielectric(1.5));
	list[l++] = new sphere(vec3(0, 150, 145), 50, new metal(vec3(0.8f, 0.8f, 0.9f), 10.0f));
	hitable* boundary = new sphere(vec3(360, 150, 145), 70, new dielectric(1.5f));
	list[l++] = boundary;
	list[l++] = new constant_medium(boundary, 0.2f, new constant_texture(vec3(0.2f, 0.4f, 0.9f)));
	boundary = new sphere(vec3(0, 0, 0), 5000, new dielectric(1.5f));
	list[l++] = new constant_medium(boundary, 0.0001f, new constant_texture(vec3(1.0f, 1.0f, 1.0f)));
	int nx, ny, nn;
	unsigned char* tex_data = stbi_load("earthmap.jpeg", &nx, &ny, &nn, 0);
	material* emat = new lambertian(new image_texture(tex_data, nx, ny));
	list[l++] = new sphere(vec3(400, 200, 400), 100, emat);
	texture* pertext = new noise_texture(4);
	list[l++] = new sphere(vec3(220, 280, 300), 80, new lambertian(pertext));
	int ns = 1000;
	for (int j = 0; j < ns; j++) {
		boxlist2[j] = new sphere(vec3(165 * drand48(), 165 * drand48(), 165 * drand48()), 10, white);
	}
	list[l++] = new translate(new rotate_y(new bvh_node(boxlist2, ns, 0.0f, 1.0f), 15), vec3(-100, 270, 395));
	return new hitable_list(list, l);
}

vec3 color(const ray& r, hitable* world, int depth)
{
	hit_record rec;
	if (world->hit(r, 0.001f, FLT_MAX, rec))
	{
		ray scattered;
		vec3 attenuation;
		vec3 emitted = rec.mat_ptr->emitted(rec.u, rec.v, rec.p);
		if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered))
		{
			return emitted + attenuation * color(scattered, world, depth + 1);
		}
		else
			return emitted;
	}
	else
		return vec3(0.0f, 0.0f, 0.0f);
}

const int width = 600, height = 420, ns = 300;
char* data = new char[width * height * 3];

hitable* world = final_world();

vec3 lookfrom = vec3(278.0f, 278.0f, -800.0f);
vec3 lookat = vec3(278.0f, 278.0f, 0.0f);
float dist_to_focus = 10.0f;
float aperture = 0.0f;
float vfov = 40.0;
camera cam(lookfrom, lookat, vec3(0.0f, 1.0f, 0.0f), vfov, float(width) / float(height), aperture, dist_to_focus, 0.0f, 1.0f);

std::vector<std::future<void>> c_Futures;
std::mutex c_Mutex;

void pixel_color(int i, int j)
{
	std::cout << "[+] Thread " << std::this_thread::get_id() << " started...\n";
	// std::this_thread::sleep_for(std::chrono::milliseconds(50000));

	vec3 col(0.0f, 0.0f, 0.0f);
	for (int s = 0; s < ns; s++)
	{
		float u = float(i + drand48()) / float(width);
		float v = float(j + drand48()) / float(height);
		ray r = cam.get_ray(u, v);
		col += color(r, world, 0);
	}

	col /= float(ns);
	col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
	int ir = int(255.99 * col[0]);
	int ig = int(255.99 * col[1]);
	int ib = int(255.99 * col[2]);
	
	int idx = (i + ((height - 1) - j) * width) * 3;

	std::lock_guard<std::mutex> lock(c_Mutex);
	data[idx + 0] = std::clamp(ir, 0, 255);
	data[idx + 1] = std::clamp(ig, 0, 255);
	data[idx + 2] = std::clamp(ib, 0, 255);

	std::cout << "[-] Thread " << std::this_thread::get_id() << " ended...\n";
}

int main()
{
	unsigned int nThreads = std::thread::hardware_concurrency();

	auto timeStart = std::chrono::high_resolution_clock::now();

	for (int j = height - 1; j >= 0; j--)
	{
		for (int i = 0; i < width; i++)
		{
			c_Futures.push_back(std::async(std::launch::async, pixel_color, i, j));
		}
	}

	for (auto& future : c_Futures)
	{
		future.get();
	}

	auto timeEnd = std::chrono::high_resolution_clock::now();
	auto timeTaken = std::chrono::duration<double, std::milli>(timeEnd - timeStart).count();

	std::cout << "\n[+] Finished executing all Threads\n";
	std::cout << "[+] Total Threads Used : " << std::thread::hardware_concurrency() << std::endl;
	std::cout << "[+] Time Taken : " << timeTaken / 1000 << " seconds" << std::endl;

	stbi_write_png("img_9.png", width, height, 3, data, width * 3);
	delete[] data;

	return 0;
}