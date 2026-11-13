param([Parameter(Mandatory=$true)][string]$Build,[string]$Toolchains)
$ErrorActionPreference='Stop'
$root=Split-Path $PSScriptRoot -Parent
$build=[IO.Path]::GetFullPath($Build)
if(!(Test-Path (Join-Path $build Release\bridge_tests.exe))){throw 'Build the synthetic fixture with Build.ps1 -Test first'}
$tools=if($Toolchains){Get-Content $Toolchains -Raw|ConvertFrom-Json}else{$null}
$log=Join-Path $root obj\Tests;New-Item -ItemType Directory -Force $log|Out-Null
$results=[Collections.Generic.List[object]]::new()
function Quote([string]$Value){
 $b=[Text.StringBuilder]::new();[void]$b.Append('"');$slashes=0
 foreach($c in $Value.ToCharArray()){
  if($c -eq '\'){$slashes++;continue}
  if($c -eq '"'){[void]$b.Append(('\'*(2*$slashes+1)));[void]$b.Append('"')}
  else{[void]$b.Append(('\'*$slashes));[void]$b.Append($c)}
  $slashes=0
 }
 [void]$b.Append(('\'*(2*$slashes)));[void]$b.Append('"');return $b.ToString()
}
function Run([string]$Name,[string]$Exe,[string[]]$Arguments,[string]$Directory=$root){
 if(!$Exe){Write-Output "SKIP $Name (tool not installed)";$results.Add(@{name=$Name;status='skipped'});return}
 $p=[Diagnostics.Process]::new();$p.StartInfo=[Diagnostics.ProcessStartInfo]::new();$p.StartInfo.FileName=$Exe;$p.StartInfo.Arguments=($Arguments|ForEach-Object{Quote $_}) -join ' ';$p.StartInfo.WorkingDirectory=$Directory;$p.StartInfo.UseShellExecute=$false;$p.StartInfo.CreateNoWindow=$true;$p.StartInfo.RedirectStandardOutput=$true;$p.StartInfo.RedirectStandardError=$true
 try{
  $clock=[Diagnostics.Stopwatch]::StartNew();if(!$p.Start()){throw 'Cannot start test'}
  $stdout=$p.StandardOutput.ReadToEndAsync();$stderr=$p.StandardError.ReadToEndAsync()
  if(!$p.WaitForExit(180000)){& "$env:WINDIR\System32\taskkill.exe" /PID $p.Id /T /F|Out-Null;throw ('Timed out: '+$Name)}
  $out=$stdout.GetAwaiter().GetResult();$err=$stderr.GetAwaiter().GetResult();[IO.File]::WriteAllText((Join-Path $log ($Name+'.log')),$out+$err,[Text.UTF8Encoding]::new($false))
  if($p.ExitCode){Write-Output ($out+$err);throw ('Test failed: '+$Name+'; exit '+$p.ExitCode)}
  Write-Output ('PASS '+$Name);$results.Add(@{name=$Name;status='passed';elapsed_ms=$clock.ElapsedMilliseconds})
 }finally{$p.Dispose()}
}
function Command([string]$Name){$c=Get-Command $Name -ErrorAction SilentlyContinue;if($c){return $c.Source};return ''}
$variables=@('INTERCEPTOR_TEST_BACKEND','INTERCEPTOR_NATIVE_PATH','INTERCEPTOR_NODE_PATH','PYTHONPATH','INTERCEPTOR_LIB_DIR','CARGO_TARGET_DIR','PATH')
$saved=@{};foreach($n in $variables){$saved[$n]=[Environment]::GetEnvironmentVariable($n,'Process')}
try{
 $env:INTERCEPTOR_TEST_BACKEND='1';$env:INTERCEPTOR_NATIVE_PATH=Join-Path $build Release\InterceptorBridge.dll;$env:INTERCEPTOR_NODE_PATH=Join-Path $build Release\interceptor_node.node;$env:PYTHONPATH=Join-Path $root bindings\python;$env:INTERCEPTOR_LIB_DIR=Join-Path $build Release;$env:CARGO_TARGET_DIR=Join-Path $root obj\RustTest
 Run Native (Join-Path $build Release\bridge_tests.exe) @()
 Run Python (Command python.exe) @('-m','unittest','discover','-s',(Join-Path $root tests),'-p','test_python.py','-v')
 if(Test-Path $env:INTERCEPTOR_NODE_PATH){Run Node (Command node.exe) @('--test',(Join-Path $root tests\node.test.cjs))}
 Run Dotnet (Command dotnet.exe) @('run','--project',(Join-Path $root tests\dotnet\BindingTests.csproj),'--configuration','Release')
 $javac=if($tools -and $tools.java){$tools.java}else{Command javac.exe}
 if($javac){
  $javaOut=Join-Path $root obj\Java;New-Item -ItemType Directory -Force $javaOut|Out-Null
  Run JavaCompile $javac @('--release','22','-encoding','UTF-8','-d',$javaOut,(Join-Path $root bindings\java\src\titancsa\interceptor\Interceptor.java),(Join-Path $root tests\JavaTests.java))
  Run Java (Join-Path (Split-Path $javac) java.exe) @('--enable-native-access=ALL-UNNAMED','-cp',$javaOut,'JavaTests')
 }else{$results.Add(@{name='Java';status='skipped'})}
 $go=if($tools -and $tools.go){$tools.go}else{Command go.exe}
 Run Go $go @('test','-v') (Join-Path $root bindings\go)
 if($tools -and $tools.rustc){$env:PATH=(Split-Path $tools.rustc)+';'+(Split-Path $tools.cargo)+';'+$env:PATH;$cargo=$tools.cargo}else{$cargo=Command cargo.exe}
 Run Rust $cargo @('test','--manifest-path',(Join-Path $root bindings\rust\Cargo.toml),'--','--include-ignored')
 $results|ConvertTo-Json -Depth 3|Set-Content (Join-Path $log Result.json) -Encoding UTF8
 Write-Output 'Fixture checks finished. No physical driver was opened.'
}finally{foreach($n in $variables){[Environment]::SetEnvironmentVariable($n,$saved[$n],'Process')}}
