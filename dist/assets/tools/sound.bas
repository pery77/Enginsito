import "assets/tools/ui.bas"
'sfx.set(0,0)
'sfx.set(1,3)
def tick()
    'IF key.pressed(32) THEN sfx.play(0) ENDIF
    'IF key.released(32) THEN sfx.play(1) ENDIF
    
enddef
p=0
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

    p = slider(100,16,p)
    'q = slider(129,86,q)

    

    drawPalette(180)
    drawmouse()
enddef