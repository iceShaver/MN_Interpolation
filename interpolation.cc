//
// Created by kamil on 05.06.18.
//
#include "interpolation.hh"
#include <fstream>
#include <iterator>
#include <tuple>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/lu.hpp>


auto interpolation::lagrange_x(double x, std::vector<point> const &points) {
    auto result = 0.0;
    for (auto i = 0u; i < points.size(); ++i) {
        auto[xi, yi] = points[i];
        auto mul = yi;
        for (auto j = 0u; j < points.size(); ++j) {
            if (j == i) { continue; }
            auto xj = points[j].first;
            mul *= ((x - xj) / (xi - xj));
        }
        result += mul;
    }
    return result;
}


void interpolation::lagrange(std::vector<point> const &points,
                             std::string const &output_filename,
                             unsigned interpolation_step) {
    auto lagrange_out_file = std::ofstream(output_filename);
    for (auto x = 0u; x < (unsigned) points.back().first; x += interpolation_step) {
        lagrange_out_file << x << ',' << lagrange_x(x, points) << '\n';
    }
}


auto interpolation::build_equations_matrices(const std::vector<point> &points) {
    auto const N = 4 * (points.size() - 1);
    auto A = boost::numeric::ublas::matrix<double>(N, N, 0);
    auto B = boost::numeric::ublas::vector<double>(N, 0);

    for (auto i = 0u;; i++) {
        auto[x0, y0] = points[i];
        auto[x1, y1] = points[i + 1];
        auto h = x1 - x0;
        // generate X
        B(4 * i) = y0;
        B(4 * i + 1) = y1;
        // 1. x0 value
        A(4 * i + 0, 4 * i + 0) = 1;                // a
        // 2. x1 value
        A(4 * i + 1, 4 * i + 0) = 1;                // a
        A(4 * i + 1, 4 * i + 1) = h;                // b
        A(4 * i + 1, 4 * i + 2) = std::pow(h, 2);   // c
        A(4 * i + 1, 4 * i + 3) = std::pow(h, 3);   // d

        if (i >= points.size() - 2) { break; } // check if not edge
        // 3. x1 first derivative continuity
        A(4 * i + 2, 4 * i + 0) = 0;                    // a
        A(4 * i + 2, 4 * i + 1) = 1;                    // b
        A(4 * i + 2, 4 * i + 2) = 2 * h;                // c
        A(4 * i + 2, 4 * i + 3) = 3 * std::pow(h, 2);   // d
        A(4 * i + 2, 4 * i + 5) = -1; // == b1
        // 4. x1 second derivative continuity
        A(4 * i + 3, 4 * i + 2) = 2;
        A(4 * i + 3, 4 * i + 3) = 6 * h;
        A(4 * i + 3, 4 * i + 6) = -2;
    }
    // second derivative = 0 at the beginning and at the end
    auto h = points[points.size() - 1].first - points[points.size() - 2].first;
    A(N - 2, 2) = 1;
    A(N - 1, N - 2) = 2;
    A(N - 1, N - 1) = 6 * h;
    return std::tuple(std::move(A), std::move(B));
}

void interpolation::cubic_spline(std::vector<point> const &points,
                                 std::string const &output_filename,
                                 unsigned interpolation_step) {
    // compute coefficients
    auto[A, B] = build_equations_matrices(points);
    auto coeffs = boost::numeric::ublas::vector<double>(B.size());
    auto P = boost::numeric::ublas::permutation_matrix<double>(B.size());
    boost::numeric::ublas::lu_factorize(A, P);
    boost::numeric::ublas::lu_substitute(A, P, coeffs);

    // lambda which returns interpolated y value in x point
    auto f = [&coeffs, &points](auto const x) {
        auto index = 0u;
        while (points[index + 1].first < x) index++;
        auto x0 = points[index].first;
        return coeffs(4 * index) +                             // a
               coeffs(4 * index + 1) * (x - x0) +              // b
               coeffs(4 * index + 2) * std::pow(x - x0, 2) +   // c
               coeffs(4 * index + 3) * std::pow(x - x0, 3);    // d
    };

    // compute and save results
    auto inter_out = std::ofstream(output_filename);
    auto max = (unsigned)points.back().first;
    for (auto x = 0u; x <= max; x += interpolation_step) {
        inter_out << x << ',' << f(x) << '\n';
    }
}
