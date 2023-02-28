print "Pads";
nPads = 0
for p = 0 to 8
    if pad.isaviable(p) <> 0 then     
        print pad.name(p);
        nPads = nPads + 1
    endif
next

print intToText("Found %i ", nPads);
setfontspacing(-1)
def drawPad(id)
    x = 10
    y = 20
    col = 1
    for b = 1 to 17
        if pad.Down(id,b) then 
            col = 6
            colT = 3
        else 
            col = 1
            colT = 0
        endif
        draw.circle(x + (b - 1) * 18,y + id * 40, 8, 1,col)
        draw.circle(x + (b - 1) * 18,y + id * 40, 8, 0, 2)
        draw.text(intToText("%02i",b), x + (b - 1) * 18 - 7,y + id * 40-4, 1,colT)
    next
        
enddef

def draw()
    cls(0)
    for p = 0 to nPads - 1
        drawpad(p)
    next
    draw.text(intToText("Last Pressed: %02i",pad.get()), 0,0, 1, 3)
enddef