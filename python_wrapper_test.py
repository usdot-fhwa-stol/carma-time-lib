#!/usr/bin/python3
import unittest
import carma_clockd
import time

"""Test Case for testing basic CarmaClock functionality in python
"""
class TestCarmaClock(unittest.TestCase):

    def testSimClockInitializeException(self):
        sim_clock = carma_clockd.CarmaClock(True)
        with self.assertRaises(ValueError) as e:
            time = sim_clock.nowInMilliseconds()
        
        self.assertTrue('Clock is not initialized!' in str(e.exception))

    def testRealClockTime(self):
        clock = carma_clockd.CarmaClock()
        self.assertAlmostEqual(round(time.time()*1000), clock.nowInMilliseconds(),delta=10)

    def testSimClockUpdate(self):
        sim_clock = carma_clockd.CarmaClock(True)
        sim_clock.update(100)
        self.assertEqual(100, sim_clock.nowInMilliseconds())
        sim_clock.update(1500)
        self.assertEqual(1500, sim_clock.nowInMilliseconds())

if __name__ == '__main__': 
    unittest.main()