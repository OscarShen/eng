/************************************************************************ 
 * @description :  
 * @author		:  $username$
 * @creat 		:  $time$
************************************************************************ 
 * Copyright @ OscarShen 2017. All rights reserved. 
************************************************************************/  
#pragma once
#ifndef ENG_PRINT_H_
#define ENG_PRINT_H_
#include <eng.h>
namespace eng {
	inline std::ostream &operator<<(std::ostream &os, const Vector3f &v) {
		os << " [" << v.x << ", " << v.y << ", " << v.z << "] ";
		return os;
	}
}

#endif // !ENG_PRINT_H_
