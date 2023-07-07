def isHover(x,y,w,h)
    return mouse_X > x AND mouse_Y > y AND mouse_X < x + w AND mouse_Y < y + h
enddef

def button(x,y,txt)
    colB = 1
    colH = 2
    hover = isHover(x,y,40,14)

    if hover then 
        colB = 9
        colH = 3 
        if mouse_down(0) then 
            colH = colB
            colB = 1
        endif
    endif

    rect(x,y,40,14,0,colB)
    rect(x,y,40,14,1,colH)
    xc = 20 - (measureText(txt,1) * 0.5)
    text(txt,x + xc,y+3,1,colH)

    return (mouse_released(0) AND hover)
enddef

let a = 99
let oldx = 0
let oldy = 0

def draw()
	cls(0)
	line(160,0,160,200,1,4)	
	line(0,100,320,100,1,4)
	
	if (button(10,10, "Add")) then a = a + 1	
	if (button(10,30, "Sub")) then a = a - 1
	text(inttotext("Score:%03i", a), 170,10,2,13)
	
	'mouse whell add and sub score
	if mouse_x > 160 and mouse_y < 100 then
		rect(160,0,158,100,2,5)
		a = a + mouse_wheel()
	endif
	
	if mouse_x < 160 and mouse_y > 100 then
		mouse_setpos( 20, 20)
	endif
	
	if mouse_pressed(2) then
		oldx = mouse_x
		oldy = mouse_y
	endif
	
	if mouse_down(2) then
		line(oldx,oldy,mouse_x,mouse_y,2,6)
	endif
	
	meta(0,mouse_x,mouse_y)
enddef