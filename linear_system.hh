//
// Created by kamil on 3/30/18.
//

#ifndef MN_SYSTEMSOFLINEAREQUATIONS_LINEAR_SYSTEM_H
#define MN_SYSTEMSOFLINEAREQUATIONS_LINEAR_SYSTEM_H

#include <vector>
#include <string>
#include "matrix.hh"
#include <algorithm>
#include <numeric>
#include <tuple>


namespace linear_system {
	using std::tuple;
	using std::vector;
	using std::string;
	using std::make_tuple;
	using std::move;
	using std::max;
	using std::min;
	using std::abs;

	unsigned getDigit(unsigned number, unsigned index);

	unsigned getN(unsigned index_no);

	Matrix<double> genA(size_t n, double a1, double a2, double a3);

	vector<double> genB(unsigned n, size_t f);

	template <typename T>
	T euclideanNorm(const vector<T>& vec) {
		auto result = 0.0;
		for (auto &x : vec) result += x * x;
		return std::sqrt(result);
	}

	template<typename T>
	tuple<vector<T>, unsigned> solveJacobi(const Matrix<T> &A, vector<T> &b, double stopNorm) {
		if (!A.isSquare()) { throw NotASquareMatrixException(); }
		if (A.cols() != b.size()) { throw MatrixSizeMismatchException(); }

		auto iters = 1u;
		auto N = A.cols();
		auto x = vector<T>(N, 1);
		while (true) {
			auto newX = vector<T>(N);
			for (int row = 0; row < A.rows(); ++row) {
				auto sum = 0.0;
				for (int col = 0; col < A.cols(); ++col)
					if (col != row)
						sum += A(row, col) * x[col];
				newX[row] = (b[row] - sum) / A(row, row);
			}
			x = move(newX);
			if (euclideanNorm(A * x - b) < stopNorm)
				break;
			++iters;
		}
		return make_tuple(move(x), iters);
	}

	template<typename T>
	tuple<vector<T>, unsigned> solveGaussSeidel(const Matrix<T> &A, const vector<T> &b, double stopNorm) {
		if (!A.isSquare()) { throw NotASquareMatrixException(); }
		if (A.cols() != b.size()) { throw MatrixSizeMismatchException(); }

		auto N = A.cols();
		auto x = vector<T>(N, 1);
		auto iters = 0u;
		while (++iters, true) {
			for (int row = 0; row < A.rows(); ++row) {
				auto sum = 0.0;
				for (int col = 0; col < A.cols(); ++col)
					if (col != row)	sum += A(row, col) * x[col];
				x[row] = (b[row] - sum) / A(row, row);
			}
			if (euclideanNorm(A * x - b) < stopNorm)
				break;
		}
		return make_tuple(move(x), iters);
	}


	/**
	* \brief LU Decomposition using Doolittle algorithm
	* \param A Matrix to decompose
	* \return L, U decomposed matrices
	*/
	template<typename T>
	tuple<Matrix<T>, Matrix<T>> luDecomposition(const Matrix<T> &A) {
		if (!A.isSquare()) { throw NotASquareMatrixException(); }

		auto L = Matrix<T>{ A.rows(), A.cols() };
		for (int i = 0; i < A.rows(); ++i) {
			L(i, i) = 1;
		}

		auto U = Matrix<T>{ A.rows(), A.cols() };
		for (int i = 0; i < A.rows(); ++i) {
			for (int j = i; j < A.cols(); ++j) {
				auto sum = 0.0;
				for (int k = 0; k < i; ++k) {
					sum += L(i, k) * U(k, j);
				}
				U(i, j) = A(i, j) - sum;

			}
			for (int j = i + 1; j < A.cols(); ++j) {
				auto sum = 0.0;
				for (int k = 0; k < i; ++k) {
					sum += L(j, k) * U(k, i);
				}
				L(j, i) = (1 / U(i, i)) * (A(j, i) - sum);
			}

		}
		return make_tuple(move(L), move(U));
	}

	template<typename T>
	vector<T> luFactorization(const Matrix<T> &A, const vector<T> &b) {
		if (!A.isSquare()) { throw NotASquareMatrixException(); }

		auto[L, U] = luDecomposition(A);
		auto y = forwardSubstitution(L, b);
		auto x = backwardSubstitution(U, y);
		return x;
	}

	template<typename T>
	vector<T> forwardSubstitution(const Matrix<T>& L, const vector<T>& b) {
		if (!L.isSquare()) { throw NotASquareMatrixException(); }
		if (L.cols() != b.size()) { throw MatrixSizeMismatchException(); }

		auto n = L.cols();
		auto y = vector<T>(n);
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
	vector<T> backwardSubstitution(const Matrix<T>& U, const vector<T>& y) {
		if (!U.isSquare()) { throw NotASquareMatrixException(); }
		if (U.cols() != y.size()) { throw MatrixSizeMismatchException(); }

		auto n = U.cols();
		auto x = vector<T>(n);
		x[n - 1] = y[n - 1] / U(n - 1, n - 1);
		for (int i = n - 2; i >= 0; --i) {
			auto sum = 0.0;
			for (int j = n - 1; j > i; --j)
				sum += U(i, j)*x[j];
			x[i] = (y[i] - sum) / U(i, i);
		}
		return x;
	}


};

#endif //MN_SYSTEMSOFLINEAREQUATIONS_LINEAR_SYSTEM_H