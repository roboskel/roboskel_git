; Auto-generated. Do not edit!


(cl:in-package face_rec-msg)


;//! \htmlinclude faceDataArray.msg.html

(cl:defclass <faceDataArray> (roslisp-msg-protocol:ros-message)
  ((faces
    :reader faces
    :initarg :faces
    :type (cl:vector face_rec-msg:faceData)
   :initform (cl:make-array 0 :element-type 'face_rec-msg:faceData :initial-element (cl:make-instance 'face_rec-msg:faceData)))
   (total
    :reader total
    :initarg :total
    :type cl:integer
    :initform 0))
)

(cl:defclass faceDataArray (<faceDataArray>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <faceDataArray>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'faceDataArray)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name face_rec-msg:<faceDataArray> is deprecated: use face_rec-msg:faceDataArray instead.")))

(cl:ensure-generic-function 'faces-val :lambda-list '(m))
(cl:defmethod faces-val ((m <faceDataArray>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader face_rec-msg:faces-val is deprecated.  Use face_rec-msg:faces instead.")
  (faces m))

(cl:ensure-generic-function 'total-val :lambda-list '(m))
(cl:defmethod total-val ((m <faceDataArray>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader face_rec-msg:total-val is deprecated.  Use face_rec-msg:total instead.")
  (total m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <faceDataArray>) ostream)
  "Serializes a message object of type '<faceDataArray>"
  (cl:let ((__ros_arr_len (cl:length (cl:slot-value msg 'faces))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __ros_arr_len) ostream))
  (cl:map cl:nil #'(cl:lambda (ele) (roslisp-msg-protocol:serialize ele ostream))
   (cl:slot-value msg 'faces))
  (cl:let* ((signed (cl:slot-value msg 'total)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 4294967296) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) unsigned) ostream)
    )
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <faceDataArray>) istream)
  "Deserializes a message object of type '<faceDataArray>"
  (cl:let ((__ros_arr_len 0))
    (cl:setf (cl:ldb (cl:byte 8 0) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 16) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 24) __ros_arr_len) (cl:read-byte istream))
  (cl:setf (cl:slot-value msg 'faces) (cl:make-array __ros_arr_len))
  (cl:let ((vals (cl:slot-value msg 'faces)))
    (cl:dotimes (i __ros_arr_len)
    (cl:setf (cl:aref vals i) (cl:make-instance 'face_rec-msg:faceData))
  (roslisp-msg-protocol:deserialize (cl:aref vals i) istream))))
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'total) (cl:if (cl:< unsigned 2147483648) unsigned (cl:- unsigned 4294967296))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<faceDataArray>)))
  "Returns string type for a message object of type '<faceDataArray>"
  "face_rec/faceDataArray")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'faceDataArray)))
  "Returns string type for a message object of type 'faceDataArray"
  "face_rec/faceDataArray")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<faceDataArray>)))
  "Returns md5sum for a message object of type '<faceDataArray>"
  "ea2fd606988b2ff49ed26c5c6c737612")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'faceDataArray)))
  "Returns md5sum for a message object of type 'faceDataArray"
  "ea2fd606988b2ff49ed26c5c6c737612")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<faceDataArray>)))
  "Returns full string definition for message of type '<faceDataArray>"
  (cl:format cl:nil "  faceData[] faces~%  int32 total~%~%================================================================================~%MSG: face_rec/faceData~%  int32 id~%  int32 x~%  int32 y~%  float32 radius~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'faceDataArray)))
  "Returns full string definition for message of type 'faceDataArray"
  (cl:format cl:nil "  faceData[] faces~%  int32 total~%~%================================================================================~%MSG: face_rec/faceData~%  int32 id~%  int32 x~%  int32 y~%  float32 radius~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <faceDataArray>))
  (cl:+ 0
     4 (cl:reduce #'cl:+ (cl:slot-value msg 'faces) :key #'(cl:lambda (ele) (cl:declare (cl:ignorable ele)) (cl:+ (roslisp-msg-protocol:serialization-length ele))))
     4
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <faceDataArray>))
  "Converts a ROS message object to a list"
  (cl:list 'faceDataArray
    (cl:cons ':faces (faces msg))
    (cl:cons ':total (total msg))
))
