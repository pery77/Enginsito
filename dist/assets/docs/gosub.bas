'GOSUB   ->  Transfers the program execution to a specified subroutine or procedure and returns to the calling code afterwards.
'RETURN  ->  Marks the end of a subroutine or procedure and returns to the calling code.
'END     ->  Marks the end of the program or a code block.

REM This program is an example of MY-BASIC
REM For more information, see https://github.com/paladin-t/my_basic/
'yes, copy-pasted ...

'[
CALL/DEF/ENDDEF statements and instructional routine with the
GOSUB/RETURN (and GOTO) statements, but they can't be mixed
together in one program.
']

begin:
	n = 10
	dim arr(n)
	gosub calc
	gosub show
	print "END"
	end	'end here !!

calc:
	arr(0) = 1
	for i = 1 to n - 1
		if i = 1 then arr(i) = 1 else arr(i) = arr(i - 1) + arr(i - 2)
	next
	return

show:
	for i = 0 to n - 1
		print arr(i)
	next
	return