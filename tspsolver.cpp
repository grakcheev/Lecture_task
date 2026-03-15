#include "tspsolver.h"
#include <algorithm>
#include <cmath>
#include <vector>

std::vector<int> TSPSolver::solveTSP(const std::vector<std::vector<double>>& distances, int method) {
  if (distances.empty()) return {};
  int n = distances.size();
  std::vector<int> bestTour;
  double bestLength = 1e18;

  for (int start = 0; start < TSP_MULTI_START; start++) {
    std::vector<int> tour(n);
    for (int i = 0; i < n; i++) tour[i] = i;
    std::random_shuffle(tour.begin(), tour.end());

    if (method == 0)
      twoOpt(tour, distances);
    else
      threeOpt(tour, distances);

    double length = calculateTourLength(tour, distances);
    if (length < bestLength) {
      bestLength = length;
      bestTour = std::move(tour);
    }
  }
  return bestTour;
}

double TSPSolver::calculateTourLength(const std::vector<int>& tour, const std::vector<std::vector<double>>& dist) const{
  double length = 0.0;
  int n = tour.size();
  for (int i = 0; i < n - 1; i++)
    length += dist[tour[i]][tour[i + 1]];
  length += dist[tour.back()][tour[0]];
  return length;
}

std::vector<std::vector<double>> TSPSolver::buildDistanceMatrix(const std::vector<Point>& points) {
  int n = points.size();
  std::vector<std::vector<double>> dist(n, std::vector<double>(n));
  for (int i = 0; i < n; i++)
    for (int j = 0; j < n; j++) {
      double dx = points[i].x - points[j].x;
      double dy = points[i].y - points[j].y;
      dist[i][j] = std::sqrt(dx*dx + dy*dy);
    }
  return dist;
}

void TSPSolver::twoOpt(std::vector<int>& tour, const std::vector<std::vector<double>>& dist) {
  int n = tour.size();
  bool improved = true;
  int iteration = 0;
  while (improved && iteration < TSP_MAX_ITERATIONS) {
    improved = false;
    for (int i = 0; i < n - 2; i++) {
      for (int j = i + 2; j < n; j++) {
        int a = tour[i], b = tour[(i + 1) % n];
        int c = tour[j], d = tour[(j + 1) % n];
        double current = dist[a][b] + dist[c][d];
        double reversed = dist[a][c] + dist[b][d];
        if (reversed < current - 1e-9) {
          std::reverse(tour.begin() + i + 1, tour.begin() + j + 1);
          improved = true;
        }
      }
    }
    ++iteration;
  }
}




// funcs for 3opt:
std::vector<int> concatenate(const std::vector<int>& a, const std::vector<int>& b,
                             const std::vector<int>& c, const std::vector<int>& d) {
  std::vector<int> result;
  result.reserve(a.size() + b.size() + c.size() + d.size());
  result.insert(result.end(), a.begin(), a.end());
  result.insert(result.end(), b.begin(), b.end());
  result.insert(result.end(), c.begin(), c.end());
  result.insert(result.end(), d.begin(), d.end());
  return result;
}


std::vector<int> reversed(const std::vector<int>& v) {
  std::vector<int> r = v;
  std::reverse(r.begin(), r.end());
  return r;
}


void TSPSolver::threeOpt(std::vector<int>& tour, const std::vector<std::vector<double>>& dist) {
  int n = tour.size();
  bool improved = true;
  int iteration = 0;
  const double eps = 1e-9;

  while (improved && iteration < TSP_MAX_ITERATIONS) {
    improved = false;
    for (int i = 0; i < n - 3; i++) {
      for (int j = i + 2; j < n - 1; j++){
        for (int k = j + 2; k < n; k++){
          std::vector<int> seg1(tour.begin(), tour.begin() + i + 1);
          std::vector<int> seg2(tour.begin() + i + 1, tour.begin() + j + 1);
          std::vector<int> seg3(tour.begin() + j + 1, tour.begin() + k + 1);
          std::vector<int> seg4(tour.begin() + k + 1, tour.end());

          std::vector<std::vector<int>> candidates;
          candidates.reserve(8);
          candidates.push_back(concatenate(seg1, seg2, seg3, seg4));
          candidates.push_back(concatenate(seg1, seg2, reversed(seg3), seg4));
          candidates.push_back(concatenate(seg1, reversed(seg2), seg3, seg4));
          candidates.push_back(concatenate(seg1, seg2, seg3, reversed(seg4)));
          candidates.push_back(concatenate(seg1, reversed(seg3), seg2, seg4));
          candidates.push_back(concatenate(seg1, reversed(seg3), reversed(seg2), seg4));
          candidates.push_back(concatenate(seg1, seg2, reversed(seg4), seg3));
          candidates.push_back(concatenate(seg1, reversed(seg2), reversed(seg4), seg3));

          double curLen = calculateTourLength(tour, dist);
          int bestIdx = -1;
          double bestLen = curLen;

          for (int idx = 0; idx < candidates.size(); idx++) {
            double len = calculateTourLength(candidates[idx], dist);
            if (len < bestLen - eps) {
              bestLen = len;
              bestIdx = idx;
            }
          }

          if (bestIdx != -1) {
            tour = candidates[bestIdx];
            improved = true;
            break;
          }
        }
        if (improved) break;
      }
      if (improved) break;
    }
    iteration++;
  }
}