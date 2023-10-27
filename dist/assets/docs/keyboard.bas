'KEYPRESSED(key)    -> ' return true when key is pressed  (one frame)
'KEYRELEASED(key)   -> ' return true when key is released (one frame)
'KEYDOWN(key)       -> ' return true while key is down
'KEYUP(key)         -> ' return true while key is not pressed

keyForTest = KEY_W 'KEY_W = 87, see Documentation Key Codes

def draw()
	cls(0)
	
	if keyDown(keyForTest) then 
		kDown = 1
	else 
		kDown = 0
	endif

	if keyUp(keyForTest) then 
		kUp = 1
	else 
		kUp = 0
	endif
	
	text("Down: " + str(kDown), 10,10,1,3)
	text("Up:   " + str(kUp), 10,20,1,3)
	
	if keypressed(keyForTest)  then print "Pressed"  endif
	if keyreleased(keyForTest) then print "Released" endif
	
enddef