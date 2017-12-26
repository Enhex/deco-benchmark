#include "common.h"

#include <cctype>
#include <random>
#include <vector>


// a vector of all the graphical chracters
const auto graphical_characters = []() {
	std::vector<char> chars;

	constexpr auto max = std::numeric_limits<unsigned char>::max();
	for (unsigned char c = 0; c < max; ++c) {
		if (std::isgraph(c))
			chars.emplace_back(c);
	}
	// max value too (can't do it in the loop with `unsigned char` since it will cause an integer overflow)
	if (std::isgraph(max))
		chars.emplace_back(max);

	return chars;
}();


std::mt19937 rng;
auto distribution_int = std::uniform_int_distribution<>(std::numeric_limits<int>::min(), std::numeric_limits<int>::max());

//auto distribution_float = std::uniform_real_distribution<>(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max());
auto distribution_float = std::uniform_real_distribution<float>(-10000, 10000);

auto distribution_graph_char = std::uniform_int_distribution<unsigned short>(0, (unsigned short)graphical_characters.size() - 1);


void seed_rng(unsigned seed)
{
	rng.seed(seed);
}

int random_int() {
	return distribution_int(rng);
};

int random_int(int max) {
	return std::uniform_int_distribution<>(0, max)(rng);
}

int random_int(int min, int max) {
	return std::uniform_int_distribution<>(min, max)(rng);
}

float random_float() {
	return distribution_float(rng);
};

char random_char() {
	return graphical_characters[distribution_graph_char(rng)];
};

std::string random_string(unsigned size) {
	std::string str;
	str.reserve(size);
	while (size-- > 0)
		str += random_char();
	return str;
};