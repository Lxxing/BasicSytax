import PYWrapper
#from PYWrapper import TestCallBack

PYWrapper.greet()
planet = PYWrapper.World()
planet.set('tgest')
planet.greet()
pln = PYWrapper.World('con')
pln.greet()
x = PYWrapper.Var('pi')
x.value = 3.14
print (x.name, 'is around', x.value)


def t(sz):
    print(sz)
    
PYWrapper.TestCallBack("py 2 c++",t)
