import time
from hashlib import sha1

class InfArray():
    def __init__(self):
        self.left = [0]*16
        self.right = [0]*16

    def getarr(self, ind):
        arr = self.right
        if ind < 0: arr, ind = self.left, -ind-1
        if ind >= len(arr): arr.extend([0]* (key - len(arr) + 10))
        return arr, ind

    def __getitem__(self, key):
        arr, key = self.getarr(key)
        return arr[key]

    def __setitem__(self, key, item):
        arr, key = self.getarr(key)
        arr[key] = item

    def __str__(self):
        return ' '.join(map(str, reversed(self.left))) + ' ' + ' '.join(map(str, self.right))

def interpreter(prog: str, inp: str):
    arr = InfArray()
    mptr = 0
    pptr = 0
    iptr = 0
    start = time.time()
    while pptr < len(prog) and time.time() - start <= 10*60:
        c = ord(prog[pptr]) % 6
        if   c == 0: mptr -= 1
        elif c == 1: mptr += 1
        elif c == 2: arr[mptr] += 1
        elif c == 3: arr[mptr] -= 1
        elif c == 4: 
            arr[mptr] = ord(inp[iptr])
            iptr = (iptr + 1) % len(inp)
        elif arr[mptr] == 0: pptr = pptr + 2
        else:
           if len(prog) - pptr <= 2: unhashed = prog[pptr+1:]
           else: unhashed = prog[pptr+1:pptr+3]
           pptr = int(sha1(unhashed.encode('utf8')).hexdigest(), 16) % len(prog) - 1
        pptr = pptr + 1

    return pptr < len(prog), str(arr)


def test_all(progs, inp):
    for prog in progs:
        nohalt, tape = interpreter(prog, inp)
        printf('f {nohalt} {prog} {tape}')
