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

	// 光线
	class Ray
	{
	public:
		Vector3f orig, dir;

		Ray(const Vector3f &orig, const Vector3f &dir)
			:orig(orig), dir(dir) {}

		Vector3f get_point(Float t) const { return orig + t * dir; }
	};

	inline Ray operator*(const Matrix4f &transform, const Ray &ray) {
		return Ray(multiply_matrix4(transform, ray.orig, 1), multiply_matrix4(transform, ray.dir, 0));
	}

	// 三维包围盒
	template <typename T>
	class Bound3
	{
	public:
		glm::tvec3<T, glm::highp> p_min, p_max;

		Bound3() : p_min(std::numeric_limits<T>::max()), p_max(std::numeric_limits<T>::lowest()) { }
		Bound3(const glm::tvec3<T, glm::highp> &p) : p_min(p), p_max(p) {}
		Bound3(const glm::tvec3<T, glm::highp> &p1, const glm::tvec3<T, glm::highp> &p2)
			: p_min(std::min(p1.x, p2.x), std::min(p1.y, p2.y),
				std::min(p1.z, p2.z)),
			p_max(std::max(p1.x, p2.x), std::max(p1.y, p2.y),
				std::max(p1.z, p2.z)) {}

		bool intersect(const Ray &ray, Float *hitt0 = nullptr, Float *hitt1 = nullptr);
	};

	template<typename T>
	inline bool Bound3<T>::intersect(const Ray & ray, Float * hitt0, Float * hitt1)
	{
		Float t0 = 0, t1 = fInfinity;
		for (int i = 0; i < 3; ++i) {
			Float inv_ray_dir = 1 / ray.dir[i];
			Float t_near = (p_min[i] - ray.orig[i]) * inv_ray_dir;
			Float t_far  = (p_max[i] - ray.orig[i]) * inv_ray_dir;
			if (t_near > t_far) std::swap(t_near, t_far);

			t0 = t_near > t0 ? t_near : t0;
			t1 = t_far < t1 ? t_far : t1;
			if (t0 > t1)
				return false;
		}
		if (hitt0) *hitt0 = t0;
		if (hitt1) *hitt1 = t1;
		return true;
	}

	template<typename T>
	Bound3<T> Union(const Bound3<T>& b, const glm::tvec3<T, glm::highp>& p)
	{
		return Bound3<T>(
			glm::tvec3<T, glm::highp>(std::min(b.p_min.x, p.x), std::min(b.p_min.y, p.y),
				std::min(b.p_min.z, p.z)),
			glm::tvec3<T, glm::highp>(std::max(b.p_max.x, p.x), std::max(b.p_max.y, p.y),
				std::max(b.p_max.z, p.z)));
	}

	template<typename T>
	Bound3<T> Union(const Bound3<T>& b1, const Bound3<T>& b2)
	{
		return Bound3<T>(glm::tvec3<T, glm::highp>(std::min(b1.p_min.x, b2.p_min.x),
			std::min(b1.p_min.y, b2.p_min.y),
			std::min(b1.p_min.z, b2.p_min.z)),
			glm::tvec3<T, glm::highp>(std::max(b1.p_max.x, b2.p_max.x),
				std::max(b1.p_max.y, b2.p_max.y),
				std::max(b1.p_max.z, b2.p_max.z)));
	}

	typedef Bound3<Float> Bound3f;
	typedef Bound3<int> Bound3i;

	inline Bound3f operator*(const Matrix4f &transform, const Bound3f &bound) {
		Bound3f ret(multiply_matrix4(transform, Vector3f(bound.p_min.x, bound.p_min.y, bound.p_min.z), 1.0f));
		ret = Union(ret, multiply_matrix4(transform, Vector3f(bound.p_max.x, bound.p_min.y, bound.p_min.z), 1.0f));
		ret = Union(ret, multiply_matrix4(transform, Vector3f(bound.p_min.x, bound.p_max.y, bound.p_min.z), 1.0f));
		ret = Union(ret, multiply_matrix4(transform, Vector3f(bound.p_min.x, bound.p_min.y, bound.p_max.z), 1.0f));
		ret = Union(ret, multiply_matrix4(transform, Vector3f(bound.p_min.x, bound.p_max.y, bound.p_max.z), 1.0f));
		ret = Union(ret, multiply_matrix4(transform, Vector3f(bound.p_max.x, bound.p_max.y, bound.p_min.z), 1.0f));
		ret = Union(ret, multiply_matrix4(transform, Vector3f(bound.p_max.x, bound.p_min.y, bound.p_max.z), 1.0f));
		ret = Union(ret, multiply_matrix4(transform, Vector3f(bound.p_max.x, bound.p_max.y, bound.p_max.z), 1.0f));
		return ret;
	}


	// 几何形状
	class Shape
	{
	public:
		Matrix4f local2world;
		Matrix4f world2local;
		Vector3f color; // 临时使用

	public:
		Shape(const Matrix4f &local2world, const Matrix4f &world2local) : local2world(local2world), world2local(world2local) {
			color = Vector3f(rand(), rand(), rand()); // 临时使用
		}
		virtual bool intersect(const Ray &ray, Float &t_hit, Intersection &iset) const = 0;
		virtual Bound3f get_bound() const = 0;
	};

	class Sphere : public Shape
	{
	private:
		//Vector3f center;
		Float radius, radius_square;// 半径平方

	public:
		Sphere(const Matrix4f &local2world, const Matrix4f &world2local, Float radius) 
			: Shape(local2world, world2local), radius(radius), radius_square(radius * radius) {}

		virtual bool intersect(const Ray &ray, Float &t_hit, Intersection &iset) const override;

		virtual Bound3f get_bound() const override {
			return Bound3f(Vector3f(-radius), Vector3f(radius));
		}
	};

	class Disk : public Shape
	{
	private:
		Float radius;

	public:
		// 默认圆盘中心位于原点，法向量指向Z轴正方向
		Disk(const Matrix4f &local2world, const Matrix4f &world2local, Float radius)
			: Shape(local2world, world2local), radius(radius) {}

		virtual bool intersect(const Ray &ray, Float &t_hit, Intersection &iset) const override;

		virtual Bound3f get_bound() const override {
			return Bound3f(Vector3f(-radius, -radius, -1e-6f), Vector3f(radius, radius, 1e-6f));
		}
	};
}

#endif // !ENG_PRIMITIVE_H_
