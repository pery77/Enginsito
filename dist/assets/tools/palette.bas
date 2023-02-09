def load(file)
    if file = 0 then
        import "assets/lib/palettes/gray.bas"
    endif
    if file = 1 then
        import "assets/lib/palettes/pico.bas"
    endif
    if file = 2 then
        import "assets/lib/palettes/test.inc"
    endif

enddef



import "assets/lib/ui.bas"

ui.colorbase = 1
ui.colorBaseHover = 5
ui.colorHi = 15
ui.colorHiHover = 14
ui.buttonW = 80

selectedColor = 0

redK = getColor(0,0)
greenK = getColor(0,1)
blueK = getColor(0,2)

def colorBox(id)
    x=id*20
    y = 24
    hover = ui.isHover(x,y,19,16)
    IF hover AND mouse.pressed(0) THEN
        selectedColor = id
        redK = getColor(id,0)
        greenK = getColor(id,1)
        blueK = getColor(id,2)
    ENDIF
    
    draw.rect(x,y,19,16,0,id)

    IF id = 0 THEN
        draw.rect(x,y,19,16,1,1)
    ENDIF
    
    IF selectedColor = id THEN
        draw.rect(x,y,19,16,1,15)
    ENDIF

    draw.font(intToText("%02i",id), x+4, y+20, 1, 15)
enddef

def colorBoxSelector(y)
    draw.font(intToText("Color: %02i",selectedColor), 2, y+1, 1, 15)
    draw.rect(78,y,80,10,1, 15)
    draw.rect(79,y+1,78,8,0, selectedColor)
    
    for id = 0 to 15
        colorBox(id)
    next
enddef

def files(x,y)
    path$ = "lib/palettes"
    fo = getFolders(path$)
    if (fo) then
        for i = 0 to len(fo)-1
            print fo(i);
        Next
    endif

    f = getFiles(path$)
    if (f) then
        for i = 0 to len(f)-1
            IF ui.button (x,y+i*10,f(i)) THEN
                load(f(i))
            ENDIF
        Next
    endif
enddef

def drawKnobs(x,y)
    sp = 38
    redK = ui.knob(redK,x,y, 0,255)
    greenK = ui.knob(greenK,x+sp,y, 0,255)
    blueK = ui.knob(blueK,x+sp+sp,y,0,255)
    draw.font("RED",x-12,y+18,1,15)
    draw.font("GREEN",x-20+sp,y+18,1,15)
    draw.font("BLUE",x-14+sp+sp,y+18,1,15)
enddef

def draw()
    cls(0)
    colorBoxSelector(4)

    'files(4,62)

    IF ui.button (168,3,"Restore") THEN restorePalette() ENDIF
    'IF ui.button (4,62,"PicoPal") THEN setPicoPalette() ENDIF

drawKnobs(18,88)

    IF mouse.down(0) AND ui.mouseWorking THEN
        setColor(selectedColor, redK, greenK,blueK)
    ENDIF


     IF ui.button (200,180,"Save") THEN
        t$ = ""
        for col = 0 to 15
            r = getColor(col,0)
            g = getColor(col,1)
            b = getColor(col,2)
            t$ = t$ + intToText("setcolor(%i,%i,%i,%i)", col,r,g,b) + Chr(13)
        next
        saveFile("assets/lib/palettes/test.inc", t$)
    ENDIF
    IF ui.button (20,170,"load") THEN
        load(2)
    
    endif
  
    
    IF mouse.down(2) THEN ui.drawPalette() ENDIF
    ui.drawmouse()
    
enddef