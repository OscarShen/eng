/************************************************************************
* @description :
* @author		:  $username$
* @creat 		:  $time$
************************************************************************
* Copyright @ OscarShen 2017. All rights reserved.
************************************************************************/
#pragma once
#ifndef ENG_H_
#define ENG_H_
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>
#include <memory>
#include <vector>
#include <iostream>
#include <string>
#include <functional>
#include <map>
#include <sstream>
#include <fstream>

namespace eng {

#undef max
#undef min

#define CHECK_INFO(x, str) if(!(x)) { std::cout << (str) << "\n file: "<< __FILE__<<",line: " << __LINE__<<std::endl; }
#define CHECK(x) CHECK_INFO(x, "check equal failed!")
#define CHECK_EQ(x, y) if((x)==(y)) { std::cout << "check equal failed!" << "\n file: "<< __FILE__<<",line: " << __LINE__<<std::endl; }
#define CHECK_NE(x, y) if((x)!=(y)) { std::cout << "check not equal failed!" << "\n file: "<< __FILE__<<",line: " << __LINE__<<std::endl }
#define ERROR(info) CHECK_INFO(false, info)

	typedef long long int64;

	typedef glm::ivec2 Vector2i;
	typedef glm::ivec3 Vector3i;
	typedef glm::ivec4 Vector4i;

	typedef glm::vec2 Vector2f;
	typedef glm::vec3 Vector3f;
	typedef glm::vec4 Vector4f;

	typedef glm::dvec2 Vector2d;
	typedef glm::dvec3 Vector3d;
	typedef glm::dvec4 Vector4d;

	typedef float Float;
	typedef glm::mat2 Matrix2f;
	typedef glm::mat3 Matrix3f;
	typedef glm::mat4 Matrix4f;

	constexpr Float epsilon = 1e-6f;

	constexpr float fInfinity = std::numeric_limits<Float>::max();
	constexpr Float shaow_epsilon = 0.0001f;
	constexpr Float pi = 3.14159265358979323846f;
	constexpr Float inv_pi = 0.31830988618379067154f;
	constexpr Float inv2pi = 0.15915494309189533577f;
	constexpr Float inv4pi = 0.07957747154594766788f;
	constexpr Float pi_over_2 = 1.57079632679489661923f;
	constexpr Float pi_over_4 = 0.78539816339744830961f;
	constexpr Float sqrt_square = 1.41421356237309504880f;

	class Shape;

}

#endif // !ENG_H_
