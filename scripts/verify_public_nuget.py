from pathlib import Path
import json,time
from nuget_payload import fetch,verify
root=Path(__file__).resolve().parents[1];pending=[];verified=[]
for row in json.loads((root/'nuget-manifest.json').read_text()):
 data=fetch(row)
 if data is None:pending.append(row['name']);continue
 verify(data,root/row['artifacts'][0]['path']);verified.append(row['name']);print('Public payload verified:',row['name'],flush=True)
(root/'nuget-public-verification.json').write_text(json.dumps({'verified':verified,'indexing_pending':pending},indent=2)+'\n')
if pending:raise SystemExit('NuGet indexing is pending for '+str(len(pending))+' packages; resume verification without republishing.')
