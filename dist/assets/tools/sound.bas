import "assets/tools/ui.bas"

def tick()
    IF key.pressed(32) THEN sfx.play(0) ENDIF  
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
    l = slider(l,150,120,41,15)

    c = toogle(c,100,100)

    k = knob(k,200,100)

    drawPalette(180)
    drawmouse()

enddef