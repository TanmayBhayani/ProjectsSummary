import numpy as np
import cv2 as cv


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

def draw_lines(img, lines,lane):
    img = np.copy(img)
    global lyp0,lyp1,lwp0,lwp1
    # blank_image = np.zeros((img.shape[0],img.shape[1],3),dtype=np.uint8)
    points = get_points(lines)
    if points is not None:
        fit_line = cv.fitLine(np.array(points, dtype=np.int32), cv.DIST_L2,0,0.01,0.01)
        t0 = (img.shape[0]/1.4-fit_line[3])/fit_line[1]
        t1 = (img.shape[0]-fit_line[3])/fit_line[1]
        p0 = np.clip(fit_line[2:4] + (t0 * fit_line[0:2]),0,img.shape[1]).astype(np.uint32)
        p1 = np.clip(fit_line[2:4] + (t1 * fit_line[0:2]),0,img.shape[1]).astype(np.uint32)
        if lane == 'yellow':
            lyp0 = p0
            lyp1 = p1
        elif lane == 'white':
            lwp0 = p0
            lwp1 = p1
        else:
            raise Exception('lane value can only be white or yellow!')
    if lane == 'yellow':
        cv.line(img, tuple(lyp0.ravel()), tuple(lyp1.ravel()), (0, 255, 0), 3)
    elif lane == 'white':
        cv.line(img, tuple(lwp0.ravel()), tuple(lwp1.ravel()), (0, 255, 0), 3)
    else:
        raise Exception('lane value can only be white or yellow!')   
    # if lines is not None:
    # 	for line in lines:
    # 		x1,y1,x2,y2 = line[0]
    # 		cv.line(img, (x1,y1), (x2,y2), (0,255,0), thickness=3)
    # img = cv.addWeighted(img,0.8,blank_image,1,0.0)
    return img

cap = cv.VideoCapture('challenge_video.mp4')

# fourcc = cv.VideoWriter_fourcc(*'MP4V')
# out = cv.VideoWriter('output_challenge.mp4', fourcc, 20.0, (640,  480))
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
    # extracted_lanes_mask = cv.bitwise_or(extracted_yellow_lanes_mask,extracted_white_lanes_mask)
    # closing = cv.morphologyEx(extracted_lanes_mask, cv.MORPH_CLOSE, kernel)
    # dilation = cv.dilate(extracted_lanes_mask,kernel,iterations = 1)
    blured_extracted_yellow_lanes_mask = cv.GaussianBlur(extracted_yellow_lanes_mask,(15,15),0)
    blured_extracted_white_lanes_mask = cv.GaussianBlur(extracted_white_lanes_mask,(15,15),0)
    # threshold_img = cv.adaptiveThreshold(gray_frame,255,cv.ADAPTIVE_THRESH_GAUSSIAN_C,cv.THRESH_BINARY,11,4)
    # threshold_img = cv.bitwise_not(threshold_img)

    # opening = cv.morphologyEx(threshold_img, cv.MORPH_OPEN, kernel)
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
    frame_with_lanes = draw_lines(frame,yellow_lines,"yellow")
    frame_with_lanes = draw_lines(frame_with_lanes,white_lines,"white")
    cv.line(frame,region_of_interest_vertices1[0],region_of_interest_vertices1[1], (0,255,0),3)
    cv.line(frame,region_of_interest_vertices1[1],region_of_interest_vertices1[2],(0,255,0),3)
    cv.line(frame,region_of_interest_vertices1[2],region_of_interest_vertices1[0],(0,255,0),3)
    # cv.imshow('frame', frame)
    cv.imshow('frame_with_lanes', frame_with_lanes)
    # cv.imshow('closing', closing)
    # cv.imshow('gray_frame', gray_frame)
    # cv.imshow('threshold_img', threshold_img)
    # cv.imshow('Lanes', frame_with_lanes)
    # cv.imshow('edges', cropped_edges_frame)
    # cv.imshow('extracted_lanes_mask',extracted_lanes_mask)
    # cv.imshow('extracted_white_lanes_mask', extracted_white_lanes_mask)
    # out.write(frame_with_lanes)
    key = cv.waitKey(100)
    if key == 32:
        cv.waitKey()
    elif key == ord('q'):
        break
cap.release()
# out.release()
cv.destroyAllWindows()
