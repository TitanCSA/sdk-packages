# Interceptor MultiLang for Python

Requires Python 3.10+, Windows x64 and the matching combined Interceptor 1.4.22.0
driver. The wheel bundles the native bridge DLL. Installing this package does
not install the kernel driver.

```sh
python -m pip install interceptor-multilang
```

The installed module is `interceptor`:

```python
from interceptor import Interceptor
with Interceptor() as io:
    print(io.info())
```

For capture, install the matching driver separately and use an administrator
terminal. Split Keyboard/Mouse editions and Interception V2 use different APIs.
Always forward the complete received input batch or explicitly mark events as
blocked. Closing the context releases capture. Ctrl+Alt+Delete is the physical
driver recovery shortcut.

API guide and examples are bundled in both wheel and source distribution.
The native wrapper is MIT; driver demo restrictions/licensing remain separate.

[Documentation](https://github.com/TitanCSA/Interceptor-MultiLang/tree/main/Interceptor-MultiLang-Python-pypi) | [Downloads](https://github.com/TitanCSA/Interceptor-MultiLang/releases)
