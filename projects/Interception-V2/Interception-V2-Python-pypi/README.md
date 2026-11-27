# Interception-V2 Python SDK

Download the [Windows x64 wheel](https://raw.githubusercontent.com/TitanCSA/sdk-packages/f6abf965e5a3e229c1b450322212aa07ec5a4cfc/release-assets/Interception-V2/language-sdks-v1.0.0/interception_v2-1.0.0-py3-none-win_amd64.whl) and open PowerShell in the download folder.

```powershell
python -m pip install .\interception_v2-1.0.0-py3-none-win_amd64.whl
```

Windows x64 Python3.10+. No driver is installed by this package.

[First check and device inventory with the signed demo](https://titancsa.github.io/guides/interception-v2-migration/#python-sdk).

```python
from interception_v2 import Client
with Client() as client:
    info = client.call("check", {"device": 1})
```

[Operation schema and API](docs/API.md). Check errors and the product licensing state before device creation.
