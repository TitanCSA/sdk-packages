Set-StrictMode -Version Latest
if (-not $IsWindows -or [IntPtr]::Size -ne 8) { throw 'Interceptor requires Windows x64.' }
Add-Type -Path (Join-Path $PSScriptRoot 'lib/FyraInputKeyboardMouseSDKPowerShell.dll')

function Connect-FyraInputKeyboardMouseSDK {
    <#
.SYNOPSIS
Opens a lazy SDK context without installing or capturing from a driver.
#>
    [CmdletBinding()] param()
    [TitanCSA.Products.FyraInputKeyboardMouse.PowerShell.Interceptor]::new()
}
function Initialize-FyraInputKeyboardMouseSDK {
    <#
.SYNOPSIS
Associates supported devices using an already installed driver.
#>
    [CmdletBinding()] param([Parameter(Mandatory)]$Client)
    $Client.Initialize()
}
function Get-FyraInputKeyboardMouseSDKInfo {
    <#
.SYNOPSIS
Returns driver status and SDK information.
#>
    [CmdletBinding()] param([Parameter(Mandatory)]$Client)
    $Client.GetInfo()
}
function Get-FyraInputKeyboardMouseSDKDevice {
    <#
.SYNOPSIS
Lists devices exposed by the matching installed driver.
#>
    [CmdletBinding()] param([Parameter(Mandatory)]$Client)
    $Client.Devices()
}
function Receive-FyraInputKeyboardMouseSDKInput {
    <#
.SYNOPSIS
Receives a complete input batch; forward it or explicitly discard it.
#>
    [CmdletBinding()] param([Parameter(Mandatory)]$Client, [ValidateRange(-1,2147483647)][int]$Timeout = -1)
    Write-Output -NoEnumerate ($Client.Receive($Timeout))
}
function Send-FyraInputKeyboardMouseSDKBatch {
    <#
.SYNOPSIS
Forwards an owned received batch, including edits and blocked flags.
#>
    [CmdletBinding()] param([Parameter(Mandatory)]$Client, [Parameter(Mandatory)]$Batch)
    $Client.Send($Batch)
}
function Remove-FyraInputKeyboardMouseSDKBatch {
    <#
.SYNOPSIS
Deliberately discards the received batch instead of forwarding it.
#>
    [CmdletBinding()] param([Parameter(Mandatory)]$Client, [Parameter(Mandatory)]$Batch)
    $Client.Discard($Batch)
}
function Stop-FyraInputKeyboardMouseSDKWait {
    <#
.SYNOPSIS
Cancels an outstanding native receive operation.
#>
    [CmdletBinding()] param([Parameter(Mandatory)]$Client)
    $Client.Cancel()
}
function Get-FyraInputKeyboardMouseSDKDescriptor {
    <#
.SYNOPSIS
Returns common descriptor data or the native HID report descriptor.
#>
    [CmdletBinding()] param([Parameter(Mandatory)]$Client, [Parameter(Mandatory)][string]$Token, [switch]$Report)
    $Client.GetDescriptor($Token, $Report.IsPresent)
}
function Send-FyraInputKeyboardMouseSDKInput {
    <#
.SYNOPSIS
Injects named state fields into an existing supported input source.
#>
    [CmdletBinding()] param([Parameter(Mandatory)]$Client,
        [Parameter(Mandatory)][ValidateSet('keyboard','mouse','hid','xinput','xinputhid','gip')][string]$Kind,
        [Parameter(Mandatory)][System.Collections.IDictionary]$State, [string]$Token, [byte[]]$Report)
    $values = [System.Collections.Generic.Dictionary[string,long]]::new()
    foreach ($entry in $State.GetEnumerator()) { $values.Add([string]$entry.Key, [long]$entry.Value) }
    $Client.Inject($Kind, $values, $Token, $Report)
}
function Disconnect-FyraInputKeyboardMouseSDK {
    <#
.SYNOPSIS
Cancels pending waits and disposes the SDK context.
#>
    [CmdletBinding()] param([Parameter(Mandatory)]$Client)
    $Client.Dispose()
}
