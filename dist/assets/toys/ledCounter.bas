def drawLeds(x,y,l)
	i=0
	for r = 0 to 2
		for c = 0 to 4
			meta(l(i),x+r*8,y+c*8)
			i=i+1
		next
	next
	
enddef

def draw()
	cls(0)
	drawLeds(10,10,list(0,1,0,0,1,1,1,1,1,1,0,0,0,0,1))
	drawLeds(55,10,list(1,0,1,1,1,1,0,1,0,1,1,1,1,0,1))
	drawLeds(85,10,list(1,0,0,0,1,1,0,1,0,1,1,1,1,1,1))
enddef