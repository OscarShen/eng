/************************************************************************ 
 * @description :  
 * @author		:  $username$
 * @creat 		:  $time$
************************************************************************ 
 * Copyright @ OscarShen 2017. All rights reserved. 
************************************************************************/  
#pragma once
#ifndef ENG_FRESNEL_H_
#define ENG_FRESNEL_H_
#include <eng.h>
#include <math/linalg.h>
namespace eng {

	/************************************************************************
	* @description  :  Compute Fresnel equation
					   \param I is the incident view direction
					   \param N is the normal at the intersection point
					   \param ior is the mateural refractive index
					   \param[out] kr is the amount of light reflected 
	* @creat 		:  2017Äê4ÔÂ13ÈÕ10:39:47
	************************************************************************/
	void fresnel(const Vector3f &I, const Vector3f &N, const Float &ior, Float &kr);
}

#endif // !ENG_FRESNEL_H_
