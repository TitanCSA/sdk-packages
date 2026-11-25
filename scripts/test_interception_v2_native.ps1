param(
    [string]$NuGet = 'nuget.exe',
    [string]$Version,
    [string]$Source
)
$ErrorActionPreference = 'Stop'
$root = Split-Path -Parent $PSScriptRoot
$name = 'Interception-V2.Native'
if (!$Source) { $Source = Join-Path $root 'distributions/nuget' }
if (!$Version) {
    $entries = Get-Content -LiteralPath (Join-Path $root 'nuget-manifest.json') -Raw | ConvertFrom-Json
    $latest = $entries | Where-Object { $_.name -eq $name } |
        Sort-Object -Property { [version]$_.version } -Descending | Select-Object -First 1
    $Version = $latest.version
}
$vswhere = Join-Path ${env:ProgramFiles(x86)} 'Microsoft Visual Studio/Installer/vswhere.exe'
$msbuild = & $vswhere -latest -products '*' -requires Microsoft.Component.MSBuild -find 'MSBuild\**\Bin\MSBuild.exe' |
    Select-Object -First 1
if (!$msbuild) { throw 'Install Visual Studio 2022 with Desktop development with C++.' }

$work = Join-Path ([System.IO.Path]::GetTempPath()) ('TitanCSA-Native-V2-' + [guid]::NewGuid().ToString('N'))
New-Item -ItemType Directory -Path $work | Out-Null
Copy-Item -Path (Join-Path $PSScriptRoot 'tests/interception-v2-native/*') -Destination $work

$packages = Join-Path $work 'packages.config'
Set-Content -LiteralPath $packages -Encoding utf8 -Value "<packages><package id=`"$name`" version=`"$Version`" targetFramework=`"native`" /></packages>"
& $NuGet restore $packages -Source $Source -PackagesDirectory $work -NonInteractive -DirectDownload -NoHttpCache
if ($LASTEXITCODE) { throw "NuGet restore failed. Check $work." }
$targets = Join-Path $work "$name.$Version/build/native/$name.targets"
if (!(Test-Path -LiteralPath $targets)) { throw "Package targets are missing in $work." }
$project = Join-Path $work 'Consumer.vcxproj'
foreach ($arch in ('Win32','x64')) {
    & $msbuild $project /nologo /v:minimal /p:Configuration=Release "/p:Platform=$arch" "/p:NativePackageTargets=$targets"
    if ($LASTEXITCODE) { throw "The /W4 /WX $arch native build failed. Check $work." }
    $exe = Join-Path $work "bin/$arch/Consumer.exe"
    $result = & $exe
    if ($LASTEXITCODE -or $result -ne 'Keyboard slots: 10; mouse slots: 10.') {
        throw "The $arch library check failed. Check $work."
    }
    Write-Output "$arch $name $Version -> $result"
}
