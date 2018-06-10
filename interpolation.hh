//
// Created by kamil on 05.06.18.
//

#ifndef MN_INTERPOLATION_INTERPOLATION_TOOLS_HH
#define MN_INTERPOLATION_INTERPOLATION_TOOLS_HH

#include <vector>
#include <string>
#include <tuple>

namespace interpolation {
    using point = std::pair<double, double>;

    auto lagrange_x(double x, std::vector<point> const &points);

    auto build_equations_matrices(std::vector<point> const &points);

    void lagrange(std::vector<point> const &points,
                  std::string const &output_filename,
                  unsigned long interpolation_step = 1u);

    void cubic_spline(std::vector<point> const &points,
                      std::string const &output_filename,
                      unsigned long interpolation_step = 1u);

}
#endif //MN_INTERPOLATION_INTERPOLATION_TOOLS_HH
