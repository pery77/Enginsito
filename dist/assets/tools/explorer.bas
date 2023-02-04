import "assets/lib/ui.bas"

setColor(0,10,10,10)
setColor(13,254,190,25)
setColor(14,7,160,194)
setColor(15,56,212,248)

def drawFile(name, x, y)
    col = 14
    hover = ui.isHover(x,y,100,8)
    if hover then 
        col = 0 
        draw.rect(x,y-1,100,10,0,13)
    endif
    draw.font(name,x+2,y,8,col)
enddef

def files(x,y)
    path$ = "tools"
    fo = getFolders(path$)
    if (fo) then
        for i = 0 to len(fo)-1
            print fo(i);
        Next
    endif

    f = getFiles(path$)
    if (f) then
        for i = 0 to len(f)-1
            drawFile(f(i), x,y+i*10)
        Next
    endif
enddef

def draw()
    cls(0)
    draw.rect(0,0,319,199,1,15)
    draw.rect(1,1,317,8,0,14)
    draw.font("00:00",2,1,8,0)

    files(4,16)

    ui.drawmouse()

enddef