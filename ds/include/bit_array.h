#ifndef __BIT_ARRAY_H__
#define __BIT_ARRAY_H__

#include <stddef.h>               /*size_t*/
#include <stdint.h>               /*uint64_t*/

typedef uint64_t bit_array_t;

/* @Desc: Return bit_array_t of 64 bits where all bits are on (1)
   @Params: bit_array_t
   @Return value: a copy of bit_array_t*/
   
bit_array_t BitArraySetAll(bit_array_t array);

/* @Desc: Return bit_array_t of 64 bits where all bits are off (0)
   @Params: bit_array_t
 */
bit_array_t BitArrayResetAll(bit_array_t array);

/* @Desc:  Return bit_array_t with bit on at index between 0 and 63
   @Params: bit_array_t, index*/

bit_array_t BitArraySetBitOn(bit_array_t array, size_t index);

/* @Desc: Return bit_array_t with bit off at index between 0 and 63
   @Params: bit_array_t, index*/

bit_array_t BitArraySetBitOff(bit_array_t array, size_t index);

/* @Desc: 
   @Params: */

bit_array_t BitArraySetBit(bit_array_t array, size_t index, int value);

/* @Desc: Function to return the value of bit at index between 0 and 63 
   @Params: */
   
int BitArrayGetValue(bit_array_t array, size_t index);

/* @Desc:  Function to return the mirror of bit_array_t
   @Params: */
   
bit_array_t BitArrayMirror(bit_array_t array);

/* @Desc: Function to flip a bit at index  between 0 and 63 from 0 to 1 or vice versa
   @Params: */
   
bit_array_t BitArrayFlipBit(bit_array_t array, size_t index);

/* @Desc:  Function to rotatate all bits right
   @Params: */
   
bit_array_t BitArrayRotateRight(bit_array_t array, size_t n_rotations);

/* @Desc:  Function to rotatate all bits left 
   @Params: */
   
bit_array_t BitArrayRotateLeft(bit_array_t array, size_t n_rotations);

/* @Desc:  Function to count all bits on
   @Params: */

size_t BitArrayCountOn(bit_array_t array);

/* @Desc: Function to count all bits off 
   @Params: */
   
size_t BitArrayCountOff(bit_array_t array);

/* @Desc:  Function to copy all bits to string
   @Params: dest should be big enough to hold 64 characters */
   
void BitArrayToString(bit_array_t array, char* dest);

#endif  /*End of header guard*/
