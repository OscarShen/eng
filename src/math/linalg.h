/************************************************************************ 
 * @description :  
 * @author		:  $username$
 * @creat 		:  $time$
************************************************************************ 
 * Copyright @ OscarShen 2017. All rights reserved. 
************************************************************************/  
#pragma once
#ifndef ENG_LINALG_H_
#define ENG_LINALG_H_
#include <eng.h>
#include <util/print.h>
namespace eng {
	// 点乘
	inline Float dot(const Vector3f &a, const Vector3f &b) {
		return glm::dot(a, b);
	}

	// 单位化
	inline Vector3f normalize(const Vector3f &a) {
		return glm::normalize(a);
	}

	// 求逆
	inline Matrix4f inverse(const Matrix4f &m) {
		return glm::inverse(m);
	}

	// 长度
	inline Float length(const Vector3f &v) {
		return glm::length(v);
	}

	// 叉积
	inline Vector3f cross(const Vector3f &v1, const Vector3f &v2) {
		return glm::cross(v1, v2);
	}

	// 转置
	inline Matrix4f transpose(const Matrix4f &m) {
		return glm::transpose(m);
	}
	// 反射
	inline Vector3f reflect(const Vector3f &I, const Vector3f &N) {
		return glm::reflect(I, N);
	}
	// 折射
	inline Vector3f refract(const Vector3f &I, const Vector3f &N, Float ior) {
		return glm::refract(I, N, ior);
	}

	inline Vector4f fract(const Vector4f &v) {
		return glm::fract(v);
	}

	// 混合
	inline Vector3f mix(const Vector3f &v1, const Vector3f &v2, Float mix) {
		return glm::mix(v1, v2, mix);
	}

	// 矩阵和三维向量相乘， w=0 表示向量， w=1 表示点
	inline Vector3f multiply_matrix4(const Matrix4f &m, const Vector3f &v, Float w) {
		//Vector4f tmp(v, w);
		//tmp = m * tmp;
		//return Vector3f(tmp.x, tmp.y, tmp.z);
		return Vector3f(m * Vector4f(v, w));
	}

	// 角度转弧度
	constexpr Float radians(Float degrees) {
		return degrees * static_cast<Float>(0.01745329251994329576923690768489);
	}

	template<typename T>
	inline T clamp(T a, T min, T max) {
		if (a < min) return min;
		if (a > max) return max;
		return a;
	}

	template<typename T>
	inline T clamp(T a) {
		if (a < 0) return 0;
		if (a > 1) return 1;
		return a;
	}

	// 线性插值
	template<typename T, typename V>
	inline V lerp(T a, V x_0, V x_1) {
		return (T(1) - a) * x_0 + a * x_1;
	}

	// 随机数
	inline float rand() {
		static unsigned int x = 123456789, y = 362436069, z = 521288629, w = 88675123;
		unsigned int t = x ^ (x << 11);
		x = y;
		y = z;
		z = w;
		return (w = (w ^ (w >> 19)) ^ (t ^ (t >> 8))) * (1.0f / 4294967296.0f);
	}

	// 以 normal 为 y 轴，返回一个指向 y 轴正方向的向量
	inline Vector3f set_up(const Vector3f &a, const Vector3f &y) {
		Vector3f x, z;
		if (std::abs(y.y) > 1.0f - epsilon) {
			x = Vector3f(1, 0, 0);
		}
		else {
			x = normalize(cross(y, Vector3f(0, 1, 0)));
		}
		z = cross(x, y);
		return a.x * x + a.y * y + a.z * z;
	}

	// 获得一个随机漫反射
	inline Vector3f random_diffuse(const Vector3f &normal, Float u, Float v) {
		if (u > v) {
			std::swap(u, v);
		}
		if (v < epsilon) {
			v = epsilon;
		}
		u /= v;
		Float xz = v, y = sqrt(1 - v * v);
		Float phi = u * pi * 2;
		return set_up(Vector3f(xz * cos(phi), y, xz * sin(phi)), normal);
	}

	inline Vector3f random_diffuse(const Vector3f &normal) {
		return random_diffuse(normal, rand(), rand());
	}

	inline bool inside_unit_cube(const Vector3f &p) {
		return 0 <= p.x && p.x < 1 && 0 <= p.y && p.y < 1 && 0 <= p.z && p.z < 1;
	}

	template<typename T>
	T square(const T &a) {
		return a * a;
	}

	template<typename T>
	T cube(const T &a) {
		return a * a * a;
	}

	inline int sgn(float a) {
		if (a < -epsilon)
			return -1;
		else if (a > epsilon)
			return 1;
		return 0;
	}

	inline int sgn(double a) {
		if (a < -epsilon)
			return -1;
		else if (a > epsilon)
			return 1;
		return 0;
	}

	inline Vector3f sample_sphere(float u, float v) {
		float x = u * 2 - 1;
		float phi = v * 2 * pi;
		float yz = sqrt(1 - x * x);
		return Vector3f(x, yz * cos(phi), yz * sin(phi));
	}

