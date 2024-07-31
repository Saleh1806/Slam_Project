#!/usr/bin/env python3

import rclpy
from rclpy.node import Node
from rclpy.qos import QoSProfile
import tf2_ros
from sensor_msgs.msg import LaserScan
from geometry_msgs.msg import TransformStamped
import math

class LidarPoseEstimator(Node):
    def __init__(self, robot_base_frame="base_footprint"):
        super().__init__('lidar_pose_estimator')
        self.__robot_base_frame = robot_base_frame
        self.transform_stamped = TransformStamped()
        self.scan_subscriber = self.create_subscription(
            LaserScan,
            '/scan',
            self.scan_callback,
            QoSProfile(depth=1))
        self.br = tf2_ros.TransformBroadcaster(self)
        self.get_logger().warning("Lidar Pose Estimator node READY!")

    def scan_callback(self, msg):
        if len(msg.ranges) == 0:
            return
        angle_increment = msg.angle_increment
        num_ranges = len(msg.ranges)
        # Calculate the angle at which the lidar is pointing
        angle = msg.angle_min
        # Initialize variables for calculating the position
        sum_x = 0
        sum_y = 0
        count = 0
        for i in range(num_ranges):
            # Ignore invalid measurements
            if msg.ranges[i] < msg.range_min or msg.ranges[i] > msg.range_max:
                continue
            # Calculate the position of the measurement in Cartesian coordinates
            x = msg.ranges[i] * math.cos(angle)
            y = msg.ranges[i] * math.sin(angle)
            # Accumulate the positions
            sum_x += x
            sum_y += y
            count += 1
            # Increment the angle for the next measurement
            angle += angle_increment
        if count == 0:
            return
        # Calculate the average position
        avg_x = sum_x / count
        avg_y = sum_y / count
        # Publish the transform between "odom" and the robot base frame
        self.broadcast_new_tf(avg_x, avg_y)

    def broadcast_new_tf(self, avg_x, avg_y):
        self.transform_stamped.header.stamp = self.get_clock().now().to_msg()
        self.transform_stamped.header.frame_id = "odom"
        self.transform_stamped.child_frame_id = self.__robot_base_frame
        self.transform_stamped.transform.translation.x = avg_x
        self.transform_stamped.transform.translation.y = avg_y
        self.transform_stamped.transform.translation.z = 0.0
        self.transform_stamped.transform.rotation.x = 0.0
        self.transform_stamped.transform.rotation.y = 0.0
        self.transform_stamped.transform.rotation.z = 0.0
        self.transform_stamped.transform.rotation.w = 1.0

        self.br.sendTransform(self.transform_stamped)

def main(args=None):
    rclpy.init()
    lidar_pose_estimator = LidarPoseEstimator()
    rclpy.spin(lidar_pose_estimator)
    rclpy.shutdown()

if __name__ == '__main__':
    main()

