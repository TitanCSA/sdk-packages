from pathlib import Path
import zipfile,io,urllib.request,urllib.error
def verify(payload,expected):
 with zipfile.ZipFile(io.BytesIO(payload)) as actual, zipfile.ZipFile(expected) as local:
  wanted={n:local.read(n) for n in local.namelist() if not n.endswith('/') and n!='.signature.p7s'}
  received={n:actual.read(n) for n in actual.namelist() if not n.endswith('/') and n!='.signature.p7s'}
  assert received==wanted,'Published package differs from reviewed payload; repository signatures are the only allowed extra file'
def fetch(row):
 name=row['name'].lower();version=row['version'].lower();url='https://api.nuget.org/v3-flatcontainer/'+name+'/'+version+'/'+name+'.'+version+'.nupkg'
 try:
  with urllib.request.urlopen(url,timeout=60) as response:return response.read()
 except urllib.error.HTTPError as error:
  if error.code==404:return None
  raise
