/************************************************************************ 
 * @description :  
 * @author		:  $username$
 * @creat 		:  $time$
************************************************************************ 
 * Copyright @ OscarShen 2017. All rights reserved. 
************************************************************************/  
#pragma once
#ifndef ENG_TRIANGLE_H_
#define ENG_TRIANGLE_H_
#include <eng.h>
#include "primitive.h"
namespace eng {
	//struct TriangleMesh
	//{
	//	const int num_triangle, num_vertex;
	//	std::vector<int> vertex_indices;
	//	std::unique_ptr<Vector3f[]> p;
	//	std::unique_ptr<Vector3f[]> n;
	//	std::unique_ptr<Vector2f[]> uv;

	//	TriangleMesh(const Matrix4f &local2world, const Matrix4f &world2local, int num_triangle, const int *vertex_indices,
	//		int num_vertex, const Vector3f *P, const Vector3f *N,
	//		const Vector2f *UV) : num_triangle(num_triangle), num_vertex(num_vertex),
	//		vertex_indices(vertex_indices, vertex_indices + 3 * num_triangle) {
	//		// Transform mesh to world space
	//		p.reset(new Vector3f[num_vertex]);
	//		for (int i = 0; i < num_vertex; ++i) p[i] = multiply_matrix4(local2world, P[i], 1);

	//		if (UV) {
	//			uv.reset(new Vector2f[num_vertex]);
	//			memcpy(uv.get(), UV, num_vertex * sizeof(Vector2f));
	//		}
	//		if (N) {
	//			n.reset(new Vector3f[num_vertex]);
	//			const Matrix4f tran_world2local = transpose(world2local);
	//			for (int i = 0; i < num_vertex; ++i) n[i] = multiply_matrix4(tran_world2local, N[i], 0);
	//		}
	//	}
	//};

	class TriangleMesh : public Shape
	{
		typedef std::shared_ptr<std::vector<Vector3f>> Vector3fs_s;
		typedef std::shared_ptr<std::vector<int>> ints_s;
		typedef std::shared_ptr<std::vector<Vector2f>> Vector2fs_s;
	public:
		int num_triangles;
		Vector3fs_s p;              // triangles vertex position
		Vector3fs_s n;              // triangles vertex normals
		Vector2fs_s uv;				// triangles texture coordinates
		ints_s vertex_indices;		// vertex index array

		TriangleMesh(
			const Matrix4f &local2world, const Matrix4f &world2local,
			const Vector3fs_s &p, const Vector3fs_s &n, const Vector2fs_s &uv,
			const ints_s &vertex_indices, int num_triangles);


		virtual bool intersect(const Ray &ray, Float &t_hit, Intersection &iset) const override;
		virtual Bound3f get_bound() const override;
	};
}

#endif // !ENG_TRIANGLE_H_
