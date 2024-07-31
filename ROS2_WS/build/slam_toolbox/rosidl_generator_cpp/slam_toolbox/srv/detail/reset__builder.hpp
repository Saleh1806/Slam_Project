// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from slam_toolbox:srv/Reset.idl
// generated code does not contain a copyright notice

#ifndef SLAM_TOOLBOX__SRV__DETAIL__RESET__BUILDER_HPP_
#define SLAM_TOOLBOX__SRV__DETAIL__RESET__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "slam_toolbox/srv/detail/reset__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace slam_toolbox
{

namespace srv
{

namespace builder
{

class Init_Reset_Request_pause_new_measurements
{
public:
  Init_Reset_Request_pause_new_measurements()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  ::slam_toolbox::srv::Reset_Request pause_new_measurements(::slam_toolbox::srv::Reset_Request::_pause_new_measurements_type arg)
  {
    msg_.pause_new_measurements = std::move(arg);
    return std::move(msg_);
  }

private:
  ::slam_toolbox::srv::Reset_Request msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::slam_toolbox::srv::Reset_Request>()
{
  return slam_toolbox::srv::builder::Init_Reset_Request_pause_new_measurements();
}

}  // namespace slam_toolbox


namespace slam_toolbox
{

namespace srv
{

namespace builder
{

class Init_Reset_Response_result
{
public:
  Init_Reset_Response_result()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  ::slam_toolbox::srv::Reset_Response result(::slam_toolbox::srv::Reset_Response::_result_type arg)
  {
    msg_.result = std::move(arg);
    return std::move(msg_);
  }

private:
  ::slam_toolbox::srv::Reset_Response msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::slam_toolbox::srv::Reset_Response>()
{
  return slam_toolbox::srv::builder::Init_Reset_Response_result();
}

}  // namespace slam_toolbox

#endif  // SLAM_TOOLBOX__SRV__DETAIL__RESET__BUILDER_HPP_
