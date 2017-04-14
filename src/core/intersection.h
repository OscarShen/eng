/************************************************************************ 
 * @description :  
 * @author		:  $username$
 * @creat 		:  $time$
************************************************************************ 
 * Copyright @ OscarShen 2017. All rights reserved. 
************************************************************************/  
#pragma once
#ifndef ENG_INTERSECTION_H_
#define ENG_INTERSECTION_H_
#include <eng.h>
#include <math/linalg.h>
#include <util/print.h>
namespace eng {

	struct Intersection
	{
		Vector3f p_hit;
		Vector3f normal;
		Vector2f uv;
		const Shape *shape;

		Intersection() {}
		Intersection(const Vector3f &p_hit, const Vector3f &normal,
			const Vector2f &uv, const Shape *shape) : p_hit(p_hit),
			normal(normal), uv(uv), shape(shape) {}
	};

	inline Intersection transform(const Matrix4f &transform, const Matrix4f &inv_transform, const Intersection &isec) {
		Vector3f p_hit = multiply_matrix4(transform, isec.p_hit, 1);
		Vector3f normal = normalize(multiply_matrix4(transpose(inv_transform), isec.normal, 0));
		return Intersection(p_hit, normal, isec.uv, isec.shape);
	}
}

#endif // !ENG_INTERSECTION_H_
