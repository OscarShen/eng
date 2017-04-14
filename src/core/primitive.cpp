#include "primitive.h"

namespace eng {
	ENG_INTERFACE_DEF(Shape, "shape");

	class Sphere : public Shape
	{
	private:
		//Vector3f center;
		Float radius, radius_square;// 半径平方

	public:
		virtual void initialize(const Config &config) override {
			Shape::initialize(config);
			this->radius = config.get_Float("radius");
			this->radius_square = radius * radius;
		}

		virtual bool intersect(const Ray &ray, Float &t_hit, Intersection &iset) const override {
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
	};

	ENG_IMPLEMENTATION(Shape, Sphere, "sphere");
}
