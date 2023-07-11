def drawF(x,y)
	rect(x,y,101,100,1,2)
	for i = 0 to 100
		j = y + 50 + sin(i*mouse_X)*40
		pixel(i+x,j,3)
	next
enddef

def draw()
	cls(8)
	drawF(10,10)
enddef
