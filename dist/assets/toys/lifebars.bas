'zelda style
def drawHearts(x,y,size,fill)
	for v = 0 to size-1
		if v/size < fill then
			sprite(1,(v*8)+x,y,6)
		endif	
		sprite(0,(v*8)+x,y,3)	
	next
enddef

'bar style
def drawBar(x,y,size,fill)
	col = 4
	if fill < 0.3 then col = 6 endif
	rect(x,y,size*fill,6,0,col)
	rect(x,y,size,6,1,3)
enddef

'masking
def drawMask(x,y,size,fill)
	rect(x,y+1,size*8,6,0,1)
	rect(x,y+1,size*fill*8,6,0,10)
	sprite(2,x,y,0)
	sprite(4,((size-1)*8)+x,y,0)
	for v = 1 to size-2
		sprite(3,(v*8)+x,y,0)
	next
enddef

'meta
def drawMeta(x,y,size,fill)
	for v = 0 to size-1
		id=0
		if v/size > fill then id = 1
		meta(id,v*6+x,y)
	next
enddef

def draw()
	cls(0)
	fill = abs(cos(t)))
	drawHearts(5,5,10,fill)
	drawBar(5,20,80,fill)
	drawMask(5,35,10,fill)
	drawMeta(5,50,13,fill)
	t=t+0.01
enddef