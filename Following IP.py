import numpy as np
import cv2
import math

from serial import Serial
import time
uno="COM8"
cap=cv2.VideoCapture(0)

ble=Serial(uno,9600)
flag=0
i=0



sums=0
count_defects=0
while True:
        
    _,frame=cap.read()
    cv2.imshow('frame',frame)
    if cv2.waitKey(1) & 0xFF==27:
        img=frame
        break
cv2.destroyWindow('frame')
r=cv2.selectROI("capture",img,False)
imcrop = img[int(r[1]):int(r[1]+r[3]), int(r[0]):int(r[0]+r[2])]
cv2.imshow("Image", imcrop)
cap.release()
cv2.waitKey(0)
gray = cv2.cvtColor(imcrop, cv2.COLOR_BGR2GRAY)
value=(5,5)
blur=cv2.GaussianBlur(gray,value,0)
_,thresh=cv2.threshold(blur,50,255,cv2.THRESH_BINARY_INV+cv2.THRESH_OTSU)
_,contours,h = cv2.findContours(thresh.copy(),cv2.RETR_EXTERNAL,cv2.CHAIN_APPROX_NONE)
    
area_array=[]
contour=[]
a=1
for cnt in contours:
    hull=cv2.convexHull(cnt,returnPoints=False) 
    defects=cv2.convexityDefects(cnt,hull)
for i,c in enumerate(contours):
    area=cv2.contourArea(c)
    area_array.append(area)
        

mapped=zip(area_array,contours)

sort=sorted(mapped,key=lambda x: x[0],reverse=True)

print("larger area:",sort[0][0]) #largest area x coordinate
print("smaller area:",sort[1][0]) #second largest area y coordinate
cnt=sort[1][1]
approx = cv2.approxPolyDP(sort[0][1],0.01*cv2.arcLength(sort[0][1],True),True)
hull=cv2.convexHull(sort[1][1],returnPoints=False)
defects=cv2.convexityDefects(sort[1][1],hull)

for i in range(defects.shape[0]):
    s,e,f,d = defects[i,0]
    start = tuple(cnt[s][0])
    end = tuple(cnt[e][0])
    far = tuple(cnt[f][0])

            # find length of all sides of triangle
    a = math.sqrt((end[0] - start[0])**2 + (end[1] - start[1])**2)
    b = math.sqrt((far[0] - start[0])**2 + (far[1] - start[1])**2)
    c = math.sqrt((end[0] - far[0])**2 + (end[1] - far[1])**2)
        
    angle = math.acos((b**2 + c**2 - a**2)/(2*b*c)) * 57 #apply cosine rule to find angle
    if angle <= 120:
        count_defects += 1
            
print("defects")
print(count_defects)
print("approx")
print (len(approx))
sums=len(approx)
print("-----------")



        
    #finding x coordinate with len(approx)
cnt=sort[0][1]
if sums==3:
    print ("triangle,X:0")
    x=0
    cv2.drawContours(imcrop,[cnt],0,(0,255,0),-1)
elif sums==4:
    print ("square,X:1")
    x=1
    cv2.drawContours(imcrop,[cnt],0,(0,0,255),-1)
elif sums==5:
    print ("pentagon,X:2")
    x=2
    cv2.drawContours(imcrop,[cnt],0,255,-1)
elif sums== 6:
    print ("hexagon,X:3")
    x=3
    cv2.drawContours(imcrop,[cnt],0,(255,255,0),-1)
elif sums==7:
    print ("heptagon,X:4")
    x=4
    cv2.drawContours(imcrop,[cnt],0,(0,255,255),-1)
elif sums==8:
    print("X:2")
    x=2
    cv2.drawContours(imcrop,[cnt],0,(255,0,255),-1)
    a=0

    #ble.write(str.encode(x))

    #finding y coordinate with defect


cnt=sort[1][1]
if count_defects==1:
    print("Y:0")
    y=0
    cv2.drawContours(imcrop,[cnt],0,(0,255,0),-1)
elif count_defects==4:
    print("Y:1")
    y=1
    cv2.drawContours(imcrop,[cnt],0,(0,0,255),-1)

elif count_defects==5:
    print("Y:2")
    y=2
    cv2.drawContours(imcrop,[cnt],0,255,-1)
elif count_defects==6:
    print("Y:3")
    y=3
    cv2.drawContours(imcrop,[cnt],0,(255,255,0),-1)

elif count_defects==7:
    print("Y:4")
    y=4
    cv2.drawContours(imcrop,[cnt],0,(0,255,255),-1)
elif count_defects==8:
    print("Y:-1")
    cv2.drawContours(imcrop,[cnt],0,(255,0,255),-1)
    a=0

    #ble.write(str.encode(y))
    
cv2.imshow('img',img)
cv2.imshow('capturd',imcrop)
print("end1")
cv2.imshow('thersh',thresh)
if y==-1:
    C=5
    ble.write(str.encode(C))
else:
    x=10*x
    C=x+y
    C=str(C)
    print(C)
    ble.write(str.encode(C))
       

    

    




