from pathlib import Path
import json,os,subprocess
from nuget_payload import fetch,verify
root=Path(__file__).resolve().parents[1];key=os.environ['NUGET_API_KEY']
for row in json.loads((root/'nuget-manifest.json').read_text()):
 artifact=root/row['artifacts'][0]['path'];existing=fetch(row)
 if existing is not None:
  verify(existing,artifact);print('Existing reviewed version verified:',row['name'],flush=True);continue
 result=subprocess.run(['dotnet','nuget','push',str(artifact),'--source','https://api.nuget.org/v3/index.json'],capture_output=True,text=True,env=os.environ.copy())
 print((result.stdout+result.stderr).replace(key,'***'),flush=True)
 if result.returncode:raise SystemExit('NuGet rejected '+row['name']+'; inspect the redacted output. Existing versions are never replaced.')
 print('Submitted',row['name'],row['version'],flush=True)
