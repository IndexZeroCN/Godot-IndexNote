<div align="center">

<img width="1660" height="380" alt="header-light" src="https://github.com/user-attachments/assets/d68cf6b2-d78c-4f6b-af8a-211d16379e77" />

<img src="https://img.shields.io/badge/Godot%20Engine-4.3%2B-blue"> <img src="https://img.shields.io/badge/GDExtension-C%2B%2B-478CBF?logo=c%2B%2B&logoColor=white"> <img src="https://img.shields.io/badge/Platform-Windows-lightgrey?logo=windows"> <img src="https://img.shields.io/badge/Version-0.1.0-green">

</div>

**Index Note** 是一个基于 GDExtension（C++）的 Godot 插件，用于管理音游（节奏游戏）中的 BPM 事件、速度（流速）事件与音符轨道。它以 units（细分拍单位）为基准组织音符，并自动计算每个音符的**时间（秒）**与**位移（displacement）**，方便实现变速、变 BPM 的下落式谱面。

最低兼容版本为 Godot 4.3，可在 Godot 4.3 及更高版本（如 4.7）中使用。

## 特性

- **BpmEventList**：管理 BPM 变化事件，提供 units / beats / 秒之间的换算。
- **SpeedEventList**：管理速度（流速）变化事件，支持线性变速区间与位移换算。
- **NoteKey**：单个音符，携带 `units`、`secs`、`displacement` 与自定义 `properties` 字典。
- **NoteTrack**：音符轨道，自动按 units 排序维护音符，支持二分查找，可选按位移排序。
- 纯 C++ 实现，查找与排序开销小，适合高密度谱面。

## 安装

1. 将 `addons/index-note` 目录复制到你的项目 `addons/` 下（或直接解压 Releases 中的 `addons.zip`）。
2. 在 **项目 > 项目设置 > 插件** 中启用 **Index Note**。
3. 插件为 GDExtension，启用后即可在 GDScript 中直接使用 `NoteTrack`、`NoteKey`、`BpmEventList`、`SpeedEventList` 等类。

## 使用示例（NoteTrack）

```gdscript
# 1. 创建 BPM 事件列表：初始 BPM 120，每拍 480 units
var bpm_events := BpmEventList.create(120.0, 480)

# 2. 创建速度事件列表：初始流速 1.0
var speed_events := SpeedEventList.create(1.0)

# 3. 创建音符轨道（第三个参数为 true 时，额外维护一份按位移排序的音符列表）
var track := NoteTrack.create(bpm_events, speed_events, true)

# 4. 添加 BPM / 速度事件（轨道会自动重算所有音符的 secs 与 displacement）
track.add_bpm(480 * 4, 150.0)              # 第 4 拍起变为 150 BPM
track.add_speed(0, 480 * 8, 1.0, 2.0)      # 前 8 拍流速从 1.0 渐变到 2.0

# 5. 添加音符，可附带任意自定义属性
var note := NoteKey.create(480 * 2)        # 第 2 拍
note.properties = { "type": "tap", "lane": 0 }
track.add_note(note)

print(note.secs)          # 音符时间（秒）
print(note.displacement)  # 音符位移（用于下落位置）

# 6. 按 units 查询（二分查找，返回不大于给定值的最大音符）
var index := track.get_note_index(960.0)
var note2 := track.get_note(960.0)
var note3 := track.get_note_at(0)

# 7. 按位移查询（需创建轨道时开启 sort_note_by_displacement）
var note4 := track.get_note_by_displacement(3.5)
var index2 := track.get_note_index_by_displacement(3.5)

# 8. 删除音符
track.remove_note(note)
track.remove_note_at(0)

# 9. 手动增删事件后，也可手动触发重算（参数为重算起始下标）
track.update_displacements(0)
```

> 注意：`add_note` 会自动按 `units` 保持轨道有序，并从插入位置起重算后续音符；若时间列表已在 NoteTrack 中，请在NoteTrack中进行事件的修改，否则在直接修改 BPM / 速度事件列表后，请调用 `update_displacements(0)` 刷新。

## 从源码构建

需要 Python 与 SCons。仓库以子模块形式包含 `godot-cpp`：

```bash
git submodule update --init
scons target=editor          # 编辑器构建
scons target=template_debug  # 调试模板
```

产物输出到 `addons/index-note/bin/`。
