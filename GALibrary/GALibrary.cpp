#include <iostream>
#include <stdexcept>
#include <stdio.h>
#include <string>
#include <array>
#include <memory>
#include "include/GAOptimizer.h"

std::string exec(const char* cmd) {
        std::array<char, 128> buffer{};
        std::string result;
#if defined(_WIN32)
        std::unique_ptr<FILE, decltype(&_pclose)> pipe(_popen(cmd, "r"), _pclose);
#else
        std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
#endif
        if (!pipe)
                throw std::runtime_error("popen() failed!");

        while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
                result += buffer.data();
        }

        return result;
}

class TravelOptimizer : public EinsGAO::GAOptimizer
{
public:
	TravelOptimizer() {};

	double Loss(TensorI gen) override
	{
		std::string cmd = "\".\\model\\GeneticAlgorithm.exe\"";
		for (int i = 0; i < gen.size(); i++)
		{
			int city = gen[i].value();

			switch (city)
			{
			case 0:
				cmd += " A";
				break;
			case 1:
				cmd += " B";
				break;
			case 2:
				cmd += " C";
				break;
			case 3:
				cmd += " D";
				break;
			case 4:
				cmd += " E";
				break;
			default:
				assert(false);
			}
		}

		std::string a = exec(cmd.c_str());
		int cost = atoi(a.c_str());
		return cost;
	}
};

int main()
{
	TravelOptimizer a;
	a.SetGeneticEnv(5, 0, 4);
	EinsGAO::Output output = a.search(16, 0, 100);
}