// Generated by gencpp from file robotarm/Robot_GoTo.msg
// DO NOT EDIT!


#ifndef ROBOTARM_MESSAGE_ROBOT_GOTO_H
#define ROBOTARM_MESSAGE_ROBOT_GOTO_H

#include <ros/service_traits.h>


#include <robotarm/Robot_GoToRequest.h>
#include <robotarm/Robot_GoToResponse.h>


namespace robotarm
{

struct Robot_GoTo
{

typedef Robot_GoToRequest Request;
typedef Robot_GoToResponse Response;
Request request;
Response response;

typedef Request RequestType;
typedef Response ResponseType;

}; // struct Robot_GoTo
} // namespace robotarm


namespace ros
{
namespace service_traits
{


template<>
struct MD5Sum< ::robotarm::Robot_GoTo > {
  static const char* value()
  {
    return "9ece4e7a2a3b7948a071eddd60812052";
  }

  static const char* value(const ::robotarm::Robot_GoTo&) { return value(); }
};

template<>
struct DataType< ::robotarm::Robot_GoTo > {
  static const char* value()
  {
    return "robotarm/Robot_GoTo";
  }

  static const char* value(const ::robotarm::Robot_GoTo&) { return value(); }
};


// service_traits::MD5Sum< ::robotarm::Robot_GoToRequest> should match 
// service_traits::MD5Sum< ::robotarm::Robot_GoTo > 
template<>
struct MD5Sum< ::robotarm::Robot_GoToRequest>
{
  static const char* value()
  {
    return MD5Sum< ::robotarm::Robot_GoTo >::value();
  }
  static const char* value(const ::robotarm::Robot_GoToRequest&)
  {
    return value();
  }
};

// service_traits::DataType< ::robotarm::Robot_GoToRequest> should match 
// service_traits::DataType< ::robotarm::Robot_GoTo > 
template<>
struct DataType< ::robotarm::Robot_GoToRequest>
{
  static const char* value()
  {
    return DataType< ::robotarm::Robot_GoTo >::value();
  }
  static const char* value(const ::robotarm::Robot_GoToRequest&)
  {
    return value();
  }
};

// service_traits::MD5Sum< ::robotarm::Robot_GoToResponse> should match 
// service_traits::MD5Sum< ::robotarm::Robot_GoTo > 
template<>
struct MD5Sum< ::robotarm::Robot_GoToResponse>
{
  static const char* value()
  {
    return MD5Sum< ::robotarm::Robot_GoTo >::value();
  }
  static const char* value(const ::robotarm::Robot_GoToResponse&)
  {
    return value();
  }
};

// service_traits::DataType< ::robotarm::Robot_GoToResponse> should match 
// service_traits::DataType< ::robotarm::Robot_GoTo > 
template<>
struct DataType< ::robotarm::Robot_GoToResponse>
{
  static const char* value()
  {
    return DataType< ::robotarm::Robot_GoTo >::value();
  }
  static const char* value(const ::robotarm::Robot_GoToResponse&)
  {
    return value();
  }
};

} // namespace service_traits
} // namespace ros

#endif // ROBOTARM_MESSAGE_ROBOT_GOTO_H
