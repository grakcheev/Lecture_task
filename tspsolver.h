#ifndef TSPSOLVER_H
#define TSPSOLVER_H

#include "solver.h"
#include <vector>

#define TSP_MAX_ITERATIONS 1000
#define TSP_MULTI_START 10

struct Point {
  double x, y;
};

class TSPSolver : public Solver {
public:
  TSPSolver() = default;
  std::vector<int> solveTSP(const std::vector<std::vector<double>>& distances, int method); //0 - 2-opt.  1 - 3-opt
  double calculateTourLength(const std::vector<int>& tour, const std::vector<std::vector<double>>& dist) const;
  static std::vector<std::vector<double>> buildDistanceMatrix(const std::vector<Point>& points);

private:
  void twoOpt(std::vector<int>& tour, const std::vector<std::vector<double>>& dist);
  void threeOpt(std::vector<int>& tour, const std::vector<std::vector<double>>& dist);\
};

#endif