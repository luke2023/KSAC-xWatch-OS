import paho.mqtt.client as mqtt
import picamera
import time
from io import BytesIO

ip_address = "mqttgo.io"  # Update with your MQTT broker address
topic = "image"
capture_interval = 5  # Capture an image every 5 seconds


def on_publish(client, userdata, mid):
    print("Image published with mid:", mid)


def capture_and_publish_image():
    try:
        # Capture an image using the Raspberry Pi camera module
        with picamera.PiCamera() as camera:
            # Adjust camera settings as needed
            time.sleep(2)  # Allow time for the camera to warm up
            stream = BytesIO()
            camera.capture(stream, format='jpeg')
            image_data = stream.getvalue()

        # Publish the image data to the specified MQTT topic
        result = client.publish(topic, payload=image_data, qos=1)
        t = client.publish("image", payload="hello", qos=1)
        print("hee")
        client.on_publish = on_publish

        # Wait for the message to be published
        while not result.is_published():
            time.sleep(0.1)

        print("Image published successfully")

    except Exception as e:
        print(f"Error capturing and publishing image: {e}")


def on_connect(client, userdata, flags, rc):
    print("Connected with result code " + str(rc))
    client.subscribe(topic)


# MQTT setup
client = mqtt.Client()
# client.username_pw_set("user", user")
client.connect(ip_address, 1883, 60)
client.on_connect = on_connect

# Main loop to capture and publish images every 'capture_interval' seconds
try:
    while True:
        capture_and_publish_image()
        time.sleep(capture_interval)

except KeyboardInterrupt:
    print("Script terminated by user")

finally:
    # Disconnect from the MQTT broker
    client.disconnect()
