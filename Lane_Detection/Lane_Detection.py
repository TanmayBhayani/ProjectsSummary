import numpy as np
import cv2 as cv

def make_ROI(img,vertices):
	mask = np.zeros_like(img)
	if img.ndim == 3:
		channel_count = img.shape[2]
	else:
		channel_count = 1
	match_mask_color = (255,) * channel_count
	cv.fillPoly(mask,vertices,match_mask_color)
	masked_image = cv.bitwise_and(img,mask)
	return masked_image

def draw_lines(img, lines):
	img = np.copy(img)
	# blank_image = np.zeros((img.shape[0],img.shape[1],3),dtype=np.uint8)
	if lines is not None:
		for line in lines:
			x1,y1,x2,y2 = line[0]
			cv.line(img, (x1,y1), (x2,y2), (0,255,0), thickness=3)
	# img = cv.addWeighted(img,0.8,blank_image,1,0.0)
	return img

cap = cv.VideoCapture('challenge_video.mp4')
while cap.isOpened():
    ret, frame = cap.read()
    if not ret:
        print("Can't receive frame (stream end?). Exiting ...")
        break
    # gray_frame = cv.cvtColor(frame, cv.COLOR_BGR2GRAY)
    hsv_frame = cv.cvtColor(frame, cv.COLOR_BGR2HSV)
    lower_yellow = np.array([21,50,80])
    upper_yellow = np.array([38,255,255])
    lower_white = np.array([0,0,200])
    upper_white = np.array([179,30,255])
    extracted_yellow_lanes_mask = cv.inRange(hsv_frame,lower_yellow,upper_yellow)
    extracted_white_lanes_mask = cv.inRange(hsv_frame,lower_white,upper_white)
    extracted_lanes_mask = cv.bitwise_or(extracted_yellow_lanes_mask,extracted_white_lanes_mask)
    blured_extracted_lanes_mask = cv.GaussianBlur(extracted_lanes_mask,(15,15),0)
    edges_frame = cv.Canny(blured_extracted_lanes_mask,100,200)
    height = frame.shape[0]
    width = frame.shape[1]
    region_of_interest_vertices = [(int(0.1*width),height),(int(width/1.9),int(height/1.7)),(int(width*0.9),height)]
    cropped_edges_frame = make_ROI(edges_frame,np.array([region_of_interest_vertices],np.int32))
    lines = cv.HoughLinesP(cropped_edges_frame,rho=1,theta=np.pi/60,threshold=60,lines=np.array([]),minLineLength=10,maxLineGap=100)
    frame_with_lanes = draw_lines(frame,lines)
    cv.line(frame,region_of_interest_vertices[0],region_of_interest_vertices[1], (0,255,0),3)
    cv.line(frame,region_of_interest_vertices[1],region_of_interest_vertices[2],(0,255,0),3)
    cv.line(frame,region_of_interest_vertices[2],region_of_interest_vertices[0],(0,255,0),3)
    # cv.imshow('frame', frame)
    cv.imshow('Lanes', frame_with_lanes)
    # cv.imshow('edges', cropped_edges_frame)
    # cv.imshow('blured_extracted_lanes_mask',blured_extracted_lanes_mask)
    # cv.imshow('extracted_white_lanes_mask', extracted_white_lanes_mask)
    key = cv.waitKey(100)
    if key == 32:
        cv.waitKey()
    elif key == ord('q'):
        break
cap.release()
cv.destroyAllWindows()
