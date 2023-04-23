import "assets/lib/ui.bas"

setColor(0,10,10,10)
setColor(13,254,190,25)
setColor(14,7,160,194)
setColor(15,56,212,248)

setSprite(0,8,12,242,129,242,12,8,0) 'arrow
setSprite(1,120,142,191,193,193,130,254,0) 'folder
renderSprites()

def drawFile(name, x, y)
    col = 14
    colSprite = 13
    hover = ui.isHover(x,y,100,8)
    if hover then 
        col = 0 
        colSprite = 0
        draw.rect(x,y-1,100,10,0,13)
    endif
    draw.sprite(0,x+1,y+1,colSprite,0)
    draw.text(name,x+10,y,1,col)
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
    draw.text("00:00",2,1,1,0)

    files(4,16)

    ui.drawmouse()

enddef
