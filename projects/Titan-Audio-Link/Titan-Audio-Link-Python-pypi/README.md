# Titan-Audio-Link Python SDK

Download the [Windows x64 wheel](https://raw.githubusercontent.com/TitanCSA/sdk-packages/f6abf965e5a3e229c1b450322212aa07ec5a4cfc/release-assets/Titan-Audio-Link/language-sdks-v1.0.0/titan_audio_link-1.0.0-py3-none-win_amd64.whl) and open PowerShell in the download folder.

```powershell
python -m pip install .\titan_audio_link-1.0.0-py3-none-win_amd64.whl
```

Windows x64 Python3.10+. No driver is installed by this package.

[Verify an active playback and recording pair with the signed demo](https://titancsa.github.io/guides/titan-audio-link-first-run/#python-sdk).

```python
from titan_audio_link import Client
with Client() as client:
    info = client.call("list")
```

[Operation schema and API](docs/API.md). Check errors and the product licensing state before device creation.
