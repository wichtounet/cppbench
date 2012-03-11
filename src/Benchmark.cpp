#include <iostream>
#include <sstream>
#include <climits>

#include <sys/time.h>

#include "Benchmark.hpp"

bench::Function::Function(const std::string& name, std::function<void()> function) : name(name), function(function) {}

bench::Benchmark::Benchmark() : bench_duration(10) {
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

void bench::Benchmark::setBenchDuration(unsigned int seconds){
    bench_duration = seconds;
}

unsigned long getOperations(bench::Function& function, unsigned int bench_duration){
    unsigned long total = 0;

    //TODO Find a way to make that faster
    unsigned long operations = 50000;
    
    for(unsigned int i = 0; i < operations; ++i){
        timespec ts1 = {0,0};
        timespec ts2 = {0,0};

        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &ts1);
        function.function();
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &ts2);

        unsigned long duration = (ts2.tv_sec - ts1.tv_sec) * 1000000000l + (ts2.tv_nsec - ts1.tv_nsec);

        total += duration;
    }
    
    unsigned long avg = total / operations;

    return (bench_duration * 1000000000L) / avg;
}

std::string withCorrectUnit(unsigned long duration){
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

void bench::Benchmark::bench(bench::Function& function){
    unsigned long total = 0;
    unsigned long min = LONG_MAX;
    unsigned long max = 0;

    unsigned long operations = getOperations(function, bench_duration);

    for(unsigned int i = 0; i < operations; ++i){
        timespec ts1 = {0,0};
        timespec ts2 = {0,0};

        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &ts1);
        function.function();
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &ts2);

        unsigned long duration = (ts2.tv_sec - ts1.tv_sec) * 1000000000l + (ts2.tv_nsec - ts1.tv_nsec);

        min = std::min(min, duration);
        max = std::max(max, duration);

        total += duration;
    }

    unsigned long avg = total / operations;

    function.results.total = total;
    function.results.avg = avg;
    function.results.min = min;
    function.results.max = max;
}

void bench::Benchmark::printResults(){
    std::cout << "Each benchmark lasted about " << bench_duration << "s" << std::endl;

    for(auto& function : functions){
        std::cout << "Function : " << function.name << " : " 
            << "avg=" << withCorrectUnit(function.results.avg) 
            << ", max=" << withCorrectUnit(function.results.max) 
            << ", min=" << withCorrectUnit(function.results.min) 
            << std::endl;
    }
}
