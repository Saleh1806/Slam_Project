#!/usr/bin/env python3
import rclpy
from rclpy.node import Node
import random
from std_msgs.msg import Float32

class SecondNode(Node):

    def __init__(self):
        super().__init__("second_node")
        self.random_value_ = 0
        self.cmd_vel_pub_ = self.create_publisher(Float32, "math_2", 10)
        self.timer_ = self.create_timer(0.5, self.send_random_value)
        
    def send_random_value(self):
        self.get_logger().info("le 2e noeud publie" + str(self.random_value_))
        self.random_value_= random.uniform(5, 10)
        msg = Float32()
        msg.data = self.random_value_
        self.cmd_vel_pub_.publish(msg)


def main(args=None):
    rclpy.init(args=args)
    node = SecondNode()
    rclpy.spin(node)
    rclpy.shutdown()

if __name__ == '__main__':
    main()