FILE(REMOVE_RECURSE
  "../msg_gen"
  "../src/sdc2130_skel/msg"
  "../msg_gen"
  "CMakeFiles/ROSBUILD_genmsg_py"
  "../src/sdc2130_skel/msg/__init__.py"
  "../src/sdc2130_skel/msg/_teleop_skel.py"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang)
  INCLUDE(CMakeFiles/ROSBUILD_genmsg_py.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
