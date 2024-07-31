// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from slam_toolbox:srv/Reset.idl
// generated code does not contain a copyright notice

#ifndef SLAM_TOOLBOX__SRV__DETAIL__RESET__STRUCT_HPP_
#define SLAM_TOOLBOX__SRV__DETAIL__RESET__STRUCT_HPP_

#include <algorithm>
#include <array>
#include <memory>
#include <string>
#include <vector>

#include "rosidl_runtime_cpp/bounded_vector.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


#ifndef _WIN32
# define DEPRECATED__slam_toolbox__srv__Reset_Request __attribute__((deprecated))
#else
# define DEPRECATED__slam_toolbox__srv__Reset_Request __declspec(deprecated)
#endif

namespace slam_toolbox
{

namespace srv
{

// message struct
template<class ContainerAllocator>
struct Reset_Request_
{
  using Type = Reset_Request_<ContainerAllocator>;

  explicit Reset_Request_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::DEFAULTS_ONLY == _init)
    {
      this->pause_new_measurements = false;
    } else if (rosidl_runtime_cpp::MessageInitialization::ZERO == _init) {
      this->pause_new_measurements = false;
    }
  }

  explicit Reset_Request_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    (void)_alloc;
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::DEFAULTS_ONLY == _init)
    {
      this->pause_new_measurements = false;
    } else if (rosidl_runtime_cpp::MessageInitialization::ZERO == _init) {
      this->pause_new_measurements = false;
    }
  }

  // field types and members
  using _pause_new_measurements_type =
    bool;
  _pause_new_measurements_type pause_new_measurements;

  // setters for named parameter idiom
  Type & set__pause_new_measurements(
    const bool & _arg)
  {
    this->pause_new_measurements = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    slam_toolbox::srv::Reset_Request_<ContainerAllocator> *;
  using ConstRawPtr =
    const slam_toolbox::srv::Reset_Request_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<slam_toolbox::srv::Reset_Request_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<slam_toolbox::srv::Reset_Request_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      slam_toolbox::srv::Reset_Request_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<slam_toolbox::srv::Reset_Request_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      slam_toolbox::srv::Reset_Request_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<slam_toolbox::srv::Reset_Request_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<slam_toolbox::srv::Reset_Request_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<slam_toolbox::srv::Reset_Request_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__slam_toolbox__srv__Reset_Request
    std::shared_ptr<slam_toolbox::srv::Reset_Request_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__slam_toolbox__srv__Reset_Request
    std::shared_ptr<slam_toolbox::srv::Reset_Request_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const Reset_Request_ & other) const
  {
    if (this->pause_new_measurements != other.pause_new_measurements) {
      return false;
    }
    return true;
  }
  bool operator!=(const Reset_Request_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct Reset_Request_

// alias to use template instance with default allocator
using Reset_Request =
  slam_toolbox::srv::Reset_Request_<std::allocator<void>>;

// constant definitions

}  // namespace srv

}  // namespace slam_toolbox


#ifndef _WIN32
# define DEPRECATED__slam_toolbox__srv__Reset_Response __attribute__((deprecated))
#else
# define DEPRECATED__slam_toolbox__srv__Reset_Response __declspec(deprecated)
#endif

namespace slam_toolbox
{

namespace srv
{

// message struct
template<class ContainerAllocator>
struct Reset_Response_
{
  using Type = Reset_Response_<ContainerAllocator>;

  explicit Reset_Response_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->result = 0;
    }
  }

  explicit Reset_Response_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    (void)_alloc;
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->result = 0;
    }
  }

  // field types and members
  using _result_type =
    uint8_t;
  _result_type result;

  // setters for named parameter idiom
  Type & set__result(
    const uint8_t & _arg)
  {
    this->result = _arg;
    return *this;
  }

  // constant declarations
  static constexpr uint8_t RESULT_SUCCESS =
    0u;

  // pointer types
  using RawPtr =
    slam_toolbox::srv::Reset_Response_<ContainerAllocator> *;
  using ConstRawPtr =
    const slam_toolbox::srv::Reset_Response_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<slam_toolbox::srv::Reset_Response_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<slam_toolbox::srv::Reset_Response_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      slam_toolbox::srv::Reset_Response_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<slam_toolbox::srv::Reset_Response_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      slam_toolbox::srv::Reset_Response_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<slam_toolbox::srv::Reset_Response_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<slam_toolbox::srv::Reset_Response_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<slam_toolbox::srv::Reset_Response_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__slam_toolbox__srv__Reset_Response
    std::shared_ptr<slam_toolbox::srv::Reset_Response_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__slam_toolbox__srv__Reset_Response
    std::shared_ptr<slam_toolbox::srv::Reset_Response_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const Reset_Response_ & other) const
  {
    if (this->result != other.result) {
      return false;
    }
    return true;
  }
  bool operator!=(const Reset_Response_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct Reset_Response_

// alias to use template instance with default allocator
using Reset_Response =
  slam_toolbox::srv::Reset_Response_<std::allocator<void>>;

// constant definitions
#if __cplusplus < 201703L
// static constexpr member variable definitions are only needed in C++14 and below, deprecated in C++17
template<typename ContainerAllocator>
constexpr uint8_t Reset_Response_<ContainerAllocator>::RESULT_SUCCESS;
#endif  // __cplusplus < 201703L

}  // namespace srv

}  // namespace slam_toolbox

namespace slam_toolbox
{

namespace srv
{

struct Reset
{
  using Request = slam_toolbox::srv::Reset_Request;
  using Response = slam_toolbox::srv::Reset_Response;
};

}  // namespace srv

}  // namespace slam_toolbox

#endif  // SLAM_TOOLBOX__SRV__DETAIL__RESET__STRUCT_HPP_
