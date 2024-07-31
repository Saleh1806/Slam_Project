// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from slam_toolbox:srv/Reset.idl
// generated code does not contain a copyright notice

#ifndef SLAM_TOOLBOX__SRV__DETAIL__RESET__STRUCT_H_
#define SLAM_TOOLBOX__SRV__DETAIL__RESET__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

/// Struct defined in srv/Reset in the package slam_toolbox.
typedef struct slam_toolbox__srv__Reset_Request
{
  bool pause_new_measurements;
} slam_toolbox__srv__Reset_Request;

// Struct for a sequence of slam_toolbox__srv__Reset_Request.
typedef struct slam_toolbox__srv__Reset_Request__Sequence
{
  slam_toolbox__srv__Reset_Request * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} slam_toolbox__srv__Reset_Request__Sequence;


// Constants defined in the message

/// Constant 'RESULT_SUCCESS'.
enum
{
  slam_toolbox__srv__Reset_Response__RESULT_SUCCESS = 0
};

/// Struct defined in srv/Reset in the package slam_toolbox.
typedef struct slam_toolbox__srv__Reset_Response
{
  uint8_t result;
} slam_toolbox__srv__Reset_Response;

// Struct for a sequence of slam_toolbox__srv__Reset_Response.
typedef struct slam_toolbox__srv__Reset_Response__Sequence
{
  slam_toolbox__srv__Reset_Response * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} slam_toolbox__srv__Reset_Response__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // SLAM_TOOLBOX__SRV__DETAIL__RESET__STRUCT_H_
