'MOUSEWHEEL()		-> ' get the wheel value
'MOUSESETPOS(x, y)  -> ' set the cursor at position x, y

'Mouse_X -> 'position x of mouse 
'Mouse_Y -> 'position y of mouse 

count = 0

def draw()
	cls(0)
	count = count + mouseWheel();
	text("Wheel " + str(count), 10,10,1,3)
	
	if abs(count) > 10 then 'is count is > 10 or < -10 move mouse at center.
		count = 0
		mousesetpos(160,100)
	endif
	
	'Draw a cursor (sprite 225) at mouse position
	sprite(225, mouse_x, mouse_y, 4)
enddef