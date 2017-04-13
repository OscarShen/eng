/************************************************************************ 
 * @description :  
 * @author		:  $username$
 * @creat 		:  $time$
************************************************************************ 
 * Copyright @ OscarShen 2017. All rights reserved. 
************************************************************************/  
#pragma once
#ifndef ENG_PRIMITIVE_H_
#define ENG_PRIMITIVE_H_
#include <eng.h>
namespace eng {

	class Ray
	{
	public:
		Vector3f orig, dir;

		Ray(const Vector3f &orig, const Vector3f &dir)
			:orig(orig), dir(dir) {}
	};
}

#endif // !ENG_PRIMITIVE_H_
