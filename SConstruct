#!/usr/bin/env python

env = SConscript("godot-cpp/SConstruct")

env.Append(CPPPATH=["src/", "src/notes/", "src/events/"])
sources = Glob("src/*.cpp") + Glob("src/notes/*.cpp") + Glob("src/events/*.cpp")

library = env.SharedLibrary(
    "addons/bin/godotindexnote{}{}".format(env["suffix"], env["SHLIBSUFFIX"]),
    source=sources,
)

Default(library)
