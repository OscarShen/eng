#include "fresnel.h"

namespace eng {
	void fresnel(const Vector3f & I, const Vector3f & N, const Float & ior, Float & kr)
	{
		Float cosi = clamp(dot(I, N), -1.0f, 1.0f);
		Float etai = 1, etat = ior;
		if (cosi > 0) { std::swap(etai, etat); }
		// Compute sini using Snell's law
		Float sint = etai / etat * sqrtf(std::max(0.f, 1 - cosi * cosi));
		// Total internal reflection
		if (sint >= 1) {
			kr = 1;
		}
		else {
			Float cost = sqrtf(std::max(0.f, 1 - sint * sint));
			cosi = fabsf(cosi);
			Float Rs = ((etat * cosi) - (etai * cost)) / ((etat * cosi) + (etai * cost));
			Float Rp = ((etai * cosi) - (etat * cost)) / ((etai * cosi) + (etat * cost));
			kr = (Rs * Rs + Rp * Rp) / 2;
		}
		// As a consequence of the conservation of energy, transmittance is given by:
		// kt = 1 - kr;
	}

}
