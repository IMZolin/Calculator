#pragma once
#include <string>
#include <functional>
#include <stack>
using realisation = std::function<void(std::stack<double>&)>;
class Operation {
public:
    ~Operation() = default;
    std::string getName() { return name; }
    int getPrior() { return prior; }
    //realisation getFunc(std::stack<double>& stack) { return func(stack); }
    Operation(Operation const& other) : name(other.name), prior(other.prior), isBinary(other.isBinary), isPrefix(other.isPrefix), func(other.func) {}
    Operation(std::string name, int prior, bool isBinary, bool isPrefix, realisation func) : name(name), prior(prior), isBinary(isBinary), isPrefix(isPrefix), func(func) {}
    realisation func;
private:
    std::string name;// + - * / ( ) sin cos pow  ln exp
    int prior;//[0,3]             
    bool isBinary;
    bool isPrefix;
};
