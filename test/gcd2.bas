input m, n
r = m MOD n
while r <> 0
    m = n
    n = r
    r = m MOD n
wend
print n