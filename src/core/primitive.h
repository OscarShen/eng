/************************************************************************ 
 * @description :  
 * @author		:  $username$
 * @creat 		:  $time$
************************************************************************ 
 * Copyright @ OscarShen 2017. All rights reserved. 
************************************************************************/  
#pragma once
#ifndef ENG_PRIMITIVE_H_
#define ENG_PRIMITIVE_H_
#include <eng.h>
#include <common/meta.h>
#include <core/intersection.h>
#include <math/linalg.h>
#include <util/print.h>
namespace eng {

	class Ray
	{
	public:
		Vector3f orig, dir;

		Ray(const Vector3f &orig, const Vector3f &dir)
			:orig(orig), dir(dir) {}
	};

	inline Ray operator*(const Matrix4f &transform, const Ray &ray) {
		return Ray(multiply_matrix4(transform, ray.orig, 1), multiply_matrix4(transform, ray.dir, 0));
	}

	class Shape : public Unit
	{
	public:
		Matrix4f local2world;
		Matrix4f world2local;
		Vector3f color; // ¡Ÿ ± π”√

	public:
		virtual void initialize(const Config &config) override {
			Vector3f translate = config.get("translate", Vector3f(0.0f));
			Vector3f scale = config.get("scale", Vector3f(1.0f));
			Vector3f rotate = config.get("rotate", Vector3f(0.0f));
			local2world = matrix4_scale(local2world, scale);
			local2world = matrix4_rotate_eular(local2world, rotate);
			local2world = matrix4_translate(local2world, translate);
			world2local = inverse(local2world);
			color = Vector3f(rand(), rand(), rand());
		}
		virtual bool intersect(const Ray &ray, Float &t_hit, Intersection &iset) const = 0;
	};

	ENG_INTERFACE(Shape);
}

#endif // !ENG_PRIMITIVE_H_
