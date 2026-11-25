"""Check that each SDK catalogue entry resolves to one central project guide."""

import json
from pathlib import Path
import re
from urllib.parse import urlsplit


ROOT = Path(__file__).resolve().parents[1]
REPO_PATH = re.compile(r"/(?:TitanCSA|titanfyre)/[A-Za-z0-9._-]+", re.IGNORECASE)


def verify_catalogue(root):
    data = json.loads((root / "centralized-projects.json").read_text(encoding="utf-8"))
    repositories = set()
    directories = set()
    for row in data["projects"]:
        project = row["project"]
        repo = urlsplit(row["original_repository"])
        directory = row["central_directory"]
        if not re.fullmatch(r"[A-Za-z0-9_-]+", project) or directory != f"projects/{project}":
            raise ValueError(f"{project}: expected its own directory under projects/")
        if repo.scheme != "https" or repo.netloc != "github.com" or not REPO_PATH.fullmatch(repo.path):
            raise ValueError(f"{project}: expected an official GitHub project URL")
        if repo.path.casefold() in repositories or directory.casefold() in directories:
            raise ValueError(f"{project}: duplicate repository or SDK directory")
        guide = root / directory / "README.md"
        if not guide.is_file() or row["original_repository"] not in guide.read_text(encoding="utf-8"):
            raise ValueError(f"{project}: {guide.relative_to(root)} must link to the product repository")
        repositories.add(repo.path.casefold())
        directories.add(directory.casefold())
    project_guides = {
        f"projects/{folder.name}".casefold() for folder in (root / "projects").iterdir()
        if folder.is_dir() and (folder / "README.md").is_file()
    }
    if not directories or directories != project_guides:
        raise ValueError(f"SDK guides and index differ: {sorted(directories ^ project_guides)}")
    return len(directories)


if __name__ == "__main__":
    print(f"Verified {verify_catalogue(ROOT)} central SDK project guides.")
