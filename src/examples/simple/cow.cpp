#include <eng.h>
#include <core/camera.h>
#include <core/sampler.h>
#include <core/primitive.h>
#include <core/triangle.h>
#include <chrono>
#include <util/model_loader.h>

using namespace eng;

bool trace_cow(const Ray &ray, const std::vector<std::shared_ptr<Shape>> &shapes, Float &t_near, Intersection &isec)
{
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

Vector3f sample_cow(const Ray &ray, const Intersection &isec) {
	Float NdotView = std::max(0.f, dot(isec.normal, -ray.dir)); 
	return Vector3f(0.7f * NdotView);
}

void cow()
{
	int width = 800, height = 600;
	Config camera_config = Config().set("origin", Vector3f(0.0f, 0.0f, 5.0f)).set("look_at", Vector3f(0.0f, 0.0f, 0.0f))
		.set("up", Vector3f(0.0f, 1.0f, 0.0f)).set("width", width).set("height", height).set("fov", 45.0f);
	std::shared_ptr<Camera> camera = create_initialized_instance<Camera>("pinhole", camera_config);

	std::vector<std::shared_ptr<Shape>> shapes;

	ModelData modeldata;
	ModelLoader::loadModel("./result/cow.obj", modeldata);

	Matrix4f transform = matrix4_rotate_eular(Matrix4f(), Vector3f(0.0f, -45.0f, 0.0f));
	transform = matrix4_translate(transform, Vector3f(0.0f, -1.0f, 0.0f));

	shapes.push_back(std::shared_ptr<Shape>(new TriangleMesh(transform, inverse(transform), modeldata.vertices,
		modeldata.normals, modeldata.uvs, modeldata.indices, modeldata.num_triangles)));

	auto timeStart = std::chrono::high_resolution_clock::now();

	for (int j = 0; j < height; ++j) {
		for (int i = 0; i < width; ++i) {
			Ray ray = camera->generate_ray(Vector2i(i, j));
			Float t_near = fInfinity;
			Intersection isec;
			Vector3f color;
			if (trace_cow(ray, shapes, t_near, isec)) {
				color = sample_cow(ray, isec);
			}
			camera->get_film()->set_color(i, j, color);
		}
		fprintf(stderr, "\r%3d%c", int(j / (float)height * 100), '%');
	}
	auto timeEnd = std::chrono::high_resolution_clock::now();
	auto passedTime = std::chrono::duration<double, std::milli>(timeEnd - timeStart).count();
	fprintf(stderr, "\rDone: %.2f (sec)\n", passedTime / 1000);

	auto film = camera->get_film().get();
	std::ofstream ofs("./result/cow.ppm", std::ios::out | std::ios::binary);
	ofs << "P6\n" << camera->get_width() << " " << camera->get_height() << "\n255\n";
	for (int i = 0; i < camera->get_width() * camera->get_height(); ++i) {
		char r = (char)(255 * clamp(film->frame_buffer[i].x));
		char g = (char)(255 * clamp(film->frame_buffer[i].y));
		char b = (char)(255 * clamp(film->frame_buffer[i].z));
		ofs << r << g << b;
	}

	ofs.close();
}

//void main()
//{
//	cow();
//}