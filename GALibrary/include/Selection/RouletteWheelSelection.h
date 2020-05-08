#ifndef _GA_ROULETTEWHEEL_H_
#define _GA_ROULETTEWHEEL_H_ "Roulette"

#include "./Selection.h"

namespace EinsGAO
{
	class RouletteWheelSelection : public Selection
	{
	public:
		RouletteWheelSelection() : Selection(_GA_ROULETTEWHEEL_H_)
		{}
		~RouletteWheelSelection()
		{}

	};
}

#endif // !_GA_ROULETTEWHEEL_H_
