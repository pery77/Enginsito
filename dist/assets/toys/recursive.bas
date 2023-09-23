REM SUPER SLOW :(
def fibo(n)
    if n = 0 OR n = 1 then 
        return n
    else
        r = fibo(n-1) + fibo(n-2)
        'print formatText("%i> %i", n, r)
        return r
    endif
enddef

PRINT fibo(21)