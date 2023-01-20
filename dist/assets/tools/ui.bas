colorbutton = 5
colorbuttonHover = 6
colorButtonText = 14
colorButtonTextHover = 2


def drawmouse()
    draw.triangle(mouse.x(), mouse.y(),mouse.x() , mouse.y()+9,mouse.x()+5 , mouse.y()+7,1,15)
enddef

def button(x,y,txt)
    cb = colorbutton
    ct = colorButtonText
    hover = mouse.x() > x AND mouse.y() > y AND mouse.x() < x + 50 AND mouse.y() < y + 9

    IF hover THEN 
        cb = colorbuttonHover
        ct = colorButtonTextHover 
    ENDIF

    draw.rect(x,y,50,9,1,cb)
    'xc = x + measureText(txt) * 0.5
xc = x +5
    draw.text(txt,xc,y,8,ct)

    return (mouse.released(0) AND hover)
enddef    