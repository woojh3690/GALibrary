#ifndef _GA_SIMPLECROSSOVER_H_
#define _GA_SIMPLECROSSOVER_H_ "Simple"

#include "Crossover.h"

namespace EinsGAO
{
	class SimpleCrossover : public Crossover
	{
	public:
		SimpleCrossover() : Crossover(_GA_SIMPLECROSSOVER_H_)
		{}
		~SimpleCrossover()
		{}
	};
}
#endif // !_GA_SIMPLECROSSOVER_H_
