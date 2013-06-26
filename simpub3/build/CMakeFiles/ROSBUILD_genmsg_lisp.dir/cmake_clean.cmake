FILE(REMOVE_RECURSE
  "../msg_gen"
  "../src/simpub3/msg"
  "../msg_gen"
  "CMakeFiles/ROSBUILD_genmsg_lisp"
  "../msg_gen/lisp/mast_position.lisp"
  "../msg_gen/lisp/_package.lisp"
  "../msg_gen/lisp/_package_mast_position.lisp"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang)
  INCLUDE(CMakeFiles/ROSBUILD_genmsg_lisp.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
