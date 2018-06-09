#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <iterator>
#include <string>
#include <tuple>
#include "interpolation.hh"
#include "string_tools.hh"

// loads data with ommiting n values
auto load_data(std::string const &fileName, unsigned long step) {
    auto data = std::vector<interpolation::point>();
    auto file = std::ifstream(fileName);
    std::for_each(std::istream_iterator<std::string>(file),
                  std::istream_iterator<std::string>(),
                  [step, i = 0, &data](auto const &line) mutable -> void {
                      if (!(i++ % step)) {
                          auto const slices = ::split(line, ',');
                          data.emplace_back(std::stod(slices[0]), std::stod(slices[1]));
                      }
                  });
    return data;
}


int main(int argc, char **argv) {
    if (argc != 3) {
        std::cout << "Invalid arguments. Usage:\n program [input_filename] [step]\n"
                     "Input file should contain values separated by comma"
                     "Output files are: lagrange_input_filename, cubic_spline_input_filename\n";
        return -1;
    }
    auto input_filename = std::string(argv[1]);
    auto step = std::stoul(argv[2]);
    auto data = ::load_data("data1.txt", step);
    // save loaded data for plot generation
    {
        auto data_out_file = std::ofstream("data_" + input_filename);
        for (auto[x, y]:data) { data_out_file << x << ',' << y << '\n'; }
    }
    // process and save data
    std::cout << "Performing lagrange interpolation..." << std::flush;
    interpolation::lagrange(data, "lagrange_" + input_filename, 1);
    std::cout << "DONE\nPerforming cubic spline interpolation..." << std::flush;
    interpolation::cubic_spline(data, "cubic_spline_" + input_filename, 1);
    std::cout << "DONE" << std::endl;
    return 0;
}
