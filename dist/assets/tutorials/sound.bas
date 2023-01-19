sfx.set(0,0)
sfx.set(1,3)
def tick()
    IF key.pressed(32) THEN sfx.play(0) ENDIF
    IF key.released(32) THEN sfx.play(1) ENDIF
enddef
def draw()
    cls(1)
     time = time + delta()
    draw.rect(0,0,320,9,1,0)
    draw.text(textformat("T: %06i",time), 0, 0, 1, 15)
    draw.text(textformat("D: %04i", delta()), 54, 0, 1, 15)



    draw.triangle(mouse.x(), mouse.y(),mouse.x() , mouse.y()+9,mouse.x()+5 , mouse.y()+7,1,15)
enddef