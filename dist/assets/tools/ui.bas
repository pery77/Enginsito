colorbutton = 0
colorbuttonHover = 5
colorButtonText = 12
colorButtonTextHover = 15
buttonW = 50
buttonH = 10

def drawPalette(y)
    for i = 0 to 15
        x = i*20
        draw.rect(x,y,20,10,true,i)
        draw.rect(x,y,20,10,false,5)
        draw.text(textformat("%02i",i), x+4, y+9, 8, 15)
    next
enddef

def drawmouse()
    draw.triangle(mouse.x(), mouse.y(), mouse.x(), mouse.y()+9, mouse.x()+7, mouse.y()+7, 0, 0)
    draw.triangle(mouse.x(), mouse.y(), mouse.x(), mouse.y()+9, mouse.x()+7, mouse.y()+7, 1, 15)
enddef

def isHover(x,y,w,h)
    return mouse.x() > x AND mouse.y() > y AND mouse.x() < x + w AND mouse.y() < y + h
enddef

def button(x,y,txt)
    cb = colorbutton
    ct = colorButtonText
    hover = isHover(x,y,buttonW, buttonH)

    IF hover THEN 
        cb = colorbuttonHover
        ct = colorButtonTextHover 
        IF mouse.down() THEN 
            t = ct
            ct = cb
            cb = colorbutton
        ENDIF
    ENDIF

    draw.rect(x,y,buttonW,buttonH,1,cb)
    draw.rect(x,y,buttonW,buttonH,0,ct)
    xc = (buttonW * 0.5) - (measureText(txt,8) * 0.5)
    draw.text(txt,x + xc,y,8,ct)

    return (mouse.released(0) AND hover)
enddef

dragPos = 0
def slider(x,y,v)
    w = 100
    cb = colorButtonText

    colN = 8
    colNH = 13
    col = colN

    mx = x+v+1
    my = y+1

    hover = isHover(x,y,w,buttonH)
    IF hover THEN
        col = colNH
        cb = colorButtonTextHover
        v=v+mouse.wheel();
    ENDIF

    IF mouse.pressed(0) AND hover THEN
        dragPos = mouse.x() - mx
        print(dragPos);
    ENDIF

    IF mouse.down(0) AND hover THEN
        v = mouse.x() - dragPos - mx
        print(v);
    ENDIF

    IF v < 0 THEN v = 0 ENDIF
    IF v > w THEN v = w ENDIF
    
    draw.rect(x,y,w + 10,buttonH,1,0)
    draw.rect(x,y,w + 10,buttonH,0,cb)
    draw.rect(mx,my,8,8,1,col)
    textPos = textformat("%02i",v)
    draw.text(textPos, x - measureText(textPos ,8)-2, y,8, cb)


    return v
enddef