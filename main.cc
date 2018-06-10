#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <iterator>
#include <string>
#include <eigen3/Eigen/Dense>
#include "interpolation.hh"
#include "string_tools.hh"


auto load_data(std::string const &file_name, unsigned long points_number) {
    if (points_number < 2) { throw std::runtime_error("points_number must be >= 2"); }
    auto data = std::vector<interpolation::point>();
    auto file = std::ifstream(file_name);
    auto const tmp = std::vector<std::string>(std::istream_iterator<std::string>(file),
                                              std::istream_iterator<std::string>());
    if (points_number > tmp.size()) { points_number = tmp.size(); }
    data.reserve(points_number);
    std::for_each(tmp.begin(), tmp.end(),
                  [step = (tmp.size() - 1) / (points_number - 1), i = 0, &data](auto const &line) mutable {
                      if (i++ % step != 0) { return; }
                      auto const slices = ::split(line, ',');
                      data.emplace_back(std::stod(slices[0]), std::stod(slices[1]));
                  });
    return data;
}


int main(int argc, char **argv) {
    if (argc != 4) {
        std::cout << "Invalid arguments.\nUsage:\nprogram [input_filename] [points_number] [interpolation_step]\n"
                     "Input file should contain values separated by comma\n"
                     "Output files are: lagrange_input_filename, cubic_spline_input_filename\n";
        return -1;
    }
    auto const input_filename = std::string(argv[1]);
    auto const points_number = std::stoul(argv[2]);
    auto const interpolation_step = std::stoul(argv[3]);
    auto const data = ::load_data(input_filename, points_number);
    // save loaded data for plot generation
    {
        auto data_out_file = std::ofstream("points_" + input_filename);
        for (auto const[x, y]:data) { data_out_file << x << ',' << y << '\n'; }
    }
    // process and save data
    std::cout << "Performing lagrange interpolation..." << std::flush;
    interpolation::lagrange(data, "lagrange_" + input_filename, interpolation_step);
    std::cout << "DONE\nPerforming cubic spline interpolation..." << std::flush;
    interpolation::cubic_spline(data, "cubic_spline_" + input_filename, interpolation_step);
    std::cout << "DONE" << std::endl;
    return 0;
}
