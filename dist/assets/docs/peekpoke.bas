'PEEK(direction)           return the memory value at this direction
'POKE(direction, newValue) set the memory value at this direction

'In 0xffb or 4091 is stored crt effect enabled
print peek(0xffb)
print peek(4091) ' same as above but in decimal

def tick()
	'When you press space, toggle the crt effect.
	if keyreleased(key_space) then
		poke(4091, 1 - peek(4091)
	endif
enddef

'Learn more about memory in Documentation/Main/MemoryMap