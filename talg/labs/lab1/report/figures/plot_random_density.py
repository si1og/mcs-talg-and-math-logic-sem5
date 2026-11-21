"""Generate the report figure with GNU plotutils from automaton_analysis CSV."""

import csv
import subprocess
import sys
import xml.etree.ElementTree as ET
from collections import defaultdict
from pathlib import Path


SVG = "http://www.w3.org/2000/svg"
ET.register_namespace("", SVG)
COLORS = ("#275d92", "#266c46", "#92531e", "#883f80", "#a02e37")
PLOT_LEFT, PLOT_BOTTOM, PLOT_WIDTH, PLOT_HEIGHT = 0.115, 0.38, 0.78, 0.34
VIEW_TOP, VIEW_WIDTH, VIEW_HEIGHT = 0.267, 0.95, 0.415
PANEL_WIDTH, PANEL_HEIGHT = 300, 130


def chart(series, last_generation, tick):
    blocks = []
    for generations, color in series:
        points = "\n".join(
            f"{generation} {density}" for generation, density in generations
            if generation <= last_generation
        )
        blocks.append(points)
    pens = ":".join(f"{i}={color}" for i, (_, color) in enumerate(series, 1))
    # Blank lines advance graph's line mode to the next pen.
    data = "\n\n".join(blocks) + "\n"
    result = subprocess.run(
        ["graph", "-T", "svg", "-g", "3", "-C", "--pen-colors", pens,
         "--frame-color", "#67717d", "-W", "0.001",
         "-w", str(PLOT_WIDTH), "-h", str(PLOT_HEIGHT),
         "-r", str(PLOT_LEFT), "-u", str(PLOT_BOTTOM),
         "-x", "0", str(last_generation), str(tick),
         "-y", "0", "1", "0.25"],
        input=data, stdout=subprocess.PIPE, encoding="latin-1", check=True,
    )
    svg = ET.fromstring(result.stdout)
    svg.attrib.clear()
    svg.set("viewBox", f"0 {VIEW_TOP} {VIEW_WIDTH} {VIEW_HEIGHT}")
    svg.set("preserveAspectRatio", "none")
    content = svg.find(f"{{{SVG}}}g")
    for tick_label in content.findall(f"{{{SVG}}}text"):
        content.remove(tick_label)
    return svg


def label(parent, x, y, content, size=16, color="#222222", anchor="middle"):
    text = ET.SubElement(parent, f"{{{SVG}}}text", {
        "x": str(x), "y": str(y), "text-anchor": anchor,
        "font-family": "Arial, sans-serif", "font-size": str(size), "fill": color,
    })
    text.text = content
    return text


def add_panel(root, x, y, title, series, last_generation, tick):
    svg = chart(series, last_generation, tick)
    svg.attrib.update(x=str(x), y=str(y), width=str(PANEL_WIDTH), height=str(PANEL_HEIGHT))
    root.append(svg)
    left = x + PANEL_WIDTH * PLOT_LEFT / VIEW_WIDTH
    width = PANEL_WIDTH * PLOT_WIDTH / VIEW_WIDTH
    bottom = y + PANEL_HEIGHT * (1 - PLOT_BOTTOM - VIEW_TOP) / VIEW_HEIGHT
    height = PANEL_HEIGHT * PLOT_HEIGHT / VIEW_HEIGHT
    title_color = series[0][1] if len(series) == 1 else "#222222"
    label(root, left + width / 2, y - 7, title, color=title_color)
    for t in range(0, last_generation + 1, tick):
        label(root, left + width * t / last_generation, bottom + 13,
              f"{t:,}".replace(",", "\u202f"), size=11)
    for rank, density in enumerate(("0", "0,25", "0,5", "0,75", "1")):
        label(root, left - 5, bottom - height * rank / 4 + 4,
              density, size=11, anchor="end")


def main():
    series = defaultdict(list)
    for row in csv.DictReader(sys.stdin):
        series[row["probability"]].append(
            (int(row["generation"]), float(row["mean_density"]))
        )
    root = ET.Element(f"{{{SVG}}}svg", viewBox="0 0 660 494")
    ET.SubElement(root, f"{{{SVG}}}title").text = "Средняя плотность живых клеток"
    curves = list(zip(series.values(), COLORS, strict=True))
    for index, (p, curve) in enumerate(zip(series, curves, strict=True)):
        x, y = 35 + index % 2 * 310, 22 + index // 2 * 153
        add_panel(root, x, y, f"p = {p.replace('.', ',')}", [curve], 10000, 2500)
    add_panel(root, 345, 328, "Первые 120 поколений", curves, 120, 30)
    label(root, 345, 485, "Поколение")
    vertical = label(root, 16, 224, "Средняя плотность")
    vertical.set("transform", "rotate(-90 16 224)")
    ET.ElementTree(root).write(
        Path(__file__).with_name("population-10x10.svg"), encoding="utf-8",
        xml_declaration=True,
    )


if __name__ == "__main__":
    main()
