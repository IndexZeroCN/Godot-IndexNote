从源码构建
==========

环境要求
--------

- Python 3.x
- SCons
- Visual Studio（Windows 下用于 MSVC 编译）
- Godot 4.3+ 的 godot-cpp 绑定（以子模块形式包含在仓库中）

构建步骤
--------

.. code-block:: bash

    # 1. 初始化子模块
    git submodule update --init

    # 2. 构建编辑器版本
    scons target=editor

    # 3. 构建调试模板（可选）
    scons target=template_debug

构建产物会输出到 ``addons/index-note/bin/`` 目录下。
