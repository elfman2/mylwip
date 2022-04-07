#include <stdio.h>
#include "lwip/init.h"
#include "lwip/netif.h"
#include "lwip/udp.h"
#include "lwip/prot/ethernet.h"
#include "lwip/etharp.h"
#include <string.h>
#define printf
static void my_udp_recv_fn (void *arg, struct udp_pcb *pcb, struct pbuf *p, const ip_addr_t *addr, u16_t port)
{
   printf("UDP packet received: \n");
   printf("from remote ip = %s\n", ip4addr_ntoa(addr));
   printf("from remote port = %d\n", port);
   printf("with data = %s", (char *)(p->payload));
}
typedef enum lwip_error_t{
    LWIP_ERR_ADD,
    LWIP_ERR_NEW,
    LWIP_ERR_BIND,
    
}lwip_error_t;
static  struct netif my_netif;
static char MAC[6]={0x00,0x01,0x02,0x03,0x04,0x05};
static err_t init_if(struct netif *netif){
  netif->output     = etharp_output;
  netif->flags      = NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP | NETIF_FLAG_ETHERNET | NETIF_FLAG_IGMP | NETIF_FLAG_MLD6;
  SMEMCPY(netif->hwaddr, MAC, ETH_HWADDR_LEN);
  netif->hwaddr_len = ETH_HWADDR_LEN;
  return ERR_OK;
}

static int  nif(void){
   ip4_addr_t my_ipaddr;
   ip4_addr_t my_netmask;
   ip4_addr_t my_gw;

   ip4_addr_set_zero(&my_ipaddr);
   ip4_addr_set_zero(&my_netmask);
   ip4_addr_set_zero(&my_gw);
   IP4_ADDR((&my_ipaddr),   192,168,  1,200);
   IP4_ADDR((&my_netmask),  255,255,255,  0);
   IP4_ADDR((&my_gw),       192,168,  1,  1);
   if (netif_add(&my_netif, &my_ipaddr, &my_netmask, &my_gw, NULL,init_if, netif_input)==NULL) return LWIP_ERR_ADD;
   netif_set_default(&my_netif);
   netif_set_link_up(&my_netif);
   netif_set_up(netif_default);
   struct udp_pcb* my_udp_pcb = udp_new();
   if (NULL == my_udp_pcb) return LWIP_ERR_NEW;
   if (ERR_OK != udp_bind(my_udp_pcb, IP_ANY_TYPE, 60001)) return LWIP_ERR_BIND;
   udp_recv(my_udp_pcb, my_udp_recv_fn, NULL);
}
int initmylwip(void)
{
   printf("Hello World!\n");
   /* Init lwip stack */
   lwip_init();
   nif();

}
typedef struct my_custom_pbuf
{
   struct pbuf_custom p;
   void* dma_descriptor;
} my_custom_pbuf_t;

LWIP_MEMPOOL_DECLARE(RX_POOL, 10, sizeof(my_custom_pbuf_t), "Zero-copy RX PBUF pool");

void lwip_run(uint8_t *p_data_eth,int size){   
  my_custom_pbuf_t* my_pbuf  = (my_custom_pbuf_t*)LWIP_MEMPOOL_ALLOC(RX_POOL);

  struct pbuf* p = pbuf_alloced_custom(PBUF_RAW,
     size-ETH_HWADDR_LEN,
     PBUF_REF,
     &my_pbuf->p,
     p_data_eth,
     size);
  if(my_netif.input(p, &my_netif) != ERR_OK) {
    pbuf_free(p);
  }    
}

