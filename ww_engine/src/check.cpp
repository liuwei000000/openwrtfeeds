#include <arpa/inet.h>
#include "check.h"
#include <string.h>
#include <stdlib.h>
#include <time.h>

//#define DEBUG(a...)  fprintf(stdout, ##a);
#define DEBUG(a...)             //
#define ERROR(a...)    fprintf(stderr, ##a)

Check check;
extern "C" int check_init()
{
    int rc = check.init_conf();
    return rc;
}

extern "C" int check_proc(char *data, size_t len, char *source, char *dest)
{
    check.parse_http(data, len);
    check.process(source, dest);
    return 1;
}

static string& trim(string &s)
{
    if (s.empty()) {
        return s;
    }
    s.erase(0,s.find_first_not_of(" "));
    s.erase(s.find_last_not_of(" ") + 1);
    s.erase(0,s.find_first_not_of("\t"));
    s.erase(s.find_last_not_of("\t") + 1);
    return s;
}

//字符串分割函数
set<string> split(const string& s, const string &pattern)
{
    string str = s;
    string::size_type pos;
    set<string> result;
    if (str.empty()) return result;
    str+=pattern;//扩展字符串以方便操作
    string::size_type size=str.size();

    for(string::size_type i=0; i<size; i++)
    {
        pos=str.find(pattern,i);
        if(pos<size)
        {
            string s=str.substr(i,pos-i);
            s = trim(s);
            result.insert(s);
            i=pos+pattern.size()-1;
        }
    }
    return result;
}

bool set_val(const string &t, const string &key, string &val)
{
    if (t == key) return true;
    if (t.size() > key.size() && !memcmp(t.c_str(), key.c_str(), key.size()) ) {
        val = t.substr(key.size());
        val = trim(val);
        return true;
    }
    return false;
}

bool Check::init_conf()
{
    DEBUG("init_conf\n");
    string t, strs;
    ifstream conf("/etc/ww_engine.conf");

    getline(conf,t);
    bool rule_flag = false;
    bool set_time = false;
    rule  r;
    while ( !conf.eof() ) {
        DEBUG("read line| %s\n", t.c_str());
        t = trim(t);
        if ( !t.empty() && t[0] != '#' ) {
            // 非空且不为注释
            string tmp;
            if (!set_time && set_val(t, "time:", tmp )) {
                frequency = atoi(tmp.c_str());
                DEBUG("time: %d\n", frequency);
                set_time = true;
            }

            if (!rule_flag && set_val(t, "rule:", r.log ) ) {
                rule_flag = true;
                r.count = 0;
                DEBUG("rule log: %s\n", r.log.c_str());
            }
            bool item_flag = false;
            rule_item ritem;
            while (rule_flag && !conf.eof()) {
                getline(conf,t);
                DEBUG("rule read line| %s\n", t.c_str());
                t = trim(t);
                if ( !t.empty() && t[0] != '#' ) {
                    DEBUG("do line ...\n");
                    if ( !item_flag) {
                        tmp.clear();
                        if (set_val(t, "rule_end", tmp)) {
                            DEBUG("rule_end \n");
                            rule_flag = false;
                            rules.push_back(r);
                            r.rule_items.clear();
                            r.log.clear();
                            r.out_sec = 0;
                            r.count = 0;
                            break;
                        }

                        tmp.clear();
                        if (!set_val(t , "item_start", tmp) ) {
                            ERROR("must have rule_item\n");
                            return false;
                        }
                        else {
                            DEBUG("item start\n");
                            item_flag = true;
                            continue;
                        }
                    }
                    if ( item_flag && set_val(t , "type:", ritem.type)) {
                        DEBUG("type: %s\n", ritem.type.c_str());
                        continue;
                    }

                    tmp.clear();
                    if ( item_flag && set_val(t , "sub:", tmp)) {
                        DEBUG("subs:%s\n", tmp.c_str());
                        ritem.subs = split(tmp, ",");
                        continue;
                    }

                    tmp.clear();
                    if ( item_flag && set_val(t , "full:", tmp)) {
                        DEBUG("fulls:%s\n", tmp.c_str());
                        ritem.fulls = split(tmp, ",");
                        continue;
                    }

                    tmp.clear();
                    if( item_flag && set_val(t, "item_end", tmp) ) {
                        DEBUG("item_end \n");
                        item_flag = false;
                        (r.rule_items).push_back(ritem);
                        ritem.subs.clear();
                        ritem.fulls.clear();
                        ritem.type.clear();
                        continue;
                    }
                }
            }
        }
        getline(conf,t);
    }
    for (unsigned int i = 0; i < rules.size(); i++) {
        DEBUG("%d rules item size:%d\n", i, rules[i].rule_items.size());
    }
    DEBUG("init finish\n");
    return true;
}

