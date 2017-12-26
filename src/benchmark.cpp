#include "common.h"
#include "Object.h"
#include <cassert>
#include <deco/NVP.h>
#include <deco/set.h>
#include <deco/types/arithmetic.h>
#include <deco/types/string.h>
#include <deco/types/vector.h>
#include <fstream>
#include <gs/serializer.h>
#include <iostream>


void benchmark_deco()
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

		auto stream = deco::make_InputStream(file_str.cbegin());

		Object parsed_object;

		// benchmark parsing
		const auto time = benchmark([&] {
			stream = deco::make_InputStream(file_str.cbegin());
			parsed_object = Object();
		}, [&stream, &parsed_object] {
			deco::serialize(stream, parsed_object);
		});

		std::cout << "parse: " << time / 1000.f << "ms" << '\n';

		assert(object == parsed_object);
	}
}


int main()
{
	benchmark_deco();
	//std::getchar();	// pause
}