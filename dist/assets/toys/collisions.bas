def clamp (v,min,max)
	if v <= min then return min
	if v >= max then return max
	return v
enddef

def collision(rx,ry,rw,rh,cx,cy,cr)
	px = clamp(cx, rx, rx+rw)
	py = clamp(cy, ry, ry+rh)
	dis = sqr((px - cx)^2 + (py - cy)^2)
	return  dis <= cr
enddef

def draw()
	cls(0)

	mx = mouse_x    'circle
	my = mouse_y
	r = 6
	
	x = 120			'rect
	y = 50
	w = 60
	h = 20

	color = 1
	if collision(x, y, w ,h, mx, my, r) then color = 2

	circle(mx, my, r, 0, color + 2)
	rect(x, y, w, h, 1, color)
	text(str(x+w/2-mx),10,10,1,4)

enddef