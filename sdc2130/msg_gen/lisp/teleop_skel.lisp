; Auto-generated. Do not edit!


(cl:in-package sdc2130_skel-msg)


;//! \htmlinclude teleop_skel.msg.html

(cl:defclass <teleop_skel> (roslisp-msg-protocol:ros-message)
  ((header
    :reader header
    :initarg :header
    :type std_msgs-msg:Header
    :initform (cl:make-instance 'std_msgs-msg:Header))
   (left
    :reader left
    :initarg :left
    :type cl:fixnum
    :initform 0)
   (right
    :reader right
    :initarg :right
    :type cl:fixnum
    :initform 0))
)

(cl:defclass teleop_skel (<teleop_skel>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <teleop_skel>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'teleop_skel)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name sdc2130_skel-msg:<teleop_skel> is deprecated: use sdc2130_skel-msg:teleop_skel instead.")))

(cl:ensure-generic-function 'header-val :lambda-list '(m))
(cl:defmethod header-val ((m <teleop_skel>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader sdc2130_skel-msg:header-val is deprecated.  Use sdc2130_skel-msg:header instead.")
  (header m))

(cl:ensure-generic-function 'left-val :lambda-list '(m))
(cl:defmethod left-val ((m <teleop_skel>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader sdc2130_skel-msg:left-val is deprecated.  Use sdc2130_skel-msg:left instead.")
  (left m))

(cl:ensure-generic-function 'right-val :lambda-list '(m))
(cl:defmethod right-val ((m <teleop_skel>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader sdc2130_skel-msg:right-val is deprecated.  Use sdc2130_skel-msg:right instead.")
  (right m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <teleop_skel>) ostream)
  "Serializes a message object of type '<teleop_skel>"
  (roslisp-msg-protocol:serialize (cl:slot-value msg 'header) ostream)
  (cl:let* ((signed (cl:slot-value msg 'left)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 65536) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    )
  (cl:let* ((signed (cl:slot-value msg 'right)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 65536) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    )
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <teleop_skel>) istream)
  "Deserializes a message object of type '<teleop_skel>"
  (roslisp-msg-protocol:deserialize (cl:slot-value msg 'header) istream)
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'left) (cl:if (cl:< unsigned 32768) unsigned (cl:- unsigned 65536))))
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'right) (cl:if (cl:< unsigned 32768) unsigned (cl:- unsigned 65536))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<teleop_skel>)))
  "Returns string type for a message object of type '<teleop_skel>"
  "sdc2130_skel/teleop_skel")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'teleop_skel)))
  "Returns string type for a message object of type 'teleop_skel"
  "sdc2130_skel/teleop_skel")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<teleop_skel>)))
  "Returns md5sum for a message object of type '<teleop_skel>"
  "fc27340d1c328ae5b356b2f2a019aa82")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'teleop_skel)))
  "Returns md5sum for a message object of type 'teleop_skel"
  "fc27340d1c328ae5b356b2f2a019aa82")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<teleop_skel>)))
  "Returns full string definition for message of type '<teleop_skel>"
  (cl:format cl:nil "Header header~%int16 left~%int16 right~%~%================================================================================~%MSG: std_msgs/Header~%# Standard metadata for higher-level stamped data types.~%# This is generally used to communicate timestamped data ~%# in a particular coordinate frame.~%# ~%# sequence ID: consecutively increasing ID ~%uint32 seq~%#Two-integer timestamp that is expressed as:~%# * stamp.secs: seconds (stamp_secs) since epoch~%# * stamp.nsecs: nanoseconds since stamp_secs~%# time-handling sugar is provided by the client library~%time stamp~%#Frame this data is associated with~%# 0: no frame~%# 1: global frame~%string frame_id~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'teleop_skel)))
  "Returns full string definition for message of type 'teleop_skel"
  (cl:format cl:nil "Header header~%int16 left~%int16 right~%~%================================================================================~%MSG: std_msgs/Header~%# Standard metadata for higher-level stamped data types.~%# This is generally used to communicate timestamped data ~%# in a particular coordinate frame.~%# ~%# sequence ID: consecutively increasing ID ~%uint32 seq~%#Two-integer timestamp that is expressed as:~%# * stamp.secs: seconds (stamp_secs) since epoch~%# * stamp.nsecs: nanoseconds since stamp_secs~%# time-handling sugar is provided by the client library~%time stamp~%#Frame this data is associated with~%# 0: no frame~%# 1: global frame~%string frame_id~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <teleop_skel>))
  (cl:+ 0
     (roslisp-msg-protocol:serialization-length (cl:slot-value msg 'header))
     2
     2
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <teleop_skel>))
  "Converts a ROS message object to a list"
  (cl:list 'teleop_skel
    (cl:cons ':header (header msg))
    (cl:cons ':left (left msg))
    (cl:cons ':right (right msg))
))
