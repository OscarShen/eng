#include <core/camera.h>
#include <util/print.h>
using namespace eng;

Vector3f trace(Ray &ray) {
	return (ray.dir + Vector3f(1)) * 0.5f;
}

void pinhole() {
	Config camera_config = Config().set("origin", Vector3f(0.0f)).set("look_at", Vector3f(0.0f, 0.0f, -1.0f))
		.set("up", Vector3f(0.0f, 1.0f, 0.0f)).set("width", 800).set("height", 600).set("fov", 90.0f);
	std::shared_ptr<Camera> camera = create_initialized_instance<Camera>("pinhole", camera_config);
	for (int j = 0; j < 600; ++j) {
		for (int i = 0; i < 800; ++i) {
			Ray ray = camera->generate_ray(Vector2i(i, j));
			Vector3f color = trace(ray);
			camera->get_film()->set_color(i, j, color);
		}
	}

	auto film = camera->get_film().get();
	std::ofstream ofs("./result/pinhole.ppm", std::ios::out | std::ios::binary);
	ofs << "P6\n" << camera->get_width() << " " << camera->get_height() << "\n255\n";
	for (int i = 0; i < camera->get_width() * camera->get_height(); ++i) {
		char r = (char)(255 * clamp(film->frame_buffer[i].x));
		char g = (char)(255 * clamp(film->frame_buffer[i].y));
		char b = (char)(255 * clamp(film->frame_buffer[i].z));
		ofs << r << g << b;
	}

	ofs.close();
}

//void main() {
//	pinhole();
//}