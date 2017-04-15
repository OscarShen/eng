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

		virtual Bound3f get_bound() const override {
			return Bound3f(Vector3f(-radius), Vector3f(radius));
		}

	};

	ENG_IMPLEMENTATION(Shape, Sphere, "sphere");

	class Disk : public Shape
	{
	private:
		Float radius, radius_square; // 平面宽度

	public:
		virtual void initialize(const Config &config) override {
			Shape::initialize(config);
			this->radius = config.get_Float("radius");
			this->radius_square = radius * radius;
		}

		virtual bool intersect(const Ray &ray, Float &t_hit, Intersection &iset) const override {
			Ray local_ray = world2local * ray;
			Float t = 0;

			Vector3f p_hit;
			Float distance_suqare;
			Vector3f normal = Vector3f(0.0f, 0.0f, 1.0f);
			// 默认圆盘中心位于原点，法向量指向Z轴正方向
			if (intersect_plane(normal, Vector3f(0.0f), local_ray.orig, local_ray.dir, t)) {
				t_hit = t;

				p_hit = local_ray.get_point(t);
				distance_suqare = dot(p_hit, p_hit);
				if (distance_suqare > radius_square)
					return false;

				if (dot(normal, local_ray.dir) > 0)
					normal = -normal;
				Float phi = acos(dot(normalize(p_hit), Vector3f(1.0f, 0.0f, 0.0f)));
				if (p_hit.y < 0) { // 说明角度大于 pi
					phi = 2 * pi - phi;
				}
				Float u = phi / (2 * pi);
				Float v = sqrt(distance_suqare) / radius;

				iset = transform(local2world, world2local, Intersection(p_hit, normal, Vector2f(u, v), this));
				return true;
			}
			return false;
		}

		virtual Bound3f get_bound() const override {
			return Bound3f(Vector3f(-radius, -radius, -1e-6f), Vector3f(radius, radius, 1e-6f));
		}
	};

	ENG_IMPLEMENTATION(Shape, Disk, "disk");
}
