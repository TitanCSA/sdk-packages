# Build and maintenance

## Organize a project README

1. **First steps.** Say what the project does. Give the supported Windows version, the first download or command, and the result to look for. Link to a longer procedure when it helps.
2. **Develop.** For a packaged SDK, link to the project under `projects/` in this repository. Put registry commands, prerequisites and examples there. For a standalone app, link to its own build or integration instructions.
3. **Maintain.** Keep build steps near the product source. Put longer setup and troubleshooting pages in the product documentation, and keep examples with the code they exercise.

An installer configures the product. A driver is loaded by Windows. An SDK is used in another application. Say which parts are required for the first example. For a driver sold without the one-hour demo limit, direct the reader to **titancsa** on Discord for private purchase details.

## Find the source and build files

Start at the product's root `README.md` and use its build instructions. For the four related input demos, the public source uses the same layout.

| Task | Where to work |
| --- | --- |
| Build the demo and see its targets | `Build.ps1` and `CMakeLists.txt` |
| Change the Windows driver | `driver/` |
| Change setup | `Installer/UI/` for the setup window, `Installer/Console/` for command-line setup |
| Change an example | `Examples/` |
| Update installation or API instructions | `docs/` |
| Update a language binding or registry package | The product's folder under `projects/` in this repository |

The [Interceptor](https://github.com/TitanCSA/Interceptor#readme), [Keyboard & Mouse](https://github.com/TitanCSA/Interceptor-Keyboard-Mouse#readme), [Gamepad & Controller](https://github.com/TitanCSA/Interceptor-Gamepad-Controller#readme) and [Vdevice](https://github.com/TitanCSA/Vdevice#readme) repositories use these paths. For older layouts, follow the product's documented entry points and preserve paths used by existing builds and SDK imports. Keep temporary build output outside the source directories.

## Update an SDK or release

1. Find the project with `centralized-projects.json`. Change the package source, recipe or example in its existing central directory. Keep published package names and versioned imports usable.
2. Update the central package guide and its `SOURCE-MANIFEST.json` with the exact tested command and version. Check installation from the real registry before marking a version available. Record the driver or app version used for the example.
3. For a new product demo, update the approved download and SHA-256 in the [portal release catalogue](https://github.com/TitanCSA/TitanCSA.github.io/blob/main/releases.json). The [portal maintenance commands](https://github.com/TitanCSA/TitanCSA.github.io#update-a-published-sdk-example) verify the downloads and build the first SDK examples.

Run `python scripts/verify_project_catalogue.py` from this repository to check the project index and its documentation paths.
