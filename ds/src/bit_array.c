#include <assert.h>                  /* assert */
#include <limits.h>                  /* CHAR_BIT */

#include "bit_array.h"

#define BIT_ARRAY_SIZE (sizeof(bit_array_t) * CHAR_BIT)
#define BIT_ARRAY_ONE ((bit_array_t)1)

bit_array_t BitArraySetAll(bit_array_t array)
{
    array = 0;

    return ~array;
}

bit_array_t BitArrayResetAll(bit_array_t array)
{
    array = 0;

    return array;
}

bit_array_t BitArraySetBitOn(bit_array_t array, size_t index)
{
    assert(BIT_ARRAY_SIZE > index);

    return array | (BIT_ARRAY_ONE << index);
}

bit_array_t BitArraySetBitOff(bit_array_t array, size_t index)
{
    assert(BIT_ARRAY_SIZE > index);

    return array & (~(BIT_ARRAY_ONE << index));
}

bit_array_t BitArraySetBit(bit_array_t array, size_t index, int value)
{
    assert(BIT_ARRAY_SIZE > index);

    return BitArraySetBitOff(array, index) | (value << index);
}

int BitArrayGetValue(bit_array_t array, size_t index)
{
    assert(BIT_ARRAY_SIZE > index);

    return (array >> index) & 0x01;
}

bit_array_t BitArrayMirror(bit_array_t array)
{
    bit_array_t result = 0;
    size_t i = 0;
    
    for ( ; i < BIT_ARRAY_SIZE; ++i)
    {
        result <<= 1;
        result |= (array & 1);
        array >>= 1;
    }
    
    return result;
}

bit_array_t BitArrayFlipBit(bit_array_t array, size_t index)
{
    assert(BIT_ARRAY_SIZE > index);

    return array ^ (BIT_ARRAY_ONE << index);
}

bit_array_t BitArrayRotateRight(bit_array_t array, size_t n_rotations)
{ 
    bit_array_t tmp;
    
    n_rotations %= BIT_ARRAY_SIZE;
    tmp = array & ((BIT_ARRAY_ONE << n_rotations) - 1);
    array >>= n_rotations;
    array |= tmp << ((BIT_ARRAY_SIZE - n_rotations) % BIT_ARRAY_SIZE);

    return array;
}

bit_array_t BitArrayRotateLeft(bit_array_t array, size_t n_rotations)
{
    return BitArrayRotateRight(array, BIT_ARRAY_SIZE - n_rotations);
}

size_t BitArrayCountOn(bit_array_t array)
{
    size_t array_count = 0;
    
    while (0 < array)
    {
        array &= array - 1;
        ++array_count;
    }
    
    return array_count;
}

size_t BitArrayCountOff(bit_array_t array)
{
    return BitArrayCountOn(~array);
}

void BitArrayToString(bit_array_t array, char* dest)
{
    char* runner = dest + BIT_ARRAY_SIZE;
    
    assert(NULL != dest);
    
    *runner = '\0';
    --runner;
    
    while (runner >= dest)
    {
        *runner = (array & 0x01) + '0';
        array >>= 1;
        --runner;
    }
}
