#ifndef Util_UnorderedMapPair_h__
#define Util_UnorderedMapPair_h__

#include <boost/functional/hash.hpp>

namespace std
{
	template<typename S, typename T> struct hash<pair<S, T>>
	{
		inline size_t operator()(const pair<S, T> & v) const
		{
			size_t seed = 0;
			boost::hash_combine(seed, v.first);
			boost::hash_combine(seed, v.second);
			return seed;
		}
	};
}

#endif // Util_UnorderedMapPair_h__