#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <iterator>
#include <tuple>
#include "string_tools.hh"
#include "matrix.hh"
#include "iostream_tools.hh"
#include "linear_system.hh"

int main() {

    // read data
    auto data = std::vector<std::tuple<double, double>>();
    auto input = std::vector<std::string>();
    {
        auto file = std::ifstream("data1.txt");
        if (file.fail()) {
            std::cout << "Reading file failed" << std::endl;
            return -1;
        }
        std::copy(std::istream_iterator<std::string>(file),
                  std::istream_iterator<std::string>(),
                  std::back_inserter(input));
    }
    for (auto &line : input) {
        auto slices = split(line, ',');
        data.emplace_back(std::stod(slices[0]), std::stod(slices[1]));
    }
//    auto A = Matrix<double>{
//            {4, -1, -0.2, 2},
//            {-1, 5, 0, -2},
//            {0.2, 1, 10, -1},
//            {0, -2, -1, 4}
//    };
//    auto b = std::vector<double>{30, 0, -10, 5};
//    std::cout << A * linear_system::luFactorization(A, b)<< std::endl;
    auto X = 3.5;
    for (auto[x, y] : data) {

    }
    return 0;
}
