<p align="center">
  <img src="banner.png" alt="Index Note Banner" />
</p>

<p align="center">
  <a href="https://github.com/IndexZeroCN/Godot-IndexNote/releases">
    <img src="https://img.shields.io/github/v/release/IndexZeroCN/Godot-IndexNote?include_prereleases&style=flat-square" alt="Release" />
  </a>
  <a href="#platform-support">
    <img src="https://img.shields.io/badge/platform-Windows-lightgrey?style=flat-square&logo=windows" alt="Platform" />
  </a>
  <a href="https://godotengine.org">
    <img src="https://img.shields.io/badge/Godot-4.3%2B-478CBF?style=flat-square&logo=godot-engine" alt="Godot Version" />
  </a>
  <a href="https://godot-indexnote.readthedocs.io">
    <img src="https://img.shields.io/badge/docs-online-2ea44f?style=flat-square" alt="Documentation" />
  </a>
</p>

<p align="center">
  <a href="README_CN.md">中文</a>
</p>

**Index Note** is a Godot plugin built with GDExtension (C++) for managing BPM events, speed events, and note tracks in rhythm games. It organizes notes based on **units** (subdivided beat units) and automatically computes each note's **time (seconds)** and **displacement**, making it easy to implement variable speed and variable BPM scrolling charts.

Minimum compatible version is Godot 4.3; it works with Godot 4.3 and later (e.g., 4.7).

## Features

- **BpmEventList** — Manages BPM change events, providing conversions between units, beats, and seconds.
- **SpeedEventList** — Manages speed change events, supporting linear speed transitions and displacement calculations.
- **NoteKey** — A single note carrying `units`, `secs`, `displacement`, and a custom `properties` dictionary.
- **NoteTrack** — A note track that maintains notes sorted by units, supports binary search, and optionally maintains a displacement-sorted note list.
- Pure C++ implementation with low lookup and sorting overhead, suitable for high-density charts.

## Installation

1. Copy the `addons/index-note` directory into your project's `addons/` folder (or extract `addons.zip` from Releases).
2. Enable **Index Note** in **Project > Project Settings > Plugins**.
3. The plugin is a GDExtension — once enabled, classes such as `NoteTrack`, `NoteKey`, `BpmEventList`, and `SpeedEventList` are available directly in GDScript.

## Usage Example (NoteTrack)

```gdscript
# 1. Create a BPM event list: initial BPM 120, 480 units per beat
var bpm_events := BpmEventList.create(120.0, 480)

# 2. Create a speed event list: initial speed 1.0
var speed_events := SpeedEventList.create(1.0)

# 3. Create a note track (third param = true maintains an extra displacement-sorted note list)
var track := NoteTrack.create(bpm_events, speed_events, true)

# 4. Add BPM / speed events (the track auto-recalculates secs & displacement for all notes)
track.add_bpm(480 * 4, 150.0)              # Beat 4 onward switches to 150 BPM
track.add_speed(0, 480 * 8, 1.0, 2.0)      # First 8 beats: speed 1.0 → 2.0 linear

# 5. Add notes with arbitrary custom properties
var note := NoteKey.create(480 * 2)        # Beat 2
note.properties = { "type": "tap", "lane": 0 }
track.add_note(note)

print(note.secs)          # Note time in seconds
print(note.displacement)  # Note displacement (for scroll position)

# 6. Query by units (binary search, returns the largest note ≤ given units)
var index := track.get_note_index(960.0)
var note2 := track.get_note(960.0)
var note3 := track.get_note_at(0)

# 7. Query by displacement (requires sort_note_by_displacement enabled when creating the track)
var note4 := track.get_note_by_displacement(3.5)
var index2 := track.get_note_index_by_displacement(3.5)

# 8. Remove notes
track.remove_note(note)
track.remove_note_at(0)

# 9. After manually adding/removing events, you can also manually trigger recalculation (param = start index)
track.update_displacements(0)
```

> Note: `add_note` automatically keeps the track sorted by `units` and recalculates subsequent notes from the insertion point. If the event lists are held by the NoteTrack, modify events through the NoteTrack; otherwise, after directly modifying the BPM / speed event lists, call `update_displacements(0)` to refresh.

## Building from Source

Requires Python and SCons. The repository includes `godot-cpp` as a submodule:

```bash
git submodule update --init
scons target=editor          # Editor build
scons target=template_debug  # Debug template
```

Output goes to `addons/index-note/bin/`.
