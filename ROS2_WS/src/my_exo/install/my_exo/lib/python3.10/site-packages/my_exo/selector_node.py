#!/usr/bin/env python3
import rclpy
from rclpy.node import Node
from std_msgs.msg import Float32, Int32

class SelectorNode(Node):

    def __init__(self):
        super().__init__("selector_node")
        self.selected_topic = None
        self.selected_topic_subscription = self.create_subscription(
            Int32, "selected_topic", self.selected_topic_callback, 10
        )

    def selected_topic_callback(self, msg: Int32):
        self.selected_topic = msg.data
        self.get_logger().info(f"Selected topic set to: {self.selected_topic}")

        # Unsubscribe from all subscriptions
        self.destroy_subscription(self.first_suscriber_)
        self.destroy_subscription(self.second_suscriber_)
        self.destroy_subscription(self.third_suscriber_)
        self.destroy_subscription(self.fourth_suscriber_)

        # Subscribe to the appropriate topic based on the selected_topic
        if self.selected_topic == 1:
            self.first_suscriber_ = self.create_subscription(
                Float32, "math_1", self.first_callback, 10
            )
        elif self.selected_topic == 2:
            self.second_suscriber_ = self.create_subscription(
                Float32, "math_2", self.second_callback, 10
            )
        elif self.selected_topic == 3:
            self.third_suscriber_ = self.create_subscription(
                Float32, "math_3", self.third_callback, 10
            )
        elif self.selected_topic == 4:
            self.fourth_suscriber_ = self.create_subscription(
                Float32, "math_4", self.fourth_callback, 10
            )

    def first_callback(self, msg: Float32):
        self.get_logger().info(f"Received from node 1: {msg.data}")

    def second_callback(self, msg: Float32):
        self.get_logger().info(f"Received from node 2: {msg.data}")

    def third_callback(self, msg: Float32):
        self.get_logger().info(f"Received from node 3: {msg.data}")

    def fourth_callback(self, msg: Float32):
        self.get_logger().info(f"Received from node 4: {msg.data}")

def main(args=None):
    rclpy.init(args=args)
    node = SelectorNode()
    rclpy.spin(node)
    rclpy.shutdown()

if __name__ == "__main__":
    main()
