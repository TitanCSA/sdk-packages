@{
RootModule = 'InterceptorMultiLang.psm1'
ModuleVersion = '1.0.0'
GUID = '6ab8236d-bcd4-4cab-85be-bd13b821dbad'
Author = 'TitanCSA'
Description = 'Windows x64 Interceptor input SDK for PowerShell 7.4+. Driver installed separately.'
PowerShellVersion = '7.4'
CompatiblePSEditions = @('Core')
FunctionsToExport = @('Connect-Interceptor','Initialize-Interceptor','Get-InterceptorInfo','Get-InterceptorDevice','Receive-InterceptorInput','Send-InterceptorBatch','Remove-InterceptorBatch','Stop-InterceptorWait','Get-InterceptorDescriptor','Send-InterceptorInput','Disconnect-Interceptor')
CmdletsToExport = @()
VariablesToExport = @()
AliasesToExport = @()
PrivateData = @{ PSData = @{ Tags = @('Windows','SDK','Keyboard','Mouse','Gamepad'); LicenseUri = 'https://github.com/TitanCSA/Interceptor-MultiLang/blob/main/LICENSE'; ProjectUri = 'https://github.com/TitanCSA/Interceptor-MultiLang'; ReleaseNotes = 'First PowerShell SDK; docs and native bridge included.' } }
}
