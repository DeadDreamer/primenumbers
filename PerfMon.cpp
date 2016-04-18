#include <chrono>

using namespace std::chrono;

class PerfMon
{
public:
    void Start()
    {
        startTime = high_resolution_clock::now();
    }

    void Stop()
    {
        finishTime = high_resolution_clock::now();
    }

    int GetMilliseconds()
    {
        return duration_cast<milliseconds>(finishTime - startTime).count();
    }
private:
    high_resolution_clock::time_point startTime;
    high_resolution_clock::time_point finishTime;
};
