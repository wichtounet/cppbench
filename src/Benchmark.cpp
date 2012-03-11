#include <iostream>
#include <sstream>

#include <sys/time.h>

#include "Benchmark.hpp"

bench::Function::Function(const std::string& name, std::function<void()> function) : name(name), function(function) {}

bench::Benchmark::Benchmark(){
    //
}

void bench::Benchmark::addFunction(const std::string& name, std::function<void()> function){
   functions.push_back({name, function}); 
}

void bench::Benchmark::launchBenchs(){
    for(auto& function : functions){
        bench(function);
    }
}

void bench::Benchmark::bench(bench::Function& function){
    unsigned long total = 0;

    for(int i = 0; i < 10000; ++i){
        timespec ts1 = {0,0};
        timespec ts2 = {0,0};

        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &ts1);
        function.function();
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &ts2);

        unsigned long duration = (ts2.tv_sec - ts1.tv_sec) * 1000000000l + (ts2.tv_nsec - ts1.tv_nsec);

        total += duration;
    }

    function.results.total = total;
}

std::string withCorrectSuffix(unsigned long duration){
    std::stringstream stream;

    if(duration > 1000000000L){
        double subduration = duration / 1000000000.0;

        stream << subduration << "s";
    } else if(duration > 1000000L){
        double subduration = duration / 1000000.0;

        stream << subduration << "ms";
    } else if(duration > 1000L){
        double subduration = duration / 1000.0;

        stream << subduration << "us";
    } else {
        stream << duration << "ns";
    }

    return stream.str();
}

void bench::Benchmark::printResults(){
    for(auto& function : functions){
        std::cout << "Function : " << function.name << " : Total=" << withCorrectSuffix(function.results.total) << std::endl;
    }
}
