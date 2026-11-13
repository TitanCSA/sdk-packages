@{
RootModule = 'FyraInput.psm1'
ModuleVersion = '1.0.0'
GUID = '9277f49d-78de-54ef-9949-e2c81608127e'
Author = 'TitanCSA'
Description = 'Windows x64 FyraInput input SDK for PowerShell 7.4+. Driver installed separately.'
PowerShellVersion = '7.4'
CompatiblePSEditions = @('Core')
FunctionsToExport = @('Connect-FyraInputSDK','Initialize-FyraInputSDK','Get-FyraInputSDKInfo','Get-FyraInputSDKDevice','Receive-FyraInputSDKInput','Send-FyraInputSDKBatch','Remove-FyraInputSDKBatch','Stop-FyraInputSDKWait','Get-FyraInputSDKDescriptor','Send-FyraInputSDKInput','Disconnect-FyraInputSDK')
CmdletsToExport = @()
VariablesToExport = @()
AliasesToExport = @()
PrivateData = @{ PSData = @{ Tags = @('Windows','SDK','Keyboard','Mouse','Gamepad'); LicenseUri = 'https://github.com/TitanCSA/FyraInput-SDK/blob/main/LICENSE'; ProjectUri = 'https://github.com/TitanCSA/FyraInput-SDK'; ReleaseNotes = 'First PowerShell SDK; docs and native bridge included.' } }
}
