#ifndef CHECK_H
#define CHECK_H
#include <iostream>
#include <set>
#include <string>

using namespace std;
class Check {
    public:
        Check() {};
        ~Check() {};
        void init_conf();

    private:
        set<string>     subs;
        set<string>     fulls;
};

#endif
