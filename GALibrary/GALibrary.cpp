#include <iostream>
#include <stdexcept>
#include <stdio.h>
#include <string>
#include "include/GAOptimizer.h"

std::string exec(const char* cmd) {
	char buffer[128];
	std::string result = "";
	FILE* pipe = _popen(cmd, "r");
	if (!pipe) throw std::runtime_error("popen() failed!");
	try {
		while (fgets(buffer, sizeof(buffer), pipe) != NULL) {
			result += buffer;
		}
	}
	catch (...) {
		_pclose(pipe);
		throw;
	}
	_pclose(pipe);
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