#!/usr/bin/env python

env = SConscript("godot-cpp/SConstruct")

env.Append(CPPPATH=["src/", "src/notes/", "src/events/"])
sources = Glob("src/*.cpp") + Glob("src/notes/*.cpp") + Glob("src/events/*.cpp")

lib_prefix = "lib" if env["platform"] == "android" else ""
library = env.SharedLibrary(
    "addons/index-note/bin/{}godotindexnote{}{}".format(lib_prefix, env["suffix"], env["SHLIBSUFFIX"]),
    source=sources,
)

Default(library)
