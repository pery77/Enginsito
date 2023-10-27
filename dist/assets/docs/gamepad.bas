'JOYISAVIABLE(id)           ->  return true if the gamepad id is ok
'JOYNAME(id)                ->  return the name of the gamepad id
'JOYPRESSED(id, button)     ->  return true when button is pressed  (one frame)
'JOYRELEASED(id, button)    ->  return true when button is released (one frame)
'JOYDOWN(id, button)        ->  return true while button is down
'JOYUP(id, button)          ->  return true while key is not pressed
'JOYGET()                   ->  return the last joy button pressed
'JOYAXISCOUNT(id)           ->  return how many axis the joy have
'JOYAXISVALUE(id, axisId)   ->  return axis value from -100 to 100


print "Gamepads"
nPads = 0
for p = 0 to 8
    if joyIsAviable(p) <> 0 then     
        print joyName(p);
        nPads = nPads + 1
    endif
next
print formatText("Found %i ", nPads);
fontSpace(-1)

def drawPad(id)
    x = 10
    y = 20
    y = y +id * 90
    col = 1
    for b = 1 to 17
        if joyDown(id,b) then 
            col = 6
            colT = 3
        else 
            col = 1
            colT = 0
        endif
        circle(x + (b - 1) * 18,y, 8, 0,col)
        circle(x + (b - 1) * 18,y, 8, 1, 2)
        text(formattext("%02i",b), x+(b-1)*18-7,y-4, 1,colT)
    next
    axCount = joyAxisCount(id)

    text(formattext("Axis Count: %i", axCount), x-4, y+10, 1, 3)
    for ax = 0 to axCount - 1
        av = joyAxisValue(id,ax)
        text(formattext("Axis %02i: %03i",ax, av), x,y + ax * 10 + 20, 1, 3)
        rect(x+100, y + ax * 10 + 20, 190, 6, 0, 2)
        rect(x+100, y + ax * 10 + 20, 190 * ((av + 100)/200.0) , 6, 0, 6)
    next
        
enddef

def draw()
	cls(0)
	for p = 0 to nPads - 1
		drawpad(p)
	next
	
	if nPads > 0 then
		text(formattext("Last Pressed: %02i",joyget()), 2, 2, 1, 3)
	else
		text("No Gamepads found", 2, 2, 1, 3)
	endif
	
enddef