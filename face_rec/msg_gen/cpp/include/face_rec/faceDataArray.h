/* Auto-generated by genmsg_cpp for file /home/skel/roboskel_workspace/sandbox/face_rec/msg/faceDataArray.msg */
#ifndef FACE_REC_MESSAGE_FACEDATAARRAY_H
#define FACE_REC_MESSAGE_FACEDATAARRAY_H
#include <string>
#include <vector>
#include <map>
#include <ostream>
#include "ros/serialization.h"
#include "ros/builtin_message_traits.h"
#include "ros/message_operations.h"
#include "ros/time.h"

#include "ros/macros.h"

#include "ros/assert.h"

#include "face_rec/faceData.h"

namespace face_rec
{
template <class ContainerAllocator>
struct faceDataArray_ {
  typedef faceDataArray_<ContainerAllocator> Type;

  faceDataArray_()
  : faces()
  , total(0)
  {
  }

  faceDataArray_(const ContainerAllocator& _alloc)
  : faces(_alloc)
  , total(0)
  {
  }

  typedef std::vector< ::face_rec::faceData_<ContainerAllocator> , typename ContainerAllocator::template rebind< ::face_rec::faceData_<ContainerAllocator> >::other >  _faces_type;
  std::vector< ::face_rec::faceData_<ContainerAllocator> , typename ContainerAllocator::template rebind< ::face_rec::faceData_<ContainerAllocator> >::other >  faces;

  typedef int32_t _total_type;
  int32_t total;


  typedef boost::shared_ptr< ::face_rec::faceDataArray_<ContainerAllocator> > Ptr;
  typedef boost::shared_ptr< ::face_rec::faceDataArray_<ContainerAllocator>  const> ConstPtr;
  boost::shared_ptr<std::map<std::string, std::string> > __connection_header;
}; // struct faceDataArray
typedef  ::face_rec::faceDataArray_<std::allocator<void> > faceDataArray;

typedef boost::shared_ptr< ::face_rec::faceDataArray> faceDataArrayPtr;
typedef boost::shared_ptr< ::face_rec::faceDataArray const> faceDataArrayConstPtr;


template<typename ContainerAllocator>
std::ostream& operator<<(std::ostream& s, const  ::face_rec::faceDataArray_<ContainerAllocator> & v)
{
  ros::message_operations::Printer< ::face_rec::faceDataArray_<ContainerAllocator> >::stream(s, "", v);
  return s;}

} // namespace face_rec

namespace ros
{
namespace message_traits
{
template<class ContainerAllocator> struct IsMessage< ::face_rec::faceDataArray_<ContainerAllocator> > : public TrueType {};
template<class ContainerAllocator> struct IsMessage< ::face_rec::faceDataArray_<ContainerAllocator>  const> : public TrueType {};
template<class ContainerAllocator>
struct MD5Sum< ::face_rec::faceDataArray_<ContainerAllocator> > {
  static const char* value() 
  {
    return "ea2fd606988b2ff49ed26c5c6c737612";
  }

  static const char* value(const  ::face_rec::faceDataArray_<ContainerAllocator> &) { return value(); } 
  static const uint64_t static_value1 = 0xea2fd606988b2ff4ULL;
  static const uint64_t static_value2 = 0x9ed26c5c6c737612ULL;
};

template<class ContainerAllocator>
struct DataType< ::face_rec::faceDataArray_<ContainerAllocator> > {
  static const char* value() 
  {
    return "face_rec/faceDataArray";
  }

  static const char* value(const  ::face_rec::faceDataArray_<ContainerAllocator> &) { return value(); } 
};

template<class ContainerAllocator>
struct Definition< ::face_rec::faceDataArray_<ContainerAllocator> > {
  static const char* value() 
  {
    return "  faceData[] faces\n\
  int32 total\n\
\n\
================================================================================\n\
MSG: face_rec/faceData\n\
  int32 id\n\
  int32 x\n\
  int32 y\n\
  float32 radius\n\
\n\
";
  }

  static const char* value(const  ::face_rec::faceDataArray_<ContainerAllocator> &) { return value(); } 
};

} // namespace message_traits
} // namespace ros

namespace ros
{
namespace serialization
{

template<class ContainerAllocator> struct Serializer< ::face_rec::faceDataArray_<ContainerAllocator> >
{
  template<typename Stream, typename T> inline static void allInOne(Stream& stream, T m)
  {
    stream.next(m.faces);
    stream.next(m.total);
  }

  ROS_DECLARE_ALLINONE_SERIALIZER;
}; // struct faceDataArray_
} // namespace serialization
} // namespace ros

namespace ros
{
namespace message_operations
{

template<class ContainerAllocator>
struct Printer< ::face_rec::faceDataArray_<ContainerAllocator> >
{
  template<typename Stream> static void stream(Stream& s, const std::string& indent, const  ::face_rec::faceDataArray_<ContainerAllocator> & v) 
  {
    s << indent << "faces[]" << std::endl;
    for (size_t i = 0; i < v.faces.size(); ++i)
    {
      s << indent << "  faces[" << i << "]: ";
      s << std::endl;
      s << indent;
      Printer< ::face_rec::faceData_<ContainerAllocator> >::stream(s, indent + "    ", v.faces[i]);
    }
    s << indent << "total: ";
    Printer<int32_t>::stream(s, indent + "  ", v.total);
  }
};


} // namespace message_operations
} // namespace ros

#endif // FACE_REC_MESSAGE_FACEDATAARRAY_H
