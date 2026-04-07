export module matrix:trait;
import std;
using std::size_t;

export constexpr bool All()
{
	return true;
}

export template<typename... Args>
constexpr bool All(bool b, Args... args)
{
	return b && All(args...);
}

struct substitution_failure{};

template<typename T>
struct substitution_succeeded: std::true_type {};

template<>
struct substitution_succeeded<substitution_failure>: std::false_type{};

template<typename 

