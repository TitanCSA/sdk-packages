import ctypes as C
import os
import threading
import time
import unittest
from interceptor import Interceptor, Error

class BindingTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        if os.environ.get("INTERCEPTOR_TEST_BACKEND")!="1":raise unittest.SkipTest("Run Build.ps1 -Test; physical-driver tests are never implicit")
    def test_round_trip(self):
        with Interceptor() as io:
            self.assertTrue(io.initialize().ready)
            self.assertEqual(len(io.devices()),6)
            self.assertIn('\u00e9',io.devices()[0].name)
            inputs=io.receive(0)
            self.assertEqual(len(inputs),6)
            self.assertEqual(inputs[0].sequence,0x8000000000000001)
            inputs[0].state.scan_code=0x30;inputs[1].state.y=-inputs[1].state.y
            self.assertEqual(len(inputs[2].state.axes),2)
            self.assertNotEqual(inputs[2].state.axes[0].collection,inputs[2].state.axes[1].collection)
            inputs[2].state.axes[0].value=50;inputs[2].state.axes[0].normalized=.5
            inputs[2].state.buttons[0].pressed=False
            inputs[5].blocked=True
            self.assertEqual(io.send(inputs),5)
            self.assertEqual(io.send(inputs),0)
            dll=io._dll;dll.icb_test_seen.argtypes=[C.c_uint32,C.c_uint32];dll.icb_test_seen.restype=C.c_int64
            self.assertEqual(dll.icb_test_seen(2,1),-7)
            self.assertEqual(dll.icb_test_seen(3,1),50)
            self.assertEqual(dll.icb_test_seen(3,3),0)
            self.assertFalse(io.descriptor('fixture:4',report=True).applicable)
            self.assertEqual(io.inject('mouse',x=200),1)
            newer=io.receive(0)
            with self.assertRaises(ValueError):io.send(inputs)
            io.discard(newer)
    def test_cancel_and_close(self):
        io=Interceptor();io._dll.icb_test_mode(1)
        result=[]
        reader=threading.Thread(target=lambda:result.append(io.receive()))
        reader.start();time.sleep(.03);io.cancel();reader.join(2)
        self.assertFalse(reader.is_alive());self.assertEqual(len(result[0]),0)
        io._dll.icb_test_mode(0);io.close()
        with self.assertRaises(Error):io.receive(0)
    def test_failed_receive_invalidates_previous_batch(self):
        with Interceptor() as io:
            batch=io.receive(0);io.send(batch)
            io._dll.icb_test_mode(3)
            try:
                with self.assertRaises(Error):io.receive(0)
                with self.assertRaises(ValueError):io.send(batch)
            finally:io._dll.icb_test_mode(0)

if __name__=='__main__':unittest.main()
