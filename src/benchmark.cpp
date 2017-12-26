#include "Object.h"
#include "common.h"

#include <deco/NVP.h>
#include <deco/set.h>
#include <deco/types/arithmetic.h>
#include <deco/types/string.h>
#include <deco/types/vector.h>
#include <gs/serializer.h>

#include <cereal/archives/json.hpp>
#include <cereal/archives/xml.hpp>

#include <cassert>
#include <fstream>
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
			deco::serialize(stream, object);
		});

		std::cout << "serialize: " << time / 1000.f << "ms\n";

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

		std::cout << "parse: " << time / 1000.f << "ms\n";

		assert(object == parsed_object);
	}
}

void benchmark_json()
{
	Object object;

	const cereal::JSONOutputArchive::Options archive_options = cereal::JSONOutputArchive::Options(std::numeric_limits<double>::max_digits10, cereal::JSONOutputArchive::Options::IndentChar::tab, 1);
	constexpr auto file_name = "out.json";

	// write file
	{
		std::ostringstream file_str;

		// benchmark serialization
		const auto time = benchmark([&] {
			// generate object hierarchy for serialization
			object = create_object();
		}, [&file_str, &object, &archive_options] {
			cereal::JSONOutputArchive archive(file_str, archive_options);
			cereal::serialize(archive, object);
		});

		std::cout << "serialize: " << time / 1000.f << "ms\n";

		std::ofstream os(file_name, std::ios::binary);
		os << file_str.str();
	}

	// read file
	{
		std::istringstream file_str({
			std::istreambuf_iterator<char>(std::ifstream(file_name, std::ios::binary)),
			std::istreambuf_iterator<char>() });

		Object parsed_object;

		// benchmark parsing
		const auto time = benchmark([&] {
			parsed_object = Object();
		}, [&file_str, &parsed_object] {
			cereal::JSONInputArchive archive(file_str);
			cereal::serialize(archive, parsed_object);
		});

		std::cout << "parse: " << time / 1000.f << "ms\n";

		assert(object == parsed_object);
	}
}

void benchmark_xml()
{
	Object object;

	const cereal::XMLOutputArchive::Options archive_options = cereal::XMLOutputArchive::Options(std::numeric_limits<double>::max_digits10);
	constexpr auto file_name = "out.xml";


	// write file
	{
		std::ostringstream file_str;

		// benchmark serialization
		const auto time = benchmark([&] {
			// generate object hierarchy for serialization
			object = create_object();
		}, [&file_str, &object, &archive_options] {
			cereal::XMLOutputArchive archive(file_str, archive_options);
			cereal::serialize(archive, object);
		});

		std::cout << "serialize: " << time / 1000.f << "ms\n";

		std::ofstream os(file_name, std::ios::binary);
		os << file_str.str();
	}

	// read file
	{
		std::istringstream file_str({
			std::istreambuf_iterator<char>(std::ifstream(file_name, std::ios::binary)),
			std::istreambuf_iterator<char>() });

		Object parsed_object;

		// benchmark parsing
		const auto time = benchmark([&] {
			parsed_object = Object();
		}, [&file_str, &parsed_object] {
			cereal::XMLInputArchive archive(file_str);
			cereal::serialize(archive, parsed_object);
		});

		std::cout << "parse: " << time / 1000.f << "ms\n";

		assert(object == parsed_object);
	}
}

int main()
{
	std::puts("deco");
	benchmark_deco();
	
	std::puts("json");
	benchmark_json();

	std::puts("xml");
	benchmark_xml();

	//std::getchar();	// pause
}