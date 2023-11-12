LOGO = -1
MENU = 0
GAME = 1
WIN = 2
LOSE = 3

PLAY_MUSIC_IN_MENU = false

screen = LOGO

screenStartX = 80
screenStartY = 28

'[ __  __ _    _  _____ _____ _____             _____ ________   __
  |  \/  | |  | |/ ____|_   _/ ____|   ___     / ____|  ____\ \ / /
  | \  / | |  | | (___   | || |       ( _ )   | (___ | |__   \ V / 
  | |\/| | |  | |\___ \  | || |       / _ \/\  \___ \|  __|   > <  
  | |  | | |__| |____) |_| || |____  | (_>  <  ____) | |     / . \ 
  |_|  |_|\____/|_____/|_____\_____|  \___/\/ |_____/|_|    /_/ \_\ ']

def soundStop()
	chstop(0)
	chstop(1)
	chstop(2)
	chstop(3)
enddef

def playMusic()
	pulse = "t100"

	cfg0 = "@8" + pulse + "O2V80"
	cfg_cho1 = "@9" + pulse + "O4V60"
	cfg_cho2 = "@9" + pulse + "O4V60"
	cfg3 = "@10" + pulse + "O4"
	cfg_metro = "@11" + pulse + "O5"

	metronome = "O5[>d32r8^16^32<d32r8^16^32d32r8^16^32d32r8^16^32]64"

	whole_rest = "r1^1"
		
	'
	' --- intro ---
	'

	line_pre = "r1^2c8r8c+8r8"
	chor1a = "r8f+16r8g16r4f+16r8e16r8"
	chor1b = "r8a16 r8a16r4a16 r8a16r8"

	p0_i = cfg0 + line_pre
	p1_i = cfg_cho1 + chor1a + chor1a
	p2_i = cfg_cho2 + chor1b + chor1b

	'
	' --- 1 ---
	'

	line0 = "d16r4^8d16c8r16c16f16r8f16"

	line_trans1_2 = "d16r4^8d16c8r16c16f16r8f+16"

	solo1 = "r8a16 r8a16r4a4 e16f16f+16a16>d16f16f+16r8f+16r8d16r4c16<"
	solo1b = "d16@12r2^4^16e16f16f+16a16>d16f16f+16r8f+16r8d16r4c16"

	p0_1 = cfg0 + "[" + line0 + "]5" + line_trans1_2
	p1_1 = cfg_cho1 + "[" + chor1a + "]6"
	p2_1 = cfg_cho2 +       chor1b + solo1 + cfg3 + solo1b + "<" + chor1b

	'
	' --- 2 ---
	'

	line2 = "g16r4^8g16c16r16c16r16f16r16f+16r16"
	line2b = "g16r4^8g16c16r16c16r16f16r16c16r16"

	chor2a = "f4e4d8^16e16r8^16f16"
	chor2b = "d4c4<b8^16>c16r8^16d16"

	chor2a2 = "r16f16r8e16r16d16d8r4^16^8"
	chor2b2 = "r16d16r8c16r16<b16b8>r4^16e16f16"


	p0_2 = cfg0 + line2 + line2b
	p1_2 = cfg_cho1 + chor2a + chor2a2
	p2_2 = cfg_cho2 + chor2b + chor2b2

	'
	' --- 3 ---
	'

	chor3a3 = "c8^4r4    f+16r8e16r8"
	chor3b3 = "f+8>d4<r4 a16 r8a16r8"

	line0_4 = "d16r4^8d16c8r16c16f16r8<g+16"

	p0_3 = cfg0 + line0 + line0_4
	p1_3 = cfg_cho1 + chor3a3 + chor1a
	p2_3 = cfg_cho2 + chor3b3 + chor1b

	'
	' --- 4 ---
	'

	line4a =    "a32r32  a32r32  a32r32  a16r16  a16r16  g+16r16g+16r16g32r32>g16r8^16"
	chor4_1_a = "c+32r32 c+32r32 c+32r32 c+16r16 c+16r16 c16r16 c16r16<b32r32b16r8^16"
	chor4_2_a = "g32r32  g32r32  g32r32  g16r16  g16r16  g16r16 g16r16 f32r32f16r8^16"

	line4b =    "e32r32  e32r32  e32r32  e16r16  e16r16 <a16 r16a16 r16a32r32a16r8^16"
	chor4_1_b = ">g32r32  g32r32  g32r32  g16r16  g16r16@12g16r16g16f16e16f16g16f16c+16"
	chor4_2_b = "d32r32  d32r32  d32r32  d16r16  d16r16  c+16r16c+16r16c+32r32c+32r8^16^32"

	p0_4 = cfg0 + "O1" + line4a + line4b
	p1_4 = cfg_cho1 + chor4_1_a + chor4_1_b
	p2_4 = cfg_cho2 + chor4_2_a + chor4_2_b

	'
	' --- end ---
	'

	p0_e = cfg0 + "d1"
	p1_e = cfg_cho1 + "r16f+8>e1"
	p2_e = cfg_cho2 + "r8>c1"

	'
	' --- song ---
	'

	p0 = p0_i + "[" + p0_1 + p0_2 + p0_3 + p0_4 + "]2" + p0_e
	p1 = p1_i + "[" + p1_1 + p1_2 + p1_3 + p1_4 + "]2" + p1_e
	p2 = p2_i + "[" + p2_1 + p2_2 + p2_3 + p2_4 + "]2" + p2_e
	p3 = cfg3 + metronome

	chset(0, p0)
	chset(1, p1)
	chset(2, p2)

	chplay(0)
	chplay(1)
	chplay(2)
enddef


def playNintendo()
	chplay(3, "@4O6c32>c6")
enddef


def playJump()
	chplay(3, "@0O5v100c4")
enddef
                              

def playLose()
	soundStop()

	chplay(2, "@1O5V50c8")
	chplay(3, "@2O2V80c1")
enddef


def playWin()
	soundStop()
	
	cfg_cho = "@9t100O4V60"
	
	chplay(0, "@8t100O2V80 c16>c16<c16>d16<a16f+16e2")
	chplay(1, "@9t100O4V60 c16e16g16d16f+16a16b2")
	chplay(2, "@8t100O4V40 e16e16e16f+16f+16f+16g+2")
	chplay(3, "@8t100O4V40 g16g16g16a16a16a+16e2")
enddef

def playSteps()
	stopSteps()
	chplay(3, "@3[g16c16]9999")
enddef

def stopSteps()
	chstop(3)
enddef

'[   _    _ _______ _____ _       _____ 
    | |  | |__   __|_   _| |     / ____|
    | |  | |  | |    | | | |    | (___  
    | |  | |  | |    | | | |     \___ \ 
    | |__| |  | |   _| |_| |____ ____) |
     \____/   |_|  |_____|______|_____/   ']

def checkCollisionAABB(x1, y1, w1, h1, x2, y2, w2, h2)
    xOverlap = (x1 + w1 >= x2) and (x2 + w2 >= x1)
    yOverlap = (y1 + h1 >= y2) and (y2 + h2 >= y1)
    return xOverlap and yOverlap
enddef

metaTile = dict()
metaTile("grass1") = list(23,13,0,25,14,0,24,15,0)
metaTile("grass2") = list(23,13,0)
metaTile("grass3") = list(23,13,0,26,15,0,27,14,0)
metaTile("grass4") = list(23,13,0,26,15,8,27,14,8)
metaTile("dec1") = list(28,14,0,29,13,0)
metaTile("dec2") = list(30,14,0,31,13,0)
metaTile("dec3") = list(32,14,0,33,13,0)
metaTile("dec4") = list(34,14,0,35,13,0)
metaTile("dec5") = list(36,14,0,37,13,0)
metaTile("dec6") = list(38,13,0)
metaTile("dec7") = list(39,13,0)

def drawMeta(tile,posX,posY)
	it = iterator(metaTile(tile))
	while move_next(it)
		id = get(it)
		move_next(it)
		col = get(it)
		move_next(it)		
		flag = get(it)
		sprite(id,posX,posY,col,flag)
	wend
enddef

def drawBackground(background,cameraX,cameraY)
    x = screenStartX + floor(-cameraX/8)*8
    y = screenStartY + floor(-cameraY/8)*8
    
	for i = floor(-cameraY/8) to len(background)-1 step 1
	   for k = floor(-cameraX/8) to len(background(str(i)))-1 step 1
	   j = get(background(str(i)),k)

	   if j = "dec1" or j = "dec2" or j = "dec3" or j = "dec4" or j = "dec5" or j = "dec6" or j = "dec7" then
	    drawMeta(j, x+cameraX, y+cameraY)
	   elseif j = "c" or j = "sh" or j = "sv" or j = "bh" or j = "bv" or j = "g" or j = "go" then
	   
	   elseif NOT (j = 99) then
	    meta(j, x+cameraX, y+cameraY)
	   endif
	   
	   x = x+8
	   
	   if x + cameraX > screenStartX + 160 then
	   		exit	   
	   endif
	   
	   next
	   
	   x = screenStartX + floor(-cameraX/8)*8
	   y = y+8
	   
	   if y + cameraY > screenStartY + 144 then
	   		exit	   
	   endif
	next
enddef

def drawKey(x,y)
	rect(x,y+10,13,6,10,13)
	rect(x,y+11,13,5,1,15)
	rect(x,y,13,12,1,15)	
enddef

def drawWinCharacter(winX,winY)
	sprite(131,winX,winY,13)
	sprite(132,winX,winY+8,13)
	sprite(133,winX+8,winY+8,13)
	sprite(134,winX,winY+16,13)
	sprite(135,winX+8,winY+16,13)
	sprite(136,winX,winY+8,14)
	sprite(137,winX,winY+16,14)
	sprite(138,winX,winY+24,14)
	sprite(139,winX+8,winY+24,14)
	sprite(123,winX,winY,15)
	sprite(124,winX+8,winY,15)
	sprite(125,winX,winY+8,15)
	sprite(126,winX+8,winY+8,15)
	sprite(127,winX,winY+16,15)
	sprite(128,winX+8,winY+16,15)
	sprite(129,winX,winY+24,15)
	sprite(130,winX+8,winY+24,15)
enddef

def drawLogo(x,y)
	sprite(40,x,y,15)
	sprite(41,x+8,y,15)
	sprite(42,x,y+8,15)
	sprite(43,x+8,y+8,15)
	sprite(44,x,y+16,15)
	sprite(45,x+8,y+16,15)
	sprite(46,x,y,14)
	sprite(47,x,y+8,14)
	sprite(59,x,y+16,14)
	sprite(60,x+30,y,15)
	sprite(61,x+30+8,y,15)
	sprite(62,x+30+16,y,15)
	sprite(63,x+30,y+8,15)
	sprite(64,x+30+8,y+8,15)
	sprite(91,x+30+16,y+8,15)
	sprite(92,x+30,y+16,15)
	sprite(93,x+30+8,y+16,15)
	sprite(94,x+30+16,y+16,15)
	sprite(95,x+30,y,14)
	sprite(96,x+30+8,y,14)
	sprite(99,x+30+16,y,14)
	sprite(102,x+30,y+8,14)
	sprite(102,x+30+6,y+8,14)
	sprite(102,x+30+11,y+8,14)
	sprite(102,x+30+19,y+8,14)
	sprite(103,x+30,y+16,14)
	sprite(104,x+30+8,y+16,14)
	sprite(106,x+30+16,y+16,14)
	line(x+54,y+11,x+58,y+11,1,15)
	sprite(107,x+60,y,15)
	sprite(108,x+60+8,y,15)
	sprite(109,x+60+16,y,15)
	sprite(112,x+60+24,y,15)
	sprite(113,x+60+32,y,15)
	sprite(115,x+60+40,y,15)
	sprite(117,x+60,y+8,15)
	sprite(118,x+60+8,y+8,15)
	sprite(119,x+60+16,y+8,15)
	sprite(120,x+60+24,y+8,15)
	sprite(122,x+60+32,y+8,15)	
	sprite(140,x+60+40,y+8,15)
	sprite(141,x+60+48,y+8,15)
	sprite(142,x+60,y+16,15)
	sprite(143,x+60+8,y+16,15)
	sprite(169,x+60+16,y+16,15)
	sprite(170,x+60+24,y+16,15)
	sprite(171,x+60+32,y+16,15)	
	sprite(172,x+60+40,y+16,15)	
	sprite(173,x+60+48,y+16,15)
	sprite(174,x+60,y,14)	
	sprite(175,x+60+8,y,14)
	sprite(218,x+60+16,y,14)
	sprite(219,x+60+32,y,14)
	sprite(220,x+60+40,y,14)
	sprite(102,x+60,y+8,14)
	sprite(221,x+60+8,y+8,14)
	sprite(222,x+60+16,y+8,14)
	sprite(223,x+60+32,y+8,14)
	sprite(231,x+60+40,y+8,14)
	sprite(232,x+60,y+16,14)
	sprite(233,x+60+8,y+16,14)
	sprite(234,x+60+24,y+16,14)
	sprite(235,x+60+40,y+16,14)
	sprite(236,x+60+16,y+16,14)
enddef

'[   _      ____   _____  ____  
    | |    / __ \ / ____|/ __ \ 
    | |   | |  | | |  __| |  | |
    | |   | |  | | | |_ | |  | |
    | |___| |__| | |__| | |__| |
    |______\____/ \_____|\____/   ']

logoMusicPlayed = false

def drawNintendoLogo()
	y = frame
	if y > 92 then
		y = 92
	endif
	text("Nintendo",93,y,2,14)
	sprite(237,223,y,14)
	if frame > 92 and not logoMusicPlayed then
		playNintendo()
		logoMusicPlayed = true
	endif
	if frame > 180 then
		screen = MENU
		if PLAY_MUSIC_IN_MENU then
			playMusic()
		endif
	endif
enddef

'[   _____          __  __ ______ 
    / ____|   /\   |  \/  |  ____|
   | |  __   /  \  | \  / | |__   
   | | |_ | / /\ \ | |\/| |  __|  
   | |__| |/ ____ \| |  | | |____ 
    \_____/_/    \_\_|  |_|______|  ']

testMapRaw1 = "0x03,0x0B,0x0B,0x06,0x0B,0x0B,0x0B,0x0B,0x0B,0x0B,0x0B,0x0B,0x06,0x0B,0x0B,0x02,0x0A,0x00,0x00,0x0A,0x00,0x00,0x00,0x00,0xFC,0x00,0x00,0x00,0x0A,0x00,0x00,0x0A,0x0A,0x00,0x00,0x0A,0x00,0x55,0x00,0x00,0x00,0x00,0x56,0x56,0x0A,0x00,0x00,0x0A,0x0A,0xFD,0x00,0x0A,0x00,0x55,0x0E,0x0B,0x0B,0x0B,0x0B,0x0B,0x09,0x00,0x00,0x0A,0x0A,0x00,0x00,0x0A,0x00,0x55,0x00,0x00,0x00,0x00,0xF9,0x00,0x0A,0x00,0x00,0x0A,0x0A,0x00,0x00,0x0D,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0D,0x00,0x00,0x0A,0x0A,0x00,0x00,0x00,0x24,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0A,0x0A,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0A,0x0A,0x56,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0C,0xFA,0x00,0x0A,0x0A,0x00,0x00,0x0E,0x0B,0x0B,0x0B,0x0B,0x0B,0x0B,0x0B,0x0B,0x09,0x00,0x00,0x0A,0x0A,0x00,0x00,0x00,0xFB,0x00,0x00,0x00,0x00,0x00,0xFE,0x00,0x0A,0x00,0x00,0x0A,0x0A,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x54,0x00,0x00,0x00,0x0A,0x00,0x00,0x0A,0x04,0x0B,0x0B,0x0B,0x0B,0x0B,0x0B,0x0B,0x0B,0x0B,0x0B,0x0B,0x08,0x0B,0x0B,0x05"

finishMapRaw1 = "0x03,0x0B,0x0B,0x0B,0x0B,0x0B,0x0B,0x0B,0x0B,0x06,0x0B,0x0B,0x0B,0x0B,0x0B,0x0B,0x0B,0x0B,0x0B,0x0B,0x0B,0x0B,0x0B,0x0B,0x0B,0x0B,0x0B,0x06,0x0B,0x0B,0x0B,0x0B,0x0B,0x06,0x0B,0x0B,0x0B,0x0B,0x0B,0x0B,0x0B,0x0B,0x0B,0x0B,0x0B,0x0B,0x0B,0x0B,0x0B,0x0B,0x0B,0x06,0x0B,0x0B,0x0B,0x0B,0x0B,0x0B,0x0B,0x0B,0x02,0x0A,0x24,0x00,0x00,0x00,0x00,0x00,0x00,0x52,0x0A,0x00,0x00,0x00,0x00,0x00,0x00,0x53,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFB,0x00,0x00,0x0A,0x00,0x00,0x00,0x00,0x00,0x0A,0x56,0x56,0x56,0x56,0x00,0x00,0x00,0xFC,0x00,0x00,0x00,0xFC,0x00,0x00,0x00,0x00,0x00,0x0A,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0A,0x0A,0x00,0x00,0x00,0x00,0x55,0x00,0x00,0x00,0x0A,0x56,0x56,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x56,0x0A,0x00,0x00,0x00,0x00,0x00,0x0A,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0A,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x52,0x0A,0x0A,0x00,0x00,0x0C,0x00,0x55,0x0C,0x00,0x00,0x0A,0x00,0x00,0x03,0x0B,0x0B,0x0B,0x0B,0x0B,0x0B,0x0B,0x0B,0x02,0x00,0x00,0x0C,0x00,0x00,0x0D,0x00,0x00,0x0C,0x00,0x00,0x04,0x0B,0x0B,0x02,0x00,0x00,0x0E,0x0B,0x0B,0x0B,0x0B,0x0B,0x0B,0x0B,0x0B,0x02,0x00,0x00,0x0A,0x00,0x00,0x03,0x0B,0x0B,0x0F,0x00,0x00,0x0A,0x0A,0x00,0x00,0x0A,0x00,0x55,0x0A,0x00,0x00,0x0A,0xFD,0x00,0x0A,0x00,0x00,0x00,0xFC,0x00,0x00,0x00,0x00,0x0A,0x00,0x00,0x0A,0x00,0x00,0x00,0x00,0x52,0x0A,0x00,0x00,0x00,0x00,0x00,0x0A,0x50,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFB,0x00,0x0A,0x00,0x00,0x0A,0x56,0x00,0x0A,0x00,0x00,0x00,0x00,0x00,0x0A,0x0A,0x00,0x00,0x0A,0x51,0x00,0x0A,0x00,0x00,0x0A,0x00,0x00,0x0A,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0A,0x00,0x00,0x0A,0x00,0x00,0x00,0x00,0x00,0x0A,0x00,0x00,0x00,0x00,0x00,0x0A,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x54,0x00,0x00,0x00,0x0A,0x00,0x00,0x0A,0x00,0x00,0x0A,0x00,0x00,0x00,0x00,0x00,0x0A,0x0A,0x00,0x00,0x0A,0x00,0x00,0x07,0x0B,0x0B,0x05,0x00,0x00,0x0A,0x00,0x00,0x03,0x0B,0x0B,0x02,0x00,0x00,0x0A,0x50,0x00,0x04,0x0B,0x0B,0x0B,0x0B,0x0B,0x01,0x0B,0x0B,0x0F,0x00,0x00,0x04,0x0B,0x0B,0x0B,0x0B,0x0B,0x06,0x0B,0x0B,0x0F,0x00,0x00,0x0A,0x00,0x00,0x04,0x0B,0x0B,0x05,0x00,0x00,0x0C,0x00,0x00,0x0A,0x0A,0x00,0x00,0x0A,0x00,0x00,0x0A,0x00,0x00,0x00,0x00,0x52,0x0A,0x00,0x00,0x0A,0x00,0x56,0x0A,0x00,0x00,0x0A,0x00,0x00,0x00,0xFC,0x00,0x00,0x00,0x00,0x0A,0x00,0x00,0x00,0x00,0x00,0x00,0x55,0x00,0x00,0xFA,0x00,0x0A,0x00,0x00,0x00,0x00,0x00,0x0A,0x51,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0A,0x00,0x00,0x0A,0x0A,0x56,0x00,0x0A,0x00,0x00,0x0A,0x00,0x00,0x00,0x00,0x00,0x0A,0x00,0x00,0x0A,0x00,0x00,0x0A,0x00,0x00,0x0A,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0A,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0A,0x00,0x00,0x00,0x00,0x00,0x0A,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x54,0x0A,0x00,0x56,0x0A,0x0A,0x00,0x00,0x0A,0x00,0x00,0x0A,0x00,0x00,0x0E,0x0B,0x0B,0x05,0x00,0x00,0x0A,0x00,0x00,0x0A,0x00,0x00,0x04,0x0B,0x0B,0x06,0x0B,0x0B,0x02,0x00,0x00,0x04,0x0B,0x0B,0x0B,0x0B,0x0B,0x06,0x0B,0x0B,0x0F,0x00,0x00,0x0A,0x00,0x00,0x0E,0x0B,0x0B,0x08,0x0B,0x0B,0x06,0x0B,0x0B,0x06,0x0B,0x0B,0x05,0x00,0x00,0x0A,0x0A,0x00,0x00,0x0A,0x00,0x00,0x0A,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0A,0x00,0x00,0x0A,0x00,0x00,0x00,0x00,0x00,0x0A,0x00,0x00,0x0A,0x55,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0A,0x00,0x00,0x00,0x00,0x00,0x0A,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0A,0x00,0x55,0x0A,0x50,0x00,0x00,0xFA,0x00,0x0A,0x0A,0x00,0x00,0x0A,0x00,0x00,0x0A,0x00,0x54,0x00,0x00,0x00,0x00,0x00,0x00,0x0A,0x00,0x00,0x0A,0x00,0x00,0x00,0x00,0x00,0x0A,0x00,0x00,0x0A,0x55,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0A,0x00,0x00,0x00,0x00,0x52,0x0A,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0A,0x00,0x55,0x0A,0x00,0x00,0x00,0x00,0x00,0x0A,0x07,0x0B,0x0B,0x05,0x00,0x00,0x07,0x0B,0x0B,0x0B,0x0B,0x0B,0x0B,0x0B,0x0B,0x09,0x51,0x00,0x04,0x0B,0x0B,0x02,0x00,0x00,0x0A,0x00,0x00,0x04,0x0B,0x0B,0x0B,0x0B,0x0B,0x02,0x00,0x00,0x04"
finishMapRaw2 = "0x0B,0x0B,0x02,0x00,0x00,0x07,0x0B,0x0B,0x0B,0x0B,0x0B,0x02,0x00,0x00,0x0A,0x00,0x55,0x0A,0x00,0x00,0x0E,0x0B,0x0B,0x09,0x0A,0x00,0x00,0x00,0xFA,0x00,0x0A,0x00,0x00,0x00,0x00,0x00,0x00,0xF9,0x00,0x0A,0x00,0x00,0x00,0x00,0x55,0x0A,0x00,0x00,0x0A,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0A,0x00,0x00,0x00,0x00,0x00,0x0A,0x00,0x00,0x0A,0x53,0x00,0x00,0x00,0x00,0x0A,0x00,0x00,0x0A,0x00,0x55,0x0A,0x00,0x00,0x00,0x00,0x00,0x0A,0x0A,0x00,0x00,0x00,0x00,0x52,0x0A,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0A,0x00,0x00,0x00,0x00,0x55,0x0A,0x00,0x00,0x0A,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x52,0x0A,0x00,0x00,0x00,0x00,0x00,0x0A,0x00,0x00,0x0A,0x00,0x00,0x00,0x00,0x00,0x0A,0x00,0x00,0x0A,0x00,0x00,0x0A,0x00,0x00,0x00,0x00,0x00,0x0A,0x0A,0x00,0x00,0x03,0x0B,0x0B,0x05,0x00,0x00,0x0C,0x00,0x00,0x0E,0x0B,0x0B,0x05,0x00,0x00,0x0C,0x00,0x55,0x0A,0x00,0x00,0x04,0x0B,0x0B,0x02,0x00,0x00,0x0C,0x00,0x00,0x04,0x0B,0x0B,0x02,0x00,0x00,0x0A,0x00,0x00,0x0A,0x00,0x00,0x0C,0x00,0x00,0x0D,0x00,0x00,0x0A,0x00,0x00,0x07,0x0B,0x0B,0x0F,0x00,0x00,0x0A,0x0A,0x00,0x00,0x0A,0x00,0x00,0x00,0x00,0x56,0x0A,0x00,0x00,0x00,0xFC,0x00,0x00,0x00,0x00,0x0A,0x00,0x00,0x0A,0x00,0x00,0x00,0x00,0x00,0x0A,0x00,0x00,0x0A,0x00,0x00,0x00,0x00,0x00,0x0A,0x50,0x00,0x0A,0x00,0x00,0x0A,0x00,0x00,0x0A,0x00,0x00,0x00,0x00,0x00,0x0A,0x50,0x00,0x0A,0x00,0x00,0x00,0x00,0x00,0x0A,0x0A,0x00,0x00,0x0A,0x00,0x00,0x00,0x00,0x00,0x0A,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0A,0x00,0x00,0x0A,0x00,0x54,0x00,0x00,0x00"
finishMapRaw3 = "0x0A,0x00,0x00,0x0A,0x00,0x00,0x00,0x00,0x00,0x0A,0x00,0x00,0x0A,0x00,0x00,0x0A,0x00,0x00,0x0A,0x00,0x00,0x00,0x00,0x00,0x0A,0x00,0x00,0x0A,0x00,0x00,0x00,0x00,0x00,0x0A,0x0A,0x50,0x00,0x0D,0x00,0x00,0x03,0x0B,0x0B,0x08,0x0B,0x0B,0x0B,0x0B,0x0B,0x0B,0x0B,0x0B,0x09,0x00,0x00,0x04,0x0B,0x0B,0x02,0x00,0x00,0x0D,0x00,0x00,0x07,0x0B,0x0B,0x0B,0x0B,0x0B,0x05,0x00,0x00,0x0A,0x00,0x00,0x0D,0x00,0x00,0x04,0x0B,0x0B,0x0B,0x0B,0x0B,0x05,0x00,0x00,0x0A,0x00,0x00,0x03,0x0B,0x0B,0x09,0x0A,0x00,0x00,0x00,0x00,0x00,0x0A,0x00,0x00,0x53,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x52,0x0A,0x00,0x00,0x00,0x00,0x00,0x0A,0x00,0x00,0x00,0x00,0x00,0x0A,0x00,0x53,0x00,0x00,0x00,0x00,0x00,0x00,0x0A,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFB,0x00,0x0A,0xFE,0x00,0x0A,0x00,0x00,0x0A,0x0A,0x00,0x00,0x00,0x00,0x00,0x0A,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0A,0x00,0x00,0x00,0x00,0x00,0x0A,0x50,0x00,0x00,0x00,0x00,0x0A,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0A,0x00,0x00,0x00,0x00,0x54,0x00,0x55,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0A,0x00,0x00,0x0A,0x00,0x00,0x0A,0x0A,0x00,0x00,0x03,0x0B,0x0B,0x08,0x0B,0x0B,0x02,0x00,0x00,0x0E,0x0B,0x0B,0x02,0x00,0x00,0x0A,0x00,0x00,0x0C,0x00,0x00,0x04,0x0B,0x0B,0x02,0x00,0x00,0x0A,0x00,0x00,0x03,0x0B,0x0B,0x0F,0x00,0x00,0x07,0x0B,0x0B,0x0B,0x0B,0x0B,0x0B,0x0B,0x0B,0x0B,0x0B,0x0B,0x0B,0x0B,0x0B,0x08,0x0B,0x0B,0x09,0x00,0x00,0x0A,0x0A,0xFD,0x00,0x0A,0x00,0x56,0x56,0x56,0x56,0x0A,0x00,0x00,0x00,0x00,0x00,0x0A,0x00,0x00,0x0A,0x00,0x00,0x0A,0x00,0x00,0x00,0x00,0x00,0x0A,0x00,0x00,0x0A,0x00,0x00,0x0A,0xF9,0x56,0x56,0x56,0x00,0x0A,0x51,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x56,0x56,0x56,0x0A,0x00,0x00,0x0A,0x0A,0x00,0x00,0x0A,0x00,0x00,0x00,0x00,0x00,0x0A,0x00,0x00,0x00,0x00,0x54,0x0A,0x00,0x00,0x0A,0xFA,0x00,0x0A,0x00,0x00,0x00,0x55,0x00,0x0A,0x00,0x00,0x0A,0x00,0x00,0x0A,0x00,0x00,0x00,0x00,0x00,0x0A,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0A,0x00,0x00,0x0A,0x0A,0x00,0x00,0x0D,0x00,0x00,0x0C,0x00,0x00,0x04,0x0B,0x0B,0x0B,0x0B,0x0B,0x05,0x00,0x00,0x0A,0x00,0x00,0x04,0x0B,0x0B,0x0B,0x0B,0x0B,0x05,0x00,0x00,0x0A,0x00,0x00,0x04,0x0B,0x0B,0x0B,0x0B,0x0B,0x05,0x00,0x00,0x0E,0x0B,0x0B,0x0B,0x0B,0x0B,0x0B,0x0B,0x0B,0x0B,0x0B,0x0B,0x02,0x00,0x00,0x0D,0x00,0x00,0x0A,0x0A,0x00,0x00,0x00,0x00,0x00,0x0A,0x00,0x00,0x00,0xFC,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0A,0xFB,0x00,0x00,0x00,0x53,0x00,0x00,0x00,0x00,0x00,0x00,0x0A,0x00,0xFB,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x53,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x52,0x0A,0x00,0x00,0x00,0x00,0x00,0x0A,0x0A,0x00,0x00,0x00,0x00,0x52,0x0A,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0A,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x56,0x56,0x56,0x0A,0x50,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x55,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0A,0x00,0x00,0x00,0x00,0x54,0x0A,0x04,0x0B,0x0B,0x0B,0x0B,0x0B,0x08,0x0B,0x0B,0x0B,0x0B,0x0B,0x0B,0x0B,0x0B,0x0B,0x0B,0x0B,0x08,0x0B,0x0B,0x0B,0x0B,0x0B,0x0B,0x0B,0x0B,0x0B,0x0B,0x0B,0x08,0x0B,0x0B,0x0B,0x0B,0x0B,0x0B,0x0B,0x0B,0x0B,0x0B,0x0B,0x0B,0x0B,0x0B,0x0B,0x0B,0x0B,0x0B,0x0B,0x0B,0x0B,0x0B,0x0B,0x08,0x0B,0x0B,0x0B,0x0B,0x0B,0x05"

testMap = false
mapToPlay = nil

xCenter = 154
yCenter = 87
margin = 10
limitX = -328
limitY = -95
cameraX = 0
cameraY = 0

def die()
	screen = LOSE
	playLose()
enddef

def checkWithBackground(posX,posY,w,h)
    x = screenStartX
    y = screenStartY
	for i = 0 to len(mapToPlay)-1 step 1	 
		for k = 0 to len(mapToPlay(str(i)))-1 step 1		
		j = get(mapToPlay(str(i)),k)
	   		if j >= 49 and j <= 63 then
	   			if checkCollisionAABB(x,y,8,8,posX,posY,w,h) then
	   				return true
	   			endif
	   		endif
	    	x = x+8
	   next
	   x = 80
	   y = y+8
	next
	return false
enddef


def getCollisionTiles(colX, colY, h, exact)
	returnData = list()
	
 	x = screenStartX
    y = screenStartY
	for i = 0 to len(mapToPlay)-1 step 1	 
		for k = 0 to len(mapToPlay(str(i)))-1 step 1		
		j = get(mapToPlay(str(i)),k)
			if j >= 49 and j <= 63 then
		   		if exact then
			   		if (x = colX and h) or (y = colY and not h) then
			   			push(returnData,x)
			   			push(returnData,y)
			   		endif
			   	else
			   		x1 = 0
			   		y1 = 0
			   		if not (h) then
				   		if x >= colX-8 and x < colX then
				   			y1 = y
				   			x1 = x
				   		elseif x >= colX and x < colX + 8 then
				   			y1 = y
				   			x1 = x
				   		elseif x >= colX+8 and x < colX + 16 then
				   			y1 = y
				   			x1 = x
				   		endif
				   	else
				   		if y >= colY-8 and y < colY then
				   			x1 = x
				   			y1 = y
				   		elseif y >= colY and y < colY + 8 then
				   			x1 = x
				   			y1 = y
				   		elseif y >= colY+8 and y < colY + 16 then
					   		x1 = x
				   			y1 = y			   		
				   		endif
			   		endif
			   		
			   		if (not (x1 = 0)) and (not (y1 = 0)) then
			   			push(returnData,x1)
			   			push(returnData,y1)
			   		endif
		   		endif
		   	endif
	    	x = x+8
	   next
	   x = 80
	   y = y+8	
	next

	return returnData
enddef

enemies = list()
char = nil
inited = false
fullInited = false

def checkCollisionWithCharacter(posX, posY, w, h)
	return checkCollisionAABB(posX, posY, w, h,  char.positionX +3, char.positionY +8, 10, 6) and not char.jumping
enddef

def hasColisionWithCachedTiles(tiles, positionX, positionY, w, h)
	for i = 0 to len(tiles)-1 step 2
		dataX = get(tiles,i)
		dataY = get(tiles,i+1)
		if checkCollisionAABB(dataX, dataY, 8,8, positionX, positionY, w, h) then
			return true	
		endif		
	next
	return false
enddef

' ===========================
' =         SPIKES          =
' ===========================

class spike
	positionX = 0
	positionY = 0
	
	spikesH = dict()
	spikesH(0) = list(38,39)
	spikesH(1) = list(40,41,42)

	spikesV = dict()
	spikesV(0) = list(43,44)
	spikesV(1) = list(45,46,47)
	
	drawElements = spikesV
	
	character = nil
	randomize = 0
	
	def initialize(posX, posY, h)
		positionX = posX
		positionY = posY
		drawElements = spikesV
		if h = 0 then
			drawElements = spikesH		
		endif
		
		randomize = rnd(0,48)
	enddef
	
	def draw()	
		frameNumber = ((frame+ randomize)/48) mod 2
		
		if frameNumber = 1 then
			if checkCollisionWithCharacter(positionX, positionY, 16, 16) then
				die()
			endif
		endif
		
		it = iterator(drawElements(frameNumber))
		while move_next(it)
			meta(get(it),positionX + cameraX, positionY + cameraY)	
		wend
	enddef
endclass

' ===========================
' =          BLADE          =
' ===========================

class blade
	speed = 0.05

	positionX = 0
	positionY = 0
	direction = 0
	orientation = 1
	
	bladeTiles = dict()
	bladeTiles(0) = list(32,33,34)
	bladeTiles(1) = list(35,36,37)
	
	collisionTiles = nil
	
	def initialize(posX, posY, dir)
		positionX = posX
		positionY = posY
		direction = dir
		collisionTiles = getCollisionTiles(posX, posY, direction, true)
	enddef
	
	def draw()
		if NOT fullInited then
			return		
		endif
	
		if checkCollisionWithCharacter(positionX, positionY, 16, 16) then
			die()					
		endif
	
		oldX = positionX
		oldY = positionY
		if direction = 0 then
			positionX = positionX - delta * 1000 * speed * orientation
		else
			positionY = positionY - delta * 1000 * speed * orientation
		endif
		
		if hasColisionWithCachedTiles(collisionTiles, positionX, positionY, 14, 14) then
			orientation = orientation * -1
			positionX = oldX				
			positionY = oldY	
		endif
	
		it = iterator(bladeTiles((frame/4) mod 2))
		while move_next(it)
			meta(get(it),positionX + cameraX, positionY + cameraY)	
		wend
	enddef
endclass

' ===========================
' =          GHOST          =
' ===========================

class ghost
	speed = 0.05

	positionX = 0
	positionY = 0
	
	ghostTiles = dict()
	ghostTiles(0) = list(24,25)
	ghostTiles(1) = list(24,26)
	ghostTiles(2) = list(24,27)
	ghostTiles(3) = list(24,28)
	
	' 0 down
	' 1 left
	' 2 right
	' 3 up
	direction = 0
	directionIndex = 0
	
	counter = 0
	
	collisionTiles = nil
	
	def initialize(posX, posY)
		positionX = posX
		positionY = posY
		direction = 0
		directionIndex = 0
		collisionTiles = getCollisionTiles(positionX, positionY, false, false)
	enddef
	
	def randomDirection() then
		direction = round(rnd(0,3))
	enddef
	
	def handleMove()
		if direction = 1 then
			backCol = hasColisionWithCachedTiles(collisionTiles,positionX - delta * 1000 * speed + 1, positionY + 1, 14, 14)
			if not backCol then				
				if NOT (directionIndex = 1) then
					collisionTiles = getCollisionTiles(positionX, positionY, true, false)
					backCol = hasColisionWithCachedTiles(collisionTiles,positionX - delta * 1000 * speed + 1, positionY + 1, 14, 14)
					if not backCol then
						positionX = positionX - delta * 1000 * speed
					else
						randomDirection()
					endif
				else														
					positionX = positionX - delta * 1000 * speed
				endif
			else
				randomDirection()
			endif
			directionIndex = 1
		elseif direction = 2 then
			backCol = hasColisionWithCachedTiles(collisionTiles,positionX + delta * 1000 * speed+ 1, positionY+ 1, 14, 14)
			if not backCol then
				if NOT (directionIndex = 2) then
					collisionTiles = getCollisionTiles(positionX, positionY, true, false)
					backCol = hasColisionWithCachedTiles(collisionTiles,positionX + delta * 1000 * speed+ 1, positionY+ 1, 14, 14)
					if not backCol then
						positionX = positionX + delta * 1000 * speed
					else
						randomDirection()
					endif
				else									
					positionX = positionX + delta * 1000 * speed
				endif
			else
				randomDirection()
			endif
			directionIndex = 2
		elseif direction = 3 then
			backCol = hasColisionWithCachedTiles(collisionTiles,positionX+ 1, positionY - delta * 1000 * speed+ 1, 14, 14)
			if not backCol then
				if NOT (directionIndex = 3) then
					collisionTiles = getCollisionTiles(positionX, positionY, false, false)
					backCol = hasColisionWithCachedTiles(collisionTiles,positionX+ 1, positionY - delta * 1000 * speed+ 1, 14, 14)
					if not backCol then
						positionY = positionY - delta * 1000 * speed
					else
						randomDirection()
					endif
				else					
					positionY = positionY - delta * 1000 * speed
				endif
			else
				randomDirection()
			endif
			directionIndex = 3
		elseif direction = 0 then
			backCol = hasColisionWithCachedTiles(collisionTiles,positionX+ 1, positionY + delta * 1000 * speed+ 1, 14, 14)		
			if not backCol then
				if NOT (directionIndex = 0) then
					collisionTiles = getCollisionTiles(positionX, positionY, false, false)
					backCol = hasColisionWithCachedTiles(collisionTiles,positionX+ 1, positionY + delta * 1000 * speed+ 1, 14, 14)	
					if not backCol then
						positionY = positionY + delta * 1000 * speed
					else
						randomDirection()
					endif
				else
					positionY = positionY + delta * 1000 * speed	
				endif
			else
				randomDirection()
			endif
			directionIndex = 0
		endif	
	enddef
	
	def draw()
		if NOT fullInited then
			return				
		endif
	
		if checkCollisionWithCharacter(positionX, positionY, 16, 16) then
			die()
			return				
		endif	
		
		handleMove()
		
		counter = counter + delta * 1000
	
		it = iterator(ghostTiles(directionIndex))
		while move_next(it)
			meta(get(it),positionX + cameraX, positionY + cameraY + sin(counter/ 100)* 2)	
		wend
	enddef
endclass

' ===========================
' =          GOAL           =
' ===========================

class goal
	positionX = 0
	positionY = 0
	character = nil
	
	def initialize(posX, posY)
		positionX = posX
		positionY = posY
		character = char
	enddef
	
	def draw()
		if checkCollisionWithCharacter(positionX, positionY, 16, 16) then
			screen = WIN
			playWin()
		endif
	
		meta(22,positionX + cameraX, positionY + cameraY)	
		meta(23,positionX + cameraX, positionY + cameraY)	
		meta(21,positionX + cameraX, positionY + cameraY)	
	enddef
endclass


' ===========================
' =        CHARACTER        =
' ===========================

class character
	speed = 0.05

	positionX = 0
	positionY = 0
	
	jumping = 0
	directionIndex = 0
	jumpCounter = 0
	jumpTime = 1000
	height = 25
	
	timeCounter = 0
	
	characterTiles = dict()
	characterTiles("0") = list(1,0,2)
	characterTiles("1") = list(4,3,5)
	characterTiles("2") = list(7,6,8)
	characterTiles("3") = list(1,9,2)
	characterTiles("4") = list(4,10,5)
	characterTiles("5") = list(7,11,8)
	characterTiles("6") = list(1,12,2)
	characterTiles("7") = list(4,13,5)
	characterTiles("8") = list(7,14,8)
	characterTiles("9") = list(1,2,15)
	characterTiles("10") = list(4,5,16)
	characterTiles("11") = list(18,7,8,17)
	
	collisionTiles = nil
	
	walking = false
	
	def initialize(posX, posY)
		positionX = posX
		positionY = posY
		collisionTiles = getCollisionTiles(positionX, positionY, false, false)
	enddef
	
	def handleMove()
		if keypressed(90) and not jumping then
			jumpCounter = 0
			jumping = 1
			walking = false
			playJump()
		endif
		
		if jumping then
			return
		endif

		startWalking = false
		if keydown(65) or keydown(263) then
			backCol = hasColisionWithCachedTiles(collisionTiles,positionX - delta * 1000 * speed +3, positionY+8, 10, 6)
			'backCol = checkWithBackground(positionX - delta * 1000 * speed +3, positionY+8, 10, 6)
			if not backCol then				
				if NOT (directionIndex = 9) then
					collisionTiles = getCollisionTiles(positionX, positionY, true, false)
					backCol = hasColisionWithCachedTiles(collisionTiles,positionX - delta * 1000 * speed +3, positionY+8, 10, 6)
					if not backCol then
						positionX = positionX - delta * 1000 * speed
					endif
				else														
					positionX = positionX - delta * 1000 * speed
				endif
			endif
			directionIndex = 9
			startWalking = true
		elseif keydown(68) or keydown(262) then
			backCol = hasColisionWithCachedTiles(collisionTiles,positionX + delta * 1000 * speed+3, positionY+8, 10, 6)
			'backCol = checkWithBackground(positionX + delta * 1000 * speed+3, positionY+8, 10, 6)
			if not backCol then
				if NOT (directionIndex = 6) then
					collisionTiles = getCollisionTiles(positionX, positionY, true, false)
					backCol = hasColisionWithCachedTiles(collisionTiles,positionX + delta * 1000 * speed+3, positionY+8, 10, 6)
					if not backCol then
						positionX = positionX + delta * 1000 * speed
					endif
				else									
					positionX = positionX + delta * 1000 * speed
				endif
			endif
			directionIndex = 6
			startWalking = true
		elseif keydown(87) or keydown(265) then
			backCol = hasColisionWithCachedTiles(collisionTiles,positionX+3, positionY - delta * 1000 * speed+8, 10, 6)
			'backCol = checkWithBackground(positionX+3, positionY - delta * 1000 * speed+8, 10, 6)
			if not backCol then
				if NOT (directionIndex = 3) then
					collisionTiles = getCollisionTiles(positionX, positionY, false, false)
					backCol = hasColisionWithCachedTiles(collisionTiles,positionX+3, positionY - delta * 1000 * speed+8, 10, 6)
					if not backCol then
						positionY = positionY - delta * 1000 * speed
					endif
				else					
					positionY = positionY - delta * 1000 * speed
				endif
			endif
			directionIndex = 3
			startWalking = true
		elseif keydown(83) or keydown(264) then
			backCol = hasColisionWithCachedTiles(collisionTiles,positionX+3, positionY + delta * 1000 * speed+8, 10, 6)
			'backCol = checkWithBackground(positionX+3, positionY + delta * 1000 * speed+8, 10, 6)			
			if not backCol then
				if NOT (directionIndex = 0) then
					collisionTiles = getCollisionTiles(positionX, positionY, false, false)
					backCol = hasColisionWithCachedTiles(collisionTiles,positionX+3, positionY + delta * 1000 * speed+8, 10, 6)
					if not backCol then
						positionY = positionY + delta * 1000 * speed	
					endif
				else
					positionY = positionY + delta * 1000 * speed	
				endif
			endif
			directionIndex = 0
			startWalking = true
		endif
		
		if (startWalking and not walking) or (not startWalking and walking) then
			walking = startWalking
			if walking then
				playSteps()
			else
				stopSteps()
			endif
		endif
	enddef
	
	def drawShadow()
		if jumping then
			if jumpCounter > jumpTime / 4 and jumpCounter < jumptime - (jumpTime / 4) then
				meta(20,positionX + cameraX, positionY + cameraY)
			else
				meta(19,positionX + cameraX, positionY + cameraY)
			endif
		else
			meta(18,positionX + cameraX, positionY + cameraY)
		endif
	enddef
	
	def draw()			
		if NOT fullInited then
			return		
		endif
	
		if jumping then
			jumpCounter = jumpCounter + delta * 1000
			jumpPosition = jumpCounter / height
			if jumpCounter > jumpTime then
				jumping = false
			endif
			
			animIndex = 2
			if jumpCounter > jumpTime / 2 then
				jumpPosition = (jumpTime-jumpCounter)/ height
				animIndex = 1
			endif
			
			it = iterator(characterTiles(str(animIndex+directionIndex)))
			while move_next(it)
				meta(get(it),positionX + cameraX, positionY + cameraY - jumpPosition)		
			wend
			
		else
		
		timeCounter = timeCounter + delta * 1000
		it = iterator(characterTiles(str(((round(timeCounter)/250) mod 2)+directionIndex)))
		while move_next(it)
			meta(get(it),positionX + cameraX, positionY + cameraY)			
		wend
		
		endif
		
		handleMove()
	enddef
endclass

' ===========================
' =           GAME          =
' ===========================

currentSeconds = 0

def convert(data)
	if data ="0x0F" then
		return 49
	elseif data ="0x0E" then		
		return 50
	elseif data ="0x0D" then		
		return 51
	elseif data ="0x0C" then		
		return 52		
	elseif data ="0x0B" then		
		return 53		
	elseif data ="0x0A" then		
		return 54
	elseif data ="0x09" then				
		return 55
	elseif data ="0x08" then				
		return 56
	elseif data ="0x07" then				
		return 57
	elseif data ="0x06" then				
		return 58
	elseif data ="0x04" then				
		return 59
	elseif data ="0x05" then				
		return 60
	elseif data = "0x03" then
		return 61
	elseif data ="0x02" then
		return 62
	elseif data ="0x01" then				
		return 63
	elseif data = "0x50" then				
		return "dec1"
	elseif data = "0x51" then					
		return "dec2"	
	elseif data = "0x52" then					
		return "dec3"
	elseif data = "0x53" then					
		return "dec4"
	elseif data = "0x54" then					
		return "dec5"
	elseif data = "0x55" then					
		return "dec6"
	elseif data = "0x56" then					
		return "dec7"
	elseif data = "0x24" then
		return "c"
	elseif data = "0xFC" then		
		return "sv"
	elseif data = "0xFD" then		
		return "sh"
	elseif data = "0xFB" then		
		return "bh"
	elseif data = "0xFA" then		
		return "bv"
	elseif data = "0xFE" then		
		return "go"
	elseif data = "0xF9" then		
		return "g"
	else
		return 99
	endif
enddef

def parser(rawData1, rawData2, rawData3, colLen)
	result = dict()
	result(str(0)) = list()
	
	rowCounter = 0
	columnCounter = 0
	
	for i = 0 to len(rawData1) step 5
		push(result(str(rowCounter)), convert(mid(rawData1,i,4)))
		columnCounter = columnCounter + 1
		
		if columnCounter >= colLen then
			columnCounter = 0
			rowCounter = rowCounter + 1
			result(str(rowCounter)) = list()
		endif
	next

	for i = 0 to len(rawData2) step 5
		push(result(str(rowCounter)), convert(mid(rawData2,i,4)))
		columnCounter = columnCounter + 1
		if columnCounter >= colLen then
			columnCounter = 0
			rowCounter = rowCounter + 1
			result(str(rowCounter)) = list()
		endif
	next
	
	for i = 0 to len(rawData3) step 5
		push(result(str(rowCounter)), convert(mid(rawData3,i,4)))
		columnCounter = columnCounter + 1
		if columnCounter >= colLen then
			columnCounter = 0
			rowCounter = rowCounter + 1
			result(str(rowCounter)) = list()
		endif
	next
	
	return result
enddef

startCounter = 0

def startGame(map)
	inited = false
	fullInited = false
	screen = GAME
	startCounter = 0
	currentSeconds = 0
	cameraX = 0
	cameraY = 0
	
	if testMap then
		mapToPlay = parser(testMapRaw1,"","",16)
	else
		mapToPlay = parser(finishMapRaw1,finishMapRaw2,finishMapRaw3,61)
	endif
	
	x = screenStartX
    y = screenStartY
	for i = 0 to len(mapToPlay)-1 step 1	 
		for k = 0 to len(mapToPlay(str(i)))-1 step 1
		j = get(mapToPlay(str(i)),k)

	   		if j = "c" then
	   			char = new (character)	
	   			char.initialize(x, y)
	   		endif
	    	x = x+8
	   next
	   x = 80
	   y = y+8			
	next
	
	clear(enemies)
	x = screenStartX
    y = screenStartY
	for i = 0 to len(mapToPlay)-1 step 1	   		
		for k = 0 to len(mapToPlay(str(i)))-1 step 1
		j = get(mapToPlay(str(i)),k)
	   		if j = "sv" then
	   			newSpike = new (spike)
				newSpike.initialize(x,y,1)
				push(enemies, newSpike)
	   		elseif j = "sh" then
	   			newSpike = new (spike)
				newSpike.initialize(x,y,0)
				push(enemies, newSpike)
	   		elseif j = "bv" then
	   			newBlade = new (blade)
	   			newBlade.initialize(x,y,1)				
	   			push(enemies, newBlade)
	   		elseif j = "bh" then
	   			newBlade = new (blade)
	   			newBlade.initialize(x,y,0)					   			
	   			push(enemies, newBlade)
	   		elseif j = "g" then
	   			newGhost = new (ghost)
	   			newGhost.initialize(x,y)					   				   			
	   			push(enemies, newGhost)
	   		elseif j = "go" then
	   			newGoal = new (goal)
	   			newGoal.initialize(x,y)	
	   			push(enemies, newGoal)
	   		endif
	    	x = x+8
	   next
	   x = 80
	   y = y+8		
	next
	
	if PLAY_MUSIC_IN_MENU then
		soundStop()
	else
		playMusic()
	endif

	inited = true
enddef

def drawBottomTime()
	currentSeconds = currentSeconds + (delta * 1000 / 1000)

	for x = screenStartX to 232 step 8
		drawMeta("grass1",x,156)
		drawMeta("grass2",x,164)		
	next
	
	drawMeta("grass4",88,164)
	drawMeta("grass1",96,164)
	drawMeta("grass1",104,164)
	drawMeta("grass1",112,164)
	drawMeta("grass1",120,164)
	drawMeta("grass3",128,164)
	
	time0 = currentSeconds mod 10
	time1 = (currentSeconds)/10 mod 10
	time2 = (currentSeconds)/100 mod 10
	time3 = (currentSeconds)/1000 mod 10	
	text(formatText("TIME: %i%i%i%i",time3,time2, time1,time0),144,164,1,15)	
enddef

def calculateCameraPos()
	if char.positionX + cameraX > xCenter + margin then
		cameraX = cameraX - ((char.positionX + cameraX) - (xCenter + margin))
	elseif char.positionX + cameraX < xCenter - margin then
		cameraX = cameraX - ((char.positionX + cameraX) - (xCenter - margin))
	endif
	
	if char.positionY + cameraY > yCenter + margin then
		cameraY = cameraY - ((char.positionY + cameraY) - (yCenter + margin))
	elseif char.positionY + cameraY < yCenter - margin then
		cameraY = cameraY - ((char.positionY + cameraY) - (yCenter - margin))	
	endif
	
	if cameraX > 0 then
		cameraX = 0
	elseif cameraX < limitX then
		cameraX = limitX
	endif
	
	if cameraY > 0 then
		cameraY = 0
	elseif cameraY < limitY then
		cameraY = limitY	
	endif
	
enddef

def drawGame()
	if NOT inited then
		return
	endif

	drawBackground(mapToPlay, cameraX, cameraY)
	
	if  startCounter < 0.2 then
		startCounter = startCounter + (delta * 1000 / 1000)
		return	
	endif
	
	fullInited = true
	
	char.drawShadow()
	
	for e in enemies
		e.draw()	
	next
	
	char.draw()
	
	drawBottomTime()
	
	calculateCameraPos()
	
	' TEST PURPOSES ONLY
	if keypressed(84) then
		if testMap then
			testMap = false
			startGame()
		else
			testMap = true	
			startGame()
		endif	
	endif
enddef


'[   __  __ ______ _   _ _    _ 
    |  \/  |  ____| \ | | |  | |
    | \  / | |__  |  \| | |  | |
    | |\/| |  __| | . ` | |  | |
    | |  | | |____| |\  | |__| |
    |_|  |_|______|_| \_|\____/    ']
    
mainMenuMap = dict()
mainMenuMap("0") = list(99,99,54)
mainMenuMap("1") = list(99,99,54,99,99,99,99,99,99,99,99,99,99,99,99,99,50,53,53,53)
mainMenuMap("2") = list(53,53,60,"dec1",99,99,99,99,99,99,99,99,99,99,99,99,99,99,"dec4")
mainMenuMap("3") = list()
mainMenuMap("4") = list()
mainMenuMap("5") = list()
mainMenuMap("6") = list()
mainMenuMap("7") = list()
mainMenuMap("8") = list()
mainMenuMap("9") = list()
mainMenuMap("10") = list()
mainMenuMap("11") = list()
mainMenuMap("12") = list(99,99,99,99,99,99,52)
mainMenuMap("13") = list(99,99,99,99,52,99,54,"dec2")
mainMenuMap("14") = list(99,99,61,53,63,53,60)
mainMenuMap("15") = list(99,99,54,99,51)
mainMenuMap("16") = list(99,"dec3",54)

def drawMainMenu()
	drawBackground(mainMenuMap, 0, 0)
	for x = screenStartX to 232 step 8
	drawMeta("grass1",x,164)
	next
	drawKey(145,106)
	text("PRESS Z TO PLAY",100,108,1,14)
	text("BY DEEBROL",152,146,1,13)
	text("AND DAVID",152,156,1,13)
	drawWinCharacter(119,60)
	drawLogo(104,64)
	if keypressed(90) then
		startGame()
	endif
enddef

'[  __          _______ _   _ 
    \ \        / /_   _| \ | |
     \ \  /\  / /  | | |  \| |
      \ \/  \/ /   | | | . ` |
       \  /\  /   _| |_| |\  |
        \/  \/   |_____|_| \_|    ']

winScreenMap = dict()
winScreenMap("0") = list(61,53,58,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,62)
winScreenMap("1") = list(54,99,54,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,"dec6",54)
winScreenMap("2") = list(54,99,54,99,61,53,58,53,62,99,52,99,52,"dec1",99,99,99,99,"dec6",54)
winScreenMap("3") = list(54,99,59,58,60,99,54,99,54,99,54,99,54,99,99,99,99,99,99,54)
winScreenMap("4") = list(54,99,99,54,"dec4",99,54,99,54,99,54,99,54,99,99,99,99,99,99,54)
winScreenMap("5") = list(54,99,99,51,99,99,59,53,56,53,56,53,60,99,99,99,99,99,99,54)
winScreenMap("6") = list(54,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,54)
winScreenMap("7") = list(57,53,62,99,61,53,53,58,49,99,52,99,52,99,99,99,99,99,99,54)
winScreenMap("8") = list(54,99,54,99,54,99,99,54,99,99,57,62,54,99,99,99,99,99,99,54)
winScreenMap("9") = list(54,99,54,52,54,99,99,54,99,99,54,59,55,99,99,99,99,99,99,54)
winScreenMap("10") = list(54,99,59,56,60,99,50,56,53,53,60,99,51,99,99,99,99,99,99,51)
winScreenMap("11") = list(54)
winScreenMap("12") = list(54)
winScreenMap("13") = list(54)
winScreenMap("14") = list(54)
winScreenMap("15") = list(54,"dec2")
winScreenMap("16") = list(54,99,99,99,99,99,99,99,99,99,99,99,99,99,99,"dec5",99,99,99,52)
winScreenMap("17") = list(59,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,60)

def drawWinScreen()
	drawBackground(winScreenMap, 0, 0)
	drawKey(101,122)
	drawKey(101,142)
	text("Z",104,124,1,14)
	text("X",104,144,1,14)
	text("RESTART",120,124,1,14)
	text("MENU",120,144,1,14)
	text("TIME",192,124,1,14)
	time0 = currentSeconds mod 10
	time1 = (currentSeconds)/10 mod 10
	time2 = (currentSeconds)/100 mod 10	
	time3 = (currentSeconds)/1000 mod 10
	text(formatText("%i%i%i%i",time3,time2, time1,time0),192,134,1,14)
	winX = 200
	winY = 70
	drawWinCharacter(winX,winY)
	if keypressed(90) then
		startGame()	
	endif
	if keypressed(88) then
		screen = MENU
	endif
enddef

'[   _      ____   _____ ______ 
    | |    / __ \ / ____|  ____|
    | |   | |  | | (___ | |__   
    | |   | |  | |\___ \|  __|  
    | |___| |__| |____) | |____ 
    |______\____/|_____/|______|  ']

loseScreenMap = dict()
loseScreenMap("0") = list(61,53,58,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,62)
loseScreenMap("1") = list(54,99,54,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,"dec6",54)
loseScreenMap("2") = list(54,99,54,99,61,53,58,53,62,99,52,99,52,"dec1",99,99,99,99,"dec6",54)
loseScreenMap("3") = list(54,99,59,58,60,99,54,99,54,99,54,99,54,99,99,99,99,99,99,54)
loseScreenMap("4") = list(54,99,99,54,"dec4",99,54,99,54,99,54,99,54,99,99,99,99,99,99,54)
loseScreenMap("5") = list(54,99,99,51,99,99,59,53,56,53,56,53,60,99,99,99,99,99,99,54)
loseScreenMap("6") = list(54,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,54)
loseScreenMap("7") = list(57,53,58,62,99,99,50,58,53,53,58,53,49,99,99,99,99,99,99,54)
loseScreenMap("8") = list(54,99,54,59,62,99,99,54,99,99,54,99,99,99,99,99,99,99,99,54)
loseScreenMap("9") = list(54,99,54,99,54,99,99,54,99,99,57,49,99,99,99,99,99,99,99,54)
loseScreenMap("10") = list(54,99,59,53,56,53,53,56,49,99,59,53,49,99,99,99,99,99,99,54)
loseScreenMap("11") = list(54,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,"dec3",54)
loseScreenMap("12") = list(54,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,54)
loseScreenMap("13") = list(54,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,51)
loseScreenMap("14") = list(54,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,"dec7","dec7","dec7")
loseScreenMap("15") = list(54,"dec2")
loseScreenMap("16") = list(54,99,99,99,99,99,99,99,99,99,99,99,99,99,99,"dec5",99,99,99,52)
loseScreenMap("17") = list(59,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,60)

def drawLoseScreen()
	drawBackground(loseScreenMap, 0, 0)
	drawKey(101,122)
	drawKey(101,142)
	text("Z",104,124,1,14)
	text("X",104,144,1,14)
	text("RESTART",120,124,1,14)	
	text("MENU",120,144,1,14)
	' Lose character
	sprite(241,195,85,13)
	sprite(244,195+8,85,13)		
	sprite(245,195+16,85,13)
	sprite(242,195,85,14)
	sprite(240,195,85,15)
	sprite(243,195+8,85,15)
	sprite(240,195+16,85,15,8)
	sprite(247,195,85+8,13)
	sprite(250,195+8,85+8,13)	
	sprite(253,195+16,85+8,13)
	sprite(248,195,85+8,14)
	sprite(251,195+8,85+8,14)		
	sprite(239,195+16,85+8,14)
	sprite(246,195,85+8,15)
	sprite(249,195+8,85+8,15)
	sprite(252,195+16,85+8,15)
	
	if keypressed(90) then
		startGame()	
	endif
	if keypressed(88) then
		screen = MENU	
	endif
enddef

'[    _____ ____  _____  ______ 
     / ____/ __ \|  __ \|  ____|
    | |   | |  | | |__) | |__   
    | |   | |  | |  _  /|  __|  
    | |___| |__| | | \ \| |____ 
     \_____\____/|_|  \_\______|   ']

