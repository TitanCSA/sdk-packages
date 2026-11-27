# Interceptor-Keyboard-Mouse Python SDK

Download the [Windows x64 wheel](https://raw.githubusercontent.com/TitanCSA/sdk-packages/f6abf965e5a3e229c1b450322212aa07ec5a4cfc/release-assets/Interceptor-Keyboard-Mouse/input-language-sdks-v1.0.0/interceptor_keyboard_mouse-1.0.0-py3-none-win_amd64.whl) and open PowerShell in the download folder.

```powershell
python -m pip install .\interceptor_keyboard_mouse-1.0.0-py3-none-win_amd64.whl
```

[Check the status of the signed demo driver](https://titancsa.github.io/guides/keyboard-mouse-first-run/#python-sdk).

Windows x64, Python 3.10+. This package uses the product's own native SDK and driver identity. Supported families: keyboard, mouse. No Python dependencies and no automatic driver installation.

```python
from interceptor_keyboard_mouse import Client
with Client() as client:
    print(client.info())
    # Input operations require the matching driver installed separately.
```

See [API guide](docs/API.md). `Client` aliases the input-oriented `Interceptor` class. Native product licensing and trial restrictions are unchanged. Installing/importing this SDK does not initialize capture.
