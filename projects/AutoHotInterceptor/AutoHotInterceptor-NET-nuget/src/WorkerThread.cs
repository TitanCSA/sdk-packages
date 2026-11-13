using System;
using System.Collections.Concurrent;
using System.Threading;
using System.Threading.Tasks;
namespace AutoHotInterception {
    class WorkerThread : IDisposable {
        private readonly CancellationTokenSource cancellation = new CancellationTokenSource();
        public BlockingCollection<Action> Actions { get; } = new BlockingCollection<Action>(8192);
        private readonly Task worker;
        public WorkerThread() { worker=Task.Run((Action)Run); }
        private void Run() {
            try { foreach(var action in Actions.GetConsumingEnumerable(cancellation.Token)) {
                try { action(); } catch(Exception error) { Manager.ReportError(error); }
            }} catch(OperationCanceledException) {} catch(ObjectDisposedException) {}
        }
        public void Dispose() { cancellation.Cancel(); Actions.CompleteAdding(); }
    }
}
