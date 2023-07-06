num = dict()
num(0) = list(1,1,1,1,1, 1,0,0,0,1, 1,1,1,1,1)
num(1) = list(0,1,0,0,1, 1,1,1,1,1, 0,0,0,0,1)
num(2) = list(1,0,1,1,1, 1,0,1,0,1, 1,1,1,0,1)
num(3) = list(1,0,0,0,1, 1,0,1,0,1, 1,1,1,1,1)
num(4) = list(1,1,1,0,0, 0,0,1,0,0, 1,1,1,1,1)
num(5) = list(1,1,1,0,1, 1,0,1,0,1, 1,0,1,1,1)
num(6) = list(1,1,1,1,1, 1,0,1,0,1, 1,0,1,1,1)
num(7) = list(1,0,0,0,0, 1,0,0,0,0, 1,1,1,1,1)
num(8) = list(1,1,1,1,1, 1,0,1,0,1, 1,1,1,1,1)
num(9) = list(1,1,1,0,0, 1,0,1,0,0, 1,1,1,1,1)

def drawLeds(x,y,l)
	i=0
	it = iterator(l)
	while move_next(it)
		row = i mod 5
	    col = i/5 mod 3
		meta(get(it),x+col*8,y+row*8)
		i= i+1
	wend
enddef

def draw()
	cls(0)
	line(160,0,160,200,1,1)
	drawLeds(133,5,num(frame/600 mod 10))
	drawLeds(162,5,num(frame/60 mod 10))	
	
enddef