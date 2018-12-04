README

Design/Tesselation Desisions

I decided to design each shape as a combination of a “barrel” and a “cap”. Essentially, each shape is composed of a base or two (the caps) and a shape that wraps around it to connect the bases (a barrel). All of my shapes are composed of a barrel and one or two caps. Specific barrels and caps are subclassed from the top level Barrel and Cap class. This allows me to easily create shapes based on logic that exists to create other shapes. 

I also chose to implement the Caps using a TRIANGLE_FAN orientation, except for the CubeCap, which was just made from Triangles. The barrels were all drawn with TRIANGLE_STRIP. For each barrel, I split the barrel into a series of horizontal strips, the number depending on the tessellation parameters. I created the TRIANGLE_STRIP orientation by just “connecting” these horizontal strips together.

The Barrel and Cap both subclass from GLWidget, which contains an OpenGLShape. This is modeled after the lab, and gives me easy access to the VAO, VBO, and other components shared by all OpenGL Shapes.

Bugs

I don’t think the normals at the point of my cone are correct.

Extra Credit

I used TRIANGLE_FAN and TRIANGLE_STRIP to draw several of the shape components. This meant that I had to draw less points, and was more efficient by reusing some points. I also implemented a Torus, one of the extra shapes.

There is a really basic implementation in “Special Shape 1” of a bagel-Klein bottle. Unfortunately I couldn’t get it quite perfect; the normals are off and it really only looks nice at higher tessellation parameters. Still looks cool though!
