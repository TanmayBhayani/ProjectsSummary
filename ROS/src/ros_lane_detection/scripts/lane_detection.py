#!/usr/bin/env python
import rospy
from sensor_msgs.msg import Image
import lane_lines
pub = rospy.Publisher('line_coordinates', lane_lines(), queue_size=10)
rospy.init_node('talker', anonymous=True)

def talker(data):
	rospy.loginfo("Image recieved!")
	message = lane_lines()
	#dummy output
	message.left = 5.5
	message.right = 6.5
	pub.publish(message)
# def callback(data):
	# rospy.loginfo("Image recieved!")
def listener():
	rospy.init_node('listener', anonymous=True)
	rospy.Subscriber("/videofile/image_raw", Image, talker)
	rospy.spin()
if __name__ == '__main__':
	listener()