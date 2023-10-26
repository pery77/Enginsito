'Draw commands ONLY WORK inside draw() function
'RECT(x, y, width, height, border, color) -> Draw a rectangle
'if border is 0, the rectangle be filled

def draw()
	cls(0)
	rect(20,20,80,40,4,2)
	rect(108,20,80,40,0,1)
enddef