"""Validate the audiovisual asset production contract.

Planned assets may not have a source yet. Anything marked ready for
integration or final must carry complete provenance and licence metadata.
"""

import json
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
MANIFEST = ROOT / "Assets/asset_production_manifest.json"
REQUIRED_FIELDS = {
    "id",
    "category",
    "priority",
    "status",
    "brief",
    "deliverables",
    "dependencies",
    "source",
    "author",
    "license",
    "license_url",
    "contains_private_reference",
}
REQUIRED_IDS = {
    "CHAR_HEROINE",
    "CHAR_ARIANE",
    "CHAR_FILLETTE",
    "CHAR_RAVISSEURS",
    "ENV_CENTRE_VILLE",
    "VEH_FOURGON",
    "AUDIO_OPENING_AMBIENCE",
}
VALID_STATUSES = {"planned", "blocked_narrative", "blockout", "ready_for_integration", "final"}
VALID_PRIORITIES = {"P0", "P1", "P2"}
PROVENANCE_REQUIRED = {"ready_for_integration", "final"}


def validate(data):
    errors = []
    privacy = data.get("privacy_policy", {})
    if privacy.get("personal_photos_allowed") is not False:
        errors.append("personal photos must remain forbidden in the public repository")

    assets = data.get("assets")
    if not isinstance(assets, list) or not assets:
        return ["assets must be a non-empty list"]

    ids = []
    for index, asset in enumerate(assets):
        label = asset.get("id", f"asset[{index}]")
        missing = sorted(REQUIRED_FIELDS - set(asset))
        if missing:
            errors.append(f"{label}: missing fields {', '.join(missing)}")
            continue
        ids.append(asset["id"])
        if asset["status"] not in VALID_STATUSES:
            errors.append(f"{label}: invalid status {asset['status']}")
        if asset["priority"] not in VALID_PRIORITIES:
            errors.append(f"{label}: invalid priority {asset['priority']}")
        if not asset["deliverables"]:
            errors.append(f"{label}: at least one deliverable is required")
        if asset["contains_private_reference"] is not False:
            errors.append(f"{label}: private references cannot be published")
        if asset["status"] in PROVENANCE_REQUIRED:
            for field in ("source", "author", "license", "license_url"):
                if not asset[field]:
                    errors.append(f"{label}: {field} is required before integration")

    duplicates = sorted({asset_id for asset_id in ids if ids.count(asset_id) > 1})
    if duplicates:
        errors.append(f"duplicate asset ids: {', '.join(duplicates)}")
    missing_ids = sorted(REQUIRED_IDS - set(ids))
    if missing_ids:
        errors.append(f"required production assets missing: {', '.join(missing_ids)}")

    known_dependencies = set(ids) | {
        "NARRATIVE_Q2_IDENTITY",
        "NARRATIVE_SPATIAL_REQUIREMENTS",
        "NARRATIVE_VAN_IDENTITY",
    }
    for asset in assets:
        unknown = sorted(set(asset.get("dependencies", [])) - known_dependencies)
        if unknown:
            errors.append(f"{asset.get('id', '?')}: unknown dependencies {', '.join(unknown)}")
    return errors


def main():
    data = json.loads(MANIFEST.read_text(encoding="utf-8"))
    errors = validate(data)
    if errors:
        raise SystemExit("Asset manifest invalid:\n- " + "\n- ".join(errors))
    priorities = {}
    for asset in data["assets"]:
        priorities[asset["priority"]] = priorities.get(asset["priority"], 0) + 1
    counts = ", ".join(f"{key}={priorities[key]}" for key in sorted(priorities))
    print(f"Asset manifest valid: {len(data['assets'])} assets ({counts}); privacy and provenance gates enabled.")


if __name__ == "__main__":
    main()
