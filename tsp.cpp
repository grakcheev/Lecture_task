#include "tspsolver.h"
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>

int main(int argc, char* argv[]) {
  if (argc < 2) {
    std::cerr << "No file" << std::endl;
    return 1;
  }

  std::srand(static_cast<unsigned>(std::time(nullptr)));

  std::ifstream file(argv[1]);
  if (!file.is_open()) {
    std::cerr << "Can't open file: " << argv[1] << std::endl;
    return 1;
  }


  std::vector<Point> points;
  double x, y;
  while (file >> x >> y)
    points.push_back({x, y});

  std::vector<std::vector<double>> dist = TSPSolver::buildDistanceMatrix(points);
  TSPSolver solver;
  std::vector<int> tour2 = solver.solveTSP(dist, 0);
  double length2 = solver.calculateTourLength(tour2, dist);

  std::vector<int> tour3 = solver.solveTSP(dist, 1);
  double length3 = solver.calculateTourLength(tour3, dist);


  std::cout << "2-opt:" << std::endl;
  std::cout << "TSP tour length: " << length2 << std::endl << "Tour order:";
  for (int city : tour2) std::cout << " " << city;
  std::cout << std::endl;

  
  std::cout << "3-opt:" << std::endl;
  std::cout << "TSP tour length: " << length3 << std::endl << "Tour order:";
  for (int city : tour3) std::cout << " " << city;
  std::cout << std::endl;

  return 0;
}