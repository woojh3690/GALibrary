#ifndef _GA_SELECTION_H_
#define _GA_SELECTION_H_

#include <string>

namespace EinsGAO
{
	class Selection
	{
	private:
		std::string m_type;

	public:
		Selection(std::string type) : m_type(type)
		{}
		virtual ~Selection()
		{}

	public:
		std::string GetType()
		{
			return m_type;
		}
	};
}

#endif // !_GA_SELECTION_H_
