from pathlib import Path
import hashlib,json,shutil,sys
root=Path(__file__).resolve().parents[1]
entries=[r for r in json.loads((root/'pypi-manifest.json').read_text()) if r['name']==sys.argv[1]]
assert len(entries)==1
dist=root/'dist';dist.mkdir(exist_ok=True)
assert not list(dist.iterdir()), 'Distribution directory must start empty'
for artifact in entries[0]['artifacts']:
    source=root/artifact['path']
    assert source.resolve().is_relative_to(root.resolve())
    assert hashlib.sha256(source.read_bytes()).hexdigest()==artifact['sha256']
    shutil.copy2(source,dist/source.name)
    print('Verified',source.name)
