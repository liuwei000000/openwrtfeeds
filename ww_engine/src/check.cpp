#include "check.h"

using namespace std;

Check check;
extern "C" void check_init()
{
    check.init_conf();
}


void Check::init_conf()
{
    
}
