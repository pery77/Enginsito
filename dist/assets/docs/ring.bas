'Draw commands ONLY WORK inside draw() function
'Draw a Ring
'RING(x, y, width, height, radiusIn, radiusOut, startAngle, endAngle, segments, style, color)
'if segment =< minSeg then segment are automatic
'minSeg is (endAngle - startAngle)/90
'style 0 filled, other border

def draw()
	cls(0)
	'	 x    y    rI  rO  sA   eA   seg st col
	ring(160, 100, 20, 28, 270, 90,  2,  1, 4)
	ring(160, 90,  40, 45,   0, 360, 8,  0, 6)
	ring(160, 90,  60, 140,  0, 360, 0,  0, 1)
enddef