import cv2
import numpy as np
import functools

def pointCmp(pnt1,pnt2):
    if pnt1[0][0]==pnt2[0][0]:
        return pnt1[0][1]-pnt2[0][1]
    return pnt1[0][0]-pnt2[0][0]

img=cv2.imread('pic.jpg')
gray=cv2.cvtColor(img,cv2.COLOR_BGR2GRAY)
blur=cv2.GaussianBlur(gray,(5,5),0)
edges=cv2.Canny(blur,100,200)
contours,_=cv2.findContours(edges,cv2.RETR_LIST,cv2.CHAIN_APPROX_SIMPLE)
contours=sorted(contours,key=cv2.contourArea,reverse=True)
for contour in contours:
    p=cv2.arcLength(contour,True)
    approx=cv2.approxPolyDP(contour,0.02*p,True)
    if len(approx)==4:
        target=approx
        break
target=sorted(target,key=functools.cmp_to_key(pointCmp))
approx=np.float32(target)
dim=np.float32([[0,0],[0,600],[400,0],[400,600]])
pt=cv2.getPerspectiveTransform(approx,dim)
fitpic=cv2.warpPerspective(img,pt,(400,600))

kernel=np.array([[-1,-1,-1],[-1,9,-1],[-1,-1,-1]])
sharpepic=cv2.filter2D(fitpic,-1,kernel)
_,threshpic = cv2.threshold(sharpepic,127,255,cv2.THRESH_TOZERO)

cv2.imshow('sharpening',sharpepic)
cv2.imshow('thresholding',threshpic)
cv2.waitKey()
