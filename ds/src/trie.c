#include <assert.h>      /*assert*/
#include <stdlib.h>      /*malloc, free*/

#include "trie.h"        /* Trie API */

typedef enum 
{
    LEFT = 0,
    RIGHT = 1,
    NUM_OF_CHILDREN = 2
} children_t;

typedef struct trie_node
{
    struct trie_node* children[NUM_OF_CHILDREN];
    int is_full_sub;
} trie_node_t;

struct trie
{
    trie_node_t root;
    size_t leaf_depth;
};

/************************Static Functions Declarations*************************/

static void RecuDestroy(trie_node_t* node);
static size_t CountFullLeaves(const trie_node_t* node);
static void UpdateIsFullNode(trie_node_t* node);
static int AllocChild(trie_node_t* node, children_t child);
static trie_status_t RecuForceAlloc(trie_node_t* node, bit_array_t* input,
                                                                size_t height);
static trie_status_t RecuAlloc(trie_node_t* node, bit_array_t* input,
                                                                size_t height);
static trie_status_t RecuFree(trie_node_t* node, bit_array_t* input,
                                                                size_t height);

/**********************Static Functions Implementation*************************/

static void RecuDestroy(trie_node_t* node)
{
    if(!node)
    {
        return;
    }

    RecuDestroy(node->children[LEFT]);
    RecuDestroy(node->children[RIGHT]);
    free(node); 
}

static size_t CountFullLeaves(const trie_node_t* node)
{
    if(!node)
    {
        return 0;
    }

    return !node->children[LEFT] && !node->children[RIGHT] ?
            (size_t)node->is_full_sub : CountFullLeaves(node->children[LEFT]) +
                                        CountFullLeaves(node->children[RIGHT]);
}

static void UpdateIsFullNode(trie_node_t* node)
{
    node->is_full_sub = node->children[LEFT] ?
                                        node->children[LEFT]->is_full_sub : 0;
    node->is_full_sub &= node->children[RIGHT] ?
                                        node->children[RIGHT]->is_full_sub : 0;
}

static int AllocChild(trie_node_t* node, children_t child)
{
    if(!node->children[child])
    {
        node->children[child] = calloc(1, sizeof(trie_node_t));
    }

    return node->children[child] != NULL;
}

static trie_status_t RecuForceAlloc(trie_node_t* node, bit_array_t* input,
                                                                size_t height)
{
    children_t i = LEFT;
    trie_status_t status = TRIE_SUCCESS;

    if(node->is_full_sub)
    {
        return TRIE_FULL;
    }

    if(height == 0)
    {
        node->is_full_sub = 1;
        return TRIE_SUCCESS;
    }

    for( ; i < NUM_OF_CHILDREN; ++i)
    {
        if(!AllocChild(node, i))
        {
            return TRIE_ALLOC_FAIL;
        }

        status = RecuForceAlloc(node->children[i], input, height - 1);

        if(status != TRIE_FULL)
        {
            UpdateIsFullNode(node);
            *input = BitArraySetBit(*input, height - 1, i);
            return status;
        }
    }

    return status;
}

static trie_status_t RecuAlloc(trie_node_t* node, bit_array_t* input,
                                                                size_t height)
{
    children_t child = LEFT;
    trie_status_t status = TRIE_SUCCESS;

    if(node->is_full_sub)
    {
        return TRIE_FULL;
    }

    if(height == 0)
    {
        node->is_full_sub = 1;
        return TRIE_SUCCESS;
    }

    child = BitArrayGetValue(*input, height - 1);
    
    if(!AllocChild(node, child))
    {
        return TRIE_ALLOC_FAIL;
    }

    status = RecuAlloc(node->children[child], input, height - 1);
    
    if(status != TRIE_FULL)
    {
        UpdateIsFullNode(node);
        return status;
    }

    if(child == RIGHT)
    {
        return TRIE_FULL;
    }

    if(!AllocChild(node, RIGHT))
    {
        return TRIE_ALLOC_FAIL;
    }

    status = RecuForceAlloc(node->children[RIGHT], input, height - 1);

    if(status != TRIE_FULL)
    {
        UpdateIsFullNode(node);
        *input = BitArraySetBitOn(*input, height - 1);
    }

    return status;
}

static trie_status_t RecuFree(trie_node_t* node, bit_array_t* input,
                                                                size_t height)
{
    children_t child = LEFT;
    trie_status_t status = TRIE_SUCCESS;

    if(!node)
    {
        return TRIE_DOUBLE_FREE;
    }

    if(height == 0)
    {
        if(!node->is_full_sub)
        {
            status = TRIE_DOUBLE_FREE;
        }

        node->is_full_sub = 0;
        return status;
    }

    child = BitArrayGetValue(*input, height - 1);
    status = RecuFree(node->children[child], input, height - 1);
    UpdateIsFullNode(node);

    return status;
}

/*****************************API Functions************************************/
trie_t* TrieCreate(size_t leaf_depth)
{
    trie_t* trie = (trie_t*)malloc(sizeof(trie_t));

    if(!trie)
    {
        return NULL;
    }

    trie->leaf_depth = leaf_depth;
    trie->root.children[LEFT] = NULL;
    trie->root.children[RIGHT] = NULL;
    trie->root.is_full_sub = 0;

    return trie;
}

void TrieDestroy(trie_t* trie)
{
    assert(trie);

    RecuDestroy(trie->root.children[LEFT]);
    RecuDestroy(trie->root.children[RIGHT]);
    free(trie);
}

size_t TrieGetNumFullLeaves(const trie_t* trie)
{
    assert(trie);

    return CountFullLeaves(&trie->root);
}

trie_status_t TrieAlloc(trie_t* trie, bit_array_t* input)
{
    assert(trie);
    assert(input);

    return RecuAlloc(&trie->root, input, trie->leaf_depth);
}

trie_status_t TrieFreeLeaf(trie_t* trie, bit_array_t* input)
{
    assert(trie);
    assert(input);

    return RecuFree(&trie->root, input, trie->leaf_depth);
}