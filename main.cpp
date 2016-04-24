#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <future>

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

int Method2(const int& min, const int& max)
{
    auto f0 = async(launch::async, Method1, min, min + max / 4);
    auto f1 = async(launch::async, Method1, min + max / 4, min + max / 2);
    auto f2 = async(launch::async, Method1, min + max / 2, min + max * 3 / 4);
    auto f3 = async(launch::async, Method1, min + max * 3 / 4, max);

    return f0.get() + f1.get() + f2.get() + f3.get();
}

int Method3(const int& min, const int& max)
{
    using task_type = int(const int&, const int&);

    packaged_task<task_type> pt0{Method1};
    packaged_task<task_type> pt1{Method1};
    packaged_task<task_type> pt2{Method1};
    packaged_task<task_type> pt3{Method1};

    future<int> f0{pt0.get_future()};
    future<int> f1{pt1.get_future()};
    future<int> f2{pt2.get_future()};
    future<int> f3{pt3.get_future()};

    thread t0{move(pt0), min, min + max / 4};
    thread t1{move(pt1), min + max / 4, min + max / 2};
    thread t2{move(pt2), min + max / 2, min + max * 3 / 4};
    thread t3{move(pt3), min + max * 3 / 4, max};

    t0.detach();
    t1.detach();
    t2.detach();
    t3.detach();

    return f0.get() + f1.get() + f2.get() + f3.get();
}

void PrintResult(const string& methodName, const vector<int>& performanceMeasurements, const int& result)
{
    cout << methodName << " primes count: " << result << " - time " <<
            (std::accumulate(performanceMeasurements.begin(), performanceMeasurements.end(), 0.0)) / performanceMeasurements.size()
            << " milliseconds" << endl;
}

int main()
{
    int max = 3000000, result = 0, count = 5;
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

    for (int i = 0; i < count; i++)
    {
        mon.Start();
        result = Method2(0, max);
        mon.Stop();
        performanceMeasurements.push_back(mon.GetMilliseconds());
    }

    PrintResult("Method 2 - async with launch::async - ", performanceMeasurements, result);
    performanceMeasurements.clear();

    for (int i = 0; i < count; i++)
    {
        mon.Start();
        result = Method3(0, max);
        mon.Stop();
        performanceMeasurements.push_back(mon.GetMilliseconds());
    }

    PrintResult("Method 3 - multithreaded, packaged tasks and futures - ", performanceMeasurements, result);
    performanceMeasurements.clear();

    return 0;
}
