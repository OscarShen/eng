#include "sampler.h"
#include <random>

namespace eng {
	ENG_INTERFACE_DEF(Sampler, "sampler");

	class PseudoRandomSampler : public Sampler
	{
	private:
		mutable std::mt19937_64 gen;
		std::uniform_real_distribution<> dis;

	public:
		void initialize(Config &config) {
			std::random_device rd;
			gen = std::mt19937_64(gen());
			dis = std::uniform_real_distribution<>(0, 1);
		}

		Float sample(int d, long long i) const override {
			return Float(dis(gen));
		}
	};
	ENG_IMPLEMENTATION(Sampler, PseudoRandomSampler, "prand");
}