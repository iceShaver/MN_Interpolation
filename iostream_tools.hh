//
// Created by kamil on 3/30/18.
//

#ifndef IOSTREAM_TOOLS_H
#define IOSTREAM_TOOLS_H

#include <iostream>
#include <vector>
#include <chrono>


template<typename T>
std::ostream &operator<<(std::ostream &o, const std::vector<T> &v) {
    o << "vector<" << typeid(T).name() << ">[" << v.size() << "]\n";
    for (auto &val : v) o << val << '\n';
    return o << "\n";
}



#endif //IOSTREAM_TOOLS_H