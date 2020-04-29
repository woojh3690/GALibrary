#ifndef _GA_CROSSOVER_H_
#define _GA_CROSSOVER_H_

#include <string>
namespace EinsGAO
{
	class Crossover
	{
	private:
		std::string m_type;

	public:
		Crossover(std::string type) : m_type(type)
		{}
		virtual ~Crossover() = 0;

	public:
		std::string GetType()
		{
			return m_type;
		}


	};
}
#endif // !_GA_CROSSOVER_H_
