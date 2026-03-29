#ifndef KNAPSACKSOLVER_H
#define KNAPSACKSOLVER_H

#include "solver.h"
#include <vector>

#define KNAPSACK_MAX_ITERATIONS 1000
#define KNAPSACK_MULTI_START 10

struct Item {
  int weight;
  int value;
};

class KnapsackSolver : public Solver {
public:
  KnapsackSolver() = default;

  std::vector<bool> solveKnapsack(const std::vector<Item>& items, int capacity, int hammingDistance);
  int calculateKnapsackValue(const std::vector<bool>& solution, const std::vector<Item>& items) const;
  int solveBB(std::vector<Item>& items, int capacity, std::vector <bool>& solution);

private:
  int calculateKnapsackWeight(const std::vector<bool>& solution, const std::vector<Item>& items) const;
  void localSearchKnapsack(std::vector<bool>& solution, const std::vector<Item>& items, int capacity, int hammingDistance);

  void BB(int ind, int current_weight, int current_value, std::vector<int>& order, std::vector<Item>& items, int capacity, int& best_value, std::vector<bool>& current_solution, std::vector<bool>& best_solution);
};

#endif