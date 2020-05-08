#ifndef _GA_CORE_H_
#define _GA_CORE_H_

#include <random>
#include <chrono>
#include <set>
#include <tensor.h>
#include <omp.h>
#include <algorithm>

// Selection
#include "./Selection/Selection.h"
#include "./Selection/RouletteWheelSelection.h"

// Crossover
#include "./Crossover/Crossover.h"
#include "./Crossover/SimpleCrossover.h"

// Mutation
#include "./Mutation/Mutation.h"
#include "./Mutation/StaticMutation.h"

using sint = const int&;
using TensorI = KDTLAB::Tensor<int>;

namespace EinsGAO
{
	struct Output
	{
		double loss;
		TensorI param;
	};

	class GAOptimizer
	{
	private:
		// ������ ȯ�� ����
		int m_GeneticLen;
		int m_GeneticMin;
		int m_GeneticMax;
		bool m_overlap;

		// Ž�� ���
		int m_population;
		Selection* select = nullptr; // ���� ������
		Crossover* crossover = nullptr; // ���� ������
		Mutation* mutation = nullptr; // ���� ������
		TensorI m_gens;

	public:
		GAOptimizer() 
		{
			select = new RouletteWheelSelection();
			crossover = new SimpleCrossover();
			mutation = new StaticMutation();
		}

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

		void SetGeneticEnv(sint len, int min, int max, bool overlap = false)
		{
			// �ߺ� ����� �ȵɶ� ������ ����� �� ���
			if (!overlap && len > (max - min + 1))
			{
				throw std::invalid_argument("Can't generate Genetic this envroment setting.");
			}

			m_GeneticLen = len;
			m_GeneticMin = min;
			m_GeneticMax = max;
			m_overlap = false;
		}

		Output search(sint population, const double& targetLoss,
			sint maxGeneration, bool minimizeLoss = true)
		{
			m_population = population;
			Initialize_Gens();
			std::cout << "�ʱ� ������" << std::endl;
			for (auto item : m_gens)
			{
				std::cout << item << std::endl;
			}

			for (int geration = 0; geration < maxGeneration; geration++)
			{
				// ���յ� ���
				vector<double> losses;
				losses.resize(m_population);
#pragma omp parallel for
				for (int i = 0; i < m_gens.size(); i++)
				{
					losses[i] = Loss(m_gens[i]);
				}

				// ���� �˻�
				for (std::size_t i = 0; i < losses.size(); i++)
				{
					if ((losses[i] < targetLoss) == minimizeLoss)
					{
						return { losses[i], m_gens[i] };
					}
				}

				// ����
				vector<double> sorted_losses(losses.begin(), losses.end());
				std::sort(sorted_losses.begin(), sorted_losses.end());

				vector<vector<int>> test = { {1,2}, {0, 4}, {3,4} };
				std::sort(m_gens.begin(), m_gens.end(), [](const TensorI& a, const TensorI& b) {
					return a[0].value() > b[1].value();
				});
				// ����
				//select.

				// ����

				// ��������
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

	private:
		void Initialize_Gens()
		{
			// ������ Ǯ ����� �� ���� �˻�
			int g_range = m_GeneticMax - m_GeneticMin + 1;
			int sum = Factorial(g_range);
			int mother = Factorial(g_range - m_GeneticLen);
			int numOfCases = sum / mother;

			if (numOfCases < m_population)
			{
				throw std::invalid_argument("Too much population!");
			}

			random_device rn;
			mt19937_64 rnd(rn());
			uniform_int_distribution<int> range(m_GeneticMin, m_GeneticMax);

			m_gens.clear();

			// ������ Ǯ ���� ����
			for (int i = 0; i < m_population; i++)
			{
				// ������ ���� ����
				TensorI new_gen;
				for (int j = 0; j < m_GeneticLen; j++)
				{
					int ranNum = range(rnd);

					// �ߺ������ �ƴ� ���
					if (!m_overlap)
					{
						// new_gen ���� �ߺ� �˻�
						bool overlap = false;
						for (auto item : new_gen)
						{
							if (item.value() == ranNum)
							{
								overlap = true;
								break;
							}
						}

						if (overlap)
							j--; // �ߺ��̶�� ���� �ٽ� �ݺ� �ƴϸ�
						else
							new_gen.append(ranNum); // �ߺ��� �ƴ϶�� �����ڿ� �Ҵ�
					}
					else
					{
						new_gen.append(ranNum);
					}
				}

				// ������ �ߺ� �˻�
				bool overlap = false;
				for (auto gen : m_gens)
				{
					if (gen == new_gen)
					{
						overlap = true;
						i--;
						break;
					}
				}

				if (!overlap)
				{
					m_gens.append(new_gen);
				}
			}
		}

		int Factorial(int num)
		{
			int factorial = 1;
			for (int i = 1; i <= num; i++)
				factorial *= i;
			return factorial;
		}
	};
}
#endif // !_GA_CORE_H_

