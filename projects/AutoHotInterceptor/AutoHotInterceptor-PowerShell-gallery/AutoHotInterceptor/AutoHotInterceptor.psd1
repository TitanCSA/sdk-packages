@{
RootModule = 'AutoHotInterceptor.psm1'
ModuleVersion = '1.0.0'
GUID = 'dd3069e7-9d63-5ae3-ba3c-419ebd650a82'
Author = 'TitanCSA'
Description = 'Windows x64 AutoHotInterceptor input SDK for PowerShell 7.4+. Driver installed separately.'
PowerShellVersion = '7.4'
CompatiblePSEditions = @('Core')
FunctionsToExport = @('Connect-AutoHotInterceptorSDK','Initialize-AutoHotInterceptorSDK','Get-AutoHotInterceptorSDKInfo','Get-AutoHotInterceptorSDKDevice','Receive-AutoHotInterceptorSDKInput','Send-AutoHotInterceptorSDKBatch','Remove-AutoHotInterceptorSDKBatch','Stop-AutoHotInterceptorSDKWait','Get-AutoHotInterceptorSDKDescriptor','Send-AutoHotInterceptorSDKInput','Disconnect-AutoHotInterceptorSDK')
CmdletsToExport = @()
VariablesToExport = @()
AliasesToExport = @()
PrivateData = @{ PSData = @{ Tags = @('Windows','SDK','Keyboard','Mouse','Gamepad'); LicenseUri = 'https://github.com/TitanCSA/AutoHotInterceptor/blob/main/LICENSE'; ProjectUri = 'https://github.com/TitanCSA/AutoHotInterceptor'; ReleaseNotes = 'First PowerShell SDK; docs and native bridge included.' } }
}
