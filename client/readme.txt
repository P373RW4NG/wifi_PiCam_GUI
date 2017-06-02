1.Open .pro file, add and configure this project in Qt creator.  

2.Edit mainWindow.h

Find and change this line to your wifi IP  

	tcp::endpoint(address::from_string("your raspberry wifi IP"), 2345)

Find and change these two lines to your opencv haar cascade classifier directory:

	face_cascade_name = "your opencv haar cascade directory/haarcascade_frontalface_alt.xml"
	eyes_cascade_name = "your opencv haar cascade directory/haarcascade_eye_tree_eyeglasses.xml"
 
3.You can put your algorithm or functions inside definition of function loop(), in file mainWindow.cpp. 
