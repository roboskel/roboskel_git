
(cl:in-package :asdf)

(defsystem "sdc2130_skel-msg"
  :depends-on (:roslisp-msg-protocol :roslisp-utils :std_msgs-msg
)
  :components ((:file "_package")
    (:file "teleop_skel" :depends-on ("_package_teleop_skel"))
    (:file "_package_teleop_skel" :depends-on ("_package"))
  ))