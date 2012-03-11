#include <iostream>

#include "Benchmark.hpp"

void base(){
    int a = 999;
    for(int i = 0; i < 1000; ++i){
        a = a + i;
    }

    a *= a;
}

void big(){
    int a = 999;
    for(int i = 0; i < 100000; ++i){
        a = a + i;
    }

    a *= a;
}

int main(){
    std::cout << "Launch tests benchmark" << std::endl;

    bench::Benchmark benchmark;
    benchmark.addFunction("Base", base);
    benchmark.addFunction("Big", big);

    benchmark.launchBenchs();
    benchmark.printResults();
}
