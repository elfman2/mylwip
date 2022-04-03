#ifndef LWIP_LWIPOPTS_H
#define LWIP_LWIPOPTS_H
#define MEMP_USE_CUSTOM_POOLS 1
#define LWIP_SINGLE_NETIF 1
#define LWIP_TCP 0
#define LWIP_TCPIP_TIMEOUT 0
#define NO_SYS       1
#define LWIP_SOCKET  0
#define LWIP_NETCONN 0
#define LWIP_TIMERS 0
#define MEM_USE_POOLS 0
#define SYS_ARCH_PROTECT(x)
#define  SYS_ARCH_UNPROTECT(x)
#define SYS_ARCH_DECL_PROTECT(x)
#undef LWIP_RAND 
/*#define LWIP_NETIF_STATUS_CALLBACK  1
#define LWIP_NETIF_LINK_CALLBACK    1
*/
#endif /* LWIP_LWIPOPTS_H */