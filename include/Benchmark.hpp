#include <functional>
#include <vector>
#include <string>

namespace bench {

struct Results {
    unsigned long total;
};

struct Function {
    std::string name;
    std::function<void()> function;
    Results results;

    Function(const std::string& name, std::function<void()> function);
};

class Benchmark {
    public:
        Benchmark();

        void addFunction(const std::string& name, std::function<void()> function);

        void launchBenchs();
        void printResults();

    private:
        std::vector<Function> functions;

        void bench(Function& function);
};

} //end of bench
