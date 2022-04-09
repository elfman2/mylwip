#include <stdio.h>
#include "lwip/init.h"
#include "lwip/netif.h"
#include "lwip/udp.h"
#include "lwip/prot/ethernet.h"
#include "lwip/etharp.h"
#include <string.h>
#define printf
typedef enum lwip_error_t{
    LWIP_ERR_ADD,
    LWIP_ERR_NEW,
    LWIP_ERR_BIND,
    
}lwip_error_t;
static  struct netif my_netif;
static char MAC[6]={0x00,0x01,0x02,0x03,0x04,0x05};

extern err_t  mynetif_linkoutput_fn (struct netif *netif, struct pbuf *p);

static err_t init_if(struct netif *netif){
  netif->output     = etharp_output;
  netif->linkoutput = mynetif_linkoutput_fn;
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
}
int initmylwip(void)
{
   printf("Hello World!\n");
   /* Init lwip stack */
   lwip_init();
   nif();

}


void lwip_run(uint8_t *buf,int len){   
  struct pbuf *p = pbuf_alloc(PBUF_RAW, len, PBUF_POOL);
  if (p != NULL) {
    pbuf_take(p, buf, len);
    /* acknowledge that packet has been read(); */
    if(my_netif.input(p, &my_netif) != ERR_OK) {
        pbuf_free(p);
    }    
  }

}

