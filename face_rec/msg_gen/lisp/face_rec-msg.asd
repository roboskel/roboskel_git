
(cl:in-package :asdf)

(defsystem "face_rec-msg"
  :depends-on (:roslisp-msg-protocol :roslisp-utils )
  :components ((:file "_package")
    (:file "faceDataArray" :depends-on ("_package_faceDataArray"))
    (:file "_package_faceDataArray" :depends-on ("_package"))
    (:file "faceData" :depends-on ("_package_faceData"))
    (:file "_package_faceData" :depends-on ("_package"))
  ))