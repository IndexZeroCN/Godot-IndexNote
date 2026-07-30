使用指南
========

基本概念
--------

- **units**：音符和事件的定位单位，通常设置为每拍包含固定数量的 units
  （如每拍 480 units）。
- **BpmEventList**：管理 BPM 变化事件的列表。
- **SpeedEventList**：管理速度/流速变化事件的列表。
- **NoteKey**：单个音符，携带 units、时间、位移与自定义属性。
- **NoteTrack**：音符轨道，将以上组件组合在一起，自动维护音符排序与重算。

快速上手（NoteTrack）
---------------------

.. code-block:: gdscript

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
    var note := NoteKey.create(480 * 2)         # 第 2 拍
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

注意事项
--------

- ``add_note`` 会自动按 ``units`` 保持轨道有序，并从插入位置起重算后续音符。
- 若时间列表已在 NoteTrack 中，请通过 NoteTrack 的方法修改事件。如果直接修改
  BPM/速度事件列表后，请调用 ``update_displacements(0)`` 刷新。
