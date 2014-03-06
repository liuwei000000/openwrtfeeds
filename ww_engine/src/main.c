#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/in_systm.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include "nids.h"

//#define DEBUG(a...) fprintf(stderr, ##a);
#define DEBUG(a...)             //
#define int_ntoa(x)	inet_ntoa(*((struct in_addr *)&x))
extern int check_init();
extern int check_proc(char *data, size_t len, char *source, char *dest);

int content_flag = 0;

// 10.0.0.1,1024,10.0.0.2,23
char * adres (struct tuple4 addr)
{
    static char buf[256];
    strcpy (buf, int_ntoa (addr.saddr));
    sprintf (buf + strlen (buf), ":%i,", addr.source);
    strcat (buf, int_ntoa (addr.daddr));
    sprintf (buf + strlen (buf), ":%i", addr.dest);
    return buf;
}

void tcp_callback (struct tcp_stream *a_tcp, void ** this_time_not_needed)
{
    char buf[1024];
    sprintf(buf, "=TCP=\t");
    strcpy (buf + strlen(buf), adres (a_tcp->addr)); // we put conn params into buf
    if (a_tcp->nids_state == NIDS_JUST_EST)
    {
        a_tcp->client.collect++;
        a_tcp->server.collect++;
        a_tcp->server.collect_urg++;
        DEBUG("%s 建立连接\n", buf);
        return;
    }
    if (a_tcp->nids_state == NIDS_CLOSE)
    {
        DEBUG("%s 关闭连接\n", buf);
        return;
    }
    if (a_tcp->nids_state == NIDS_RESET)
    {
        DEBUG("%s RESET\n", buf);
        return;
    }

    if (a_tcp->nids_state == NIDS_DATA)
    {
        struct half_stream *hlf;
        a_tcp->client.collect--;
        a_tcp->server.collect--;

        if (a_tcp->server.count_new_urg)
        {
            // new byte of urgent data has arrived 
//            strcat(buf,"(urgent->)");
//            buf[strlen(buf)+1]=0;
//            buf[strlen(buf)]= a_tcp->server.urgdata;
//            write(1,buf,strlen(buf));
            return;
        }
        if (a_tcp->client.count_new)
        {
            hlf = &a_tcp->client;
            strcat (buf, "(<-)");
        } else {
            hlf = &a_tcp->server; // analogical
            strcat (buf, "(->)");
        }
        DEBUG("%s \n",buf);
        char souce[255], dest[255];
        strcpy (souce, int_ntoa (a_tcp->addr.saddr));
        strcpy (dest, int_ntoa (a_tcp->addr.daddr));
        check_proc(hlf->data, hlf->count_new, souce, dest);
    }
    return ;
}

//void udp_callback(struct tuple4 * addr, char *buf,  int len, struct ip *iph)
//{
//    int i = 0;
//    char buffer[1024];
//    sprintf(buffer, "=UDP=\t");
//    strcpy(buffer + strlen(buffer), inet_ntoa(*((struct in_addr *) &(addr->saddr))));
//    sprintf(buffer + strlen(buffer), ": %i ", addr->source);
//    strcat(buffer, " -> ");
//    strcpy(buffer + strlen(buffer), inet_ntoa(*((struct in_addr *) &(addr->daddr))));
//    sprintf(buffer + strlen(buffer), ": %i ", addr->dest);
//    fprintf(stderr,"%s \n",buffer);
//}

int main (int argc, char* argv[])
{
    if (argc > 1)
        content_flag = 1;

    // here we can alter libnids params, for instance:
    // nids_params.n_hosts=256;
    nids_params.device = "br-lan";
    nids_params.pcap_filter = "tcp port 80";

    if (!nids_init ())
    {
        fprintf(stderr,"%s\n",nids_errbuf);
        return 1;
    }

    check_init();
    nids_register_tcp (tcp_callback);
    //nids_register_udp (udp_callback);
    nids_run ();
    return 0;
}

