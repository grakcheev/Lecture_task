#include "knapsacksolver.h"
#include <iostream>
#include <fstream>
#include <chrono>

int main(int argc, char* argv[]) {
  if (argc < 2) {
    std::cerr << "No file" << std::endl;
    return 1;
  }

  std::srand(static_cast<unsigned>(std::chrono::system_clock::now().time_since_epoch().count()));

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

  std::cout << "Hamming distances:" << std::endl;
  KnapsackSolver solver;
  std::chrono::high_resolution_clock::time_point start1 = std::chrono::high_resolution_clock::now();
  std::vector<bool> solution1 = solver.solveKnapsack(items, W, 1);
  int value1 = solver.calculateKnapsackValue(solution1, items);
  std::chrono::high_resolution_clock::time_point end1 = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double, std::milli> time1 = end1 - start1;
  
  std::cout << "Time (ms): " << time1.count() << std::endl;
  std::cout << "Knapsack max value: " << value1 << std::endl << "Selected items:";
  for (bool taken : solution1) std::cout << " " << taken;
  std::cout << std::endl;



  std::cout << "Branches&Bounds:" << std::endl;
  std::chrono::high_resolution_clock::time_point start2 = std::chrono::high_resolution_clock::now();
  std::vector<bool> solution2;
  int value2 = solver.solveBB(items, W, solution2);
  std::chrono::high_resolution_clock::time_point end2 = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double, std::milli> time2 = end2 - start2;
  
  std::cout << "Time (ms): " << time2.count() << std::endl;
  std::cout << "Knapsack max value: " << value2 << std::endl << "Selected items:";
  for (bool taken : solution2) std::cout << " " << taken;
  std::cout << std::endl;



  std::cout << "Greed algorithm:" << std::endl;
  std::chrono::high_resolution_clock::time_point start3 = std::chrono::high_resolution_clock::now();
  std::vector<bool> solution3;
  solver.greed(items, W, solution3);
  std::chrono::high_resolution_clock::time_point end3 = std::chrono::high_resolution_clock::now();
  int value3 = solver.calculateKnapsackValue(solution3, items);
  std::chrono::duration<double, std::milli> time3 = end3 - start3;
  
  std::cout << "Time (ms): " << time3.count() << std::endl;
  std::cout << "Knapsack max value: " << value3 << std::endl << "Selected items:";
  for (bool taken : solution3) std::cout << " " << taken;
  std::cout << std::endl;




  return 0;
}