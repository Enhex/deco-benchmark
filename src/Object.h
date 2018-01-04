#pragma once

#include <deco/NVP.h>
#include <deco/list.h>
#include <deco/types/vector.h>

#include <cereal/types/vector.hpp>

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
			make_list("v"sv, value.v));
#else
			value.i,
			value.f,
			value.s,
			make_list(value.v));	// must serialize as a list
#endif
#else
#ifdef	DECO_LABELED_OBJECT
		serialize(stream, make_NVP("i"sv, value.i));
		serialize(stream, make_NVP("f"sv, value.f));
		serialize(stream, make_NVP("s"sv, value.s));
		serialize(stream, make_list("v"sv, value.v));
#else
		serialize(stream, value.i);
		serialize(stream, value.f);
		serialize(stream, value.s);
		serialize(stream, make_list(value.v));	// must serialize as a list
#endif
#endif
	}
}

namespace cereal
{
	template<class Archive>
	void serialize(Archive & archive, Object & value)
	{
#define CEREAL_LABELED_OBJECT
#ifdef CEREAL_LABELED_OBJECT
		archive(
			cereal::make_nvp("i", value.i),
			cereal::make_nvp("f", value.f),
			cereal::make_nvp("s", value.s),
			cereal::make_nvp("v", value.v));
#else
		archive(value.i, value.f, value.s, value.v);
#endif
	}
}


Object random_object();

Object create_object();