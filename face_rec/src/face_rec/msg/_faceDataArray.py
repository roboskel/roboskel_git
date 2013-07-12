"""autogenerated by genpy from face_rec/faceDataArray.msg. Do not edit."""
import sys
python3 = True if sys.hexversion > 0x03000000 else False
import genpy
import struct

import face_rec.msg

class faceDataArray(genpy.Message):
  _md5sum = "ea2fd606988b2ff49ed26c5c6c737612"
  _type = "face_rec/faceDataArray"
  _has_header = False #flag to mark the presence of a Header object
  _full_text = """  faceData[] faces
  int32 total

================================================================================
MSG: face_rec/faceData
  int32 id
  int32 x
  int32 y
  float32 radius

"""
  __slots__ = ['faces','total']
  _slot_types = ['face_rec/faceData[]','int32']

  def __init__(self, *args, **kwds):
    """
    Constructor. Any message fields that are implicitly/explicitly
    set to None will be assigned a default value. The recommend
    use is keyword arguments as this is more robust to future message
    changes.  You cannot mix in-order arguments and keyword arguments.

    The available fields are:
       faces,total

    :param args: complete set of field values, in .msg order
    :param kwds: use keyword arguments corresponding to message field names
    to set specific fields.
    """
    if args or kwds:
      super(faceDataArray, self).__init__(*args, **kwds)
      #message fields cannot be None, assign default values for those that are
      if self.faces is None:
        self.faces = []
      if self.total is None:
        self.total = 0
    else:
      self.faces = []
      self.total = 0

  def _get_types(self):
    """
    internal API method
    """
    return self._slot_types

  def serialize(self, buff):
    """
    serialize message into buffer
    :param buff: buffer, ``StringIO``
    """
    try:
      length = len(self.faces)
      buff.write(_struct_I.pack(length))
      for val1 in self.faces:
        _x = val1
        buff.write(_struct_3if.pack(_x.id, _x.x, _x.y, _x.radius))
      buff.write(_struct_i.pack(self.total))
    except struct.error as se: self._check_types(se)
    except TypeError as te: self._check_types(te)

  def deserialize(self, str):
    """
    unpack serialized message in str into this message instance
    :param str: byte array of serialized message, ``str``
    """
    try:
      if self.faces is None:
        self.faces = None
      end = 0
      start = end
      end += 4
      (length,) = _struct_I.unpack(str[start:end])
      self.faces = []
      for i in range(0, length):
        val1 = face_rec.msg.faceData()
        _x = val1
        start = end
        end += 16
        (_x.id, _x.x, _x.y, _x.radius,) = _struct_3if.unpack(str[start:end])
        self.faces.append(val1)
      start = end
      end += 4
      (self.total,) = _struct_i.unpack(str[start:end])
      return self
    except struct.error as e:
      raise genpy.DeserializationError(e) #most likely buffer underfill


  def serialize_numpy(self, buff, numpy):
    """
    serialize message with numpy array types into buffer
    :param buff: buffer, ``StringIO``
    :param numpy: numpy python module
    """
    try:
      length = len(self.faces)
      buff.write(_struct_I.pack(length))
      for val1 in self.faces:
        _x = val1
        buff.write(_struct_3if.pack(_x.id, _x.x, _x.y, _x.radius))
      buff.write(_struct_i.pack(self.total))
    except struct.error as se: self._check_types(se)
    except TypeError as te: self._check_types(te)

  def deserialize_numpy(self, str, numpy):
    """
    unpack serialized message in str into this message instance using numpy for array types
    :param str: byte array of serialized message, ``str``
    :param numpy: numpy python module
    """
    try:
      if self.faces is None:
        self.faces = None
      end = 0
      start = end
      end += 4
      (length,) = _struct_I.unpack(str[start:end])
      self.faces = []
      for i in range(0, length):
        val1 = face_rec.msg.faceData()
        _x = val1
        start = end
        end += 16
        (_x.id, _x.x, _x.y, _x.radius,) = _struct_3if.unpack(str[start:end])
        self.faces.append(val1)
      start = end
      end += 4
      (self.total,) = _struct_i.unpack(str[start:end])
      return self
    except struct.error as e:
      raise genpy.DeserializationError(e) #most likely buffer underfill

_struct_I = genpy.struct_I
_struct_i = struct.Struct("<i")
_struct_3if = struct.Struct("<3if")