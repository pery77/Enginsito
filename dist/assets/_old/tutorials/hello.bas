setColor(0,3,3,3)
setColor(3,250,250,250)
def draw()
    cls(0)
    text("Hello", 140,10,1,3)
    poke(9,frame)
    poke(10,frame * 0.5)
    poke(11,frame * 0.25)
    'frame = frame + 2
    if frame > 255 then frame = 0 endif
    meta(0,100,100)
    meta(1,mouse.x(),mouse.y())

    'draw.rect(20,20,MOUSE.x(),mouse.y,101,rnd(0,15))
    text("stress test",20 + rnd(-3,3),34,3,3)
    text("Press space for the party",50,84,1,3)
    if key.down(32) then
        for p = 0 to 160
            for j = 0 to 100
                    pixel(p*2,j*2,rnd(0,15))
                    pixel(p*2-1,j*2-1,5)
            next
        next
    endif
enddef

