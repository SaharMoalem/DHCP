#ifndef __DHCP_H__
#define __DHCP_H__

#include <stddef.h>         /* size_t */

#define OCTETS (4)
typedef struct dhcp dhcp_t;

typedef enum status
{
    SUCCESS = 0,
    ALLOC_FAILURE = 1,
    DHCP_FULL = 2,
    DOUBLE_FREE = 3
} status_t;

/*
*	@desc:				Allocates new DHCP based on @subnet_ip and @mask
*	@param:				@subnet_ip: IP address to assign to DHCP
*                       @mask: mask of @subnet_ip
*	@return:			Newly allocated DHCP
*	@error:				Returns NULL if allocation failed
*	@time complexity:	O(malloc) for both AC/WC
*	@space complexity:	O(malloc) for both AC/WC
*/
dhcp_t* DHCPCreate(const unsigned char subnet_ip[OCTETS], size_t mask);

/*
*	@desc:				Frees @dhcp which was created by @DHCPCreate
*	@param:				@dhcp: preallocated DHCP
*	@return:			None
*	@error:				Undefined behavior if @dhcp is invalid
*	@time complexity:	O(free) for both AC/WC
*	@space complexity:	O(free) for both AC/WC
*/
void DHCPDestroy(dhcp_t* dhcp);

/*
*	@desc:				Allocates either @prefered_ip address or @result_ip if
                        @prefered_ip is already taken in @dhcp
*	@param:				@dhcp: preallocated DHCP
*						@prefered_ip: user's host ip to allocate
                        @result_ip: the allocated ip if @prefered_ip is
                        allocated already
*	@return:			SUCCESS if function allocates successfully,
*                       ALLOC_FAILURE if allocation failed,
*                       DHCP_FULL if DHCP is full
*	@error:				Undefined behavior if @dhcp is invalid
*	@time complexity:	O(log(n)) AC and O(n) for WC
*	@space complexity:	O(1) for AC and O(n) for WC
*/
status_t DHCPAllocateIp(dhcp_t* dhcp, const unsigned char preferred_ip[OCTETS],
                                            unsigned char result_ip[OCTETS]); 

/*
*	@desc:				frees @ip_to_free address in @dhcp
*	@param:				@dhcp: preallocated DHCP
*						@ip_to_free: user's host ip to free
*	@return:			TRIE_SUCCESS if function frees successfully,
*                       TRIE_DOUBLE_FREE if address is already empty
*	@error:				Undefined behavior if @dhcp is invalid
*	@time complexity:	O(log(n)) for both AC/WC
*	@space complexity:	O(1) for both AC/WC
*/
status_t DHCPFreeIp(dhcp_t* dhcp, unsigned char ip_to_free[OCTETS]);

/*
*	@desc:				Returns the count of free host IP addresses in @dhcp
*	@param:				@dhcp: preallocated DHCP
*	@return:			Returns the count of free host IP addresses in @dhcp
*	@error:				Undefined behavior if @dhcp is invalid
*	@time complexity:	O(n) for both AC/WC
*	@space complexity:	O(1) for both AC/WC
*/
size_t DHCPCountFree(const dhcp_t* dhcp);

#endif  /*__DHCP_H__*/
