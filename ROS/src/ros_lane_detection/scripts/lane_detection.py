#!/usr/bin/env python
import rospy
import numpy as np
from sensor_msgs.msg import Image
from ros_lane_detection.msg import lane_lines
import cv2 as cv
from cv_bridge import CvBridge, CvBridgeError

pub = rospy.Publisher('line_coordinates', lane_lines, queue_size=10)
bridge = CvBridge()

def make_ROI(img,vertices1,vertices2):
    mask = np.zeros_like(img)
    if img.ndim == 3:
    	channel_count = img.shape[2]
    else:
    	channel_count = 1
    match_mask_color = (255,) * channel_count
    cv.fillPoly(mask,vertices1,match_mask_color)
    cv.fillPoly(mask,vertices2,(0,)*channel_count)
    masked_image = cv.bitwise_and(img,mask)
    cv.imshow('masked_image',masked_image)
    return masked_image

def get_points(lines):
    points = []
    if lines is not None:
        for line in lines:
            x1,y1,x2,y2=line[0]
            points.append((x1,y1))
            points.append((x2,y2))
        return points
    return None

def get_lines(img, lines,lane):
    img = np.copy(img)
    global lyp0,lyp1,lwp0,lwp1
    # blank_image = np.zeros((img.shape[0],img.shape[1],3),dtype=np.uint8)
    points = get_points(lines)
    if points is not None:
        fit_line = cv.fitLine(np.array(points, dtype=np.int32), cv.DIST_L2,0,0.01,0.01)
        t0 = (img.shape[0]/1.4-fit_line[3])/fit_line[1]
        t1 = (img.shape[0]-fit_line[3])/fit_line[1]
        p0 = np.clip(fit_line[2:4] + (t0 * fit_line[0:2]),0,img.shape[1]).astype(np.int32)
        p1 = np.clip(fit_line[2:4] + (t1 * fit_line[0:2]),0,img.shape[1]).astype(np.int32)
        if lane == 'yellow':
            lyp0 = p0
            lyp1 = p1
        elif lane == 'white':
            lwp0 = p0
            lwp1 = p1
        else:
            raise Exception('lane value can only be white or yellow!')
    return (lyp0.ravel(),lyp1.ravel()) if lane == 'yellow' else (lwp0.ravel(),lwp1.ravel())

def talker(data):
	try:
			frame = bridge.imgmsg_to_cv2(data, "bgr8")
	except CvBridgeError as e:
			print(e)
	rospy.loginfo("Image recieved!")
	message = lane_lines()
	# #dummy output
	# message.left_lane[0] = 5.5
	# message.left_lane[1] = 6.5
	# message.right_lane[0] = 7.5
	# message.right_lane[1] = 8.5
	# pub.publish(message)
	hsv_frame = cv.cvtColor(frame, cv.COLOR_BGR2HSV)
	lower_yellow = np.array([21,50,80])
	upper_yellow = np.array([38,255,255])
	lower_white = np.array([0,0,200])
	upper_white = np.array([179,30,255])
	extracted_yellow_lanes_mask = cv.inRange(hsv_frame,lower_yellow,upper_yellow)
	extracted_white_lanes_mask = cv.inRange(hsv_frame,lower_white,upper_white)
	blured_extracted_yellow_lanes_mask = cv.GaussianBlur(extracted_yellow_lanes_mask,(15,15),0)
	blured_extracted_white_lanes_mask = cv.GaussianBlur(extracted_white_lanes_mask,(15,15),0)
	yellow_edges_frame = cv.Canny(blured_extracted_yellow_lanes_mask,100,200)
	white_edges_frame = cv.Canny(blured_extracted_white_lanes_mask,100,200)
	height = frame.shape[0]
	width = frame.shape[1]
	region_of_interest_vertices1 = [(int(0.1*width),height),(int(width/1.9),int(height/1.7)),(int(width*0.9),height)]
	region_of_interest_vertices2 = [(int(0.2*width),height),(int(width/1.9),int(height/1.5)),(int(width*0.8),height)]
	cropped_yellow_edges_frame = make_ROI(yellow_edges_frame,np.array([region_of_interest_vertices1],np.int32),np.array([region_of_interest_vertices2],np.int32))
	cropped_white_edges_frame = make_ROI(white_edges_frame,np.array([region_of_interest_vertices1],np.int32),np.array([region_of_interest_vertices2],np.int32))
	white_lines = cv.HoughLinesP(cropped_white_edges_frame,rho=1,theta=np.pi/60,threshold=60,lines=np.array([]),minLineLength=10,maxLineGap=100)
	yellow_lines = cv.HoughLinesP(cropped_yellow_edges_frame,rho=1,theta=np.pi/60,threshold=60,lines=np.array([]),minLineLength=10,maxLineGap=100)
	yp0,yp1 = get_lines(frame,yellow_lines,"yellow")
	wp0,wp1 = get_lines(frame,white_lines,"white")
	message.left_lane[0] = float(yp1[1]-yp0[1])/(yp1[0]-yp0[0])
	message.left_lane[1] = yp1[1] - message.left_lane[0]*yp1[0]
	message.right_lane[0] = float(wp1[1]-wp0[1])/(wp1[0]-wp0[0])
	message.right_lane[1] = wp1[1] - message.right_lane[0]*wp1[0]
	pub.publish(message)
	
def listener():
	rospy.init_node('listener', anonymous=True)
	rospy.Subscriber("/videofile/image_raw", Image, talker)
	rospy.spin()
if __name__ == '__main__':
	listener()