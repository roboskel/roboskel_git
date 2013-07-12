FILE(REMOVE_RECURSE
  "../msg_gen"
  "../src/face_rec/msg"
  "../msg_gen"
  "CMakeFiles/ROSBUILD_genmsg_py"
  "../src/face_rec/msg/__init__.py"
  "../src/face_rec/msg/_faceDataArray.py"
  "../src/face_rec/msg/_faceData.py"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang)
  INCLUDE(CMakeFiles/ROSBUILD_genmsg_py.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
