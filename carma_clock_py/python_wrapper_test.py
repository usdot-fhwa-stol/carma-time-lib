#!/usr/bin/python3
import unittest
import carma_clock_py
import time

"""Test Case for testing basic CarmaClock functionality in python
"""
class TestCarmaClock(unittest.TestCase):

    def testSimClockInitializeException(self):
        sim_clock = carma_clock_py.CarmaClock(True)
        with self.assertRaises(ValueError) as e:
            sim_clock.nowInMilliseconds()
        
        self.assertIn('Clock is not initialized!',str(e.exception))

    def testRealClockTime(self):
        clock = carma_clock_py.CarmaClock()
        self.assertAlmostEqual(time.time()*1000, clock.nowInMilliseconds(),delta=10)

    def testSimClockUpdate(self):
        sim_clock = carma_clock_py.CarmaClock(True)
        sim_clock.update(100)
        self.assertEqual(100, sim_clock.nowInMilliseconds())
        sim_clock.update(1500)
        self.assertEqual(1500, sim_clock.nowInMilliseconds())

if __name__ == '__main__': 
    unittest.main()