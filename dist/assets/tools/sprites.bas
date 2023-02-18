import "assets/lib/ui.bas"

pixelSize = 16
pixelList = list(0 to 63)
for i = 0 to 63
	pixelList(i) = 0
next
def getBinary(row)
    result = 0;
    counter = 7;
    for i = 0 to 7
        bit = 0
        IF pixelList(i+row*8) <> 0 THEN
            bit = 1
        ENDIF
        result = result + (bit * (2^counter))
        counter = counter - 1
    next
    return result
enddef

def setBinary(selectedSprite)    
    for l = 0 to 7
        g = getSpriteByte(selectedSprite,l)
        while g > 0
            pixelList((7 - counter) + l*8) = 0
            color = 0
            if (g MOD 2) <> 0 then color = 15 endif
            pixelList((7 - counter) + l*8) = color
            g = floor(g / 2)
            counter = counter + 1
        wend
        while counter < 8
            pixelList((7 - counter) + l*8) = 0
            counter = counter + 1
        wend
        counter = 0
    next
enddef

'def makeprites()
'    for sp = 0 to 255
'        setSprite(sp,255,0,sp,sp,sp,sp,0,255)
'        setSprite(sp,0,0,0,0,0,0,0,0)
'    next
'enddef
'
'makeprites()
'renderSprites()

selectedSprite = 0
selectedMeta = 0

def drawSprite(id,x,y)
    col = 8

    x = floor(id MOD 16) * 8 + x
    y = floor(id / 16) * 8 + y

    hover = ui.isHover(x,y,9,9)
    if hover then
        col = 13
        draw.rect(x,y,8,8,0,5)
        if mouse.pressed(0) then
            selectedSprite = id
        endif
    endif
    
    if id = selectedSprite then
        col = 13
        draw.rect(x,y,8,8,0,1)
    endif

    draw.sprite(id, x , y, col)

enddef

def drawSheet(x,y)
    draw.rect(x,y,128,128,0,0)
    for sp = 0 to 255
        drawsprite(sp,x,y)
    next

    draw.text(intToText("%03i" ,selectedSprite),136,4,1,11)

enddef

DEF pixelCanvas(x, y, offSetX, offsetY)
    color = 2
    id=offSetX+(offSetY*8)

    x = x + offSetX*pixelSize;
    y = y + offSetY*pixelSize

    hover = ui.isHover(x, y, pixelSize, pixelSize)
    IF hover THEN
        color = 11
    ENDIF

    IF mouse.down(0) and hover THEN
        draw.rect(x,y,pixelSize,pixelSize,0,selectedColor)
        pixelList(id) = 15
    ENDIF
    IF mouse.down(1) and hover THEN
        draw.rect(x,y,pixelSize,pixelSize,0,selectedColor)
        pixelList(id) = 0
    ENDIF

    draw.rect(x,y,pixelSize,pixelSize,0,pixelList(id))
    draw.rect(x,y,pixelSize,pixelSize,1,color)

    IF mouse.down(2) THEN
        draw.text(intToText("%i",id),x+2,y+2,1,7)
    ENDIF


ENDDEF

DEF drawCanvas(x,y)
    draw.rect(x-2, y-2, pixelSize * 8+4, pixelSize*8+4, 2, 4)
    FOR i = 0 to 7
        FOR j = 0 to 7
            pixelCanvas(x, y, j, i)
        NEXT
    NEXT

    FOR j = 0 to 7
        draw.text(intToText("%03i",getBinary(j)),x+132,4 + y + j * 16,1,11)
    NEXT

ENDDEF

DEF metaSprite(id, s, x, y,col,flag)
    draw.sprite(s, x, y, col, flag)
    draw.text(intToText("%i",id),x+12,y,1,15)
    col2 = ui.colorPiker(x+42,y+1, col)
    if col2 <> col then
        addmetasprite(selectedMeta,id,s,0,0,col2,flag)
    endif
    if(ui.button(x+20,y,intToText(">%i",id))) then
        addmetasprite(selectedMeta,id,selectedSprite,0,0,rnd(0,15),0)
    endif
ENDDEF

DEF drawMeta(x,y)
    draw.rect(x-2, y-2, pixelSize * 8+4, pixelSize*8+4, 0, 0)
    draw.rect(x-2, y-2, pixelSize * 8+4, pixelSize*8+4, 2, 4)

    fo = getMetaSprite(selectedMeta)
    if (fo) then
        for i = 0 to 7
            xx = 4 + x
            yy = 4 + y + i*10
            sp = i * 5
            s = fo(sp)
            col = fo(sp+3)
            flag = fo(sp+4)
            metaSprite(i,s,xx,yy,col,flag)
        Next
    endif
ENDDEF

'setSprite(0,255,128,191,176,160,160,160,160)
'renderSprites()

'id, postition, sprite_id, offset_x,  offset_y,  color, flags
'addmetasprite(0,0,0,0,0,4,0)
'addmetasprite(0,1,0,8,0,11,1)
'addmetasprite(0,2,0,8,8,7,2)
'addmetasprite(0,3,0,0,8,6,3)
'addmetasprite(0,7,200,201,202,16,3)


ui.buttonW = 20
DEF draw()
    cls(1)
    drawSheet(4,4)
    IF meta THEN
        drawMeta(164,4)
        selectedMeta = ui.slider(selectedMeta,30,140,255,255)
        
    ELSE 
        drawCanvas(164,4)
        if ui.button(138,36, "<") then
            setSprite(selectedSprite, getBinary(0),getBinary(1),getBinary(2),getBinary(3),getBinary(4),getBinary(5),getBinary(6),getBinary(7))
            renderSprites()
            updatefont()
        endif
    ENDIF

    if ui.button(138,16, ">") then
        setBinary(selectedSprite)
    endif
    
    meta = ui.toogle(meta,140,56,4)

    draw.meta(selectedMeta,30,176)
    xstep = ui.slider(xstep,30,152,8,4)
    ystep = ui.slider(ystep,30,164,8,4)
    for x = 0 to 15
        for y = 0 to 3
            draw.meta(selectedMeta,x*xstep*8 + 72 ,y*ystep*8 + 156)
        next
    next
    
    IF mouse.down(1) THEN  ui.drawPalette() ENDIF
    ui.drawmouse()


ENDDEF