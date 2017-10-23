// библиотеки STL

#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <future>
#include <numeric>

// наши библиотеки

#include "PerfMon.cpp"

inline bool isPrime(const int& num)
{
    if (num <= 3)
    {
        return true;
    }

    for (int i = 2; i <= std::sqrt(num) + 1; i++)
    {
        if ((num % i) == 0)
        {
            return false;
        }
    }

    return true;
}

int Method1(const int& min, const int& max)
{
    int num = 0;

    for (int i = min; i <= max; i++)
    {
        if (isPrime(i))
        {
            num++;
        }
    }

    return num;
}

int Method2(const int& min, const int& max)
{
    auto f0 = std::async(std::launch::async, Method1, min, min + max / 4);
    auto f1 = std::async(std::launch::async, Method1, min + max / 4, min + max / 2);
    auto f2 = std::async(std::launch::async, Method1, min + max / 2, min + max * 3 / 4);
    auto f3 = std::async(std::launch::async, Method1, min + max * 3 / 4, max);

    return f0.get() + f1.get() + f2.get() + f3.get();
}

void PrintResult(const std::string& methodName, const std::vector<int>& performanceMeasurements, const int& result)
{
    std::cout << methodName << " primes count: " << result << " - time " <<
            (std::accumulate(performanceMeasurements.begin(), performanceMeasurements.end(), 0.0)) / performanceMeasurements.size()
            << " milliseconds" << std::endl;
}

int main()
{
    int max = 3000000, result = 0, count = 5;
    std::vector<int> performanceMeasurements;

    PerfMon mon;

    for (int i = 0; i < count; i++)
    {
        mon.Start();
        result = Method1(0, max);
        mon.Stop();
        performanceMeasurements.push_back(mon.GetMilliseconds());
    }

    PrintResult("Method 1 - single threaded - ", performanceMeasurements, result);
    performanceMeasurements.clear();

    for (int i = 0; i < count; i++)
    {
        mon.Start();
        result = Method2(0, max);
        mon.Stop();
        performanceMeasurements.push_back(mon.GetMilliseconds());
    }

    PrintResult("Method 2 - async with launch::async - ", performanceMeasurements, result);
    performanceMeasurements.clear();

    return 0;
}
