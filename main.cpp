#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <iterator>
#include <tuple>
#include <future>
#include <sstream>
#include <boost/numeric/ublas/lu.hpp>
#include "string_tools.hh"
#include "matrix.hh"
#include "iostream_tools.hh"
#include "linear_system.hh"
#include "range.hh"

using Point = std::pair<double, double>;
namespace ublas = boost::numeric::ublas;
auto make_lagrange(double X, std::vector<Point> const &points);
void lagrange(std::vector<Point> const &data);
void splines(std::vector<Point> const &vector);
auto make_lagrange(double const X, std::vector<Point> const &points) {
    auto result = 0.0;
    for (auto i = 0u; i < points.size(); ++i) {
        auto[xi, yi] = points[i];
        auto mul = 1.0;
        for (auto j = 0u; j < points.size(); ++j) {
            if (j == i) { continue; }
            auto xj = points[j].first;
            mul *= ((X - xj) / (xi - xj));
        }
        result += yi * mul;
    }
    return Point(X, result);
}
void lagrange(std::vector<Point> const &data) {
    auto futures = std::vector<std::future<Point>>();
    futures.reserve(static_cast<unsigned long long>(data.back().first + 1.0));
    // find values of subsequent points in input input range, step: 1 meter
    for (auto x = data[0].first; x < data.back().first; x += 1.0/*step*/) {
        futures.push_back(async(std::launch::async, make_lagrange, x, data));
    }
    auto interpolatedOutFile = std::ofstream("inter_out.txt");
    for (auto &future : futures) {
        auto[x, y] = future.get();
        interpolatedOutFile << x << ',' << y << '\n';
    }
}

int main() {
    // read input
    auto input = std::vector<Point>();
    auto file = std::ifstream("data1.txt");
    std::transform(std::istream_iterator<std::string>(file),
                   std::istream_iterator<std::string>(),
                   std::back_inserter(input),
                   [](auto const &line) {
                       auto const slices = split(line, ',');
                       return Point(std::stod(slices[0]), std::stod(slices[1]));
                   });
    // choose points with specified step
    auto data = std::vector<Point>();
    auto step = 20;
    data.reserve(input.size() / step);
    std::for_each(input.begin(), input.end(), [step, &data, i = 0](auto const x) mutable {
        if (!(i++ % step)) { data.push_back(x); }
    });
    auto dataOutFile = std::ofstream("data_out.txt");
    for (auto[x, y]:data) { dataOutFile << x << ',' << y << '\n'; }
    ::lagrange(data);
    ::splines(data);
    return 0;
}
void splines(std::vector<Point> const &vector) {
    for (auto x : Range(0.0, 10.0, 1.0)) {
        std::cout << x << '\n';
    }
}

