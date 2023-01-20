import "assets/tools/ui.bas"

def tick()
    IF key.pressed(32) THEN sfx.play(0) ENDIF  
enddef

def synthKeys()
    y = 120
    size = 70

    for i = 0 to 15
        x = i*20
        draw.rect(x,y,20,size,1,12)
        draw.rect(x,y,20,size,0,0)
    next
    list()
    for i = 0 to 15
        paint = (i <> 3) AND  (i <> 6) AND (i <> 10 ) AND (i <> 13)
        IF paint THEN
            x = i*20
            draw.rect(x+12,y+1,14,size * 0.65,1,0)
            draw.rect(x+12,y+1,14,size * 0.65,0,5)
        ENDIF
    next


enddef


def draw()
    cls(1)
    time = time + delta()
    draw.rect(0,0,320,9,1,0)
    draw.text(textformat("T: %06i",time), 0, 0, 1, 15)
    draw.text(textformat("D: %04i", delta()), 54, 0, 1, 15)
    draw.text(textformat(">: %01i", p), 160, 0, 1, 15)

    IF button (2,16,"play") THEN print sfx.play(0) ENDIF

    IF button (2,28,"square") THEN print sfx.set(0,0) ENDIF
    IF button (2,38,"sawtooth") THEN print sfx.set(0,1) ENDIF
    IF button (2,48,"sine") THEN print sfx.set(0,2) ENDIF
    IF button (2,58,"noise") THEN print sfx.set(0,3) ENDIF

    p = slider(p,100,16,80,99)
    q = slider(q,129,86,8,3)
    l = slider(l,150,40,41,15)

    c = toogle(c,100,100,3)

    k = knob(k,200,100)

    synthKeys()

    IF mouse.down(1) THEN  drawPalette(180) ENDIF
    drawmouse()

enddef