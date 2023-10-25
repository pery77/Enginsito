'CH_POS(ch)              -> Return current song position
'CH_SIZE(ch)             -> Return channel song size
'CH_FRAME(ch, frame)     -> Return average chanel frame (0-62)
'CH_GETNOTE(ch)          -> Return last note name
'CH_GETNOTENUM(ch)       -> Return last note midi number

'copy pasted from https://en.wikipedia.org/wiki/Music_Macro_Language
let mymusic = "t104l4>c16f16a>c8<a c16f16a>c8<a c8b-8>c8<b-8 g c8a8>c8<a8 f c16f16a>c8<a c16f16a>c8<a c8b-8>c8<b-8 g >c8<f2"

chplay(0, mymusic)

def draw()

	cls(0)
	text(formattext("%04i/%i", chpos(0), chsize(0)), 10, 10, 1, 3)
	text(formattext("midi note -> %02i", chgetnotenum(0))+"  ("+chgetnote(0)+")", 10, 30, 1, 3)

	x = 10
	y = 80
	rect(x,y-32,63,64,1,4)
	
	'draw sound 
	for i = 0 to 62
		yf = chframe(ch,i)* 0.25
		lY = (y + yf)-32
		lX = i+x		
		pixel(lX,lY,13)
	next
	
	'loop
	if chpos(0) = 1128 then chplay(0, mymusic) endif

enddef