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


