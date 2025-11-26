#include <iostream>
#include <sstream>


#include <../NamePending-HPC/src/taskmanager.hpp>
#include <../NamePending-HPC/src/timer.hpp>

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
  
  StopWorkers();
}

