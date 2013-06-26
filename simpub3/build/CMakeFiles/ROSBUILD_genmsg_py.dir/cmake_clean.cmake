FILE(REMOVE_RECURSE
  "../msg_gen"
  "../src/simpub3/msg"
  "../msg_gen"
  "CMakeFiles/ROSBUILD_genmsg_py"
  "../src/simpub3/msg/__init__.py"
  "../src/simpub3/msg/_mast_position.py"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang)
  INCLUDE(CMakeFiles/ROSBUILD_genmsg_py.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
