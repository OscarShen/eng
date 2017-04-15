#include <eng.h>
#include <core/camera.h>
#include <core/sampler.h>
#include <core/primitive.h>

using namespace eng;

bool trace(const Ray &ray, const std::vector<std::shared_ptr<Shape>> &shapes, Float &t_near, Intersection &isec) {
	t_near = fInfinity;
	bool hit(false);
	for (auto it = shapes.begin(); it != shapes.end(); ++it) {
		Float t = fInfinity;
		Intersection intersection;
		if ((*it)->intersect(ray, t, intersection) && t < t_near) {
			isec = intersection;
			t_near = t;
			hit = true;
		}
	}
	return hit;
}

Vector3f sample(const Ray &ray, const Intersection &isec) {
	Float scale(8.0f);
	bool pattern = (fmodf(isec.uv.x * scale, 1.0f) > 0.5f) ^ (fmodf(isec.uv.y * scale, 1.0f) > 0.5f);
	Vector3f color = std::max(0.0f, dot(isec.normal, -ray.dir)) * mix(isec.shape->color, isec.shape->color * 0.8f, (Float)pattern);

	return color;
}

void spheres() {
	Config camera_config = Config().set("origin", Vector3f(0.0f)).set("look_at", Vector3f(0.0f, 0.0f, -1.0f))
		.set("up", Vector3f(0.0f, 1.0f, 0.0f)).set("width", 800).set("height", 600).set("fov", 45.0f);
	std::shared_ptr<Camera> camera = create_initialized_instance<Camera>("pinhole", camera_config);
	
	auto sampler = create_initialized_instance<Sampler>("prand", Config());
	RandomSequence rand(sampler, 0);


	std::vector<std::shared_ptr<Shape>> shapes;
	for (int i = 0; i < 50; ++i) {
		Config shapes_config = Config().set("radius", rand()).set("rotate", Vector3f(rand() * 90, rand() * 90, rand() * 90)).set("translate", Vector3f(rand() * 10 - 5.0f, rand() * 10 - 5.0f, -rand() * 10 - 10.0f));
		shapes.push_back(create_initialized_instance<Shape>("sphere", shapes_config));
	}

	for (int j = 0; j < 600; ++j) {
		for (int i = 0; i < 800; ++i) {
			Ray ray = camera->generate_ray(Vector2i(i, j));
			Float t_near = fInfinity;
			Intersection isec;
			Vector3f color;
			if (trace(ray, shapes, t_near, isec)) {
				color = sample(ray, isec);
			}
			camera->get_film()->set_color(i, j, color);
		}
	}

	auto film = camera->get_film().get();
	std::ofstream ofs("./result/spheres.ppm", std::ios::out | std::ios::binary);
	ofs << "P6\n" << camera->get_width() << " " << camera->get_height() << "\n255\n";
	for (int i = 0; i < camera->get_width() * camera->get_height(); ++i) {
		char r = (char)(255 * clamp(film->frame_buffer[i].x));
		char g = (char)(255 * clamp(film->frame_buffer[i].y));
		char b = (char)(255 * clamp(film->frame_buffer[i].z));
		ofs << r << g << b;
	}
}

//void main() {
//	spheres();
//}