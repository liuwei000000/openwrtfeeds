#ifndef CHECK_H
#define CHECK_H
#include <set>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

typedef struct {
    const char * ptr;
    size_t len;
} ptr_string;

class Check {
    public:
        Check() {
            host.ptr = NULL;
            content_type.ptr = NULL;
            accept.ptr = NULL;
            user_agent.ptr = NULL;
            refer.ptr = NULL;
            user_client.ptr = NULL;
            content_encoding.ptr = NULL;
            transfer_encoding.ptr = NULL;
            content_length.ptr = NULL;
            cookie.ptr = NULL;
            sessione_type.ptr = NULL;
        };
        ~Check() {};
        bool init_conf();
        void parse_http(const char *data, size_t len);

    private:
        set<string>     subs;
        set<string>     fulls;

        ptr_string      host;
        ptr_string      content_type;
        ptr_string      accept;
        ptr_string      user_agent;
        ptr_string      refer;
        ptr_string      user_client;
        ptr_string      content_encoding;
        ptr_string      transfer_encoding;
        ptr_string      content_length;
        ptr_string      cookie;
        ptr_string      sessione_type;
};

#endif
