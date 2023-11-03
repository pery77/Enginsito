let pi = 3.1415

def draw()
	cls(0)
	r=30
	n = frame*0.01 mod 13 + 1
	for a = 0 to n
		an = 2 * pi * a / n + sin(frame*0.1)
		x = r * cos(an)+160
		y = r * sin(an)+100
		circle(x,y,sin(time*0.01)*5+11,2,a)
	next
enddef