从源码构建
==========

环境要求
--------

- Python 3.x
- SCons
- Visual Studio（Windows 下用于 MSVC 编译）
- Godot 4.3+ 的 godot-cpp 绑定（以子模块形式包含在仓库中）

Windows 构建
------------

.. code-block:: bash

    # 1. 初始化子模块
    git submodule update --init

    # 2. 构建编辑器版本
    scons target=editor

    # 3. 构建调试模板（可选）
    scons target=template_debug

构建产物会输出到 ``addons/index-note/bin/`` 目录下。

Android 构建
------------

构建脚本
~~~~~~~~

仓库提供了便捷构建脚本：``build_android.bat``（Windows）和 ``build_android.sh``（Linux/macOS）。

前置要求
~~~~~~~~

- Android NDK 23.2.8568313，安装到 ``%ANDROID_HOME%\ndk\23.2.8568313\``
- 设置 ``ANDROID_HOME`` 环境变量（脚本中默认为 ``E:\AndroidSDK``）
- Python 与 SCons

构建命令
~~~~~~~~

.. code-block:: bash

    # 手动使用 scons（构建 arm64，调试模板）
    scons platform=android target=template_debug arch=arm64 ANDROID_HOME=%ANDROID_HOME%

    # 或使用构建脚本
    build_android.bat all       # Windows：构建调试 + 发布版，覆盖 arm64、arm32、x86_64
    ./build_android.sh all      # Linux/macOS：同上

产物 ``.so`` 文件输出到 ``addons/index-note/bin/``。

Android 项目配置
~~~~~~~~~~~~~~~~

导出前，请确保 Godot 编辑器中已满足以下前置条件：

- 通过 **编辑器 > 管理导出模板...** 安装 **Android 导出模板**。
- 已配置 **Android SDK**（可通过 Android Studio 安装，或设置
  ``ANDROID_HOME`` 环境变量）。

然后配置项目：

1. 将 ``addons/index-note`` 目录复制到你的项目中（与桌面平台相同）。
2. ``.gdextension`` 文件已包含 ``arm64``、``arm32``、``x86_64``
   架构的库路径，无需额外配置。
3. 安装 Android 构建模板：
   **项目 > 安装 Android 构建模板...**
4. 创建 **Android 导出预设**，在 **Gradle Build** 中将
   **Use Gradle Build** 设为 ``true``。
5. 在 **项目 > 项目设置 > 插件** 中启用 **Index Note**。
