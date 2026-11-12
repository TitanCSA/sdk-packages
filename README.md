# TitanCSA SDK package distribution

Reviewed user-mode SDK distributions and registry publishing workflows.
No registry credentials are stored in this repository. PyPI publishing uses
GitHub OIDC with one workflow per pending PyPI project.

| Package | Version | Install | Workflow |
|---|---|---|---|
| browser-control-sdk | 0.2.1 | `python -m pip install browser-control-sdk` | publish-pypi.yml |
| interceptor-multilang | 1.0.1 | `python -m pip install interceptor-multilang` | publish-interceptor-pypi.yml |

Commands become available after a successful registry publication. Workflow logs
and the registry itself determine publication status; committing an archive does
not publish it to PyPI.

Each distribution includes its README, API guide, usage example and licenses.
Interceptor MultiLang requires Windows x64 and a matching separately installed
driver; Browser Control requires the separately installed BrowserControl app.
Installing these packages does not install a kernel driver.

Sources: [Browser Control](https://github.com/TitanCSA/Browser-Control-SDK),
[Interceptor MultiLang](https://github.com/TitanCSA/Interceptor-MultiLang).
Artifact hashes are recorded in pypi-manifest.json and checked before upload.
Licenses are specific to each package and included inside its distributions.
