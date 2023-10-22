'[
LOADDATA(slot) Return data stored in this slot
SAVEDATA(slot, data)	Save data into slot
Similar to Peek and Poke, but you have 32 slot for big numbers

slots      size      direction      min max value
--------   --------  ---------      -------------
0  to 15    2 byte    3552 (0xDE0)  0 to 65535 
16 to 31    4 byte    3584 (0xE00)  -2147483648 to 2147483647
']

savedata(0, -2686)
savedata(16, 69000)
savedata(31, 10001000)

my1ByteSlot = 0xe40 ' from E40 to FEF is not used.

poke(my1ByteSlot,   asc("P")) 
poke(my1ByteSlot+1, asc("R"))
poke(my1ByteSlot+2, asc("Y"))

print loaddata(0) - 65536 'this is the way for negative numbers for 2 bytes.
print loaddata(16) 'Numbers greater than 65535 must be saved from slot 16 onwards
print loaddata(31)

'If your number is smaller than 256 you can use peek and poke
print chr(peek(my1ByteSlot))+chr(peek(my1ByteSlot+1))+chr(peek(my1ByteSlot+2))