	inline float catmull_rom(float f_m_1, float f_0, float f_1, float f_2,
		float x_r) {
		float s = (f_1 - f_0);
		float s_0 = (f_1 - f_m_1) / 2.0f;
		float s_1 = (f_2 - f_0) / 2.0f;
		s_0 = s_0 * (s_1 * s > 0);
		s_1 = s_1 * (s_1 * s > 0);
		return f_0 + x_r * s_0 + (-3 * f_0 + 3 * f_1 - 2 * s_0 - s_1) * x_r * x_r +
			(2 * f_0 - 2 * f_1 + s_0 + s_1) * x_r * x_r * x_r;
	}

	inline float catmull_rom(float *pf_m_1, float x_r) {
		return catmull_rom(*pf_m_1, *(pf_m_1 + 1), *(pf_m_1 + 2), *(pf_m_1 + 3), x_r);
	}

	inline Matrix4f matrix4_translate(const Matrix4f &transform, const Vector3f &offset) {
		return glm::translate(Matrix4f(1.0f), offset) * transform;
	}

	inline Matrix4f matrix4_scale(const Matrix4f &transform, const Vector3f &scales) {
		return glm::scale(Matrix4f(1.0f), scales) * transform;
	}

	inline Matrix4f matrix4_scale_s(const Matrix4f &transform, Float scales) {
		return matrix4_scale(transform, Vector3f(scales));
	}

	inline Matrix4f matrix4_rotate_angle_axis(const Matrix4f &transform, Float angle, const Vector3f &axis) {
		return glm::rotate(Matrix4f(1.0f), glm::radians(angle), axis) * transform;
	}

	inline Matrix4f matrix4_rotate_eular(const Matrix4f &transform, const Vector3f &euler_angles) {
		Matrix4f ret = transform;
		ret = matrix4_rotate_angle_axis(ret, euler_angles.x, Vector3f(1.0f, 0.0f, 0.0f));
		ret = matrix4_rotate_angle_axis(ret, euler_angles.y, Vector3f(0.0f, 1.0f, 0.0f));
		ret = matrix4_rotate_angle_axis(ret, euler_angles.z, Vector3f(0.0f, 0.0f, 1.0f));
		return ret;
	}

	// 生成变换矩阵
	inline Matrix4f get_transform(const Vector3f *translate = nullptr, const Vector3f *rotate = nullptr, const Vector3f *scale = nullptr) {
		Matrix4f transform = Matrix4f();
		if (scale)
			transform = matrix4_scale(transform, *scale);
		if (rotate)
			transform = matrix4_rotate_eular(transform, *rotate);
		if (translate)
			transform = matrix4_translate(transform, *translate);
		return transform;
	}

	template<typename T>
	inline T hypot2(const T &x, const T &y) {
		return x * x + y * y;
	}

	inline float pow(const float &a, const float &b) {
		return ::pow(a, b);
	}

	inline double pow(const double &a, const double &b) {
		return ::pow(a, b);
	}

	inline Vector3f abs(const Vector3f &v) {
		return Vector3f(std::abs(v.x), std::abs(v.y), std::abs(v.z));
	}

	inline Float max_component(const Vector3f &v) {
		return std::max(v.x, std::max(v.y, v.z));
	}

	inline
	bool solve_quadratic(const Float &a, const Float &b, const Float &c, Float &x0, Float &x1)
	{
		Float discr = b * b - 4 * a * c;
		if (discr < 0) return false;
		else if (discr == 0) x0 = x1 = -0.5f * b / a;
		else {
			Float q = (b > 0) ?
				-0.5f * (b + sqrt(discr)) :
				-0.5f * (b - sqrt(discr));
			x0 = q / a;
			x1 = c / q;
		}
		if (x0 > x1) std::swap(x0, x1);
		return true;
	}

	// 射线与平面交点
	inline bool intersect_plane(const Vector3f &plane_normal, const Vector3f &plane_anchor, const Vector3f &orig, const Vector3f &dir, Float &t) {
		Float denom = std::abs(dot(plane_normal, dir));
		if (denom > 1e-6f) {
			Vector3f L = plane_anchor - orig;
			if(dot(L, dir) <= 0)
				return false;
			t = std::abs(dot(L, plane_normal)) / denom;
			return true;
		}
		return false;
	}

	// 射线与三角形相交
	inline bool intersect_triangle(const Vector3f &orig, const Vector3f &dir, const Vector3f &v0, const Vector3f &v1, const Vector3f &v2,
		Float &t, Float &u, Float &v) {
		Vector3f v0v1 = v1 - v0;
		Vector3f v0v2 = v2 - v0;
		Vector3f pvec = cross(dir,v0v2);
		float det = dot(v0v1, pvec);

		// ray and triangle are parallel if det is close to 0
		if (fabs(det) < epsilon) return false;

		float invDet = 1 / det;

		Vector3f tvec = orig - v0;
		u = dot(tvec, pvec) * invDet;
		if (u < 0 || u > 1) return false;

		Vector3f qvec = cross(tvec, v0v1);
		v = dot(dir, qvec) * invDet;
		if (v < 0 || u + v > 1) return false;

		t = dot(v0v2, qvec) * invDet;

		return true;
	}
}

#endif // !ENG_LINALG_H_
