/************************************************************************ 
 * @description :  
 * @author		:  $username$
 * @creat 		:  $time$
************************************************************************ 
 * Copyright @ OscarShen 2017. All rights reserved. 
************************************************************************/  
#pragma once
#ifndef ENG_SAMPLER_H_
#define ENG_SAMPLER_H_
#include <eng.h>
#include <common/meta.h>
namespace eng {

	class StateSequence
	{
	protected:
		int cursor = 0;

	public:
		virtual Float sample() = 0;

		virtual Float operator()() {
			return sample();
		}

		int get_cursor() const {
			return cursor;
		}

		void assert_cursor_pos(int cursor) const {
			CHECK_INFO(this->cursor == cursor, std::string("Cursor position should be " + std::to_string(cursor) +
				" instead of " + std::to_string(this->cursor)));
		}

		Vector2f next2() {
			return Vector2f(sample(), sample());
		}

		Vector3f next3() {
			return Vector3f(sample(), sample(), sample());
		}

		Vector4f next4() {
			return Vector4f(sample(), sample(), sample(), sample());
		}
	};

	class Sampler : public Unit
	{
	public:
		virtual Float sample(int d, long long i) const = 0;
	};

	ENG_INTERFACE(Sampler);

	class RandomSequence : public StateSequence
	{
	private:
		std::shared_ptr<Sampler> sampler;
		long long instance;

	public:
		RandomSequence(std::shared_ptr<Sampler> sampler, long long instance)
			:sampler(sampler), instance(instance) {}

		Float sample() override {
			CHECK_INFO(sampler != nullptr, "null sampler");
			Float ret = sampler->sample(cursor++, instance);
			CHECK_INFO(ret >= 0, "sampler output should be non-neg");
			if (ret > 1 + 1e-5f) {
				printf("Warning: sampler returns value > 1: [%f]", ret);
			}
			if (ret >= 1) {
				ret = 0;
			}
			return ret;
		}
	};

}

#endif // !ENG_SAMPLER_H_
