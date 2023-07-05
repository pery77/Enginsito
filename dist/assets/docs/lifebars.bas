def drawBar(x,y,size,percent)
	for v = 0 to size
	if size/percent
		sprite(1,(v*8)+x,y,6)
		sprite(0,(v*8)+x,y,3)	
	next
enddef

def draw()
	cls(0)
	drawBar(5,5,10,33)
enddef