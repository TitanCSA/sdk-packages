from pathlib import Path
import json,os,subprocess
root=Path(__file__).resolve().parents[1]
key=os.environ['NUGET_API_KEY']
for row in json.loads((root/'nuget-manifest.json').read_text()):
    for artifact in row['artifacts']:
        cmd=['dotnet','nuget','push',str(root/artifact['path']),'--api-key',key,'--source','https://api.nuget.org/v3/index.json']
        result=subprocess.run(cmd,capture_output=True,text=True)
        print((result.stdout+result.stderr).replace(key,'***'),flush=True)
        if result.returncode:raise SystemExit('NuGet rejected '+row['name']+'; inspect the redacted output above.')
    print('Submitted',row['name'],row['version'],flush=True)
