param([switch]$Test,[switch]$Packages,[switch]$NoNode,[switch]$IncludeDriver,[string]$Toolchains)
$ErrorActionPreference='Stop'
$root=$PSScriptRoot
if(![Environment]::Is64BitOperatingSystem){throw 'Windows x64 is required'}
$obj=Join-Path $root 'obj';New-Item -ItemType Directory -Force $obj|Out-Null
$build=Join-Path $obj $(if($Test){'Fixture'}else{'Native'})
$args=@('-S',$root,'-B',$build,'-A','x64',('-DICB_TEST_BACKEND='+$(if($Test){'ON'}else{'OFF'})))
$node=Get-Command node.exe -ErrorAction SilentlyContinue
function Fetch([string]$Url,[string]$Path,[string]$Hash){
 if(!(Test-Path -LiteralPath $Path) -or (Get-FileHash $Path).Hash -ne $Hash){Invoke-WebRequest -UseBasicParsing $Url -OutFile $Path}
 if((Get-FileHash $Path).Hash -ne $Hash){throw ('Download checksum mismatch: '+$Path)}
}
if($node -and !$NoNode){
 $version=(& $node.Source -p process.versions.node).Trim()
 $cache=Join-Path $env:LOCALAPPDATA ('node-gyp\Cache\'+$version)
 if(!(Test-Path (Join-Path $cache include\node\node_api.h)) -or !(Test-Path (Join-Path $cache x64\node.lib))){
  $cache=Join-Path $obj ('Node-'+$version);New-Item -ItemType Directory -Force $cache|Out-Null
  $base='https://nodejs.org/download/release/v'+$version+'/'
  $sums=(Invoke-WebRequest -UseBasicParsing ($base+'SHASUMS256.txt')).Content
  $headers='node-v'+$version+'-headers.tar.gz'
  $hashes=@{}
  foreach($line in ($sums -split "`n")){if($line -match '^([a-f0-9]{64})\s+(.+?)\s*$'){$hashes[$matches[2]]=$matches[1]}}
  if(!$hashes[$headers] -or !$hashes['win-x64/node.lib']){throw 'Official Node checksums are incomplete'}
  Fetch ($base+$headers) (Join-Path $cache headers.tar.gz) $hashes[$headers]
  Fetch ($base+'win-x64/node.lib') (Join-Path $cache node.lib) $hashes['win-x64/node.lib']
  & tar -xzf (Join-Path $cache headers.tar.gz) -C $cache
  if($LASTEXITCODE){throw 'Cannot extract verified Node headers'}
  $nodeHeaders=Join-Path $cache ('node-v'+$version+'\include\node');$nodeLibrary=Join-Path $cache node.lib
 }else{$nodeHeaders=Join-Path $cache include\node;$nodeLibrary=Join-Path $cache x64\node.lib}
 $args+=@(('-DNODE_HEADERS='+$nodeHeaders),('-DNODE_LIBRARY='+$nodeLibrary))
}else{$args+=@('-DNODE_HEADERS=','-DNODE_LIBRARY=')}
& cmake @args
if($LASTEXITCODE){throw 'Native configuration failed'}
& cmake --build $build --config Release --parallel
if($LASTEXITCODE){throw 'Native build failed'}
if($Test){& (Join-Path $root 'tests\Run.ps1') -Build $build -Toolchains $Toolchains;return}
$runtime=Join-Path $root 'runtimes\win-x64';New-Item -ItemType Directory -Force $runtime|Out-Null
Copy-Item (Join-Path $build 'Release\InterceptorBridge.dll'),(Join-Path $build 'Release\InterceptorBridge.lib'),(Join-Path $root 'native\InterceptorBridge.h') $runtime -Force
Copy-Item (Join-Path $runtime InterceptorBridge.dll) (Join-Path $root 'bindings\python\interceptor\InterceptorBridge.dll') -Force
if($node -and !$NoNode){$prebuild=Join-Path $root 'bindings\node\prebuilds\win32-x64';New-Item -ItemType Directory -Force $prebuild|Out-Null;Copy-Item (Join-Path $build 'Release\interceptor_node.node'),(Join-Path $runtime InterceptorBridge.dll) $prebuild -Force}
$tools=if($Toolchains){Get-Content $Toolchains -Raw|ConvertFrom-Json}else{$null}
$javac=if($tools -and $tools.java){$tools.java}else{(Get-Command javac.exe -ErrorAction SilentlyContinue).Source}
if($javac){
 $javaOut=Join-Path $obj JavaPackage;New-Item -ItemType Directory -Force $javaOut|Out-Null
 & $javac --release 22 -encoding UTF-8 -d $javaOut (Join-Path $root 'bindings\java\src\titancsa\interceptor\Interceptor.java')
 if($LASTEXITCODE){throw 'Java 22+ wrapper build failed'}
 & (Join-Path (Split-Path $javac) jar.exe) --create --file (Join-Path $root 'bindings\java\Interceptor.jar') -C $javaOut .
 if($LASTEXITCODE){throw 'Java JAR creation failed'}
}
if(Get-Command dotnet.exe -ErrorAction SilentlyContinue){& dotnet build (Join-Path $root 'bindings\dotnet\Interceptor.csproj') -c Release;if($LASTEXITCODE){throw '.NET build failed'}}
if($IncludeDriver){
 $driver=Join-Path $root driver;New-Item -ItemType Directory -Force $driver|Out-Null
 $base='https://github.com/TitanCSA/Interceptor/releases/download/v1.4.22.0-demo/'
 Fetch ($base+'InterceptorSetup.exe') (Join-Path $driver InterceptorSetup.exe) 'fd9c449fa35ba40174176e1ad2f34e682fb8977730f9238c7c976e355f0b083f'
 Fetch ($base+'InterceptorSetupConsole.exe') (Join-Path $driver InterceptorSetupConsole.exe) 'adfd6fde565aee57a1c7d669b92c4cf9ea6be0789f37553bad32ee0a5eb8fbe2'
 & (Join-Path $driver InterceptorSetupConsole.exe) --self-test
 if($LASTEXITCODE){throw 'Bundled driver installer integrity check failed'}
}
if($Packages){
 $packageDirectory=Join-Path $root packages;New-Item -ItemType Directory -Force $packageDirectory|Out-Null
 if(Get-Command python.exe -ErrorAction SilentlyContinue){& python -m pip wheel (Join-Path $root 'bindings\python') --no-deps --wheel-dir $packageDirectory --disable-pip-version-check;if($LASTEXITCODE){throw 'Python wheel build failed'}}
 if($node -and !$NoNode){& npm.cmd pack (Join-Path $root 'bindings\node') --pack-destination $packageDirectory --silent;if($LASTEXITCODE){throw 'Node package build failed'}}
 if(Get-Command dotnet.exe -ErrorAction SilentlyContinue){& dotnet pack (Join-Path $root 'bindings\dotnet\Interceptor.csproj') -c Release -o $packageDirectory;if($LASTEXITCODE){throw '.NET package build failed'}}
}
Write-Output 'Interceptor user-mode wrappers built. No driver was installed.'
