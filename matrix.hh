//
// Created by kamil on 4/6/18.
//

#ifndef MATRIX_HH
#define MATRIX_HH

#include <vector>
#include <memory>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <typeinfo>
#include <functional>
#include "exceptions.hh"

//____________________________________MATRIX HEADER_____________________________________________
////////////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
class Matrix final {
public:
    template<bool isConstIterator = false>
    class IteratorTemplate;

    using ConstIterator = IteratorTemplate<true>;
    using Iterator = IteratorTemplate<false>;
    Matrix() = default;
    Matrix(size_t rows, size_t cols, const T &initVal = T()) : data_(rows * cols, initVal), rows_(rows), cols_(cols) {}
    Matrix(const std::initializer_list<std::initializer_list<T>> &initList);
    Matrix(const Matrix &other) : data_(other.data_), rows_(other.rows_), cols_(other.cols_) {}
    Matrix(Matrix &&other) noexcept : data_(std::move(other.data_)), rows_(other.rows_), cols_(other.cols_) {}
    virtual ~Matrix() = default;

    auto getRow(size_t row) const;
    auto getCol(size_t col) const;
    void swapRows(size_t row1, size_t row2);
    void swapCols(size_t col1, size_t col2);
    auto rows() const noexcept { return rows_; }
    auto cols() const noexcept { return cols_; }
    auto isSquare() const noexcept { return cols_ == rows_; }

    T &operator()(size_t row, size_t col) { return data_[row * cols_ + col]; }
    T operator()(size_t row, size_t col) const { return data_[row * cols_ + col]; };
    auto operator+(const Matrix &other) const;
    auto operator-(const Matrix &other) const;
    auto operator*(const Matrix &other) const;
    auto operator*(const std::vector<T> &vec) const;
    auto begin() { return Iterator(data_.begin()); }
    auto end() { return Iterator(data_.end()); }
    auto begin() const { return ConstIterator(data_.cbegin()); }
    auto end() const { return ConstIterator(data_.cend()); }

    friend Iterator;
    friend ConstIterator;
    friend auto &operator<<(std::ostream &o, const Matrix &m) {
        o << "Matrix<" << typeid(T).name() << ">[" << m.rows_ << ", " << m.cols_ << "]\n";
        for (int row = 0; row < m.rows_; ++row) {
            for (int col = 0; col < m.cols_; ++col) {
                o << std::setw(6) << m.operator()(row, col) << ", ";
            }
            o << "\n";
        }
        return o;
    }

private:
    std::vector<T> data_;
    size_t rows_{};
    size_t cols_{};
    Matrix(size_t rows, size_t cols, std::vector<T> data) : data_(std::move(data)), rows_(rows), cols_(cols) {};
};




//____________________________________MATRIX  ITERATOR HEADER_____________________________________________
//////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
template<bool isConstIterator>
class Matrix<T>::IteratorTemplate {
    using TRef = typename std::conditional<isConstIterator, const T &, T &>::type;
    using VectorIterator = typename std::conditional<isConstIterator,
            typename std::vector<T>::const_iterator,
            typename std::vector<T>::iterator>::type;
public:
    IteratorTemplate(const IteratorTemplate &other) : vecIterator_(other.vecIterator_) {}
    IteratorTemplate(IteratorTemplate &&other) noexcept : vecIterator_(std::move(other.vecIterator_)) {}

    virtual ~IteratorTemplate() = default;
    TRef operator*() { return *vecIterator_; }

    auto &operator++() {
        auto result = *this;
        ++vecIterator_;
        return result;
    }
    auto &operator--() {
        auto result = *this;
        --vecIterator_;
        return result;
    }
    auto operator++(int) { return ++vecIterator_, *this; }
    auto operator--(int) { return --vecIterator_, *this; }
    auto &operator+=(size_t offset) { return vecIterator_ += offset, *this; }
    auto operator+(size_t offset) const { return IteratorTemplate(vecIterator_ + offset); }
    auto &operator-=(size_t offset) { return vecIterator_ -= offset, *this; }
    auto operator-(size_t offset) const { return IteratorTemplate(vecIterator_ - offset); }
    auto operator==(const IteratorTemplate &other) const { return vecIterator_ == other.vecIterator_; }
    auto operator!=(const IteratorTemplate &other) const { return vecIterator_ != other.vecIterator_; }
    auto operator>(const IteratorTemplate &other) const { return vecIterator_ > other.vecIterator_; }
    auto operator>=(const IteratorTemplate &other) const { return vecIterator_ >= other.vecIterator_; }
    auto operator<(const IteratorTemplate &other) const { return vecIterator_ < other.vecIterator_; }
    auto operator<=(const IteratorTemplate &other) const { return vecIterator_ <= other.vecIterator_; }

