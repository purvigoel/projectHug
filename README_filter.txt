Design

I have a base filter class that all the other filters are children of. This filter class has methods that most of the filters use, like convolution. The logic for convolution is pretty central to a lot of the filters, so it was important to separate it out. This also made debugging easier; I found that convolution came down to some pretty specific bounds-checking and edge cases, and by only having to deal with this logic in one place, I saved myself a lot of time and grief. Every filter class contained methods more specific to those actual classes.  Because all of the filters had factors that were quite unique from each other, I allowed them all to talk to Settings.h to get the specifics of the filtering that they were doing. If the filters were all less specific and filtered in similar ways, I would have kept Settings.h references within the UI folder.

Specifics
My special brush was the median filter. This brush has a size of 5. It acts similarly to convolution, but rather than performing a dot product, it gathers up all the numbers within the range of a filter and finds the median of it. It looks really cute on the why_so_serious bunny picture.

 I split my edge detection kernel into two separable linear kernels.  I first perform the convolution on the horizontal filter. Then, I perform the convolution on the vertical filter. This is similar to how I calculate scaling: first, I backmap in only the x-scale plane. Then, I backmap the new image in the y-scale plane.

 To do bounds checking in the blur filter, I treat all off screen pixels as being black (essentially padding the image with zeros). So, certain blur radiuses can introduce a blurry black border around the image. This is more pronounced with larger blur radiuses. Larger blur radiuses also can be a little slower. I noticed that the runtime was terrible when I had the Gaussian as a 2-D filter, so I changed it to a 1-D Gaussian in the x direction and a 1 D Gaussian in the y direction.

Bugs

None that I know of. I tried to use std::vectors and pointers wherever I could to protect against memory mistakes.
