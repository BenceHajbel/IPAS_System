import csv
import os
from datetime import datetime

import rclpy
from rclpy.node import Node
from ipas_interfaces.msg import IPASMeasurement


CSV_PATH = os.path.expanduser(
    '~/GitHub_repose/IPAS/Simulation/data/measurements.csv'
)


class LoggerNode(Node):
    def __init__(self):
        super().__init__('ipas_logger_node')

        self.csv_path = CSV_PATH
        self._ensure_csv_header()

        self.subscription = self.create_subscription(
            IPASMeasurement,
            '/ipas/measurement',
            self.listener_callback,
            10
        )

        self.get_logger().info(f'Logging measurements to {self.csv_path}')

    def _ensure_csv_header(self):
        file_exists = os.path.isfile(self.csv_path)
        if not file_exists:
            os.makedirs(os.path.dirname(self.csv_path), exist_ok=True)
            with open(self.csv_path, mode='w', newline='') as f:
                writer = csv.writer(f)
                writer.writerow([
                    'timestamp',
                    'temperature',
                    'humidity',
                    'pressure',
                    'gas_resistance'
                ])

    def listener_callback(self, msg: IPASMeasurement):
        timestamp = datetime.now().isoformat()
        with open(self.csv_path, mode='a', newline='') as f:
            writer = csv.writer(f)
            writer.writerow([
                timestamp,
                msg.temperature,
                msg.humidity,
                msg.pressure,
                msg.gas_resistance
            ])
        self.get_logger().info(
            f'Saved: T={msg.temperature:.2f} H={msg.humidity:.2f} '
            f'P={msg.pressure:.2f} Gas={msg.gas_resistance:.0f}'
        )


def main(args=None):
    rclpy.init(args=args)
    node = LoggerNode()
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    node.destroy_node()
    rclpy.shutdown()


if __name__ == '__main__':
    main()
