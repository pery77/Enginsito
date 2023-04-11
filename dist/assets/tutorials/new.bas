time = 0
pi = 3.1415
input "asdf, a$
print a$

def draw()
	cls(1)
	r=mouse.x()/5
	for a = 0 to 3
		x = sin(a)*r/pi+160
		y = cos(a)*r/pi+100
		draw.circle(x,y,4,0,6)
	next

enddef


