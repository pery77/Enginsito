import "assets/lib/ui.bas"

class instrument
    osc = 0
    envA = 4
    envD = 1
    envS = 255
    envR = 5
    envAM =255
    lfoN = 0
    lfoA = 0
endclass

def mario()
    m1 = "T100 @1 v30 >>e16-e8-e8-c16-e8-g4-<g4>-c8.<g8.e8.a8b8a+16a8g16.>e16g16.a8f16g8e8c16d16<b8.>c8.<g8.e8.a8b8a+16a8g16.>e16g16.a8f16g8e8c16d16<b4&b16>g16f+16f16d+8e8<g+16a16>c8<a16>c16d8.g16f+16f16d+8e8>c8c16c4.<g16f+16f16d+8e8<g+16a16>c8<a16>c16d8.d+8.d8.c2&c8g16f+16f16d+8e8<g+16a16>c8<a16>c16d8.g16f+16f16d+8e8>c8c16c4.<g16f+16f16d+8e8<g+16a16>c8<a16>c16d8.d+8.d8.c2"
    m2 = "T100 @0 v100 <d16d8d8d16d8g4<g4>g8.e8.c8.f8g8f+16f8c16.>c16e16.f8d16e8c8<a16b16g8.g8.e8.c8.f8g8f+16f8c16.>c16e16.f8d16e8c8<a16b16g8.c8.g8.>c8<f8.>c16c16c16<f8c8.e8.g16>c4.&c16<g8c8.g8.>c8<f8.>c16c16c16<f8c8g+8.a+8.>c8.<g16g8c8c8.g8.>c8<f8.>c16c16c16<f8c8.g8.g16>c4.&c16<g8c8.g8.>c8<f8.>c16c16c16<f8c8g+8.a+8.>c8."
    m3 = "T100 @3 v30 >>e16-e8-e8-c16-e8-g4-<g4>-c8.<g8.e8.a8b8a+16a8g16.>e16g16.a8f16g8e8c16d16<b8.>c8.<g8.e8.a8b8a+16a8g16.>e16g16.a8f16g8e8c16d16<b4&b16>g16f+16f16d+8e8<g+16a16>c8<a16>c16d8.g16f+16f16d+8e8>c8c16c4.<g16f+16f16d+8e8<g+16a16>c8<a16>c16d8.d+8.d8.c2&c8g16f+16f16d+8e8<g+16a16>c8<a16>c16d8.g16f+16f16d+8e8>c8c16c4.<g16f+16f16d+8e8<g+16a16>c8<a16>c16d8.d+8.d8.c2"
    m4 = "T100 @5 v70 <<d16d8d8d16d8g4<g4>g8.e8.c8.f8g8f+16f8c16.>c16e16.f8d16e8c8<a16b16g8.g8.e8.c8.f8g8f+16f8c16.>c16e16.f8d16e8c8<a16b16g8.c8.g8.>c8<f8.>c16c16c16<f8c8.e8.g16>c4.&c16<g8c8.g8.>c8<f8.>c16c16c16<f8c8g+8.a+8.>c8.<g16g8c8c8.g8.>c8<f8.>c16c16c16<f8c8.g8.g16>c4.&c16<g8c8.g8.>c8<f8.>c16c16c16<f8c8g+8.a+8.>c8."

    music.set(0,m1)
    music.set(1,m2)
    music.set(2,m3)
    music.set(3,m4)
    music.set(4,"")
enddef

def tetris()
    t1 = "t130 @1 v60 >e4<b8>c8d8e16d16c8<b8a4a8>c8e4d8c8<b4.>c8d4e4c4<a4a2r8>d4f8a4g8f8e4.c8e4d8c8<b4b8>c8d4e4c4<a4a4r4"
    t2 = "t130  v80 o2e8>e8<e8>e8<e8>e8<e8>e8<a8>a8<a8>a8<a8>a8<a8>a8<g+8>g+8<g+8>g+8<e8>e8<e8>e8<a8>a8<a8>a8<a8>a8<b8>c8d8<d8r8d8r8d8a8f8c8>c8r8c8<c8g8g8r8b8>b8r8b8r8e8r8g+8<a8>e8<a8>e8<a4r4"

