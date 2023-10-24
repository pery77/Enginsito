'CHPRESET(ch, preset)   -> Set channel preset 
'CHSET(ch, sequence)    -> Set sequence in this channel
'CHPLAY(ch, [sequence]) -> Play Channel optional -> and set [sequence] optional
'CHSTOP(ch)             -> Stop Channel

'copy pasted from https://en.wikipedia.org/wiki/Music_Macro_Language
let mymusic = "t104l4>c16f16a>c8<a c16f16a>c8<a c8b-8>c8<b-8 g c8a8>c8<a8 f c16f16a>c8<a c16f16a>c8<a c8b-8>c8<b-8 g >c8<f2"
chpreset(0,1) 'also you can put @1 in sequence

chset(0,mymusic)
chplay(0)
'chplay(0,mymusic) 'same of above

def tick()
	'press space stop music
	if keydown(key_space) then chstop(0) endif
enddef