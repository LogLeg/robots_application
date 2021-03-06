// Generated by gencpp from file robotarm/Robot_Set_Straight.msg
// DO NOT EDIT!


#ifndef ROBOTARM_MESSAGE_ROBOT_SET_STRAIGHT_H
#define ROBOTARM_MESSAGE_ROBOT_SET_STRAIGHT_H

#include <ros/service_traits.h>


#include <robotarm/Robot_Set_StraightRequest.h>
#include <robotarm/Robot_Set_StraightResponse.h>


namespace robotarm
{

struct Robot_Set_Straight
{

typedef Robot_Set_StraightRequest Request;
typedef Robot_Set_StraightResponse Response;
Request request;
Response response;

typedef Request RequestType;
typedef Response ResponseType;

}; // struct Robot_Set_Straight
} // namespace robotarm


namespace ros
{
namespace service_traits
{


template<>
struct MD5Sum< ::robotarm::Robot_Set_Straight > {
  static const char* value()
  {
    return "15c5ba1c738eafebff9be7bf85aaac80";
  }

  static const char* value(const ::robotarm::Robot_Set_Straight&) { return value(); }
};

template<>
struct DataType< ::robotarm::Robot_Set_Straight > {
  static const char* value()
  {
    return "robotarm/Robot_Set_Straight";
  }

  static const char* value(const ::robotarm::Robot_Set_Straight&) { return value(); }
};


// service_traits::MD5Sum< ::robotarm::Robot_Set_StraightRequest> should match 
// service_traits::MD5Sum< ::robotarm::Robot_Set_Straight > 
template<>
struct MD5Sum< ::robotarm::Robot_Set_StraightRequest>
{
  static const char* value()
  {
    return MD5Sum< ::robotarm::Robot_Set_Straight >::value();
  }
  static const char* value(const ::robotarm::Robot_Set_StraightRequest&)
  {
    return value();
  }
};

// service_traits::DataType< ::robotarm::Robot_Set_StraightRequest> should match 
// service_traits::DataType< ::robotarm::Robot_Set_Straight > 
template<>
struct DataType< ::robotarm::Robot_Set_StraightRequest>
{
  static const char* value()
  {
    return DataType< ::robotarm::Robot_Set_Straight >::value();
  }
  static const char* value(const ::robotarm::Robot_Set_StraightRequest&)
  {
    return value();
  }
};

// service_traits::MD5Sum< ::robotarm::Robot_Set_StraightResponse> should match 
// service_traits::MD5Sum< ::robotarm::Robot_Set_Straight > 
template<>
struct MD5Sum< ::robotarm::Robot_Set_StraightResponse>
{
  static const char* value()
  {
    return MD5Sum< ::robotarm::Robot_Set_Straight >::value();
  }
  static const char* value(const ::robotarm::Robot_Set_StraightResponse&)
  {
    return value();
  }
};

// service_traits::DataType< ::robotarm::Robot_Set_StraightResponse> should match 
// service_traits::DataType< ::robotarm::Robot_Set_Straight > 
template<>
struct DataType< ::robotarm::Robot_Set_StraightResponse>
{
  static const char* value()
  {
    return DataType< ::robotarm::Robot_Set_Straight >::value();
  }
  static const char* value(const ::robotarm::Robot_Set_StraightResponse&)
  {
    return value();
  }
};

} // namespace service_traits
} // namespace ros

#endif // ROBOTARM_MESSAGE_ROBOT_SET_STRAIGHT_H