't1 = "t90l16ga+>dd+<ga+>dd+<ga+>dd+<ga+>dd+<f+a>dd+<f+a>dd+<f+a>dd+<f+a>dd+<fg+>dd+<fg+>dd+<fg+>dd+<fg+>dd+<eg>dd+<eg>dd+<eg>dd+<eg>dd+<d+g>cd<d+g>cd<d+g>cd<d+g>cd<dg>cd<dg>cd<dg>cd<dg>cd<cf+ab+f+a>cd+<a>cd+cd+f+d+f+af+ab+g4"
't2 = "o5<g2a+4>d4c+4<f+2.f2&fg+4>c+8c4<e2.l16d+dd+4.g8.>d+8.d8<dc+d4.g8.>d8.c+8<df+af+ab+a>cd+cd+f+af+d+cd+c<af+g4"
    t1 = "t120l8o5e1o2abo3cdece4d#o2bo3d#4do2a#o3d4o2abo3cdeceageceg4r4o3abo4cdece4d#o3bo4d#4do3a#o4d4o3abo4cdeceageceg4r4ef#g#abg#b4o5co4g#o5c4o4bg#b4ef#g#abg#b4o5co4g#o5c4o4b4r4ef#g#abg#b4o5co4g#o5c4o4bg#b4ef#g#abg#b4o5co4g#o5c4o4b4r4abo5cdece4d#o4bo5d#4do4a#o5d4o4abo5cdeceageceg4r4o5abo6cdece4d#o5bo6d#4do5a#o6d4o5abo6cdeceag#eg#ba4r4"
    t2 = "e1l4o1ao2eo1ao2eo1ao2eo1ao2eo1ao2eo1ao2ecgcgo1ao2eo1ao2eo1ao2eo1ao2eo1ao2eo1ao2ecgcgebebeo3co2ebebebeo3co2ebebebeo3co2ebebebeo3co2ebao3eo2ao3eo2ao3eo2ao3eo2ao3eo2ao3ecgcgo2ao3eo2ao3eo2ao3eo2ao3eo2a4r4r2r2r4o1a4"
    music.set(0,t1)
    music.set(1,t2)
    music.set(2,"")
    music.set(3,"")
    'music.set(4,"t130 @4 v80 o2c8r8c8r8")
enddef

'music.stop()
'music.play()

OSC_SINE = 0;
OSC_SQUARE50 = 1;
OSC_SQUARE25 = 2;
OSC_SQUARE12 = 3;
OSC_TRIANGLE = 4;
OSC_SAW_DIG = 5;
OSC_NOISE = 6;

def kettonote(k,o)
    IF key.pressed(k) THEN
        music.osc(0,o)
        music.note(0,69,50)
    endif
    if key.released(k) then
        music.noteoff(0)
    endif
enddef

def tick()
    k = key.get()
    IF k <> 0 THEN
        print k;
    ENDIF

    kettonote(49,OSC_SINE)
    kettonote(50,OSC_SQUARE50)
    kettonote(51,OSC_SQUARE25)
    kettonote(52,OSC_SQUARE12)
    kettonote(53,OSC_TRIANGLE)
    kettonote(54,OSC_SAW_DIG)
    kettonote(55,OSC_NOISE)

enddef

instruments = list(0, 1, 2, 3)
set(instruments, 0, new (instrument))
set(instruments, 1, new (instrument))
set(instruments, 2, new (instrument))
set(instruments, 3, new (instrument))

def drawChan(ch,x,y)
    inst = get(instruments, ch);
    inst.osc = ui.slider(inst.osc, x-36, y-4, 6, 6)
    inst.envA  = ui.knob(inst.envA, x, y, 0, 255)
    inst.envD  = ui.knob(inst.envD, x + 32, y, 0, 255)
    inst.envS  = ui.knob(inst.envS, x + 64, y, 0, 255)
    inst.envR  = ui.knob(inst.envR, x + 96, y, 0, 255)
    inst.envAM = ui.knob(inst.envAM,x + 130, y, 0, 255)
    inst.lfoN  = ui.knob(inst.lfoN, x + 170, y, 0, 255)
    inst.lfoA  = ui.knob(inst.lfoA, x + 202, y, 0, 255)
    music.osc(ch,inst.osc)
    music.env(ch,inst.envA, inst.envD, inst.envS, inst.envR, inst.envAM)
    music.lfo(ch,inst.lfoN, inst.lfoA)
enddef

def draw()
    cls(1)
    time = time + delta()

    draw.rect(0,0,320,9,0,3)
    draw.text(intToText("Time:%06i",time), 1, 1, 1, 0)
    draw.text(intToText("Tick :%04i",music.Tick()), 128, 1, 1, 0)
    draw.text(intToText("Pos:%04i/(%04i)",music.position(0), music.size(0)), 128, 10, 1, 5)
    draw.text(intToText("Pos:%04i/(%04i)",music.position(1), music.size(1)), 128, 20, 1, 5)
    draw.text(intToText("Pos:%04i/(%04i)",music.position(2), music.size(2)), 128, 30, 1, 5)
    draw.text(intToText("Pos:%04i/(%04i)",music.position(3), music.size(3)), 128, 40, 1, 5)
    draw.text(intToText("Pos:%04i/(%04i)",music.position(4), music.size(4)), 128, 50, 1, 5)

    drawChan(0,80,120)
    drawChan(1,80,160)

    if ui.button(264,12,"Play") then
        music.play()
    endif
    if ui.button(264,24,"Stop") then
        music.stop()
    endif
    if ui.button(264,45,"Tetris") then
        Tetris()
    endif
    if ui.button(264,55,"Mario") then
        mario()
    endif


    


    IF mouse.down(1) THEN  ui.drawPalette() ENDIF
    IF NOT ui.mouseWorking THEN  ui.drawmouse() ENDIF

enddef
