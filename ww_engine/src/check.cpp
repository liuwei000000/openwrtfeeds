#include <arpa/inet.h>
#include "check.h"
#include <string.h>

//#define DEBUG(a...) fprintf(stderr, ##a);
#define DEBUG(a...)             //

Check check;
extern "C" int check_init()
{
    int rc = check.init_conf();
    return rc;
}

extern "C" int check_proc(char *data, size_t len)
{
    check.parse_http(data, len);
    check.process();
    return 1;
}

//字符串分割函数
set<string> split(string str, string pattern)
{
    string::size_type pos;
    set<string> result;
    str+=pattern;//扩展字符串以方便操作
    string::size_type size=str.size();

    for(string::size_type i=0; i<size; i++)
    {
        pos=str.find(pattern,i);
        if(pos<size)
        {
            string s=str.substr(i,pos-i);
            result.insert(s);
            i=pos+pattern.size()-1;
        }
    }
    return result;
}

bool Check::init_conf()
{
    DEBUG("init_conf");
    string t, strs;
    ifstream conf("/etc/ww_engine.conf");

    conf >> frequency;
    conf >> t;
    while ( t != "0" ) {
        check_entry     e;
        if (t == HOST ) {
            conf >> strs;
            e.subs = split(strs, ",");
            conf >> strs;
            e.fulls = split(strs, ",");
            conf >> e.log;
            check_hosts.push_back(e);
        }
        conf >> t;
    }
    DEBUG("init finish");
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
    printf("== N == %s\n", a);
}

void Check::process()
{
    process_http();
}

void Check::process_http()
{
    time_t t = time((time_t*)NULL);
    for ( unsigned int i = 0; i < check_hosts.size(); i++ ) {
        set<string>::const_iterator it;
        for (it = check_hosts[i].fulls.begin(); it != check_hosts[i].fulls.end(); ++it) {
            unsigned int l = it->size() > host.len ? host.len : it->size();
            //full
            if (! memcmp(host.ptr, it->c_str(), l) &&  check_hosts[i].out_sec != t / (time_t)frequency) {
                //时间
                check_hosts[i].out_sec = t / (time_t)frequency;
                printf("%s \n", check_hosts[i].log.c_str());
            }
        }
        char s[1024];
        memcpy(s, host.ptr, host.len);
        s[host.len] = '\0';

        for (it = check_hosts[i].subs.begin(); it != check_hosts[i].subs.end(); ++it) {
            // sub
            if (strstr(s, it->c_str()) != NULL &&  check_hosts[i].out_sec != t / (time_t)frequency) {
                //时间
                check_hosts[i].out_sec = t / (time_t)frequency;
                printf("%s \n", check_hosts[i].log.c_str());
            }
        }
    }
}

static const char str[] = "GET / HTTP/1.1\r\nHost: www.baidu.com\r\nUser-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64; rv:27.0) Gecko/20100101 Firefox/27.0\r\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\nAccept-Encoding: gzip, deflate\r\nCookie: bdshare_firstime=1388495114171; BAIDUID=6686D56DD5D0B7476A84634453D97457:FG=1; Hm_lvt_9f14aaa038bbba8b12ec2a4a3e51d254=1392544674; cflag=65535:1; H_PS_TIPFLAG=; H_PS_TIPCOUNT=5; BD_CK_SAM=1; shifen[104049791_63132]=1392696400; BDRCVFR[gltLrB7qNCt]=mk3SLVN4HKm; H_PS_PSSID=4851_5138_1466_5186_5207_51\r\nConnection: keep-alive\r\n\r\n";


int main_t()
{
    Check c;
    c.parse_http(str, strlen(str));
    return 0;
}
