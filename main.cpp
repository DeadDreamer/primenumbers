// библиотеки STL
#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <future>

// наши библиотеки
#include "PerfMon.cpp"

using namespace std;

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


void PrintResult(const string& methodName, const vector<int>& performanceMeasurements, const int& result)
{
    cout << methodName << " primes count: " << result << " - time " <<
            (std::accumulate(performanceMeasurements.begin(), performanceMeasurements.end(), 0.0)) / performanceMeasurements.size()
            << " milliseconds" << endl;
}

int main()
{
    int max = 3000000, result = 0, count = 2;
    vector<int> performanceMeasurements;

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


    return 0;
}
