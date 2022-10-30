#pragma once
#include <string>
#include <functional>
#include <stack>
using realisation = std::function<void(std::stack<double>&)>;
class Operation {
public:
    ~Operation() = default;
    Operation(Operation const& other) : name(other.name), prior(other.prior), isBinary(other.isBinary), isPrefix(other.isPrefix), func(other.func) {}
    Operation(std::string name, int prior, bool isBinary, bool isPrefix, realisation func) : name(name), prior(prior), isBinary(isBinary), isPrefix(isPrefix), func(func) {}
private:
    std::string name;// + - * / ( ) sin cos pow  ln exp
    int prior;//[0,3]             
    bool isBinary;
    bool isPrefix;
    realisation func;
};