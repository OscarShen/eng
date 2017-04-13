/************************************************************************ 
 * @description :  
 * @author		:  $username$
 * @creat 		:  $time$
************************************************************************ 
 * Copyright @ OscarShen 2017. All rights reserved. 
************************************************************************/  
#pragma once
#ifndef ENG_CONSTANTS_H_
#define ENG_CONSTANTS_H_

#include <core/eng.h>
namespace eng {

	constexpr Float stefan_boltzmann_constant = 5.670373e-8f;


	inline Float luminance(const Vector3f &v) {
		Float std_y_weight[3] = { 0.212671f, 0.715160f, 0.072169f }; // CIE XYZ 中的 Y 表示计算亮度
		Float lum = 0;
		for (int i = 0; i < 3; i++) {
			lum += std_y_weight[i] * v[i];
		}
		return lum;
	}
}

#endif // !ENG_CONSTANTS_H_
