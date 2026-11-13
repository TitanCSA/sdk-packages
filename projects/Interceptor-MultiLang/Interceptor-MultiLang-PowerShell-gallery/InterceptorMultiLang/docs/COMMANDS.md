# Cmdlets

- `Connect-Interceptor`: run `Get-Help Connect-Interceptor` for its purpose.
- `Initialize-Interceptor`: run `Get-Help Initialize-Interceptor` for its purpose.
- `Get-InterceptorInfo`: run `Get-Help Get-InterceptorInfo` for its purpose.
- `Get-InterceptorDevice`: run `Get-Help Get-InterceptorDevice` for its purpose.
- `Receive-InterceptorInput`: run `Get-Help Receive-InterceptorInput` for its purpose.
- `Send-InterceptorBatch`: run `Get-Help Send-InterceptorBatch` for its purpose.
- `Remove-InterceptorBatch`: run `Get-Help Remove-InterceptorBatch` for its purpose.
- `Stop-InterceptorWait`: run `Get-Help Stop-InterceptorWait` for its purpose.
- `Get-InterceptorDescriptor`: run `Get-Help Get-InterceptorDescriptor` for its purpose.
- `Send-InterceptorInput`: run `Get-Help Send-InterceptorInput` for its purpose.
- `Disconnect-Interceptor`: run `Get-Help Disconnect-Interceptor` for its purpose.

Received batches retain their owner and ticket. A later receive invalidates older batches. Modify `batch[0].State["scan_code"]` or the HID axis/button field objects, then send the entire batch. `State.Axes` keeps repeated descriptor fields distinct. Use `Blocked` on an input to deliberately suppress it.

