#include "knapsacksolver.h"
#include <iostream>
#include <fstream>
#include <ctime>

int main(int argc, char* argv[]) {
  if (argc < 2) {
    std::cerr << "No file" << std::endl;
    return 1;
  }

  std::srand(static_cast<unsigned>(std::time(nullptr)));

  std::ifstream file(argv[1]);
  if (!file.is_open()) {
    std::cerr << "Cannot open file: " << argv[1] << std::endl;
    return 1;
  }

  int N, W;
  file >> N >> W;
  std::vector<Item> items(N);
  for (int i = 0; i < N; i++) {
    file >> items[i].value >> items[i].weight;
  }

  KnapsackSolver solver;
  std::vector<bool> solution = solver.solveKnapsack(items, W, 1);
  int totalValue = solver.calculateKnapsackValue(solution, items);

  std::cout << "Knapsack max value: " << totalValue << std::endl << "Selected items:";
  for (bool taken : solution) std::cout << " " << taken;
  std::cout << std::endl;

  return 0;
}