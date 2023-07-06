def isHover(x,y,w,h)
    return mouseX > x AND mouseY > y AND mouseX < x + w AND mouseY < y + h
enddef

def button(x,y,txt)
    colB = 1
    colH = 2
    hover = isHover(x,y,40,14)

    if hover then 
        colB = 9
        colH = 3 
        if mouse.down() then 
            colH = colB
            colB = 1
        endif
    endif

    rect(x,y,40,14,0,colB)
    rect(x,y,40,14,1,colH)
    xc = 20 - (measureText(txt,1) * 0.5)
    text(txt,x + xc,y+3,1,colH)

    return (mouse.released(0) AND hover)
enddef

let a = 99

def draw()
	cls(0)
	line(160,0,160,200,1,4)	
	line(0,100,320,100,1,4)
	
	if (button(10,10, "Add")) then a = a + 1	
	if (button(10,30, "Sub")) then a = a - 1
	text(inttotext("Score:%03i", a), 170,10,2,13)
	
	meta(0,mousex,mousey)
enddef