#!/usr/bin/env python3

import rclpy
from rclpy.node import Node
from std_msgs.msg import Int32

class InputNode(Node):

    def __init__(self):
        super().__init__("input_node")
        self.publisher_ = self.create_publisher(Int32, "selected_topic", 10)
        self.get_logger().info("Keyboard Input Node has been initialized.")
        self.keyboard_input()

    def keyboard_input(self):
        while True:
            selected_topic = int(input("Enter the topic number (1-4): "))
            msg = Int32()
            msg.data = selected_topic
            self.publisher_.publish(msg)
            self.get_logger().info(f"Published selected topic: {selected_topic}")
            
def main(args=None):
    rclpy.init(args=args)
    input_node = InputNode()
    rclpy.spin(input_node)
    rclpy.shutdown()

if __name__ == "__main__":
    main()
