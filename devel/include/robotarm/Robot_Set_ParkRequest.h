// Generated by gencpp from file robotarm/Robot_Set_ParkRequest.msg
// DO NOT EDIT!


#ifndef ROBOTARM_MESSAGE_ROBOT_SET_PARKREQUEST_H
#define ROBOTARM_MESSAGE_ROBOT_SET_PARKREQUEST_H


#include <string>
#include <vector>
#include <map>

#include <ros/types.h>
#include <ros/serialization.h>
#include <ros/builtin_message_traits.h>
#include <ros/message_operations.h>


namespace robotarm
{
template <class ContainerAllocator>
struct Robot_Set_ParkRequest_
{
  typedef Robot_Set_ParkRequest_<ContainerAllocator> Type;

  Robot_Set_ParkRequest_()
    : inTime(0)  {
    }
  Robot_Set_ParkRequest_(const ContainerAllocator& _alloc)
    : inTime(0)  {
  (void)_alloc;
    }



   typedef int64_t _inTime_type;
  _inTime_type inTime;




  typedef boost::shared_ptr< ::robotarm::Robot_Set_ParkRequest_<ContainerAllocator> > Ptr;
  typedef boost::shared_ptr< ::robotarm::Robot_Set_ParkRequest_<ContainerAllocator> const> ConstPtr;

}; // struct Robot_Set_ParkRequest_

typedef ::robotarm::Robot_Set_ParkRequest_<std::allocator<void> > Robot_Set_ParkRequest;

typedef boost::shared_ptr< ::robotarm::Robot_Set_ParkRequest > Robot_Set_ParkRequestPtr;
typedef boost::shared_ptr< ::robotarm::Robot_Set_ParkRequest const> Robot_Set_ParkRequestConstPtr;

// constants requiring out of line definition



template<typename ContainerAllocator>
std::ostream& operator<<(std::ostream& s, const ::robotarm::Robot_Set_ParkRequest_<ContainerAllocator> & v)
{
ros::message_operations::Printer< ::robotarm::Robot_Set_ParkRequest_<ContainerAllocator> >::stream(s, "", v);
return s;
}

} // namespace robotarm

namespace ros
{
namespace message_traits
{



// BOOLTRAITS {'IsFixedSize': True, 'IsMessage': True, 'HasHeader': False}
// {'std_msgs': ['/opt/ros/kinetic/share/std_msgs/cmake/../msg']}

// !!!!!!!!!!! ['__class__', '__delattr__', '__dict__', '__doc__', '__eq__', '__format__', '__getattribute__', '__hash__', '__init__', '__module__', '__ne__', '__new__', '__reduce__', '__reduce_ex__', '__repr__', '__setattr__', '__sizeof__', '__str__', '__subclasshook__', '__weakref__', '_parsed_fields', 'constants', 'fields', 'full_name', 'has_header', 'header_present', 'names', 'package', 'parsed_fields', 'short_name', 'text', 'types']




template <class ContainerAllocator>
struct IsFixedSize< ::robotarm::Robot_Set_ParkRequest_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct IsFixedSize< ::robotarm::Robot_Set_ParkRequest_<ContainerAllocator> const>
  : TrueType
  { };

template <class ContainerAllocator>
struct IsMessage< ::robotarm::Robot_Set_ParkRequest_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct IsMessage< ::robotarm::Robot_Set_ParkRequest_<ContainerAllocator> const>
  : TrueType
  { };

template <class ContainerAllocator>
struct HasHeader< ::robotarm::Robot_Set_ParkRequest_<ContainerAllocator> >
  : FalseType
  { };

template <class ContainerAllocator>
struct HasHeader< ::robotarm::Robot_Set_ParkRequest_<ContainerAllocator> const>
  : FalseType
  { };


template<class ContainerAllocator>
struct MD5Sum< ::robotarm::Robot_Set_ParkRequest_<ContainerAllocator> >
{
  static const char* value()
  {
    return "15c5ba1c738eafebff9be7bf85aaac80";
  }

  static const char* value(const ::robotarm::Robot_Set_ParkRequest_<ContainerAllocator>&) { return value(); }
  static const uint64_t static_value1 = 0x15c5ba1c738eafebULL;
  static const uint64_t static_value2 = 0xff9be7bf85aaac80ULL;
};

template<class ContainerAllocator>
struct DataType< ::robotarm::Robot_Set_ParkRequest_<ContainerAllocator> >
{
  static const char* value()
  {
    return "robotarm/Robot_Set_ParkRequest";
  }

  static const char* value(const ::robotarm::Robot_Set_ParkRequest_<ContainerAllocator>&) { return value(); }
};

template<class ContainerAllocator>
struct Definition< ::robotarm::Robot_Set_ParkRequest_<ContainerAllocator> >
{
  static const char* value()
  {
    return "int64 inTime\n\
";
  }

  static const char* value(const ::robotarm::Robot_Set_ParkRequest_<ContainerAllocator>&) { return value(); }
};

} // namespace message_traits
} // namespace ros

namespace ros
{
namespace serialization
{

  template<class ContainerAllocator> struct Serializer< ::robotarm::Robot_Set_ParkRequest_<ContainerAllocator> >
  {
    template<typename Stream, typename T> inline static void allInOne(Stream& stream, T m)
    {
      stream.next(m.inTime);
    }

    ROS_DECLARE_ALLINONE_SERIALIZER
  }; // struct Robot_Set_ParkRequest_

} // namespace serialization
} // namespace ros

namespace ros
{
namespace message_operations
{

template<class ContainerAllocator>
struct Printer< ::robotarm::Robot_Set_ParkRequest_<ContainerAllocator> >
{
  template<typename Stream> static void stream(Stream& s, const std::string& indent, const ::robotarm::Robot_Set_ParkRequest_<ContainerAllocator>& v)
  {
    s << indent << "inTime: ";
    Printer<int64_t>::stream(s, indent + "  ", v.inTime);
  }
};

} // namespace message_operations
} // namespace ros

#endif // ROBOTARM_MESSAGE_ROBOT_SET_PARKREQUEST_H
