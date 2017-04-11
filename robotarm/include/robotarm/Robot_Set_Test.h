// Generated by gencpp from file robotarm/Robot_Set_Test.msg
// DO NOT EDIT!


#ifndef ROBOTARM_MESSAGE_ROBOT_SET_TEST_H
#define ROBOTARM_MESSAGE_ROBOT_SET_TEST_H

#include <ros/service_traits.h>


#include <robotarm/Robot_Set_TestRequest.h>
#include <robotarm/Robot_Set_TestResponse.h>


namespace robotarm
{

struct Robot_Set_Test
{

typedef Robot_Set_TestRequest Request;
typedef Robot_Set_TestResponse Response;
Request request;
Response response;

typedef Request RequestType;
typedef Response ResponseType;

}; // struct Robot_Set_Test
} // namespace robotarm


namespace ros
{
namespace service_traits
{


template<>
struct MD5Sum< ::robotarm::Robot_Set_Test > {
  static const char* value()
  {
    return "019706110004b728d56d8baaa8e32797";
  }

  static const char* value(const ::robotarm::Robot_Set_Test&) { return value(); }
};

template<>
struct DataType< ::robotarm::Robot_Set_Test > {
  static const char* value()
  {
    return "robotarm/Robot_Set_Test";
  }

  static const char* value(const ::robotarm::Robot_Set_Test&) { return value(); }
};


// service_traits::MD5Sum< ::robotarm::Robot_Set_TestRequest> should match 
// service_traits::MD5Sum< ::robotarm::Robot_Set_Test > 
template<>
struct MD5Sum< ::robotarm::Robot_Set_TestRequest>
{
  static const char* value()
  {
    return MD5Sum< ::robotarm::Robot_Set_Test >::value();
  }
  static const char* value(const ::robotarm::Robot_Set_TestRequest&)
  {
    return value();
  }
};

// service_traits::DataType< ::robotarm::Robot_Set_TestRequest> should match 
// service_traits::DataType< ::robotarm::Robot_Set_Test > 
template<>
struct DataType< ::robotarm::Robot_Set_TestRequest>
{
  static const char* value()
  {
    return DataType< ::robotarm::Robot_Set_Test >::value();
  }
  static const char* value(const ::robotarm::Robot_Set_TestRequest&)
  {
    return value();
  }
};

// service_traits::MD5Sum< ::robotarm::Robot_Set_TestResponse> should match 
// service_traits::MD5Sum< ::robotarm::Robot_Set_Test > 
template<>
struct MD5Sum< ::robotarm::Robot_Set_TestResponse>
{
  static const char* value()
  {
    return MD5Sum< ::robotarm::Robot_Set_Test >::value();
  }
  static const char* value(const ::robotarm::Robot_Set_TestResponse&)
  {
    return value();
  }
};

// service_traits::DataType< ::robotarm::Robot_Set_TestResponse> should match 
// service_traits::DataType< ::robotarm::Robot_Set_Test > 
template<>
struct DataType< ::robotarm::Robot_Set_TestResponse>
{
  static const char* value()
  {
    return DataType< ::robotarm::Robot_Set_Test >::value();
  }
  static const char* value(const ::robotarm::Robot_Set_TestResponse&)
  {
    return value();
  }
};

} // namespace service_traits
} // namespace ros

#endif // ROBOTARM_MESSAGE_ROBOT_SET_TEST_H
