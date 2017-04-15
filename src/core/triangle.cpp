#include "triangle.h"

namespace eng {
	TriangleMesh::TriangleMesh(const Matrix4f & local2world, const Matrix4f & world2local, const Vector3fs_s & p, const Vector3fs_s & n, const Vector2fs_s & uv, const ints_s & vertex_indices, int num_triangles)
		: Shape(local2world, world2local), p(p), n(n), uv(uv), vertex_indices(vertex_indices),
		num_triangles(num_triangles)
	{
		std::vector<Vector3f> &P = *p.get();
		std::vector<Vector3f> &N = *n.get();
		Matrix4f trans_world2local = transpose(world2local);
		for (size_t i = 0; i < P.size(); ++i) {
			P[i] = multiply_matrix4(local2world, P[i], 1);
			N[i] = multiply_matrix4(trans_world2local, N[i], 0);
		}
	}

	bool TriangleMesh::intersect(const Ray & ray, Float & t_hit, Intersection & iset) const
	{
		int j = 0;
		bool hit(false);
		const std::vector<Vector3f> &P = *p.get();
		const std::vector<int> &indices = *vertex_indices.get();
		const std::vector<Vector2f> &UV = *uv.get();
		const std::vector<Vector3f> &N = *n.get();
		int hit_index = -1;

		Vector2f barycentric_uv;

		// t_hit & hit_index & barycentric uv
		for (int i = 0; i < num_triangles; ++i) {
			const Vector3f &v0 = P[indices[j]];
			const Vector3f &v1 = P[indices[j + 1]];
			const Vector3f &v2 = P[indices[j + 2]];
			Float t = fInfinity, u, v;
			if (intersect_triangle(ray.orig, ray.dir, v0, v1, v2, t, u, v) && t < t_hit) {
				t_hit = t;
				barycentric_uv.x = u;
				barycentric_uv.y = v;
				hit_index = i;
				hit = true;
			}
			j += 3;
		}

		if (!hit)
			return false;

		// p_hit
		Vector3f p_hit = ray.get_point(t_hit);

		// vertex normal
		const Vector3f &n0 = N[indices[hit_index * 3]];
		const Vector3f &n1 = N[indices[hit_index * 3 + 1]];
		const Vector3f &n2 = N[indices[hit_index * 3 + 2]];
		Vector3f normal = (1 - barycentric_uv.x - barycentric_uv.y) * n0 + barycentric_uv.x * n1 + barycentric_uv.y * n2;
		

		// texture uv
		const Vector2f &uv0 = UV[indices[hit_index * 3]];
		const Vector2f &uv1 = UV[indices[hit_index * 3 + 1]];
		const Vector2f &uv2 = UV[indices[hit_index * 3 + 2]];
		Vector2f texture_uv = (1 - barycentric_uv.x - barycentric_uv.y) * uv0 + barycentric_uv.x * uv1 + barycentric_uv.y * uv2;

		iset = Intersection(p_hit, normal, texture_uv, this);
		return true;
	}
	Bound3f TriangleMesh::get_bound() const
	{
		return Bound3f();
	}
}

