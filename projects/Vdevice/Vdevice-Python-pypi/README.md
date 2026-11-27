# Vdevice Python SDK

Download the [Windows x64 wheel](https://raw.githubusercontent.com/TitanCSA/sdk-packages/f6abf965e5a3e229c1b450322212aa07ec5a4cfc/release-assets/Vdevice/language-sdks-v1.0.0/vdevice-1.0.0-py3-none-win_amd64.whl) and open PowerShell in the download folder.

```powershell
python -m pip install .\vdevice-1.0.0-py3-none-win_amd64.whl
```

Windows x64 Python3.10+. No driver is installed by this package.

[First status check with the signed Vdevice demo](https://titancsa.github.io/guides/vdevice-virtual-usb-mouse/#python-sdk).

```python
from vdevice import Client
with Client() as client:
    info = client.call("status")
```

[Operation schema and API](docs/API.md). Check errors and the product licensing state before device creation.
