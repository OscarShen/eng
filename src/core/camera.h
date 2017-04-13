/************************************************************************ 
 * @description :  
 * @author		:  $username$
 * @creat 		:  $time$
************************************************************************ 
 * Copyright @ OscarShen 2017. All rights reserved. 
************************************************************************/  
#pragma once
#ifndef ENG_CAMERA_H_
#define ENG_CAMERA_H_
#include <eng.h>
#include "primitive.h"
#include <math/linalg.h>
#include <common/meta.h>
namespace eng {

	class Film
	{
	public:
		Vector3f* frame_buffer;
		int width, height;

	public:
		Film(int width, int height) : frame_buffer(new Vector3f[height * width]), width(width), height(height) {}
		~Film() { delete[] frame_buffer; }
		
		Vector3f *get_pixel(int i, int j) { return frame_buffer + j * width + i; }
		void set_color(int i, int j, const Vector3f &color) { *get_pixel(i, j) = color; }
	};

	class Camera : public Unit
	{
	protected:
		Vector3f origin, look_at, up, right, dir;
		Matrix4f local2world;
		int width, height;
		Float aspect_ratio;
		std::shared_ptr<Film> film;

	public:
		virtual void initialize(const Config &config) override {
			this->origin = config.get_vec3("origin");
			this->look_at = config.get_vec3("look_at");
			this->up = config.get_vec3("up");
			this->width = config.get_int("width");
			this->height = config.get_int("height");
			set_dir_and_right();
			this->aspect_ratio = Float(width) / height;
			this->film = std::shared_ptr<Film>(new Film(width, height));
			local2world = Matrix4f(1.0f);
		}

		std::shared_ptr<Film> get_film() const { return film; }
		int get_width() const { return width;}
		int get_height() const { return height; }

		virtual Ray generate_ray(Vector2i offset) {
			ERROR("no impl");
			return Ray(Vector3f(0), Vector3f(0));
		}

	protected:
		void set_dir_and_right() {
			this->dir = normalize(look_at - origin);
			this->up = normalize(up - dir * dot(dir, up));
			this->right = cross(dir, up);
		}
	};

	ENG_INTERFACE(Camera);
}

#endif // !ENG_CAMERA_H_
