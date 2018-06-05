#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <iterator>
#include <tuple>
#include <future>
#include <sstream>
#include <boost/numeric/ublas/lu.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/io.hpp>
#include "string_tools.hh"
#include "matrix.hh"
#include "iostream_tools.hh"
#include "linear_system.hh"
#include "range.hh"

using Point = std::pair<double, double>;
namespace ublas = boost::numeric::ublas;
auto make_lagrange(double X, std::vector<Point> const &points);
void lagrange(std::vector<Point> const &data);
void splines(std::vector<Point> const &points);
auto make_lagrange(double const X, std::vector<Point> const &points) {
    auto result = 0.0;
    for (auto i = 0u; i < points.size(); ++i) {
        auto[xi, yi] = points[i];
        auto mul = yi;
        for (auto j = 0u; j < points.size(); ++j) {
            if (j == i) { continue; }
            auto xj = points[j].first;
            mul *= ((X - xj) / (xi - xj));
        }
        result += mul;
    }
    return Point(X, result);
}
void lagrange(std::vector<Point> const &data) {
    auto futures = std::vector<std::future<Point>>();
    futures.reserve(static_cast<unsigned long long>(data.back().first + 1.0));
    // find values of subsequent points in input input range, step: 1 meter
    for (auto x = data[0].first; x < data.back().first; x += 1.0/*step*/) {
        futures.push_back(std::async(std::launch::async, make_lagrange, x, data));
    }
    auto interpolatedOutFile = std::ofstream("lagrange_out.txt");
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
    std::for_each(input.begin(), input.end(),
                  [step, &data, i = 0](auto const x) mutable -> void { if (!(i++ % step)) { data.push_back(x); }});
    auto dataOutFile = std::ofstream("data_out.txt");
    for (auto[x, y]:data) { dataOutFile << x << ',' << y << '\n'; }

    // process data
    ::lagrange(data);
    ::splines(data);
    return 0;
}

void splines(std::vector<Point> const &points) {
    // x0 - początek predziału
    // x1 - koniec przedziału

    {
        auto points = std::vector<std::pair<double, double>>{
                {1.0, 6.0},
                {3.0, -2.0},
                {5.0, 4.0}
        };

        auto data_out = std::ofstream("test_data.txt");
        for (auto[x, y] : points) {
            data_out << x << ',' << y << '\n';
        }
        auto N = 1u + 1u              // warunki brzegowe
                 + points.size() - 2  // równość/ciągłość I pochodnych w pktach wewn
                 + points.size() - 2  // równość/ciągłość II pochodnych w pktach wewn
                 + points.size() - 1  // wartości w x0 równe y0
                 + points.size() - 1
                 + 0; // wartości w x1 prz równe y1
        auto A = ublas::matrix<double>(N, N, 0);
        auto B = ublas::vector<double>(N, 0);
        auto X = ublas::vector<double>(N);

        // buduj macierz A i wektor B dla każdego pktu po kolei
        for (auto i = 0u; i < points.size() - 1; i++) {
            auto[x0, y0] = points[i];
            auto[x1, y1] = points[i + 1];
            auto h = x1 - x0;
            // generuj X
            B(4 * i) = y0;
            B(4 * i + 1) = y1;
            // 1. wartość w x0
            A(4 * i + 0, 4 * i + 0) = 1;                // a
            // 2. wartość w x1
            A(4 * i + 1, 4 * i + 0) = 1;                // a
            A(4 * i + 1, 4 * i + 1) = h;                // b
            A(4 * i + 1, 4 * i + 2) = std::pow(h, 2);   // c
            A(4 * i + 1, 4 * i + 3) = std::pow(h, 3);   // d

            if (i >= points.size() - 2) {
            } else {
                // 3. ciągłość I pochodnej w x1
                A(4 * i + 2, 4 * i + 0) = 0;                    // a
                A(4 * i + 2, 4 * i + 1) = 1;                    // b
                A(4 * i + 2, 4 * i + 2) = 2 * h;                // c
                A(4 * i + 2, 4 * i + 3) = 3 * std::pow(h, 2);   // d
                A(4 * i + 2, 4 * i + 5) = -1; // == b1
                // 4. ciągłość II pochodnej w x1
                A(4 * i + 3, 4 * i + 2) = 2;
                A(4 * i + 3, 4 * i + 3) = 6 * h;
                A(4 * i + 3, 4 * i + 6) = -2;
            }
        }
        // Zerowanie na krawędziach
        auto h = points[points.size() - 1].first - points[points.size() - 2].first;
        A(N - 2, 2) = 1;
        A(N - 1, N - 2) = 2;
        A(N - 1, N - 1) = 6 * h;
        for (auto i = 0u; i < A.size1(); ++i) {
            for (auto j = 0u; j < A.size2(); ++j) {
                std::cout << A(i, j) << '\t';
            }
            std::cout << '\n';
        }
        std::cout << B << '\n';
        ublas::permutation_matrix<double> pm(N);
        ublas::lu_factorize(A, pm);
        ublas::lu_substitute(A, pm, B);
        //std::cout << ublas::prod(A1, b) << std::endl;
        std::cout << B;

        auto wspolczynniki = std::vector<std::tuple<double, double, double, double>>();
        for (int i = 0; i < B.size(); i += 4) {
            wspolczynniki.emplace_back(B[i], B[i + 1], B[i + 2], B[i + 3]);
        }
        auto func = [&](auto const x) {
            if (x < 3.0) {
                auto[a, b, c, d] = wspolczynniki[0];
                return a + b * (x-1.0) + c * std::pow(x-1.0, 2) + d * std::pow(x-1.0, 3);
            } else {
                auto[a, b, c, d] = wspolczynniki[1];
                return a + b * (x - 3.0) + c * std::pow(x - 3.0, 2) + d * std::pow(x - 3.0, 3);
            }
        };
        std::cout << '\n';
        auto inter_out = std::ofstream("test_inter.txt");
        for (auto i = 1.0; i <= 5.0; i += 0.01) {
            inter_out << i << ',' << func(i) << '\n';
        }
    }

}

