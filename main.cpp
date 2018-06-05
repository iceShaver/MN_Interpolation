#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <iterator>
#include "matrix.hh"
#include "iostream_tools.hh"
#include "linear_system.hh"
#include "range.hh"
#include "interpolation.hh"
#include "string_tools.hh"

using Point = interpolation::Point;


auto loadData(std::string const &fileName, unsigned long step)  {
    auto data = std::vector<Point>();
    auto file = std::ifstream(fileName);
    std::for_each(std::istream_iterator<std::string>(file),
                  std::istream_iterator<std::string>(),
                  [step, i = 0, &data](auto const &line) mutable -> void {
                      if (!(i++ % step)) {
                          auto const slices = split(line, ',');
                          data.emplace_back(std::stod(slices[0]), std::stod(slices[1]));
                      }
                  });
    return data;
}


int main(int argc, char **argv) {
    if (argc != 3) {
        std::cout << "Invalid arguments. Usage:\n program [inputFileName] [step]";
        return -1;
    }
    auto inputFileName = std::string(argv[1]);
    auto step = std::stoul(argv[2]);
    auto data = loadData("data1.txt", step);
    // save loaded data for plot generation
    {
        auto dataOutFile = std::ofstream("data_" + inputFileName);
        for (auto[x, y]:data) { dataOutFile << x << ',' << y << '\n'; }
    }
    // process and save data
    std::cout << "Performing lagrange interpolation..." << std::flush;
    interpolation::lagrange(data, "lagrange_" + inputFileName, 1);
    std::cout << "DONE\nPerforming cubic splines interpolation..." << std::flush;
    interpolation::splines(data, "splines_" + inputFileName, 1);
    std::cout << "DONE" << std::endl;
    return 0;
}


