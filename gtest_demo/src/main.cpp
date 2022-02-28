# include "common.h"
# include "counter.h"
int main()
{
    Counter co;
    std::cout<<"first call AddOnce:" << co.AddOnce() << endl;
    Calc ca;
    std::cout<<"3+5="<<ca.Add(3, 5)<<std::endl;
    return 0;
}