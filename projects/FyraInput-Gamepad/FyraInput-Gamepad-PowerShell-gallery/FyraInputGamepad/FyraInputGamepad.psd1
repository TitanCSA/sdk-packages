@{
RootModule = 'FyraInputGamepad.psm1'
ModuleVersion = '1.0.0'
GUID = '8012a295-9fc9-5267-a70f-daeeb80854b0'
Author = 'TitanCSA'
Description = 'Windows x64 FyraInput-Gamepad input SDK for PowerShell 7.4+. Driver installed separately.'
PowerShellVersion = '7.4'
CompatiblePSEditions = @('Core')
FunctionsToExport = @('Connect-FyraInputGamepadSDK','Initialize-FyraInputGamepadSDK','Get-FyraInputGamepadSDKInfo','Get-FyraInputGamepadSDKDevice','Receive-FyraInputGamepadSDKInput','Send-FyraInputGamepadSDKBatch','Remove-FyraInputGamepadSDKBatch','Stop-FyraInputGamepadSDKWait','Get-FyraInputGamepadSDKDescriptor','Send-FyraInputGamepadSDKInput','Disconnect-FyraInputGamepadSDK')
CmdletsToExport = @()
VariablesToExport = @()
AliasesToExport = @()
PrivateData = @{ PSData = @{ Tags = @('Windows','SDK','Keyboard','Mouse','Gamepad'); LicenseUri = 'https://github.com/TitanCSA/FyraInput-Gamepad-SDK/blob/main/LICENSE'; ProjectUri = 'https://github.com/TitanCSA/FyraInput-Gamepad-SDK'; ReleaseNotes = 'First PowerShell SDK; docs and native bridge included.' } }
}