    // types required for STL
    using difference_type = long long;
    using value_type = T;
    using pointer = const T *;
    using reference = TRef;
    using iterator_category = std::bidirectional_iterator_tag;
    friend Matrix;
private:
    explicit IteratorTemplate(VectorIterator iter) : vecIterator_(iter) {}
    VectorIterator vecIterator_;
};


//____________________________________________MATRIX IMPLEMENTATION_________________________________________________
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
Matrix<T>::Matrix(const std::initializer_list<std::initializer_list<T>> &initList) {
    cols_ = initList.begin()->size();
    rows_ = initList.size();
    for (auto &l : initList) {
        if (l.size() != cols_) { throw MatrixSizeMismatchException(); }
        data_.insert(data_.end(), l.begin(), l.end());
    }
}

template<typename T>
auto Matrix<T>::getRow(size_t row) const {
    if (row >= this->rows_) { throw OutOfMatrixException(); }
    auto begin = this->data_.begin() + row * this->cols_;
    auto end = begin + this->cols_;
    return std::vector<T>(begin, end);
}

template<typename T>
auto Matrix<T>::getCol(size_t col) const {
    if (col >= this->cols_) { throw OutOfMatrixException(); }
    auto begin = this->data_.begin() + col;
    auto result = std::vector<T>();
    result.reserve(this->rows_);
    for (auto i = col; i < this->rows_ * this->cols_; i += this->cols_) {
        result.push_back(this->data_[i]);
    }
    return result;
}

template<typename T>
void Matrix<T>::swapRows(size_t row1, size_t row2) {
    if (row1 >= this->rows_ || row2 >= this->rows_) { throw OutOfMatrixException(); }
    for (int col = 0; col < this->cols_; ++col) {
        std::swap((*this)(row1, col), (*this)(row2, col));
    }
}

template<typename T>
void Matrix<T>::swapCols(size_t col1, size_t col2) {
    if (col1 >= this->cols_ || col2 >= this->cols_) { throw OutOfMatrixException(); }
    for (int row = 0; row < this->rows_; ++row) {
        std::swap((*this)(row, col1), (*this)(row, col2));
    }
}

template<typename T>
auto Matrix<T>::operator+(const Matrix &other) const {
    if (data_.size() != other.data_.size())
        throw MatrixSizeMismatchException();
    auto result = Matrix<T>(rows_, cols_);
    std::transform(this->data_.begin(), this->data_.end(), other.data_.begin(), result.data_.begin(), std::plus<>());
    return result;
}

template<typename T>
auto Matrix<T>::operator-(const Matrix &other) const {
    if (data_.size() != other.data_.size()) { throw MatrixSizeMismatchException(); }
    auto result = Matrix<T>(rows_, cols_);
    std::transform(this->data_.begin(), this->data_.end(), other.data_.begin(), result.data_.begin(), std::minus<>());
    return result;
}

template<typename T>
auto Matrix<T>::operator*(const Matrix &other) const {
    if (cols_ != other.rows_) { throw MatrixSizeMismatchException(); }
    auto result = Matrix<T>(rows_, cols_, std::vector<T>(data_.size()));
    for (int row = 0; row < rows_; ++row) {
        for (int col = 0; col < cols_; ++col) {
            for (int val = 0; val < cols_; ++val) {
                result(row, col) += (*this)(row, val) * other(val, col);
            }
        }
    }
    return result;
}

template<typename T>
auto Matrix<T>::operator*(const std::vector<T> &vec) const {
    if (rows_ != vec.size()) { throw MatrixSizeMismatchException(); }
    auto result = std::vector<T>(cols_, 0);
    for (int row = 0; row < rows_; ++row) {
        for (int col = 0; col < cols_; ++col) {
            result[row] += vec[col] * (*this)(row, col);
        }
    }
    return result;
}

#endif //MATRIX_HH