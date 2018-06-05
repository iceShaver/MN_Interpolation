//
// Created by kamil on 05.06.18.
//
#include "interpolation.hh"
#include <future>
#include <fstream>
#include <iterator>
#include "linear_system.hh"
#include "string_tools.hh"


auto interpolation::makeLangrangeForX(double X, std::vector<Point> const &points) {
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
void
interpolation::lagrange(std::vector<Point> const &data, std::string const &outputFileName, double interpolationStep) {
    if (interpolationStep <= 0) { throw InvalidArgumentException(); }
    auto futures = std::vector<std::future<Point>>();
    futures.reserve(static_cast<unsigned long long>(data.back().first + 1.0));
    // find values of subsequent points in input input range, step: 1 meter
    for (auto x = data[0].first; x < data.back().first; x += interpolationStep) {
        futures.push_back(std::async(std::launch::async, makeLangrangeForX, x, data));
    }
    auto interpolatedOutFile = std::ofstream(outputFileName);
    for (auto &future : futures) {
        auto[x, y] = future.get();
        interpolatedOutFile << x << ',' << y << '\n';
    }
}
auto interpolation::buildEquationsMatrices(const std::vector<Point> &points) {
    // matrix size
    auto N = 1u + 1u              // edge conditionals
             + points.size() - 2  // inner points first derivative continuity
             + points.size() - 2  // inner points second derivative continuity
             + points.size() - 1  // f(x0) = y0
             + points.size() - 1; // f(x1) = y1
    auto A = Matrix<double>(N, N, 0);
    auto B = std::vector<double>(N, 0);

    // build matrix A i vector B for each point
    for (auto i = 0u; i < points.size() - 1; i++) {
        auto[x0, y0] = points[i];
        auto[x1, y1] = points[i + 1];
        auto h = x1 - x0;
        // generuj X
        B[4 * i] = y0;
        B[4 * i + 1] = y1;
        // 1. wartość w x0
        A(4 * i + 0, 4 * i + 0) = 1;                // a
        // 2. wartość w x1
        A(4 * i + 1, 4 * i + 0) = 1;                // a
        A(4 * i + 1, 4 * i + 1) = h;                // b
        A(4 * i + 1, 4 * i + 2) = std::pow(h, 2);   // c
        A(4 * i + 1, 4 * i + 3) = std::pow(h, 3);   // d

        if (i >= points.size() - 2) { continue; } // check if not edge
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
    // second derivative = 0 at the beginning and end
    auto h = points[points.size() - 1].first - points[points.size() - 2].first;
    A(N - 2, 2) = 1;
    A(N - 1, N - 2) = 2;
    A(N - 1, N - 1) = 6 * h;
    return std::tuple(std::move(A), std::move(B));
}

void
interpolation::splines(std::vector<Point> const &points, std::string const &outputFileName, double interpolationStep) {
    if (interpolationStep <= 0) { throw InvalidArgumentException(); }

    auto[A, B] = buildEquationsMatrices(points);
    // compute coefficients
    auto coefficients = std::vector<std::tuple<double, double, double, double>>();
    {
        auto X = linear_system::luFactorization(A, B);
        coefficients.reserve(X.size() / 4 + 1);
        for (auto i = 0u; i < X.size(); i += 4) {
            coefficients.emplace_back(X[i], X[i + 1], X[i + 2], X[i + 3]);
        }
    }

    // function which returns interpolated y value in x point
    auto f = [&](auto const x) {
        auto index = std::max(
                (int) (std::find_if(points.begin(), points.end(), [&x](auto const elem) { return elem.first >= x; })
                       - points.begin() - 1), 0);
        auto x0 = points[index].first;
        auto[a, b, c, d] = coefficients[index];
        auto result = a + b * (x - x0) + c * std::pow(x - x0, 2) + d * std::pow(x - x0, 3);
        return result;
    };

    // compute and save results
    auto inter_out = std::ofstream(outputFileName);
    for (auto x = points.front().first; x <= points.back().first; x += interpolationStep) {
        inter_out << x << ',' << f(x) << '\n' << std::flush;
    }
}
