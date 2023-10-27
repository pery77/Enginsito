'MOUSEPRESSED(button)	-> ' return true when button is pressed  (one frame)
'MOUSERELEASED(button)  -> ' return true when button is released (one frame)
'MOUSEDOWN(button)      -> ' return true while button is down
'MOUSEUP(button)        -> ' return true while key is not pressed

'button -> 0, left, 1 right, 2 middel, 3...

'Mouse_X -> 'position x of mouse 
'Mouse_Y -> 'position y of mouse 
buttonForTest = MOUSE_BUTTON_LEFT 'MOUSE_BUTTON_LEFT = 0, see Documentation Key Codes

def draw()
	cls(0)
	
	if mouseDown(buttonForTest) then 
		kDown = 1
	else 
		kDown = 0
	endif

	if mouseUp(buttonForTest) then 
		kUp = 1
	else 
		kUp = 0
	endif
	
	text("Down: " + str(kDown), 10,10,1,3)
	text("Up:   " + str(kUp), 10,20,1,3)
	
	if mousepressed(buttonForTest)  then print "Pressed"  endif
	if mousereleased(buttonForTest) then print "Released" endif
	
enddef