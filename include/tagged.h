#pragma once

#include <utility>
#include <type_traits>

namespace tagged
{
	template <template <typename> typename T, typename S>
	struct is_template : std::false_type { };
	template <template <typename> typename T, typename S>
	struct is_template<T, S&> : is_template<T, S> { };
	template <template <typename> typename T, typename S>
	struct is_template<T, T<S>> : std::true_type { };

	template <template <typename> typename T, typename S>
	inline constexpr bool is_template_v = is_template<T, S>::value;

	template <template <typename> typename Tag, typename DV>
	auto get(DV&& default_value) { return std::forward<DV>(default_value); }

	template <template <typename> typename Tag, typename DV, typename First, typename... Rest>
	auto get(DV&& default_value, First&& first, Rest&&... rest)
	{
		if constexpr (is_template_v<Tag, First>)
			return *first;
		else
			return get<Tag>(std::forward<DV>(default_value), std::forward<Rest>(rest)...);
	}
}

#define DEFINE_TAG(tag)\
template <typename T>\
class tag\
{\
	template <typename S>\
	friend class tag;\
	T _value;\
public:\
	tag() = default;\
	explicit tag(T value) : _value(static_cast<T&&>(value)) { }\
	template <typename S>\
	tag(tag<S> other) : _value{std::forward<S>(other._value)} { }\
	\
	tag& operator=(std::remove_reference_t<T> value) { _value = { std::move(value) }; return *this; }\
	template <typename S>\
	tag& operator=(tag<S> other) { _value = { std::forward<S>(other._value) }; return *this; }\
	\
	T&& operator*() && { return _value; }\
	T& operator*() & { return _value; }\
	const T& operator*() const& { return _value; }\
};\
template <class T> tag(T&&) -> tag<T>
