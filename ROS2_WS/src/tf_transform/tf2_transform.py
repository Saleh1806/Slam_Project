import rclpy
from rclpy.node import Node
from geometry_msgs.msg import TransformStamped
import tf2_ros

class TransformPublisher(Node):
    def __init__(self):
        super().__init__('transform_publisher')
        self.tf_broadcaster = tf2_ros.TransformBroadcaster(self)

    def publish_transforms(self):
        while True:
            # Publish transformation between odom frame and base frame (robot base)
            odom_to_base_transform = TransformStamped()
            odom_to_base_transform.header.stamp = self.get_clock().now().to_msg()
            odom_to_base_transform.header.frame_id = 'odom'
            odom_to_base_transform.child_frame_id = 'base_footprint'
            # No translation or rotation
            self.tf_broadcaster.sendTransform(odom_to_base_transform)

            # Publish transformation between base frame (robot base) and lidar frame
            base_to_lidar_transform = TransformStamped()
            base_to_lidar_transform.header.stamp = self.get_clock().now().to_msg()
            base_to_lidar_transform.header.frame_id = 'odom'
            base_to_lidar_transform.child_frame_id = 'base_footprint'
            # No translation or rotation
            self.tf_broadcaster.sendTransform(base_to_lidar_transform)

            # Adjust the sleep time based on your publishing rate
            self.get_logger().info('Published transformations')
            rclpy.spin_once(self, timeout_sec=0.5)

def main(args=None):
    rclpy.init(args=args)
    transform_publisher = TransformPublisher()
    transform_publisher.publish_transforms()
    rclpy.shutdown()

if __name__ == '__main__':
    main()

