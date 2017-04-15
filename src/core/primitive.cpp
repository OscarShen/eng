#include "primitive.h"

namespace eng {
	bool Sphere::intersect(const Ray & ray, Float & t_hit, Intersection & iset) const
	{
		Ray local_ray = world2local * ray;
		//Ray local_ray = ray;
		Float t0, t1;
		//Vector3f L = local_ray.orig - center;
		Vector3f L = local_ray.orig;
		Float a = dot(local_ray.dir, local_ray.dir);
		Float b = 2 * dot(local_ray.dir, L);
		Float c = dot(L, L) - radius_square;
		if (!solve_quadratic(a, b, c, t0, t1)) return false;	// 无解
		if (t0 > t1) std::swap(t0, t1);

		if (t0 < 0) {
			t0 = t1;
			if (t0 < 0) return false;
		}
		t_hit = t0;
		Vector3f p_hit = local_ray.orig + local_ray.dir * t0;
		Vector3f normal = normalize(p_hit);
		if (dot(normal, local_ray.dir) > 0)
			normal = -normal;
		Float u = (1 + atan2(normal.z, normal.x) * inv_pi) * 0.5f;
		Float v = acosf(normal.y) * inv_pi;

		iset = transform(local2world, world2local, Intersection(p_hit, normal, Vector2f(u, v), this));
		return true;
	}
	bool Disk::intersect(const Ray & ray, Float & t_hit, Intersection & iset) const
	{
		Ray local_ray = world2local * ray;
		Float t = 0;

		Vector3f p_hit;
		Float distance;
		Vector3f normal = Vector3f(0.0f, 0.0f, 1.0f);
		// 默认圆盘中心位于原点，法向量指向Z轴正方向
		if (intersect_plane(normal, Vector3f(0.0f), local_ray.orig, local_ray.dir, t)) {
			t_hit = t;

			p_hit = local_ray.get_point(t);
			distance = sqrt(dot(p_hit, p_hit));
			if (distance > radius)
				return false;

			if (dot(normal, local_ray.dir) > 0)
				normal = -normal;
			Float phi = acos(dot(normalize(p_hit), Vector3f(1.0f, 0.0f, 0.0f)));
			if (p_hit.y < 0) { // 说明角度大于 pi
				phi = 2 * pi - phi;
			}
			Float u = phi / (2 * pi);
			Float v = distance / radius;

			iset = transform(local2world, world2local, Intersection(p_hit, normal, Vector2f(u, v), this));
			return true;
		}
		return false;
	}
}
