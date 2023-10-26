'Draw commands ONLY WORK inside draw() function
'TRIANGLE(x1, y1, x2, y2, x3, y3, style, color) -> Draw a triangle
'if style is 0, the triangle be filled

def draw()
	cls(0)
	'        x1   y1  x2   y2   x3   y3   st col
	triangle(160, 80, 130, 130, 190, 130, 0, 7)
enddef