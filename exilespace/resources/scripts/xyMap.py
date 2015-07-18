

def xymap(x,y):
    n = abs(x)+abs(y)
    base = 2*n*(n-1)
    print 'n:',n,'base:',base,
    if (x > 0 and y >= 0):
        print 'a. (',x,', ',y,') ',
        return base+x
    elif (x <= 0 and y > 0):
        print 'b. (',x,', ',y,') ',
        return base+n+y
    elif (x < 0 and y <= 0):
        print 'c. (',x,', ',y,') ',
        return base+2*n-x
    else:
        print 'd. (',x,', ',y,') ',
        return base+3*n-y

size = 2

for j in range(-size, size+1):
    for i in range(-size, size+1):
        if (abs(i)+abs(j) == size):
            print xymap(i,j)