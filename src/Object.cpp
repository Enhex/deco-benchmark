#include "Object.h"

#include "common.h"
#include <algorithm>

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