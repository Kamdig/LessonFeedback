#ifndef LWIPOPTS_H
#define LWIPOPTS_H

#define NO_SYS                     1
#define LWIP_SOCKET                0
#define LWIP_NETCONN               0

#define MEM_LIBC_MALLOC            1
#define MEMP_MEM_MALLOC            1

#define LWIP_DNS                   1
#define LWIP_TCP                   1
#define LWIP_UDP                   1
#define LWIP_ICMP                  1

#define TCP_MSS                    1460
#define TCP_WND                    (4 * TCP_MSS)
#define TCP_SND_BUF                (4 * TCP_MSS)

#define LWIP_HTTPD                 0
#define LWIP_IPV6                  0

#endif

