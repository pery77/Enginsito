'Draw commands ONLY WORK inside draw() function
'POLY(x, y, sides, radius, rotation, thickness, color) -> Draw a polygon

def draw()
	cls(0)
	'    x    y    s  rad  rot t   col
	poly(160, 100, 5, 100, 0,  16, 8)
enddef