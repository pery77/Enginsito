def tick()
	b = (sin(frame * 0.1)*128)+128
	poke(0xd37,b)	
	poke(0xff0,b)
	
enddef

def draw()
cls(0)
enddef