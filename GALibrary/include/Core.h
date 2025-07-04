#ifndef _GA_CORE_H_
#define _GA_CORE_H_

#include <random>
#include <chrono>
#include <set>
#include <tensor.h>
#include <omp.h>
#include <algorithm>
#include <memory>
#include <limits>
#include <cassert>

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
using TensorI = KDTLAB::Tensor<double>;

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
		// 유전자 환경 변수
		int m_GeneticLen;
		int m_GeneticMin;
		int m_GeneticMax;
		bool m_overlap;

		// 탐색 방법
		int m_population;
                std::unique_ptr<Selection> select; // 선택 연산자
                std::unique_ptr<Crossover> crossover; // 교배 연산자
                std::unique_ptr<Mutation> mutation; // 변이 연산자
		TensorI m_gens;

	public:
                GAOptimizer()
                    : select(std::make_unique<RouletteWheelSelection>()),
                      crossover(std::make_unique<SimpleCrossover>()),
                      mutation(std::make_unique<StaticMutation>())
                {}

                virtual ~GAOptimizer() = default;

	public:
		virtual double Loss(TensorI gen) = 0;

                void SetGeneticEnv(sint len, int min, int max, bool overlap = false)
                {
                        m_overlap = overlap;
                        if (!overlap && len > (max - min + 1))
                        {
                                throw std::invalid_argument("Can't generate Genetic this envroment setting.");
                        }

                        m_GeneticLen = len;
                        m_GeneticMin = min;
                        m_GeneticMax = max;
                        m_overlap = overlap;
                }

                Output search(sint population, const double& targetLoss,
                        sint maxGeneration, bool minimizeLoss = true)
                {
                        m_population = population;
                        Initialize_Gens();
			std::cout << "초기 유전자" << std::endl;
			for (auto item : m_gens)
			{
				std::cout << item << std::endl;
			}

                        TensorI bestGen;
                        double bestLoss = minimizeLoss ? std::numeric_limits<double>::infinity()
                                                     : -std::numeric_limits<double>::infinity();

                        for (int geration = 0; geration < maxGeneration; geration++)
                        {
                                // 적합도 계산
                                TensorI losses({ population, 1 });
#pragma omp parallel for
                                for (int i = 0; i < m_gens.size(); i++)
                                {
                                        losses[i] = Loss(m_gens[i]);
                                }

				// 조건 검사
                                for (std::size_t i = 0; i < losses.size(); i++)
                                {
                                        double lossVal = losses[i].value();
                                        if ((minimizeLoss && lossVal < targetLoss) ||
                                            (!minimizeLoss && lossVal > targetLoss))
                                        {
                                                return { lossVal, m_gens[i] };
                                        }

                                        if ((minimizeLoss && lossVal < bestLoss) ||
                                            (!minimizeLoss && lossVal > bestLoss))
                                        {
                                                bestLoss = lossVal;
                                                bestGen = m_gens[i];
                                        }
                                }

				TensorI lossAndGen;
				for (int i = 0; i < population; ++i)
				{
					TensorI row_loss = losses[i];
					TensorI row_gen = m_gens[i];
					for (auto item_gen : row_gen)
					{
						row_loss.append(item_gen.value());
					}
					lossAndGen.append(row_loss);
				}



				std::sort(lossAndGen.begin(), lossAndGen.end(), [](TensorI a, TensorI b) {
					return a[0].value() < b[0].value();
				});

				// 선택
				//select.

				// 교배

				// 돌연변이
                                std::cout << lossAndGen << std::endl;
                        }

                        return { bestLoss, bestGen };
                }

                void SetSelection(std::unique_ptr<Selection> select)
                {
                        this->select = std::move(select);
                }

                void SetCrossover(std::unique_ptr<Crossover> crossover)
                {
                        this->crossover = std::move(crossover);
                }

                void SetMutation(std::unique_ptr<Mutation> mutation)
                {
                        this->mutation = std::move(mutation);
                }

	private:
		void Initialize_Gens()
		{
			// 유전자 풀 경우의 수 조건 검사
			int g_range = m_GeneticMax - m_GeneticMin + 1;
			int sum = Factorial(g_range);
			int mother = Factorial(g_range - m_GeneticLen);
			int numOfCases = sum / mother;

			if (numOfCases < m_population)
			{
				throw std::invalid_argument("Too much population!");
			}

                        std::random_device rn;
                        std::mt19937_64 rnd(rn());
                        std::uniform_int_distribution<int> range(m_GeneticMin, m_GeneticMax);

			m_gens.clear();

			// 유전자 풀 생성 루프
			for (int i = 0; i < m_population; i++)
			{
				// 유전자 생성 루프
				TensorI new_gen;
				for (int j = 0; j < m_GeneticLen; j++)
				{
					int ranNum = range(rnd);

					// 중복허용이 아닐 경우
					if (!m_overlap)
					{
						// new_gen 에서 중복 검사
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
							j--; // 중복이라면 루프 다시 반복 아니면
						else
							new_gen.append(ranNum); // 중복이 아니라면 유전자에 할당
					}
					else
					{
						new_gen.append(ranNum);
					}
				}

				// 유전자 중복 검사
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

