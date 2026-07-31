安装
====

安装步骤
--------

1. 将 ``addons/index-note`` 目录复制到你的项目 ``addons/`` 下（或直接解压
   Releases 中的 ``addons.zip``）。
2. 在 **项目 > 项目设置 > 插件** 中启用 **Index Note**。
3. 插件为 GDExtension，启用后即可在 GDScript 中直接使用 ``NoteTrack``、
   ``NoteKey``、``BpmEventList``、``SpeedEventList`` 等类。

依赖要求
--------

- **Godot Engine** 4.3 或更高版本
- **Windows** / **Android** 平台

Android 导出配置
----------------

导出 Android 项目需要以下前置准备：

- **Android 导出模板**：通过 **编辑器 > 管理导出模板...** 下载安装。
- **Android SDK**：通过 Android Studio 安装。

然后配置 Godot 的 Gradle 构建系统：

1. 安装 Android 构建模板：**项目 > 安装 Android 构建模板...**
2. 创建 **Android 导出预设**，在 **Gradle Build** 中将
   **Use Gradle Build** 设为 ``true``。
3. ``.gdextension`` 文件已包含 ``arm64``、``arm32``、``x86_64``
   架构的库路径，导出时会自动包含对应架构的 ``.so`` 文件。
