#ifndef __TRIE_H__
#define __TRIE_H__

#include <stddef.h>         /* size_t */

#include "bit_array.h"		/*bit_array API*/

typedef enum trie_status
{
    TRIE_SUCCESS = 0,
    TRIE_ALLOC_FAIL = 1,
    TRIE_FULL = 2,
    TRIE_DOUBLE_FREE = 3,
    TRIE_NUM_OF_STATUS
} trie_status_t;

typedef struct trie trie_t;

/*
*	@desc:				Allocates new trie based on @leaf_depth
*	@param:				@leaf_depth: depth of all leaves in the trie
*	@return:			Newly allocated trie
*	@error:				Returns NULL if allocation failed
*	@time complexity:	O(malloc) for both AC/WC
*	@space complexity:	O(malloc) for both AC/WC
*/
trie_t* TrieCreate(size_t leaf_depth);

/*
*	@desc:				Frees @trie which was created by @TrieCreate
*	@param:				@trie: preallocated trie
*	@return:			None
*	@error:				Undefined behavior if @trie is invalid
*	@time complexity:	O(free) for both AC/WC
*	@space complexity:	O(free) for both AC/WC
*/
void TrieDestroy(trie_t* trie);

/*
*	@desc:				Returns the count of full leaves in @trie
*	@param:				@trie: preallocated trie
*	@return:			Returns the count of full leaves in @trie
*	@error:				Undefined behavior if @trie is invalid
*	@time complexity:	O(n) for both AC/WC
*	@space complexity:	O(1) for both AC/WC
*/
size_t TrieGetNumFullLeaves(const trie_t* trie);

/*
*	@desc:				Allocates new branch directed by @input in @trie
*	@param:				@trie: preallocated trie
*						@input: user's direction commmands to allocate
*	@return:			TRIE_SUCCESS if function allocates successfully,
*                       TRIE_ALLOC_FAIL if allocation failed,
*                       TRIE_FULL if trie is full
*	@error:				Undefined behavior if @trie is invalid
*	@time complexity:	O(log(n)) AC and O(n) for WC
*	@space complexity:	O(1) for both AC/WC
*/
trie_status_t TrieAlloc(trie_t* trie, bit_array_t* input);

/*
*	@desc:				frees a branch presented by @input in @trie
*	@param:				@trie: preallocated trie
*						@input: user's direction commmands to free
*	@return:			TRIE_SUCCESS if function frees successfully,
*                       TRIE_DOUBLE_FREE if branch is already empty
*	@error:				Undefined behavior if @trie is invalid
*	@time complexity:	O(log(n)) for both AC/WC
*	@space complexity:	O(1) for both AC/WC
*/
trie_status_t TrieFreeLeaf(trie_t* trie, bit_array_t* input);

#endif  /*__TRIE_H__*/