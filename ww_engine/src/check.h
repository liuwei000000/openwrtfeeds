#ifndef CHECK_H
#define CHECK_H
#include <set>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

#define HOST                    "host"
#define CONTENT_TYPE            "content_type"
#define USER_AGENT              "user_agent"
#define USER_CLIENT             "user_clinet"
#define REFER                   "refer"

typedef struct {
    const char * ptr;
    size_t len;
} ptr_string;

typedef struct {
    set<string>     subs;
    set<string>     fulls;
    string          type;
} rule_item;

typedef struct {
    vector<rule_item>           rule_items;
    string                      log;
    time_t                      out_sec;
    unsigned                    count;
} rule;

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
        void process(char *source, char *dest);
        bool process_http(ptr_string& http_header, const string &type, vector<rule_item>& item);

    private:
        vector<rule>                    rules;

        int                             frequency;
        ptr_string                      host;
        ptr_string                      content_type;
        ptr_string                      accept;
        ptr_string                      user_agent;
        ptr_string                      refer;
        ptr_string                      user_client;
        ptr_string                      content_encoding;
        ptr_string                      transfer_encoding;
        ptr_string                      content_length;
        ptr_string                      cookie;
        ptr_string                      sessione_type;     
};

#endif
