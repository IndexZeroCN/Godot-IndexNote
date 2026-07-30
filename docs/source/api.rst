API 参考
=========

所有类均可直接在 GDScript 中使用，无需额外的 ``new()`` — 请使用各类的静态
``create()`` 工厂方法构造实例。

BpmEvent
--------

单个 BPM 改变事件。

.. code-block:: gdscript

    static BpmEvent create(int units, double bpm)

    int units              # 事件发生的 units 位置
    double bpm             # 该事件处开始的 BPM 值
    double pre_secs        # 该事件之前累计经过的时间（秒，自动计算）

BpmEventList
------------

BPM 事件列表，管理从初始 BPM 到后续 BPM 变化的完整时间线。

**工厂方法**

.. code-block:: gdscript

    static BpmEventList create(double init_bpm, int units_abeat)

    double init_bpm        # 初始 BPM
    int units_abeat        # 每拍包含的 units 数量

**方法**

.. code-block:: gdscript

    void add_event(BpmEvent event)
    bool remove_event(BpmEvent event)
    void remove_event_at(int index)
    int  get_event_index(double units) const
    BpmEvent get_event(double units) const
    BpmEvent get_event_at(int index) const
    int  size() const

    void add_bpm(int units, double bpm)
    double get_bpm(double units) const

    double get_secs(double units) const       # units → 秒
    double get_units(double secs) const        # 秒 → units
    double get_beats(double secs) const        # 秒 → 拍数

    double beats_to_units(double beats) const  # 拍数 → units
    double units_to_beats(double units) const  # units → 拍数

    void update_secs(int from)                 # 从指定下标起重算时间

SpeedEvent
----------

单个速度/流速变化事件，定义一段线性变速区间。

.. code-block:: gdscript

    static SpeedEvent create(int units_start, int units_end,
                             double speed_start, double speed_end)

    int units_start        # 区间起始 units
    int units_end          # 区间结束 units
    double speed_start     # 起始流速
    double speed_end       # 结束流速（线性变化）
    double pre_displacement  # 区间前累计位移（自动计算）

    double get_displacement(double delta_secs) const
    double get_displacement_at(double delta_secs, double secs) const

SpeedEventList
--------------

速度事件列表，支持基于 BPM 事件的位移计算。

**工厂方法**

.. code-block:: gdscript

    static SpeedEventList create(double init_speed,
                                  BpmEventList bpm_event_list = null,
                                  bool allow_negative_speed = false)

**方法**

.. code-block:: gdscript

    void add_event(SpeedEvent event)
    bool remove_event(SpeedEvent event)
    void remove_event_at(int index)
    int  get_event_index(double units) const
    SpeedEvent get_event(double units) const
    SpeedEvent get_event_at(int index) const
    int  size() const

    void add_speed(int units_start, int units_end,
                   double speed_start, double speed_end)
    double get_speed(double secs) const

    double get_displacement(double secs) const         # 秒 → 位移
    double get_secs_by_displacement(double disp) const # 位移 → 秒

    void update_displacements(int from)                # 从指定下标起重算

NoteKey
-------

单个音符，包含位置、时间、位移与自定义属性。

.. code-block:: gdscript

    static NoteKey create(int units)

    int units              # 音符所在 units 位置
    Dictionary properties   # 自定义属性字典（如 type, lane 等）
    double secs             # 音符时间（秒，自动计算）
    double displacement     # 音符位移（自动计算）

NoteTrack
---------

音符轨道，将 BPM 事件、速度事件和音符集合统一管理。

**工厂方法**

.. code-block:: gdscript

    static NoteTrack create(BpmEventList bpm_events,
                            SpeedEventList speed_events,
                            bool sort_note_by_displacement = false)

    BpmEventList bpm_events
    SpeedEventList speed_events
    bool sort_note_by_displacement

**音符管理**

.. code-block:: gdscript

    void add_note(NoteKey note)             # 添加音符（自动排序+重算）
    bool remove_note(NoteKey note)          # 删除指定音符
    void remove_note_at(int index)          # 按索引删除

    int  get_note_index(double units, bool first = false) const
    NoteKey get_note(double units) const    # 二分查找
    NoteKey get_note_at(int index) const    # 按索引获取
    int  size() const

    # 按位移查询（需 sort_note_by_displacement = true）
    int  get_note_index_by_displacement(double disp) const
    NoteKey get_note_by_displacement(double disp) const
    NoteKey get_note_by_displacement_at(int index) const

**事件管理**

.. code-block:: gdscript

    # BPM 事件
    void add_bpm_event(BpmEvent event)
    bool remove_bpm_event(BpmEvent event)
    void remove_bpm_event_at(int index)
    void add_bpm(int units, double bpm)

    # 速度事件
    void add_speed_event(SpeedEvent event)
    bool remove_speed_event(SpeedEvent event)
    void remove_speed_event_at(int index)
    void add_speed(int units_start, int units_end,
                   double speed_start, double speed_end)

**重算**

.. code-block:: gdscript

    void update_displacements_and_secs(int from = 0)
