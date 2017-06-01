
1.Edit mainWindow.h

Change wifi IP in 

	tcp::endpoint(address::from_string("your raspberry wifi IP"), 2345)

Change these two lines to your opencv haar cascade classifier directory

	face_cascade_name = "your opencv haar cascade directory/haarcascade_frontalface_alt.xml"
	eyes_cascade_name = "your opencv haar cascade directory/haarcascade_eye_tree_eyeglasses.xml"
 
2.You can put your algorithm inside function loop() in file mainWindow.cpp 
