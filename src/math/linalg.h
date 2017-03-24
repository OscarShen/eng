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
	// ���
	inline Float dot(const Vector3 &a, const Vector3 &b) {
		return glm::dot(a, b);
	}

	// ��λ��
	inline Vector3 normalize(const Vector3 &a) {
		return glm::normalize(a);
	}

	// �������ά������ˣ� w=0 ��ʾ������ w=1 ��ʾ��
	inline Vector3 multiply_matrix4(const Matrix4 &m, const Vector3 &v, Float w) {
		Vector4 tmp(v, w);
		tmp = m * tmp;
		return Vector3(tmp.x, tmp.y, tmp.z);
	}
}

#endif // !ENG_LINALG_H_
