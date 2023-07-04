time = 0
l = list()

def init()
enddef


DEF tick()

    k = key.get()
    IF k <> 0 THEN 
        push(l,k)
        'IF k <> 32 then sound.note(k,26,100) endif
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
def drawpalette(o)
    for n = 0 to 16
        draw.rect(n*20, 0+o,16,8,1,n+o)
        draw.rect(n*20, 0+o,16,8,0,10)
        draw.text(textformat("%i",n+o), n*20, 7+o, 1, 9)
    next
enddef

def draw()
    time = (time + delta()) mod 1000
    'IF time < 100 THEN
        cls(0)
    'ENDIF

    for i = 0 to 15
       draw.rect(2 ,i*8 ,10 ,8, 1,i)
       draw.text(textformat("%02i",i), 12 ,i*8 , 1, 30)
    next

    x=0
    i = iterator(l)
    while move_next(i)
        draw.text(textformat("%03i",get(i)), 32, x*8+8, 1, 29)
        x=x+1
    wend

    draw.rect(0,0,319,9,1,6)
    
    mb = 2
    draw.text(textformat("%06i",time), 50, 0, 1, 2)
    draw.text(textformat("%03i",mouse.up(mb)), 50, 8, 1, 11)
    draw.text(textformat("%03i",mouse.down(mb)), 50, 16, 1, 11)
    draw.text(textformat("%03i",mouse.pressed(mb)), 50, 24, 1, 11)
    draw.text(textformat("%03i",mouse.released(mb)), 50, 32, 1, 11)
    draw.text(textformat("%03i",mouse.wheel()), 50, 40, 1, 11)

    for n = 0 to 10
        draw.pixel(rnd(0,319), rnd(0,199), rnd(0,15))
        draw.line(rnd(0,319), rnd(0,199), rnd(0,319), rnd(0,199),rnd(1,3),rnd(0,15))
        draw.circle(rnd(0,319), rnd(0,199), rnd(1,20), rnd(0,1), rnd(0,15))
        draw.ellipse(rnd(0,319), rnd(0,199), rnd(1,20), rnd(1,20),rnd(0,1), rnd(0,15))
        x = rnd(0,319)
        y = rnd(0,199)
        draw.triangle(x, y, x + rnd(-10,10), y + rnd(-20,20), x + rnd(-20,20), y + rnd(-20,20), rnd(0,1), rnd(0,15))
    next
    
    draw.circle(mouse.x(), mouse.y(),mouse.x()*0.1,1, 5+mouse.down(0)*20)
    for i = 0 to 3

next

    draw.text(textformat("mouse: %03i x ",mouse.x()) + textformat("%03i",mouse.y()), 0 , 0 , 1, 11)
    draw.rect(mouse.x(), mouse.y(),319,199,0,3)
    draw.rect(0,0,mouse.x(), mouse.y(),0,9)
    draw.text(textformat("%04f",mosue.x()/mouse.y()), 100 , 16 , 1, 7)
    'draw.rect(0,0,320,200,2,0)
    for i = 0 to 20
    sprite(mouse.x(),i*32,200-32,32);
    next

enddef

def end()
    print "zacabo";
enddef
