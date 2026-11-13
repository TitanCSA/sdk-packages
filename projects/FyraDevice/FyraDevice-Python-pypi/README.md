# FyraDevice Python SDK

`pip install fyradevice`

Windows x64 Python3.10+. No driver is installed by this package.

```python
from fyradevice import Client
with Client() as client:
    info = client.call("status")
```

[Operation schema and API](docs/API.md). Check errors and the product licensing state before device creation.
