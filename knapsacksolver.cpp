#include "knapsacksolver.h"
#include <algorithm>

std::vector<bool> KnapsackSolver::solveKnapsack(const std::vector<Item>& items, int capacity, int hammingDistance){
  int n = items.size();
  std::vector<bool> bestSolution;
  int bestValue = -1;

  for (int start = 0; start < KNAPSACK_MULTI_START; start++) {
    std::vector<bool> solution(n, false);
    int totalWeight = 0;
    std::vector<int> indices(n);
    for (int i = 0; i < n; i++) indices[i] = i;
    std::random_shuffle(indices.begin(), indices.end());
    for (int i : indices) {
      if (totalWeight + items[i].weight <= capacity) {
        solution[i] = true;
        totalWeight += items[i].weight;
      }
    }

    localSearchKnapsack(solution, items, capacity, hammingDistance);

    int currentValue = calculateKnapsackValue(solution, items);
    if (currentValue > bestValue) {
      bestValue = currentValue;
      bestSolution = solution;
    }
  }
  return bestSolution;
}

int KnapsackSolver::calculateKnapsackValue(const std::vector<bool>& solution, const std::vector<Item>& items) const {
  int value = 0;
  for (std::size_t i = 0; i < solution.size(); i++)
    if (solution[i]) value += items[i].value;
  return value;
}

int KnapsackSolver::calculateKnapsackWeight(const std::vector<bool>& solution, const std::vector<Item>& items) const{
  int weight = 0;
  for (std::size_t i = 0; i < solution.size(); i++)
    if (solution[i]) weight += items[i].weight;
  return weight;
}

void KnapsackSolver::localSearchKnapsack(std::vector<bool>& solution, const std::vector<Item>& items, int capacity, int hammingDistance){
  int n = items.size();
  bool improved = true;
  int iteration = 0;
  while (improved && iteration < KNAPSACK_MAX_ITERATIONS) {
    improved = false;
    std::vector<bool> bestNeighbor = solution;
    int bestValue = calculateKnapsackValue(solution, items);

    if (hammingDistance == 1) {
      for (int i = 0; i < n; i++) {
        std::vector<bool> neighbor = solution;
        neighbor[i] = !neighbor[i];
        if (calculateKnapsackWeight(neighbor, items) <= capacity){
          int value = calculateKnapsackValue(neighbor, items);
          if (value > bestValue) {
            bestValue = value;
            bestNeighbor = neighbor;
            improved = true;
          }
        }
      }
    } else if (hammingDistance == 2){
      for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++){
          std::vector<bool> neighbor = solution;
          neighbor[i] = !neighbor[i];
          neighbor[j] = !neighbor[j];
          if (calculateKnapsackWeight(neighbor, items) <= capacity){
            int value = calculateKnapsackValue(neighbor, items);
            if (value > bestValue) {
              bestValue = value;
              bestNeighbor = neighbor;
              improved = true;
            }
          }
        }
      }
    }

    if(improved) solution = bestNeighbor;
    iteration++;
  }
}