void Check::parse_http(const char *data, size_t len)
{
    int n = 0;
    const char *ptr;
    size_t l = 0;

    ptr = data;
    for ( size_t i = 0; i < len; i++ ) {
        if ( !(data[i] == 0x0d && data[i+1] == 0x0a)) {
            n++;
            continue;
        }
        l = (char *)&data[i] - ptr;

        if (0 == l) {
            break;
        }

        do {
            if ( l > 6 && !memcmp(ptr, "Host:", 5)) {
                if (' ' == ptr[5] ) {
                    host.ptr = &ptr[6];
                    host.len = l - 6;
                } else {
                    host.ptr = &ptr[5];
                    host.len = l - 5;
                }
                break;
            }
            if (l > 14
                    && 'C' == (0xDF&ptr[0])
                    && !memcmp(ptr + 1, "ontent-", 7)
                    && 'T' == (0xDF&ptr[8])
                    && !memcmp(ptr + 9, "ype: ", 5) ) {
                content_type.ptr = &ptr[14];
                content_type.len = l - 14;
                break;
            }
            if (l > 8
                    && !memcmp(ptr, "Accept: ", 8) ) {
                accept.ptr = &ptr[8];
                accept.len = l - 8;
                break;
            }
            if (l > 9
                    && !memcmp(ptr, "Referer: ", 9) ) {
                refer.ptr = &ptr[9];
                refer.len = l - 9;
                break;
            }
            if (l > 12
                    && !memcmp(ptr, "User-Agent: ", 12) ) {
                user_agent.ptr = &ptr[12];
                user_agent.len = l - 12;
                break;
            }
            if (l > 13
                    && !memcmp(ptr, "User-Client: ", 13) ) {
                user_client.ptr = &ptr[13];
                user_client.len = l - 13;
                break;
            }
            if (l > 18
                    && !memcmp(ptr, "Content-Encoding: ", 18) ) {
                content_encoding.ptr = &ptr[18];
                content_encoding.len = l - 18;
                break;
            }
            if (l > 19
                    && !memcmp(ptr, "Transfer-Encoding: ", 19) ) {
                transfer_encoding.ptr = &ptr[19];
                transfer_encoding.len = l - 19;
                break;
            }
            if (l > 14
                    && 'C' == (0xDF & ptr[0])
                    && !memcmp(ptr + 1, "ontent-" , 7)
                    && 'L' == (0xDF & ptr[8])
                    && !memcmp(ptr + 9, "enth: ", 7) ) {
                content_length.ptr = &ptr[16];
                content_length.len = l - 16;
                break;
            }
            if (l > 8
                    && !memcmp(ptr, "Cookie: ", 8) ) {
                cookie.ptr = &ptr[8];
                cookie.len = l - 8;
                break;
            }
            if (l > 16
                    && !memcmp(ptr, "X-Session-Type: ", 16) ) {
                sessione_type.ptr = &ptr[16];
                sessione_type.len = l - 16;
                break;
            }
        } while(false);
        ptr = &data[i+2];
        l = 0;
    }

    char a[1024];
    memcpy(a, host.ptr, host.len);
    a[host.len] = '\0';
    DEBUG("host: %s\n", a);
    memcpy(a, user_agent.ptr, user_agent.len);
    a[user_agent.len] = '\0';
    DEBUG("user agent: %s\n", a);
    DEBUG("#####################\n");
}

void Check::process(char *source, char *dest)
{
    time_t t = time((time_t*)NULL);
    for (size_t i = 0; i < rules.size(); ++i) {
        DEBUG( "macth rule[%d]\n", i);
        if (!process_http(host         ,HOST        ,rules[i].rule_items)) {
            DEBUG( HOST " fail!\n");
            continue;
        }
        if (!process_http(content_type ,CONTENT_TYPE,rules[i].rule_items)) {
            DEBUG( CONTENT_TYPE " fail!\n");
            continue;
        }
        if (!process_http(user_agent,USER_AGENT  ,rules[i].rule_items)) {
            DEBUG( USER_AGENT " fail!\n");
            continue;
        }

        if (!process_http(refer        ,REFER      ,rules[i].rule_items)) {
            DEBUG( REFER" fail!\n");
            continue;
        }
        //命中
        if ( rules[i].out_sec != t / (time_t)frequency) {
            DEBUG("Match Print");
            rules[i].out_sec = t / (time_t)frequency;
            log << t << " | "
                << rules[i].log << " | Count: " <<  rules[i].count
                << " | src: " << source << ", des: " << dest << endl;

            rules[i].count = 1;
        } else {
            DEBUG("Match count++");
            rules[i].count++;
        }
    }
}

bool Check::process_http( ptr_string &http_header, const string &type, vector<rule_item> & items)
{
    bool rt = true;
    if (items.empty()) return true;
    for (size_t i = 0; i < items.size(); ++i) {
        if ( items[i].type != type ) continue;
        rt = false;

        if (!http_header.ptr) return false;
        char s[1024];
        memcpy(s, http_header.ptr, http_header.len);
        s[http_header.len] = '\0';

        set<string>::const_iterator it;
        for (it = items[i].fulls.begin(); it != items[i].fulls.end(); ++it) {
            unsigned int l = it->size() > host.len ? host.len : it->size();
            DEBUG("match full %s vs %s\n", s, it->c_str());
            //full
            if ( memcmp(host.ptr, it->c_str(), l)) {
                //时间
                return true;
            }
        }

        for (it = items[i].subs.begin(); it != items[i].subs.end(); ++it) {
            //sub
            DEBUG("match sub %s vs %s\n", s, it->c_str());
            if (strstr(s, it->c_str()) != NULL ) {
                return true;
            }
        }
    }
    return rt;
}

static const char str[] = "GET / HTTP/1.1\r\nHost: www.baidu.com\r\nUser-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64; rv:27.0) Gecko/20100101 Firefox/27.0\r\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\nAccept-Encoding: gzip, deflate\r\nCookie: bdshare_firstime=1388495114171; BAIDUID=6686D56DD5D0B7476A84634453D97457:FG=1; Hm_lvt_9f14aaa038bbba8b12ec2a4a3e51d254=1392544674; cflag=65535:1; H_PS_TIPFLAG=; H_PS_TIPCOUNT=5; BD_CK_SAM=1; shifen[104049791_63132]=1392696400; BDRCVFR[gltLrB7qNCt]=mk3SLVN4HKm; H_PS_PSSID=4851_5138_1466_5186_5207_51\r\nConnection: keep-alive\r\n\r\n";


int main_t()
{
    Check c;
    c.parse_http(str, strlen(str));
    return 0;
}
