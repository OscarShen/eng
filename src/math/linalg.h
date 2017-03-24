/************************************************************************ 
 * @description :  
 * @author		:  $username$
 * @creat 		:  $time$
************************************************************************ 
 * Copyright @ OscarShen 2017. All rights reserved. 
************************************************************************/  
#pragma once
#ifndef ENG_LINALG_H_
#define ENG_LINALG_H_
#include <core/eng.h>
namespace eng {
	// 点乘
	inline Float dot(const Vector3 &a, const Vector3 &b) {
		return glm::dot(a, b);
	}

	// 单位化
	inline Vector3 normalize(const Vector3 &a) {
		return glm::normalize(a);
	}

	// 矩阵和三维向量相乘， w=0 表示向量， w=1 表示点
	inline Vector3 multiply_matrix4(const Matrix4 &m, const Vector3 &v, Float w) {
		Vector4 tmp(v, w);
		tmp = m * tmp;
		return Vector3(tmp.x, tmp.y, tmp.z);
	}
}

#endif // !ENG_LINALG_H_
