let radio = 10
let bx = 160
let by = 100

def collision()
	d = sqr(exp((bx - mouse_x)) + exp((by - mouse_y)))
	print d
	if (d<radio) then
		return 1
	endif
	
	return 0
enddef

def draw()
	cls(0)
	
	hit = 3
	
	if collision()=1 then
		hit = 5
	endif
	
	circle(bx,by,radio,2,hit)
	sprite(224, mouse_x,mouse_y, 3)
enddef