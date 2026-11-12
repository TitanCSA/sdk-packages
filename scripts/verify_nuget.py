from pathlib import Path
import hashlib,json
root=Path(__file__).resolve().parents[1]
for row in json.loads((root/'nuget-manifest.json').read_text()):
    for a in row['artifacts']:
        path=root/a['path']
        assert path.resolve().is_relative_to(root.resolve())
        assert hashlib.sha256(path.read_bytes()).hexdigest()==a['sha256']
    print('Verified',row['name'],row['version'])
