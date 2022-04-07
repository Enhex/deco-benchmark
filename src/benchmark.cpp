#include "Object.h"
#include "common.h"

#include <cereal/archives/json.hpp>
#include <cereal/archives/xml.hpp>

#include <cassert>
#include <fstream>
#include <iostream>


constexpr unsigned benchmark_iterations = 10;


struct BenchmarkResult {
	size_t file_size = 0;
	long long output_time = 0, input_time = 0;
};


BenchmarkResult benchmark_deco()
{
	BenchmarkResult result;

	seed_rng(0);

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
		}, benchmark_iterations);

		result.output_time = time;

		std::ofstream os("out.deco", std::ios::binary);
		os << stream.str;
	}

	// read file
	{
		auto file = std::ifstream("out.deco", std::ios::binary);
		std::string file_str{
			std::istreambuf_iterator<char>(file),
			std::istreambuf_iterator<char>()};

		result.file_size = file_str.size();

		auto stream = deco::make_InputStream(file_str.cbegin());

		Object parsed_object;

		// benchmark parsing
		const auto time = benchmark([&] {
			stream = deco::make_InputStream(file_str.cbegin());
			parsed_object = Object();
		}, [&stream, &parsed_object] {
			deco::serialize(stream, parsed_object);
		}, benchmark_iterations);

		result.input_time = time;

		assert(object == parsed_object);
	}

	return result;
}

BenchmarkResult benchmark_json()
{
	BenchmarkResult result;

	seed_rng(0);

	Object object;

	const cereal::JSONOutputArchive::Options archive_options = cereal::JSONOutputArchive::Options(std::numeric_limits<double>::max_digits10, cereal::JSONOutputArchive::Options::IndentChar::tab, 1);
	constexpr auto file_name = "out.json";

	// write file
	{
		std::ostringstream file_str;

		// benchmark serialization
		const auto time = benchmark([&] {
			object = create_object();	// generate object hierarchy for serialization
			file_str = std::ostringstream();
		}, [&file_str, &object, &archive_options] {
			cereal::JSONOutputArchive archive(file_str, archive_options);
			cereal::serialize(archive, object);
		}, benchmark_iterations);

		result.output_time = time;

		std::ofstream os(file_name, std::ios::binary);
		os << file_str.str();

		result.file_size = file_str.str().size();
	}

	// read file
	{
		std::istringstream file_str;
		Object parsed_object;

		// benchmark parsing
		const auto time = benchmark([&] {
			parsed_object = Object();
			std::ifstream is(file_name, std::ios::binary);
			file_str = std::istringstream({
				std::istreambuf_iterator<char>(is),
				std::istreambuf_iterator<char>()});
		}, [&file_str, &parsed_object] {
			cereal::JSONInputArchive archive(file_str);
			cereal::serialize(archive, parsed_object);
		}, benchmark_iterations);

		result.input_time = time;

		assert(object == parsed_object);
	}

	return result;
}

BenchmarkResult benchmark_xml()
{
	BenchmarkResult result;

	seed_rng(0);

	Object object;

	const cereal::XMLOutputArchive::Options archive_options = cereal::XMLOutputArchive::Options(std::numeric_limits<double>::max_digits10);
	constexpr auto file_name = "out.xml";


	// write file
	{
		std::ostringstream file_str;

		// benchmark serialization
		const auto time = benchmark([&] {
			object = create_object();	// generate object hierarchy for serialization
			file_str = std::ostringstream();
		}, [&file_str, &object, &archive_options] {
			cereal::XMLOutputArchive archive(file_str, archive_options);
			cereal::serialize(archive, object);
		}, benchmark_iterations);

		result.output_time = time;

		std::ofstream os(file_name, std::ios::binary);
		os << file_str.str();

		result.file_size = file_str.str().size();
	}

	// read file
	{
		std::istringstream file_str;
		Object parsed_object;

		// benchmark parsing
		const auto time = benchmark([&] {
			parsed_object = Object();
			std::ifstream is(file_name, std::ios::binary);
			file_str = std::istringstream({
				std::istreambuf_iterator<char>(is),
				std::istreambuf_iterator<char>()});
		}, [&file_str, &parsed_object] {
			cereal::XMLInputArchive archive(file_str);
			cereal::serialize(archive, parsed_object);
		}, benchmark_iterations);

		result.input_time = time;

		assert(object == parsed_object);
	}

	return result;
}


void print_result(const BenchmarkResult& result)
{
	std::cout
		<< "file size: " << result.file_size << " bytes\n"
		<< "serialize: " << result.output_time / 1000.f << "ms\n"
		<< "parse: " << result.input_time / 1000.f << "ms\n"
		;
}

auto file_size_diff(size_t a, size_t b)
{
	return 1 - a / float(b);
}

int main()
{
	std::puts("Deco");
	const auto deco_result = benchmark_deco();
	print_result(deco_result);

	std::puts("\nJSON");
	const auto json_result = benchmark_json();
	print_result(json_result);

	std::puts("\nXML");
	const auto xml_result = benchmark_xml();
	print_result(xml_result);

	std::cout
		<< "\n"
		<< "Deco is:\n"
		<< file_size_diff(deco_result.file_size, json_result.file_size) * 100.f	<< "% smaller than JSON\n"
		<< json_result.output_time / float(deco_result.output_time) * 100.f		<< "% faster output than JSON\n"
		<< json_result.input_time / float(deco_result.input_time) * 100.f		<< "% faster input than JSON\n"
		<< file_size_diff(deco_result.file_size, xml_result.file_size) * 100.f	<< "% smaller than XML\n"
		<< xml_result.output_time / float(deco_result.output_time) * 100.f		<< "% faster output than XML\n"
		<< xml_result.input_time / float(deco_result.input_time) * 100.f		<< "% faster input than XML\n"
		;

	//std::getchar();	// pause
}