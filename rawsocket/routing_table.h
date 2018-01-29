#include <sys/types.h>
#include <netinet/in.h>

#ifndef ROUTING_TABLE_H
#define ROUTING_TABLE_H

uint32_t get_local_ip_addr(in_addr_t dst_address);

#endif
