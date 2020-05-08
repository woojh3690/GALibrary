#ifndef _GA_MUTATIN_H_
#define _GA_MUTATIN_H_

#include <string>

namespace EinsGAO
{
	class Mutation
	{
	private:
		std::string m_type;

	public:
		Mutation(std::string name) : m_type(name)
		{}
		virtual ~Mutation()
		{}

	public:
		std::string GetType()
		{
			return m_type;
		}
	};
}
#endif // !_GA_MUTATIN_H_
