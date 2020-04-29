#ifndef _GA_CORE_H_
#define _GA_CORE_H_

#include <random>
#include <chrono>
#include <set>
#include <tensor.h>
#include <omp.h>

#include "./Selection/Selection.h"
#include "./Crossover/Crossover.h"
#include "./Mutation/Mutation.h"

typedef const int& sint;
typedef KDTLAB::Tensor<int> TensorI;

namespace EinsGAO
{
	class GAOptimizer
	{
	private:
		// 유전자 환경 변수
		int m_GeneticLen;
		int m_GeneticMin;
		int m_GeneticMax;

		// 탐색 방법
		unsigned int m_population;
		Selection* select = nullptr; // 선택 연산자
		Crossover* crossover = nullptr; // 교배 연산자
		Mutation* mutation = nullptr; // 변이 연산자
		TensorI gens;

	public:
		GAOptimizer() {}
		virtual ~GAOptimizer() 
		{
			if (select != nullptr)
			{
				delete select;
			}

			if (crossover != nullptr)
			{
				delete crossover;
			}

			if (mutation != nullptr)
			{
				delete mutation;
			}
		}

	public:
		virtual double Loss(TensorI gen) = 0;

		void SetGeneticEnv(sint len, int min, int max)
		{
			m_GeneticLen = len;
			m_GeneticMin = min;
			m_GeneticMax = max;
		}

		void search(sint population, sint maxGeneration, bool minimizeLoss = true)
		{
			m_population = population;

			gens = TensorI({ population, m_GeneticLen });
			gens.randomInitInt(m_GeneticMin, m_GeneticMax);

			vector<double> losses;
			losses.resize(m_population);

#pragma omp parallel for
			for (int i = 0; i < gens.size(); i++)
			{
				losses[i] = Loss(gens[i]);
			}
		}

		void SetSelection(Selection* select)
		{
			this->select = select;
		}

		void SetCrossover(Crossover* crossover)
		{
			this->crossover = crossover;
		}

		void SetMutation(Mutation* mutation)
		{
			this->mutation = mutation;
		}
	};
}
#endif // !_GA_CORE_H_

