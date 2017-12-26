#pragma once

#include <string>
#include <chrono>

void seed_rng(unsigned seed);

int random_int();

int random_int(int max);

int random_int(int min, int max);

float random_float();

char random_char();

std::string random_string(unsigned size);


template<typename Pre, typename F>
auto benchmark(Pre pre, F f, unsigned iterations = 1)
{
	using namespace std::chrono;
	using clock = high_resolution_clock;

	clock::duration total(0);
	time_point<clock> start;

	while (iterations-- > 0) {
		pre();
		start = clock::now();
		f();
		total += clock::now() - start;
	}

	return duration_cast<microseconds>(total).count();
}