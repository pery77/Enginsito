'Draw commands ONLY WORK inside draw() function
'META(id, x, y, [flags]) -> Draw a meta-sprite
'id -> id of meta-sprite, x, y position

'flags (optional)
'	rotation	0  1   2    3
'				0º 90º 180º 270º
'	flip horizontal + 8
'	flip vertical   + 16

def draw()
	cls(0)
	meta(0, 20, 20,)
	meta(0, 28, 20, 1)
	meta(0, 28, 28, 2)
	meta(0, 20, 28, 3)
	
	meta(1, 40, 20)
enddef