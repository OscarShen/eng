#include "camera.h"
namespace eng {
	ENG_INTERFACE_DEF(Camera, "camera");

	class PinholeCamera : public Camera
	{
	private:
		Float fov, tan_half_fov;

	public:
		virtual void initialize(const Config &config) override {
			Camera::initialize(config);
			this->fov = config.get_Float("fov");
			this->tan_half_fov = tan(radians(fov * 0.5f));
		}

		virtual Ray generate_ray(Vector2i offset) override {
			Float x = (2 * (offset.x + 0.5f) / (Float)width - 1) * aspect_ratio * tan_half_fov;
			Float y = (1 - 2 * (offset.y + 0.5f) / (Float)height) * tan_half_fov;
			Vector3f local_dir = normalize(Vector3f(x, y, -1.0f));
			Vector3f world_orig = multiply_matrix4(local2world, origin, 1);
			Vector3f world_dir = multiply_matrix4(local2world, local_dir, 0);		// 相机向量转换不可能发生缩放
			return Ray(world_orig, world_dir);
		}
	};
	ENG_IMPLEMENTATION(Camera, PinholeCamera, "pinhole");
}
