import "assets/lib/ui.bas"

def makeprites()
    for sp = 0 to 255
        setSprite(sp,255,0,sp,sp,sp,sp,0,255)
        setSprite(sp,0,0,0,0,0,0,0,0)
    next
enddef

makeprites()
renderSprites()

selectedSprite = 0

def drawSprite(id,x,y)
    col = 8

    x = floor(id MOD 16) * 8 + x
    y = floor(id/16)* 8 + y

    hover = ui.isHover(x,y,9,9)
    if hover then
        col = 13
        draw.rect(x,y,8,8,0,5)
        if mouse.pressed(0) then
            selectedSprite = id
        endif
    endif

    sprite(id, x , y, col)

enddef

def drawSheet(x,y)
    draw.rect(x,y,128,128,0,1)
    for sp = 0 to 255
        drawsprite(sp,x,y)
    next

    draw.font(intToText("%03i" ,selectedSprite),136,4,8,11)

enddef

DEF draw()
    cls(0)
    drawSheet(4,4)
    IF mouse.down(1) THEN  ui.drawPalette() ENDIF
    ui.drawmouse()

ENDDEF