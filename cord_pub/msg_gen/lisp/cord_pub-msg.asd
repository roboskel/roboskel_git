
(cl:in-package :asdf)

(defsystem "cord_pub-msg"
  :depends-on (:roslisp-msg-protocol :roslisp-utils )
  :components ((:file "_package")
    (:file "Coords" :depends-on ("_package_Coords"))
    (:file "_package_Coords" :depends-on ("_package"))
  ))