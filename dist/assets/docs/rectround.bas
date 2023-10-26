'Draw commands ONLY WORK inside draw() function
'RECTROUND(x, y, width, height, radius, border, color) -> Draw a rounded rectangle
'if border is 0, the rectangle be filled
'border is outlined

def draw()
	cls(0)
	'		  x    y   w    h   r   b  c
	rectround(110, 40, 100, 40, 20, 3, 6)
enddef