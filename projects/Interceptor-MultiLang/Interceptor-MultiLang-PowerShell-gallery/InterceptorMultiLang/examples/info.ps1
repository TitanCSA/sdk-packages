Import-Module InterceptorMultiLang
$io = Connect-Interceptor
try { Get-InterceptorInfo -Client $io } finally { Disconnect-Interceptor -Client $io }
