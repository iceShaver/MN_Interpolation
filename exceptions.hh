//
// Created by kamil on 4/6/18.
//

#ifndef EXCEPTIONS_HH
#define EXCEPTIONS_HH

#include <exception>

class Exception : std::exception {};
class MatrixSizeMismatchException : Exception{};
class NotASquareMatrixException : Exception{};
class OutOfMatrixException : Exception{};
class InvalidRangeException : Exception{};
#endif //EXCEPTIONS_HH