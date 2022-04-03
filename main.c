#include <stdio.h>
#include "lwip/init.h"
#include "lwip/netif.h"
static void my_udp_recv_fn (void *arg, struct udp_pcb *pcb, struct pbuf *p, const ip_addr_t *addr, u16_t port)
{
   printf("UDP packet received: \n");
   printf("from remote ip = %s\n", ip4addr_ntoa(addr));
   printf("from remote port = %d\n", port);
   printf("with data = %s", (char *)(p->payload));
}
void nif(void){
   struct netif my_netif;
   ip4_addr_t my_ipaddr;
   ip4_addr_t my_netmask;
   ip4_addr_t my_gw;

   ip4_addr_set_zero(&my_ipaddr);
   ip4_addr_set_zero(&my_netmask);
   ip4_addr_set_zero(&my_gw);
   IP4_ADDR((&my_ipaddr),   192,168,  1,200);
   IP4_ADDR((&my_netmask),  255,255,255,  0);
   IP4_ADDR((&my_gw),       192,168,  1,  1);
   netif_add(&my_netif, &my_ipaddr, &my_netmask, &my_gw, NULL, NULL, netif_input);
   netif_set_default(&my_netif);
   netif_set_up(netif_default);
   struct udp_pcb* my_udp_pcb = udp_new();
   if (NULL == my_udp_pcb) {
      printf("udp_new() has failed !\n");
      return -1;
   }
 if (ERR_OK != udp_bind(my_udp_pcb, IP_ANY_TYPE, 60001)) {
      printf("udp_bind() has failed!\n");
      return -1;
   }

   udp_recv(my_udp_pcb, my_udp_recv_fn, NULL);
}
int main(int argc, char * argv)
{
   printf("Hello World!\n");
   nif();
   /* Init lwip stack */
   lwip_init();
}
