"""Verify the public NuGet payload against the locally reviewed package."""

import argparse
import json
from pathlib import Path
import time

from nuget_payload import fetch, verify


ROOT = Path(__file__).resolve().parents[1]
POLL_SECONDS = 30


def check_packages(rows, wait_seconds):
    pending = list(rows)
    verified = []
    deadline = time.monotonic() + wait_seconds

    while pending:
        missing = []
        for package in pending:
            data = fetch(package)
            if data is None:
                missing.append(package)
                continue
            artifact = ROOT / package['artifacts'][0]['path']
            verify(data, artifact)
            verified.append(package['name'])
            print(f"Public payload verified: {package['name']} {package['version']}")

        pending = missing
        remaining = deadline - time.monotonic()
        if not pending or remaining <= 0:
            break
        print(f'{len(pending)} NuGet version(s) are still indexing.', flush=True)
        time.sleep(min(POLL_SECONDS, remaining))

    return verified, [package['name'] for package in pending]


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--wait-seconds', type=int, default=0,
                        help='Maximum time to retry versions that still return HTTP 404')
    args = parser.parse_args()
    if args.wait_seconds < 0:
        parser.error('--wait-seconds must be non-negative')

    rows = json.loads((ROOT / 'nuget-manifest.json').read_text(encoding='utf-8'))
    verified, pending = check_packages(rows, args.wait_seconds)
    result = {'verified': verified, 'indexing_pending': pending}
    output = ROOT / 'nuget-public-verification.json'
    output.write_text(json.dumps(result, indent=2) + '\n', encoding='utf-8')
    if pending:
        versions = ', '.join(pending)
        raise SystemExit(f'NuGet indexing is pending for {versions}; resume verification without republishing.')


if __name__ == '__main__':
    main()
