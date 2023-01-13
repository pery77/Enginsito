time = 0
l = list()

def init()
enddef


DEF tick()

    k = key.get()
    IF k <> 0 THEN 
        push(l,k)
        'IF k <> 32 then sound.note(k,26,100) endifh
        IF len(l)>5 THEN remove(l,0) ENDIF
    ENDIF
    m = mouse.wheel()
    IF m <> 0 THEN
        push(l, m)
    endif

    IF key.released(32) THEN sound.music("C C DDD   FF", 19) ENDIF
    IF key.pressed(72) THEN sound.note(60,20,100) ENDIF
    IF key.pressed(74) THEN sound.note(60,36,100) ENDIF

ENDDEF


def draw()
    time = (time + delta()) mod 1000
    'IF time < 100 THEN
        cls(0)
    'ENDIF
   IF 0 THEN
    for j = 0 to 11
        for i = 0 to 25
        x=j*32
        y=i*8
           draw.rect(x ,y ,10 ,8, 1,i+(j*25))
           draw.text(textformat("%03i",i+(j*25)), x+12 ,y-1 , 1, 255)
        next
    next
    ENDIF
    x=0
    i = iterator(l)
    while move_next(i)
        draw.text(textformat("%03i",get(i)), 32, x*8+8, 1, 234)
        x=x+1
    wend

    draw.rect(0,0,319,9,1,175)
    mb = 2
    draw.text(textformat("%06i",time), 0, 0, 1, 236)
    draw.text(textformat("%03i",mouse.up(mb)), 0, 8, 1, 175)
    draw.text(textformat("%03i",mouse.down(mb)), 0, 16, 1, 175)
    draw.text(textformat("%03i",mouse.pressed(mb)), 0, 24, 1, 175)
    draw.text(textformat("%03i",mouse.released(mb)), 0, 32, 1, 175)
    draw.text(textformat("%03i",mouse.wheel()), 0, 40, 1, 175)

goto no;
    for n = 0 to 1
        draw.pixel(rnd(0,319), rnd(0,199), rnd(0,255))
        draw.line(rnd(0,319), rnd(0,199), rnd(0,319), rnd(0,199),rnd(1,3),rnd(0,255))
        draw.circle(rnd(0,319), rnd(0,199), rnd(1,20), rnd(0,1), rnd(0,255))
        draw.ellipse(rnd(0,319), rnd(0,199), rnd(1,20), rnd(1,20),rnd(0,1), rnd(0,255))
        x = rnd(0,319)
        y = rnd(0,199)
        draw.triangle(x, y, x + rnd(-10,10), y + rnd(-20,20), x + rnd(-20,20), y + rnd(-20,20), rnd(0,1), rnd(0,255))

    next
    
    draw.circle(mouse.x(), mouse.y(),mouse.x()*0.1,1, 201+mouse.pressed(1)*20)
no:
    'draw.text(textformat("mouse: %03i x ",mouse.x()) + textformat("%03i",mouse.y()), 0 , 0 , 1, 2)
    'draw.rect(mouse.x(), mouse.y(),319,199,8,0)
    'draw.rect(0,0,mouse.x(), mouse.y(),6,0)
    'draw.text(textformat("%04f",7/22), 0 , 16 , 1, 26)
    'draw.rect(0,0,320,200,2,0)

enddef

def end()
    print "end";
enddef