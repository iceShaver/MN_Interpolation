//
// Created by kamil on 02.06.18.
//

#ifndef MN_INTERPOLATION_RANGE_HH
#define MN_INTERPOLATION_RANGE_HH

#include "exceptions.hh"

enum class IteratorType {
    BEGIN, END
};

template<typename T>
class Range final {
public:
    class Iterator;

    Range(T number) : min(0), max(number), step(number > 0 ? 1 : -1) {}
    Range(T from, T to) : min(from), max(to), step(max - min > 0 ? 1 : -1) {}
    Range(T from, T to, T step) : min(from), max(to), step(step) {
        if ((max-min>0 && step <= 0) || (max-min < 0 && step >= 0)) { throw InvalidRangeException(); }
    }
    auto begin() { return Iterator(*this, IteratorType::BEGIN); };
    auto end() { return Iterator(*this, IteratorType::END); };
    friend Iterator;
private:
    T min, max, step;
};

template<typename T>
class Range<T>::Iterator final {
public:
    auto operator*() { return val; }
    auto operator==(Iterator const &other) {
        return end && end == other.end ? true : end != other.end ? false : val == other.val;
    }
    bool operator!=(Iterator const &other) { return !((*this) == (other)); }
    auto operator++() {
        if (range.step > 0) {
            if ((val += range.step) >= range.max) {
                end = true;
            }
        } else {
            if ((val += range.step) <= range.max) {
                end = true;
            }
        }
    }
    friend Range;
private:
    Iterator(Range const &range, IteratorType iteratorType) : range(range) {
        switch (iteratorType) {
            case IteratorType::BEGIN:
                val = range.min;
                break;
            case IteratorType::END:
                end = true;
                break;
        }
    }
    bool end;
    Range const &range;
    T val;
};

#endif //MN_INTERPOLATION_RANGE_HH
