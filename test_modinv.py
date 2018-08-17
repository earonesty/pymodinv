from unittest import TestCase

from pymodinv import modinv

import timeit

def py_egcd(a, b):
    if a == 0:
        return (b, 0, 1)
    else:
        g, y, x = py_egcd(b % a, a)
        return (g, x - (b // a) * y, y)

def py_modinv(a, m):
    g, x, y = py_egcd(a, m)
    if g != 1:
        raise Exception('modular inverse does not exist')
    else:
        return x % m

class TestModinv(TestCase):
    def test_modinv(self):
        for tup in( (3,7), (3249867234,23), (3, 11)):
            x=py_modinv(*tup)
            y=modinv(*tup)
            assert x==y, "{} : {} != {}".format(tup, x, y)

    def test_nomodinv(self):
        for tup in( (3,22), (3249867234,8), (3, 4)):
            self.assertRaises(Exception, py_modinv,tup)
            self.assertRaises(Exception, modinv,tup)

    def test_big(self):
        m=308806913904040854273720713382602773723
        for a in range(2,1000):
            x=py_modinv(a,m)
            y=modinv(a,m)
            assert x==y, "{} : {} != {}".format((a,m), x, y)

    def test_time(self):
        m=7037571432493595415108103882256535264262010653258424750253951044516996045672112478562470086435516573756877408682623837202766124762593008963348753173298617
        a=308806913904040854273720713382602773723
        x=timeit.timeit(lambda: py_modinv(a,m), number=1000);
        y=timeit.timeit(lambda: modinv(a,m), number=1000);

        assert(y < x)


