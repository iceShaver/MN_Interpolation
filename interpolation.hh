//
// Created by kamil on 05.06.18.
//

#ifndef MN_INTERPOLATION_INTERPOLATION_TOOLS_HH
#define MN_INTERPOLATION_INTERPOLATION_TOOLS_HH

#include <vector>
#include <string>

namespace interpolation {
    using Point = std::pair<double, double>;

    auto makeLangrangeForX(double X, std::vector<Point> const &points);
    auto buildEquationsMatrices(std::vector<Point> const& points);
    void lagrange(std::vector<Point> const &data, std::string const & outputFileName, double interpolationStep = 1);
    void splines(std::vector<Point> const &points, std::string const & outputFileName, double interpolationStep = 1);
}
#endif //MN_INTERPOLATION_INTERPOLATION_TOOLS_HH
