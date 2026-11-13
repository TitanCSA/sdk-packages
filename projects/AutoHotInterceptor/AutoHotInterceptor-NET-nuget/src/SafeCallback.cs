using System;
using System.Threading;
namespace AutoHotInterception {
    static class SafeCallback {
        public static void Queue(Action callback) {
            ThreadPool.QueueUserWorkItem(_=>{try{callback();}catch(Exception error){Manager.ReportError(error);}});
        }
    }
}
