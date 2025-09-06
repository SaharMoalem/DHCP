#include <stdlib.h>
#include <string.h>

#include "test_macros.h"

#include "dhcp.h"

void TestDHCP() {
    unsigned char subnet_ip[4] = {192, 168, 1, 0};
    dhcp_t* dhcp = DHCPCreate(subnet_ip, 24);
	unsigned char preferred_ip[4] = {192, 168, 1, 10};
    unsigned char result_ip[4];
	unsigned char realloc_result_ip[4];
	unsigned char multi_ips[4] = {192, 168, 1, 20};
	size_t i = 0;
    
    TEST("Check Free IPs", DHCPCountFree(dhcp), 253);
    TEST("Allocate Specific IP", DHCPAllocateIp(dhcp, preferred_ip, result_ip),
																	SUCCESS);
    
    TEST("Allocated IP Matches Preferred",
									memcmp(preferred_ip, result_ip, 4) == 0, 1);
    
    TEST("Count again", DHCPCountFree(dhcp), 252);
    TEST("Free Allocated IP", DHCPFreeIp(dhcp, result_ip), SUCCESS);
    TEST("Reallocate Freed IP", DHCPAllocateIp(dhcp, preferred_ip,
												realloc_result_ip), SUCCESS);
    DHCPFreeIp(dhcp, realloc_result_ip);
    TEST("Double Free Handling", DHCPFreeIp(dhcp, realloc_result_ip),
																DOUBLE_FREE);
    for ( ; i < 10; ++i)
	{
        TEST("Multiple IP Allocations", DHCPAllocateIp(dhcp, multi_ips,
														result_ip), SUCCESS);
		TEST("Check Force alloc", multi_ips[3] + i, result_ip[3]);
    }
        
    DHCPDestroy(dhcp);
}

int main(void)
{
    TestDHCP();
    PASS;
    return 0;
}