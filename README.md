# DHCP

**DHCP** is a software implementation of a Dynamic Host Configuration Protocol (DHCP) mechanism.
It uses a trie data structure combined with a bit array to efficiently allocate and manage IP addresses within a given subnet.
---

## Motivation

Provide a lightweight DHCP allocator that demonstrates networking concepts such as IP address allocation, management, and release.

---

## Prerequisites

- Linux OS
- c compiler (gcc)

---

## API

### `DHCPCreate`

```c
dhcp_t* DHCPCreate(const unsigned char subnet_ip[OCTETS], size_t mask);
```

**Description:**\
Initializes a new DHCP instance for the given subnet.

**Parameters:**

- `subnet_ip` — Subnet base IP address (in 4-octet array form).
- `mask` — Subnet mask (CIDR prefix, e.g., 24 for /24).

**Returns:**

- Pointer to a new dhcp_t on success
- NULL on allocation failure

---

### `DHCPDestroy`

```c
void DHCPDestroy(dhcp_t* dhcp);
```

**Description:**\
Frees all resources used by the DHCP instance.

**Parameters:**

- `dhcp` — Pointer to a dhcp_t created by DHCPCreate.

---

### `DHCPCountFree`

```c
size_t DHCPCountFree(const dhcp_t* dhcp);
```

**Description:**\
Counts how many IP addresses are still available for allocation in the subnet.

**Parameters:**

- `dhcp` — Pointer to a dhcp_t created by DHCPCreate.

**Returns:**
Number of available IP addresses in the subnet.

---

### `DHCPAllocateIp`

```c
status_t DHCPAllocateIp(dhcp_t* dhcp, const unsigned char preferred_ip[OCTETS], unsigned char result_ip[OCTETS]);
```

**Description:**\
Attempts to allocate a preferred IP address. If unavailable, the next available IP is returned.

**Parameters:**

- `dhcp` — Pointer to a dhcp_t created by DHCPCreate.
- `preferred_ip` — Desired IP address (can be NULL for "any available").
- `result_ip` — Output parameter where the allocated IP will be stored.
  
**Returns:**

- SUCCESS — Allocation succeeded
- ALLOC_FAILURE — Allocation failed
- DHCP_FULL — No free IP addresses

---

### `DHCPFreeIp`

```c
status_t DHCPFreeIp(dhcp_t* dhcp, unsigned char ip_to_free[OCTETS]);
```

**Description:**\
Frees a previously allocated IP address back into the pool.

**Parameters:**

- `dhcp` — Pointer to a dhcp_t created by DHCPCreate.
- `ip_to_free` — IP address to free.
  
**Returns:**

- SUCCESS — Freed successfully
- DOUBLE_FREE — Attempted to free an unallocated address

---

## Setup & Usage

### Build Instructions
in DHCP/bin, run the following -

```bash
gcc -ansi -pedantic-errors -Wall -Wextra -g ../test/test_dhcp.c ../src/dhcp.c ../ds/src/trie.c ../ds/src/bit_array.c -I ../include/ -I ../ds/include/ -o dhcp
```

Use test_dhcp.c for testing or other files from projects of yours.

---

## How It Works

1. Trie-based allocation — Each bit of the host portion of the IP address maps to a level in the trie.
2. Bit array optimization — Host bits are masked and combined with the subnet to generate usable addresses.
3. Reserved addresses — The network address, broadcast address, and server address are pre-allocated.
4. Fast lookup — Allocation, freeing, and counting free IPs operate efficiently using trie traversal.

---

## Main Components

- **trie**\
  Binary trie that manages allocation of host addresses.

- **bit array**\
  Efficient representation of host address bits, used for quick IP construction.

---


