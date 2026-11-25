"""Prepare a new Interception-V2.Native version from a reviewed package.

Requires the official NuGet CLI. The original C source, headers and licenses
are reused byte for byte. Only the MSBuild target and package docs are rebuilt.
"""

from argparse import ArgumentParser
from pathlib import Path
import re
import subprocess
from tempfile import TemporaryDirectory
from xml.etree import ElementTree as ET
from zipfile import ZipFile

ROOT = Path(__file__).resolve().parents[1]
NAME = "Interception-V2.Native"
SDK = ROOT / "projects/Interception-V2/packaging/native"
PACKAGE_DIR = ROOT / "distributions/nuget"
TARGET = f"build/native/{NAME}.targets"


def prepare(source_version, version, nuget):
    with TemporaryDirectory(prefix="TitanCSA-V2-NuGet-") as scratch:
        stage = Path(scratch)
        source = PACKAGE_DIR / f"{NAME}.{source_version}.nupkg"
        with ZipFile(source) as existing:
            files = [n for n in existing.namelist() if not n.endswith("/") and (
                n.startswith(("build/", "licenses/", "docs/")) or n in ("LICENSE", "NOTICE.txt", "README.md")
            )]
            assert files and all(".." not in Path(n).parts and not Path(n).is_absolute() for n in files)
            existing.extractall(stage, files)
            nuspec = existing.read(f"{NAME}.nuspec")
        targets = stage / TARGET
        correction = "      <DisableSpecificWarnings>4996;%(DisableSpecificWarnings)</DisableSpecificWarnings>"
        canonical = (SDK / "TitanCSA.InterceptionV2.Native.targets").read_text(encoding="utf-8")
        assert correction in canonical
        target_text = targets.read_text(encoding="utf-8-sig")
        object_rule = r"<ObjectFileName>$(IntDir)TitanInterceptionV2_%(Filename).obj</ObjectFileName>"
        if correction not in target_text:
            assert target_text.count(object_rule) == 1
            new_rule = f"\n      {object_rule}\n{correction}\n    "
            targets.write_text(target_text.replace(object_rule, new_rule), encoding="utf-8", newline="\n")

        readme = (
            f"# {NAME} {version}\n\n"
            "Install this NuGet package in a Windows Visual C++ project using its NuGet Package Manager. "
            "Include `interception.h` and call `interception_is_keyboard(1)` for a first check without a context. "
            "The targets compile the original C library into your program. Use the matching product driver "
            "when you are ready to open an input context.\n\n"
            "The 2.0.1 package applies the legacy CRT warning exception to interception.c alone "
            "so it builds in projects that treat warnings as errors. The C source and header are unchanged.\n\n"
            "Project and driver: https://github.com/TitanCSA/Interception-V2\n"
        )
        (stage / "README.md").write_text(readme, encoding="utf-8", newline="\n")
        (stage / "docs/USAGE.md").write_text(readme, encoding="utf-8", newline="\n")
        manifest = ET.fromstring(nuspec)
        namespace = manifest.tag.split("}")[0][1:]
        ET.register_namespace("", namespace)
        metadata = manifest.find(f"{{{namespace}}}metadata")
        metadata.find(f"{{{namespace}}}version").text = version
        metadata.find(f"{{{namespace}}}description").text = (
            "Original Interception C SDK sources for Windows native Visual C++ projects. "
            "Includes the MSBuild integration for Interception V2. The driver is installed separately."
        )
        ET.SubElement(metadata, f"{{{namespace}}}releaseNotes").text = (
            "Scope the legacy CRT compiler warning exception to interception.c for /WX consumers."
        )
        file_list = ET.SubElement(manifest, f"{{{namespace}}}files")
        for name in files:
            attributes = {"src": name}
            if "/" in name:
                attributes["target"] = str(Path(name).parent)
            ET.SubElement(file_list, f"{{{namespace}}}file", attributes)
        spec = stage / f"{NAME}.nuspec"
        ET.ElementTree(manifest).write(spec, encoding="utf-8", xml_declaration=True)
        subprocess.run([str(nuget.resolve()), "pack", str(spec), "-BasePath", str(stage),
                        "-OutputDirectory", str(PACKAGE_DIR), "-NonInteractive"], check=True)


if __name__ == "__main__":
    parser = ArgumentParser(description=__doc__)
    parser.add_argument("--nuget", type=Path, required=True)
    parser.add_argument("source_version")
    parser.add_argument("version")
    args = parser.parse_args()
    if (not re.fullmatch(r"\d+\.\d+\.\d+", args.source_version)
            or not re.fullmatch(r"\d+\.\d+\.\d+", args.version)):
        parser.error("Expected three-part NuGet versions")
    source, destination = (tuple(map(int, value.split("."))) for value in (args.source_version, args.version))
    if destination <= source or (PACKAGE_DIR / f"{NAME}.{args.version}.nupkg").exists():
        parser.error("Choose a new version higher than the source version")
    prepare(args.source_version, args.version, args.nuget)
