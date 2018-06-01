#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <iterator>
#include <tuple>
#include <future>
#include <sstream>
#include "string_tools.hh"
#include "matrix.hh"
#include "iostream_tools.hh"
#include "linear_system.hh"

using Point = std::pair<double, double>;


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


int main() {
    // read input
    auto input = std::vector<Point>();
    {
        auto file = std::ifstream("data1.txt");
        std::transform(std::istream_iterator<std::string>(file),
                       std::istream_iterator<std::string>(),
                       std::back_inserter(input),
                       [](auto const &line) {
                           auto const slices = split(line, ',');
                           return Point(std::stod(slices[0]), std::stod(slices[1]));
                       });
    }
    // parallel lagrange and save input
    // choose points with specified step
    auto data = std::vector<Point>();
    auto step = 20;
    data.reserve(input.size()/step);
    std::for_each(input.begin(), input.end(), [&, i=0] (auto x) mutable{ if(i%step == 0) {data.push_back(x);} ++i;});
    {
        auto interpolatedOutFile = std::ofstream("inter_out.txt");
        auto dataOutFile = std::ofstream("data_out.txt");
        for(auto[x,y]:data){dataOutFile << x << ',' << y << std::endl;}
        auto futures = std::vector<std::future<Point>>();
        futures.reserve(static_cast<unsigned long long>(data.back().first + 1));
        // find values of subsequent points in input input range, step: 1 meter
        auto step = 1.0;
        for (auto x = data[0].first; x < data.back().first; x += step) {
            futures.push_back(std::async(std::launch::async, make_lagrange, x, data));
        }
        for (auto &future : futures) {
            auto[x, y] = future.get();
            interpolatedOutFile << x << ',' << y << std::endl;
        }

    }
    return 0;
}
