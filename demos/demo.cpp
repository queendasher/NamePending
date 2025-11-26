#include <iostream>
#include <sstream>

#include <../NamePending-HPC/src/taskmanager.hpp>
#include <../NamePending-HPC/src/timer.hpp>
#include <../NamePending-HPC/src/lock_guard.hpp>

using namespace ASC_HPC;
using std::cout, std::endl;


int main()
{
    timeline = std::make_unique<TimeLine>("demo.trace");

    StartWorkers(3);
    
    RunParallel(10, [] (int i, int size)
    {
        static Timer t("timer one");
        RegionTimer reg(t);
        cout << "I am task " << i << " out of " << size << endl;
    });

    int cnt = 0;
    RunParallel(100'000, [&cnt] (int i, int size) { cnt++; });
    cout << "Total count is " << cnt << endl;

    cnt = 0;
    std::atomic<bool> mutex = false;
    RunParallel(100'000, [&cnt, &mutex] (int i, int size)
    {
        ASC_HPC::LockGuard lock(mutex);
        cnt++;
    });
    cout << "Total count with LockGuard is " << cnt << endl;
    
    StopWorkers();
}

