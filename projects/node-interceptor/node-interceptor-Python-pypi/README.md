# node-interceptor Python SDK

`pip install node-interceptor`

Windows x64 Python3.10+. No driver is installed by this package.

```python
from node_interceptor import Client
with Client() as client:
    info = client.call("check", {"device": 1})
```

[Operation schema and API](docs/API.md). Check errors and the product licensing state before device creation.
