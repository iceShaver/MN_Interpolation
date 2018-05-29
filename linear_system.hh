//
// Created by kamil on 3/30/18.
//

#ifndef LINEAR_SYSTEM_H
#define LINEAR_SYSTEM_H

#include <vector>
#include <string>
#include "matrix.hh"
#include <algorithm>
#include <numeric>
#include <tuple>
#include <cmath>

namespace linear_system {

	/**
	* \brief LU Decomposition
	* \param A Matrix to decompose
	* \return L, U decomposed matrices
	*/
	template<typename T>
	std::tuple<Matrix<T>, Matrix<T>> luDecomposition(const Matrix<T> &A) {
		if (!A.isSquare()) { throw NotASquareMatrixException(); }
		auto L = Matrix<T>{ A.rows(), A.cols() };
		auto U = A;
		for (int i = 0; i < A.rows(); ++i) {
			L(i, i) = 1;
			for (int j = i+1; j < A.rows(); ++j) {
				L(j, i) = U(j, i) / U(i,i);
				for (int k = i; k < A.rows(); ++k) {
					U(j,k) = U(j,k) - L(j, i) * U(i, k);
				}
			}
		}
		return std::make_tuple(std::move(L), std::move(U));
	}
	template<typename T>
	std::vector<T> forwardSubstitution(const Matrix<T>& L, const std::vector<T>& b) {
		if (!L.isSquare()) { throw NotASquareMatrixException(); }
		if (L.cols() != b.size()) { throw MatrixSizeMismatchException(); }

		auto n = L.cols();
		auto y = std::vector<T>(n);
		y[0] = b[0];
		for (auto i = 1; i < n; ++i) {
			auto sum = 0.0;
			for (auto j = 0; j < i; ++j)
				sum += L(i, j)*y[j];
			y[i] = 1 / L(i, i) * (b[i] - sum);
		}
		return y;
	}

	template<typename T>
	std::vector<T> backwardSubstitution(const Matrix<T>& U, const std::vector<T>& y) {
		if (!U.isSquare()) { throw NotASquareMatrixException(); }
		if (U.cols() != y.size()) { throw MatrixSizeMismatchException(); }

		auto n = U.cols();
		auto x = std::vector<T>(n);
		x[n - 1] = y[n - 1] / U(n - 1, n - 1);
		for (int i = n - 2; i >= 0; --i) {
			auto sum = 0.0;
			for (int j = n - 1; j > i; --j)
				sum += U(i, j)*x[j];
			x[i] = (y[i] - sum) / U(i, i);
		}
		return x;
	}
	template<typename T>
	std::vector<T> luFactorization(const Matrix<T> &A, const std::vector<T> &b) {
		if (!A.isSquare()) { throw NotASquareMatrixException(); }
		auto[L, U] = luDecomposition(A);
		auto y = forwardSubstitution(L, b);
		auto x = backwardSubstitution(U, y);
		return x;
	}




};

#endif //LINEAR_SYSTEM_H