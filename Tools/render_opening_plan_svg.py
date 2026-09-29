"""Render a top-down SVG review sheet from the Unreal assembly plan.

The drawing is technical, not final concept art. It lets Audrey and the
narrative team review relative positions without installing Unreal.
"""

import argparse
import hashlib
import html
import json
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
PLAN_PATH = ROOT / "Game/Unreal/opening_assembly_plan.json"
OUTPUT_PATH = ROOT / "Game/Blockout/ouverture-centre-ville-plan.svg"
WIDTH, HEIGHT, PADDING = 1400, 900, 70


def colour(actor):
    name = actor["id"]
    if actor["kind"] == "gameplay_placeholder":
        return "#d85b62"
    if actor["kind"] == "event_marker":
        return "#f2ad3b"
    if "road" in name or name == "school_square":
        return "#5b626b"
    if "sidewalk" in name:
        return "#b7afa0"
    if name.startswith("tree_"):
        return "#4e7a52"
    if "roof" in name:
        return "#886657"
    return "#c4aa7a"


def build_svg():
    plan = json.loads(PLAN_PATH.read_text(encoding="utf-8"))
    actors = [actor for actor in plan["actors"] if actor["id"] != "ground"]
    points = []
    for actor in actors:
        x, y = actor["location_cm"][:2]
        sx, sy = actor.get("bounds_size_cm", [0, 0])[:2]
        points.extend([(x - sx / 2, y - sy / 2), (x + sx / 2, y + sy / 2)])
    points.extend((zone["centre_cm"][0], zone["centre_cm"][1]) for zone in plan["zones"])
    points.extend((camera["location_cm"][0], camera["location_cm"][1]) for camera in plan["cameras"])
    min_x = min(x for x, _ in points) - 400
    max_x = max(x for x, _ in points) + 400
    min_y = min(y for _, y in points) - 400
    max_y = max(y for _, y in points) + 400
    scale = min((WIDTH - 2 * PADDING) / (max_x - min_x), (HEIGHT - 2 * PADDING) / (max_y - min_y))

    def screen(position):
        x, y = position[:2]
        return PADDING + (x - min_x) * scale, HEIGHT - PADDING - (y - min_y) * scale

    lines = [
        '<?xml version="1.0" encoding="UTF-8"?>',
        f'<svg xmlns="http://www.w3.org/2000/svg" width="{WIDTH}" height="{HEIGHT}" viewBox="0 0 {WIDTH} {HEIGHT}">',
        f'<metadata>source_sha256={hashlib.sha256(PLAN_PATH.read_bytes()).hexdigest()}</metadata>',
        '<rect width="100%" height="100%" fill="#f4f0e7"/>',
        '<style>text{font-family:Arial,sans-serif;fill:#20242a}.label{font-size:13px}.small{font-size:11px}.title{font-size:24px;font-weight:700}.zone{fill:none;stroke:#2f6e9f;stroke-width:2;stroke-dasharray:8 5}.camera{fill:#2c5b88;stroke:#16334f;stroke-width:1}</style>',
        '<text x="55" y="42" class="title">Ouverture — place de l’école et ruelle (vue du dessus)</text>',
    ]

    for actor in sorted(actors, key=lambda item: item["kind"] != "static_geometry"):
        x, y = screen(actor["location_cm"])
        sx, sy = actor.get("bounds_size_cm", [35, 35])[:2]
        width, height = max(sx * scale, 4), max(sy * scale, 4)
        lines.append(
            f'<rect x="{x - width / 2:.2f}" y="{y - height / 2:.2f}" width="{width:.2f}" height="{height:.2f}" '
            f'fill="{colour(actor)}" stroke="#30343a" stroke-width="0.8"><title>{html.escape(actor["id"])}</title></rect>'
        )
        if actor["kind"] != "static_geometry":
            lines.append(f'<text x="{x + 7:.2f}" y="{y - 7:.2f}" class="small">{html.escape(actor["id"])}</text>')

    for zone in plan["zones"]:
        x, y = screen(zone["centre_cm"])
        radius = 220 * scale
        lines.append(f'<circle cx="{x:.2f}" cy="{y:.2f}" r="{radius:.2f}" class="zone"><title>{html.escape(zone["role"])}</title></circle>')
        lines.append(f'<text x="{x + radius + 4:.2f}" y="{y:.2f}" class="label">{html.escape(zone["id"])}</text>')

    for camera in plan["cameras"]:
        x, y = screen(camera["location_cm"])
        points_svg = f"{x:.2f},{y - 10:.2f} {x - 9:.2f},{y + 8:.2f} {x + 9:.2f},{y + 8:.2f}"
        lines.append(f'<polygon points="{points_svg}" class="camera"><title>{html.escape(camera["id"])}</title></polygon>')
        lines.append(f'<text x="{x + 12:.2f}" y="{y + 4:.2f}" class="small">{html.escape(camera["id"])}</text>')

    legend_y = HEIGHT - 26
    lines.extend([
        f'<text x="55" y="{legend_y}" class="small">Beige : bâtiments · gris : voirie · vert : végétation · rouge : sujets · orange : indices · cercles bleus : zones · triangles : caméras</text>',
        f'<text x="{WIDTH - 390}" y="{legend_y}" class="small">Échelle de travail : 1 m = {scale * 100:.1f} px</text>',
        '</svg>',
    ])
    return "\n".join(lines) + "\n"


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--check", action="store_true", help="fail if the committed SVG is stale")
    args = parser.parse_args()
    expected = build_svg()
    if args.check:
        if not OUTPUT_PATH.exists() or OUTPUT_PATH.read_text(encoding="utf-8") != expected:
            raise SystemExit("Opening SVG is missing or stale; rebuild it without --check")
        print("Opening SVG valid: plan, zones, subjects and cameras are represented.")
        return
    OUTPUT_PATH.write_text(expected, encoding="utf-8")
    print(OUTPUT_PATH)


if __name__ == "__main__":
    main()
