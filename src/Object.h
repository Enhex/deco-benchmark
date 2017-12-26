#pragma once

#include <string>
#include <vector>

// test class
struct Object {
	int i;
	float f;
	std::string s;
	std::vector<Object> v;
};

bool operator==(const Object& a, const Object& b);

namespace deco
{
	template<typename Stream> constexpr
	void serialize(Stream& stream, Object& value)
	{
		using namespace std::literals;

		//#define	DECO_USE_SERIALIZER
#define	DECO_LABELED_OBJECT

#ifdef	DECO_USE_SERIALIZER
		gs::serializer(stream,
#ifdef	DECO_LABELED_OBJECT
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
#else
#ifdef	DECO_LABELED_OBJECT
		serialize(stream, make_NVP("i"sv, value.i));
		serialize(stream, make_NVP("f"sv, value.f));
		serialize(stream, make_NVP("s"sv, value.s));
		serialize(stream, make_set("v"sv, value.v));
#else
		serialize(stream, value.i);
		serialize(stream, value.f);
		serialize(stream, value.s);
		serialize(stream, make_set(value.v));	// must serialize as a set
#endif
#endif
	}
}


Object random_object();

Object create_object();