#include "config.h"

namespace eng {
	void split(std::string str, std::vector<std::string>* strs, const std::string & seperator)
	{
		std::string substring;
		std::string::size_type start = 0, index;
		strs->clear();

		do {
			index = str.find(seperator, start);
			if (index != std::string::npos) {
				substring = str.substr(start, index - start);
				strs->push_back(substring);
				start = index + seperator.size();
				if (start >= str.size())
					return;
			}
		} while (index != std::string::npos);

		substring = str.substr(start);
		strs->push_back(substring);
	}

	void trim(std::string &str)
	{
		if (str.empty())
			return;
		str.erase(0, str.find_first_not_of(" "));
		str.erase(str.find_last_not_of(" ") + 1);
	}
}
