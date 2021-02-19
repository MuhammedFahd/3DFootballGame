# FootballGame
A Football Game written in C++ using the OpenGL Library.

A Simple Football game that allows the user to change the position of the ball horizontally and decide the verical position of the ball to shoot and then shoot it. The wall has several hoops drawn which the user is supposed to hit with the football and earn points.
The user has 10 attempts after which the game ends or the game ends if the user has hit all the hoops on the wall.
The bounding limits are the walls. If the user shoots the ball above the wall, the ball resets back to it's original position and the user loses an attempt. The same happens if the user hits the wall at any place other than the hoop.
A Timer Function is used to reload the frames every 25ms to ensure a smooth animation. The Ball rotates when the user shoots the ball to produce a proper illusion of a moving ball.
Uses Perspective View to produce a 3D Environment. The Vertical and Horizontal Position Bar, and all the text are displayed using Orthographic Projection (2D).
Basic Lighting used to produce the 3D Effect. The Lighting conditions change over time. The Ambient light becomes darker and then lighter. This loops throughout the game.
4 Textured planes are used for the walls - Front, Left, Right and Back wall. A Plane is used for the textured grass surface. A Textured Ball is created using gluSphere. Flowers are drawn infront of the Front Wall. Plants are drawn infront of the Right and Left walls.
8 Hoops are displayed on the front wall. Each has a score drawn within the hoop. The larger the hoop, the lower the score. The Hoop points are as follows:
- Green Hoop : 10 Points
- Blue Hoop : 20 Points
- Red Hoop : 30 Points

Uses Object Oriented Programming concepts

### Controls
- Left Arrow Key - Move the ball to the left horizontally
- Right Arrow Key - Move the ball to the right horizontally
- Up Arrow Key - Move the vertical shooting point up
- Down Arrow Key - Move the vertical shooting point down
- F - Able to obtain a closeup view. The Camera is position inches behind the ball
- z or spacebar- shoot the ball
