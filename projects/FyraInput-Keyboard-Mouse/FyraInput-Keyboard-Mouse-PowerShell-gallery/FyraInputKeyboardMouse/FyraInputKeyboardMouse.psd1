@{
RootModule = 'FyraInputKeyboardMouse.psm1'
ModuleVersion = '1.0.0'
GUID = '63266d73-6f8a-5869-93f4-08ae7648d971'
Author = 'TitanCSA'
Description = 'Windows x64 FyraInput-Keyboard-Mouse input SDK for PowerShell 7.4+. Driver installed separately.'
PowerShellVersion = '7.4'
CompatiblePSEditions = @('Core')
FunctionsToExport = @('Connect-FyraInputKeyboardMouseSDK','Initialize-FyraInputKeyboardMouseSDK','Get-FyraInputKeyboardMouseSDKInfo','Get-FyraInputKeyboardMouseSDKDevice','Receive-FyraInputKeyboardMouseSDKInput','Send-FyraInputKeyboardMouseSDKBatch','Remove-FyraInputKeyboardMouseSDKBatch','Stop-FyraInputKeyboardMouseSDKWait','Get-FyraInputKeyboardMouseSDKDescriptor','Send-FyraInputKeyboardMouseSDKInput','Disconnect-FyraInputKeyboardMouseSDK')
CmdletsToExport = @()
VariablesToExport = @()
AliasesToExport = @()
PrivateData = @{ PSData = @{ Tags = @('Windows','SDK','Keyboard','Mouse','Gamepad'); LicenseUri = 'https://github.com/TitanCSA/FyraInput-Keyboard-Mouse-SDK/blob/main/LICENSE'; ProjectUri = 'https://github.com/TitanCSA/FyraInput-Keyboard-Mouse-SDK'; ReleaseNotes = 'First PowerShell SDK; docs and native bridge included.' } }
}
