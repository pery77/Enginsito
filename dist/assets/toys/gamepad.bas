print "Pads"
nPads = 0
for p = 0 to 8
    if joyisaviable(p) <> 0 then     
        print joyname(p);
        nPads = nPads + 1
    endif
next
print formattext("Found %i ", nPads);
fontspace(-1)

def drawPad(id)
    x = 10
    y = 20
    col = 1
    for b = 1 to 17
        if joyDown(id,b) then 
            col = 6
            colT = 3
        else 
            col = 1
            colT = 0
        endif
        circle(x + (b - 1) * 18,y + id * 40, 8, 0,col)
        circle(x + (b - 1) * 18,y + id * 40, 8, 1, 2)
        text(formattext("%02i",b), x+(b-1)*18-7,y + id*40-4, 1,colT)
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


def tick()
    k = keyget()
    IF k <> 0 THEN 
        print "Key code: " 
        print k;
    ENDIF
enddef
def draw()
    cls(0)
    for p = 0 to nPads - 1
        drawpad(p)
    next
    text(formattext("Last Pressed: %02i",joy_get()), 2, 2, 1, 3)
enddef