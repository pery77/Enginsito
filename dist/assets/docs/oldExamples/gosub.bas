REM This program is an example of MY-BASIC
REM For more information, see https://github.com/paladin-t/my_basic/
'yes, copy-pasted :(

begin:
	n = 10
	dim arr(n)
	gosub calc
	gosub show
	print "END"
	end	'end finish whit ERROR 0 (no error) keep calm

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