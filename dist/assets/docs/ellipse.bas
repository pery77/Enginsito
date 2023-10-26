'Draw commands ONLY WORK inside draw() function
'ELLIPSE(x, y, radiusHorizontal, radiusVertical, style, color) -> Draw a ellipse
'if style is 0, the ellipse be filled

def draw()
	cls(0)
	'       x   y   rH rV s col
	ellipse(160,100,30,50,0,2)
	ellipse(160,100,50,20,1,1)
enddef