#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <iterator>
#include <tuple>
#include "string_tools.hh"
#include "matrix.hh"
#include "iostream_tools.hh"

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
	std::cout << input.size() << std::endl;
	auto mat = Matrix<int>{
		{5,4,8,6},
		{8,5,6,6},
		{5,6,8,7},
		{5,6,2,8},
		{5,8,9,3},
		{5,8,2,3}
	};
	auto mat1 = Matrix<int>{
			{5,4,8,6},
			{8,5,6,6},
			{5,6,8,7},
			{5,6,2,8},
			{5,8,9,3},
			{5,8,2,3}
	};
	std::cout << mat + mat1 << std::endl;
	std::cout << mat << std::endl;
	mat.swapCols(0,1);
	std::cout << mat << std::endl;
    for(auto&x : mat){
        std::cout << x << std::endl;
    }
    std::cout << mat.cols() << std::endl;
    std::cout << mat.rows() << std::endl;
    std::cout << mat.isSquare();
	// process 
	system("pause");
	return 0;
}
