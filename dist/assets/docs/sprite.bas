'Draw commands ONLY WORK inside draw() function
'SPRITE(id, x, y, color, [flags]) -> Draw a sprite
'id -> id of sprite, x, y position

'flags (optional)
'	rotation	0  1   2    3
'				0º 90º 180º 270º
'	flip horizontal + 8
'	flip vertical   + 16

def draw()
	cls(0)
	sprite(232, 20, 20,3)
	sprite(232, 28, 20,3, 1)
	sprite(232, 28, 28,4, 2)
	sprite(232, 20, 28,4, 3)
	
	sprite(233, 40, 20, 6) 		'normal
	sprite(233, 48, 20, 6, 8)	'horizontal flip
enddef