using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Threading;
using AutoHotInterception.Helpers;

namespace AutoHotInterception
{
    /*
    Tool to check Scan Codes and Press / Release states
    Note that these are raw scancodes and states as they come from Interception. Some keys (eg extended code keys) will not match AHK key codes!
    */
    public class ScanCodeChecker : IDisposable
    {
        private readonly IntPtr _deviceContext;
        private dynamic _callback;
        private int _deviceId;
        private bool _block;
        private volatile bool stopped;
        private Thread _pollThread;

        public ScanCodeChecker()
        {
            _deviceContext = ManagedWrapper.CreateContext();
        }

        public void Subscribe(int deviceId, dynamic callback, bool block = false)
        {
            _callback = callback;
            _deviceId = deviceId;
            _block = block;

            stopped=false;
            _pollThread = new Thread(PollThread) { IsBackground=true };
            _pollThread.Start();
        }

        private void PollThread()
        {
            try {
            ManagedWrapper.SetFilter(_deviceContext, IsMonitoredDevice, ManagedWrapper.Filter.All);
            int deviceId1;
            int deviceId2;
            var stroke1 = new ManagedWrapper.Stroke();
            var stroke2 = new ManagedWrapper.Stroke();
            while (!stopped)
            {
                var strokes = new List<ManagedWrapper.Stroke>();
                deviceId1=ManagedWrapper.Wait(_deviceContext);
                if(deviceId1==0) {
                    if(stopped)break;
                    throw new InvalidOperationException(ManagedWrapper.GetLastError());
                }
                if (ManagedWrapper.Receive(_deviceContext, deviceId1, ref stroke1, 1) > 0)
                {
                    strokes.Add(stroke1);
                    if (deviceId1 < 11)
                    {
                        if ((deviceId2 = ManagedWrapper.WaitWithTimeout(_deviceContext, 0)) == deviceId1 && ManagedWrapper.Receive(_deviceContext, deviceId2, ref stroke2, 1) > 0)
                        {
                            strokes.Add(stroke2);
                        }
                    }
                    if (!_block)
                    {
                        for (int i = 0; i < strokes.Count; i++)
                        {
                            var stroke = strokes[i];
                            ManagedWrapper.Send(_deviceContext, _deviceId, ref stroke, 1);
                        }
                    }
                    // Use array for callback, as the callback may be AHK code, and dealing with arrays in AHK is way simpler that Lists
                    var keyEvents = new KeyEvent[strokes.Count];
                    for (int i = 0; i < strokes.Count; i++)
                    {
                        var s = strokes[i];
                        keyEvents[i] = new KeyEvent { Code = s.key.code, State = s.key.state };
                    }
                    _callback(keyEvents);
                }
            }
            } catch(Exception error) {
                stopped=true;
                Manager.ReportError(error);
                ManagedWrapper.DestroyContext(_deviceContext);
            }
        }

        public string OkCheck()
        {
            return "OK";
        }

        private int IsMonitoredDevice(int device)
        {
            return (Convert.ToInt32(_deviceId == device) );
        }

        public void Dispose()
        {
            stopped=true;
            ManagedWrapper.Wake(_deviceContext);
            if(_pollThread!=null && _pollThread!=Thread.CurrentThread)_pollThread.Join();
            ManagedWrapper.DestroyContext(_deviceContext);
        }
    }

    public class KeyEvent
    {
        public ushort Code { get; set; }
        public ushort State { get; set; }
    }
}
