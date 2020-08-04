# Invisible Cloak(The augmented reality)

**Invisible Cloak** is the **image processing** project which tries to copy the invisibilty effect as shown in **Harry Potter** movies by Harry Potter himself.

This **GitHub** repository contains the **C++** source code as well as an installable windows application.

**Note:** The video camera must be stable as the background will be constant for the project.
**Note:** By default the code is set to take live video input, which can be changed to other source. 

## Description

This project can be explained by dividing it into three parts:
1. Live video input
2. Background image processing on input
3. Output

#### 1. Live video input
- This step involves using machine's web camera to take the live video for processing.
- The application first takes a snapshot of the scene and askes user to select a color from the scene itself by clicking at any point on the image.
- Until user clicks the desired color of his/her own the video remain paused.
- After selecting color the application sets this color to be treated as invisible.
(The colors red and pink gives best results.)

**Note**: While selecting color make sure that the color is not present in background and once color is selected the object from which color was selected must be removed, so that a stable background image can be taken.

#### 2. Background image processing on input
- Once the cloak color is set the video is sent for image processing.
- In each frame of video the cloak color's mask is taken out and this is replaced by the constant background image pixels.
- The process is repeated for all image frames.
- Techiniques used for processing are **morphological and arithmetic operations (bitwise)** provided by openCV.

#### 3. Output
The live video after image processsing is diplayed on the machine's screen. User can hide behind that colored cloth and he will not be visible in the output video.


***

##### Note:
After clicking the color the step of removing of that colored object must be fast. If you want, you can give some pause time in code to avoid this.
***
##### Some video tutorials for this type of project are availabe on YouTube, for one such tutorial, link is given below: 
[**Video Tutorial**](https://www.youtube.com/watch?v=AkY2TpvDGUo)
***
##### Disclaimer: This is an open source project and can be used as per requirement of the user.
****
### #Harry Potter fan!!
### Happy Coding!! 🙂



