#ifndef _GA_STATICMUTATION_H_
#define _GA_STATICMUTATION_H_ "Static"

#include "./Mutation.h"

namespace EinsGAO
{
	class StaticMutation : public Mutation
	{
	public:
		StaticMutation() : Mutation(_GA_STATICMUTATION_H_)
		{}
		~StaticMutation() override
		{}
	};
}
#endif // !_GA_STATICMUTATION_H_