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
    axCount = pad.AxisCount(id)
    draw.text(intToText("Axis Count: %i", axCount), x - 4, y + 10, 1, 3)
    for ax = 0 to axCount - 1
        av = pad.AxisValue(id,ax)
        draw.text(intToText("Axis %02i: %03i",ax, av), x,y + ax * 10 + 20, 1, 3)
        draw.rect(x+100,y + ax * 10 + 20,190,6,0,2)
        draw.rect(x+100,y + ax * 10 + 20, 190 * ((av + 100) /200.0) ,6,0,6)
    next
        
enddef

def draw()
    cls(0)
    for p = 0 to nPads - 1
        drawpad(p)
    next
    draw.text(intToText("Last Pressed: %02i",pad.get()), 0,0, 1, 3)
    
enddef