def drawGameBoyScreen()
	rect(0,0,80,200,200,1)
	rect(240,0,80,200,200,1)
	rect(0,0,320,28,200,1)	
	rect(0,172,320,28,200,1)
	rect(10,10,45,2,10,2)
	rect(10,14,45,2,10,3)
	rect(290,10,20,2,10,2)	
	rect(290,14,20,2,10,3)
	text("DOT MATRIX WITH STEREO SOUND",60,10,1,4)
	circle(40,60,3,0,5)
	text("battery",15,70,1,4)
	text("Nintendo GAME BOY TM",80,180,1,6)
enddef
  
def changeColor()
	for a = 0x018 to 0x023 step 1
		color = peek(a+12)
		poke(a+12, peek(a))
		poke(a, color)
	next
enddef

def pause()
	rect(screenStartX-2,73,200,57,200,13)
	rect(screenStartX-2,73,200,57,2,15)
	text("PAUSE",140,85,1,15)
	text("PRESS ANY KEY",110,100,1,14)		
	text("TO CONTINUE",118,110,1,14)
	drawGameBoyScreen()
enddef

def draw()
	cls(12)	
	
	if keypressed(32) then
		changeColor()
	endif
	
	if screen=GAME then
		drawGame()
	elseif screen = MENU then
		drawMainMenu()
	elseif screen = LOGO then	
		drawNintendoLogo()
	elseif screen = WIN then	
		drawWinScreen()
	elseif screen = LOSE then	
		drawLoseScreen()
	endif
	
	drawGameBoyScreen()
enddef