#include <assert.h>      /*assert*/
#include <stdlib.h>      /*malloc, free*/
#include <stdint.h>      /* uint32_t */
#include <limits.h>      /* CHAR_BIT */

#include "dhcp.h"        /* DHCP API */
#include "trie.h"        /* TRIE API */

#define ASSIGN_OCTET(i) ((CHAR_BIT * (OCTETS - 1)) - (i * CHAR_BIT))

struct dhcp
{
    trie_t* trie;
    uint32_t subnet;
    size_t host_bits;
};

/************************Static Functions Declarations*************************/

static uint32_t CreateInputIP(const unsigned char subnet_ip[OCTETS]);
static int Preallocated(dhcp_t* dhcp);
static void AssignResultIP(bit_array_t ip, unsigned char result_ip[OCTETS]);

/**********************Static Functions Implementation*************************/

static uint32_t CreateInputIP(const unsigned char subnet_ip[OCTETS])
{
    uint32_t ip = 0;
    size_t i = 0;

    for( ; i < OCTETS; ++i)
    {
        ip |= subnet_ip[i] << ASSIGN_OCTET(i);
    }

    return ip;
}

static int Preallocated(dhcp_t* dhcp)
{
    bit_array_t network = 0;
    bit_array_t broadcast = BitArraySetAll(network);
    bit_array_t server = BitArraySetBitOff(broadcast, 0);

    return TrieAlloc(dhcp->trie, &network) == TRIE_SUCCESS &&
        TrieAlloc(dhcp->trie, &server) == TRIE_SUCCESS &&
        TrieAlloc(dhcp->trie, &broadcast) == TRIE_SUCCESS;
}

static void AssignResultIP(bit_array_t ip, unsigned char result_ip[OCTETS])
{
    size_t i = 0;

    for( ; i < OCTETS; ++i)
    {
        result_ip[i] = ip >> ASSIGN_OCTET(i);
    }
}

/*****************************API Functions************************************/

dhcp_t* DHCPCreate(const unsigned char subnet_ip[OCTETS], size_t mask)
{
    dhcp_t* dhcp = NULL;
    size_t leaf_depth = 0;

    assert(mask != 0 && mask < CHAR_BIT * OCTETS - 1);

    dhcp = (dhcp_t*)malloc(sizeof(dhcp_t));
    if(!dhcp)
    {
        return NULL;
    }

    leaf_depth = CHAR_BIT * OCTETS - mask;
    dhcp->trie = TrieCreate(leaf_depth);

    if(!dhcp->trie)
    {
        free(dhcp);
        return NULL;
    }

    dhcp->host_bits = leaf_depth;
    dhcp->subnet = (CreateInputIP(subnet_ip) >> leaf_depth) << leaf_depth;

    if(!Preallocated(dhcp))
    {
        DHCPDestroy(dhcp);
        return NULL;
    }

    return dhcp;
}

void DHCPDestroy(dhcp_t* dhcp)
{
    assert(dhcp);

    TrieDestroy(dhcp->trie);
    free(dhcp);
}

size_t DHCPCountFree(const dhcp_t* dhcp)
{
    assert(dhcp);

    return (1 << dhcp->host_bits) - TrieGetNumFullLeaves(dhcp->trie);
}

status_t DHCPAllocateIp(dhcp_t* dhcp, const unsigned char preferred_ip[OCTETS],
                                            unsigned char result_ip[OCTETS])
{
    bit_array_t host_address = 0;

    assert(dhcp);

    host_address = CreateInputIP(preferred_ip);
    host_address &= (bit_array_t)((1 << dhcp->host_bits) - 1);

    switch(TrieAlloc(dhcp->trie, &host_address))
    {
        case TRIE_ALLOC_FAIL:
            return ALLOC_FAILURE;

        case TRIE_FULL:
            return DHCP_FULL;

        default:
            break;
    }

    host_address |= dhcp->subnet;
    AssignResultIP(host_address, result_ip);

    return SUCCESS;
}

status_t DHCPFreeIp(dhcp_t* dhcp, unsigned char ip_to_free[OCTETS])
{
    bit_array_t host_address = 0;

    assert(dhcp);

    host_address = CreateInputIP(ip_to_free);

    return TrieFreeLeaf(dhcp->trie, &host_address) == TRIE_SUCCESS ? SUCCESS:
                                                                    DOUBLE_FREE;
}