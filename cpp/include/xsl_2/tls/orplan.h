#ifndef ORPLAN_H
#define ORPLAN_H
//Overall planning
#include<iostream>
namespace tls
{
	template<typename Accessor, typename Func>
	void foreach(Accessor begin, Accessor end, Func f)
	{
		std::cout << "fe_do(deliverer&& start, deliverer&& finish, func f)\n";
		for (; begin != end; ++begin)
			f(*begin);
	}
}
#endif//ORPLAN_H