'Draw commands ONLY WORK inside draw() function
'CIRCLE(x, y, radius, border, color) -> Draw a rectangle
'if border is 0, the circle be filled

def draw()
	cls(0)
	circle(50,40,20,0,2)
	circle(100,40,20,1,6)
	circle(150,40,20,10,4)
enddef