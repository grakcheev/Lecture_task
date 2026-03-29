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



int findBound(int ind, int current_weight, int capacity, std::vector<int>& order, std::vector<Item>& items){
  int bound = 0;
  int r = capacity - current_weight;
  for(int i = ind; i<order.size(); i++){
    int w = items[order[i]].weight;
    if (w<=r){
      bound += items[order[i]].value;
      r -= w;
    }
    else break;
  }
  return bound;
}


int KnapsackSolver::solveBB(std::vector<Item>& items, int capacity, std::vector <bool>& solution){
  int n = items.size();
  if(n==0){
    solution.clear();
    return 0;
  }


  std::vector<int> order(n);
  for(int i=0; i<n; i++) order[i] = i;
  
  std::sort(order.begin(), order.end(), [&](int a, int b){return items[a].value > items[b].value;});

  int best_val = 0;
  std::vector<bool> cur_solution(n, false);
  solution.assign(n, false);

  BB(0, 0, 0, order, items, capacity, best_val, cur_solution, solution);
  return best_val;
}


void KnapsackSolver::BB(int ind, int current_weight, int current_value, std::vector<int>& order, std::vector<Item>& items, int capacity, int& best_value, std::vector<bool>& current_solution, std::vector<bool>& best_solution){
  if(ind == order.size()){
    if(current_value > best_value){
      best_value = current_value;
      best_solution = current_solution;
    }
    return;
  }

  int bound = current_value + findBound(ind, current_weight, capacity, order, items);
  if(bound <= best_value) return;

  int k = order[ind];
  Item& item = items[k];

  if(current_weight + item.weight <= capacity){
    current_solution[k] = true;
    BB(ind+1, current_weight + item.weight, current_value + item.value, order, items, capacity, best_value, current_solution, best_solution);
    current_solution[k] = false;
  }

  BB(ind+1, current_weight, current_value, order, items, capacity, best_value, current_solution, best_solution);
}

