import random
import rclpy
from rclpy.node import Node
from ipas_interfaces.msg import IPASMeasurement


class BME680Node(Node):
    def __init__(self):
        super().__init__('bme680_node')

        self.measurement_pub = self.create_publisher(
            IPASMeasurement,
            '/ipas/measurement',
            10
        )

        # Alapértékek, amik körül ingadozni fognak a mért adatok
        self.base_temperature = 24.5
        self.base_humidity = 48.0
        self.base_pressure = 1013.2
        self.base_gas = 12000.0

        self.timer = self.create_timer(1.0, self.publish_data)

    def publish_data(self):
        # Véletlenszerű ingadozás hozzáadása az alapértékekhez
        msg = IPASMeasurement()
        msg.temperature = self.base_temperature + random.uniform(-0.5, 0.5)
        msg.humidity = self.base_humidity + random.uniform(-2.0, 2.0)
        msg.pressure = self.base_pressure + random.uniform(-1.5, 1.5)
        msg.gas_resistance = self.base_gas + random.uniform(-500.0, 500.0)

        self.measurement_pub.publish(msg)

        self.get_logger().info(
            f'T={msg.temperature:.2f} °C | '
            f'H={msg.humidity:.2f} % | '
            f'P={msg.pressure:.2f} hPa | '
            f'Gas={msg.gas_resistance:.0f} Ω'
        )


def main(args=None):
    rclpy.init(args=args)
    node = BME680Node()
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    node.destroy_node()
    rclpy.shutdown()


if __name__ == '__main__':
    main()
