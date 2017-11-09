#include <cassert>
#include <cctype>
#include <chrono>
#include <deco/NVP.h>
#include <deco/set.h>
#include <deco/types/arithmetic.h>
#include <deco/types/string.h>
#include <deco/types/vector.h>
#include <fstream>
#include <iostream>
#include <random>
#include <string>

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
auto random_int() {
	return distribution_int(rng);
};

auto random_int(int max) {
	return std::uniform_int_distribution<>(0, max)(rng);
}

auto random_int(int min, int max) {
	return std::uniform_int_distribution<>(min, max)(rng);
}

//auto distribution_float = std::uniform_real_distribution<>(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max());
auto distribution_float = std::uniform_real_distribution<float>(-10000, 10000);
auto random_float() {
	return distribution_float(rng);
};

auto distribution_graph_char = std::uniform_int_distribution<unsigned short>(0, (unsigned short)graphical_characters.size() - 1);
auto random_char() {
	return graphical_characters[distribution_graph_char(rng)];
};

auto random_string(unsigned size) {
	std::string str;
	str.reserve(size);
	while (size-- > 0)
		str += random_char();
	return str;
};



// test class
struct Object {
	int i;
	float f;
	std::string s;
	std::vector<Object> v;
};

bool operator==(const Object& a, const Object& b)
{
	const float error_margin = abs(std::max(a.f, b.f) / 100000.f);

#ifdef _DEBUG
	auto t1 = a.i == b.i;
	auto t2 = std::abs(a.f - b.f) <= error_margin;
	auto t3 = a.s == b.s;
	auto t = t1 && t2 && t3;
#endif

	return
		a.i == b.i &&
		std::abs(a.f - b.f) <= error_margin &&
		a.s == b.s &&
		a.v == b.v;
}

namespace gs
{
	template<typename Stream> constexpr
	void serialize(Stream& stream, Object& value) {
		using namespace deco;
		using namespace std;

		gs::serializer(stream,
#define DECO_LABELED_OBJECT
#ifdef DECO_LABELED_OBJECT
			make_NVP("i"sv, value.i),
			make_NVP("f"sv, value.f),
			make_NVP("s"sv, value.s),
			make_set("v"sv, value.v));
#else
			value.i,
			value.f,
			value.s,
			make_set(value.v));	// must serialize as a set
#endif
	}
}


Object random_object() {
	Object object;
	object.i = random_int();
	object.f = random_float();
	object.s = random_string(random_int(1, 20));
	return object;
}


Object create_object()
{
	auto object = random_object();
	auto constexpr num_children = 5762;
	object.v.reserve(num_children);
	for (int i = 0; i < num_children; ++i) {
		auto& child = object.v.emplace_back(random_object());
		auto constexpr num_children = 4;
		child.v.reserve(num_children);
		// children with children
		for (int i = 0; i < num_children; ++i) {
			auto& child_child = child.v.emplace_back(random_object());
			auto constexpr num_children = 2;
			child_child.v.reserve(num_children);
			for (int i = 0; i < num_children; ++i) {
				child_child.v.emplace_back(random_object());
				/*
				// deep nesting
				auto child_p = &child_child.v.back();
				for (int i = 0; i < 20; ++i) {
					child_p = &child_p->v.emplace_back(random_object());
				}
				*/
			}
		}
	}
	return object;
}


template<typename Pre, typename F>
auto benchmark(Pre pre, F f, unsigned iterations = 1)
{
	using namespace std::chrono;
	using clock = high_resolution_clock;

	clock::duration total(0);
	time_point<clock> start;

	while(iterations-- > 0) {
		pre();
		start = clock::now();
		f();
		total += clock::now() - start;
	}

	return duration_cast<microseconds>(total).count();
}


int main()
{
	Object object;

	// write file
	{
		deco::OutputStream_indent stream;

		// benchmark serialization
		const auto time = benchmark([&] {
			// generate object hierarchy for serialization
			object = create_object();
			stream = deco::OutputStream_indent();
		}, [&stream, &object]{
			gs::serializer(stream, object);
		});

		std::cout << "serialize: " << time / 1000.f << "ms" << '\n';

		std::ofstream os("out.deco", std::ios::binary);
		os << stream.str;
	}

	// read file
	{
		auto file = std::ifstream("out.deco", std::ios::binary);
		std::string file_str{
			std::istreambuf_iterator<char>(file),
			std::istreambuf_iterator<char>()};

		auto stream = deco::InputStream(file_str.cbegin());

		Object parsed_object;

		// benchmark parsing
		const auto time = benchmark([&] {
			stream = deco::InputStream(file_str.cbegin());
			parsed_object = Object();
		}, [&stream, &parsed_object] {
			gs::serializer(stream, parsed_object);
		});

		std::cout << "parse: " << time / 1000.f << "ms" << '\n';

		assert(object == parsed_object);
	}
	//std::getchar();
}