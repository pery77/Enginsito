
dim states(112)

def drawBrick(x,y)
    id = x + y * 14
    col = states(id) * 5
    x = x * 16 + 16
    y = y * 8 + 32

    hover = 2
	rect(x+1,y+1,14,6,0,col)
	

    if Mouse_X > x and Mouse_X< 17 + x then
		if Mouse_Y > y and Mouse_Y < y+8 then
            hover = 5
            if mousereleased(0) then 
                states(id) = 1-states(id)
            endif
		endif
	endif
	sprite(5,x,y,hover)
	sprite(5,x+8,y,hover,2)
enddef



def draw()
    cls(1)
    for y = 0 to 7
        st = 0
        f = 1
        for x = 0 to 13
            drawBrick(x,y)
            st = st + states((y*14)+x) * f
            f = f * 2
        next
        text(formatText("%05i",st),248,y*8+32,1,3)
    next

    sprite(17,Mouse_X,Mouse_Y,3)
enddef