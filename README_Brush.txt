README_Brush

Design

I separated out most of the functionality of the brushes into Brush.cpp. so that I could reuse as much code as possible. To avoid repetitive code, I made use of inheritance so that every class interfacing from Brush would only contain code specific to it’s own handling of events like brushDown, brushDragged, and so on. 

I did make the choice to repeat code in Brush.cpp, SmudgeBrush.cpp, and SpecialBrush.cpp, for finding the particular square around the mouse click that we were acting on. I believed that extracting this code out to another method would make the code harder to read; when an outside eye sees methods iterating on a bounded box, I thought it would be better for them to see how the bounds of the box were calculated every time because this was so important to the method.

Extra Credit/Half-credit Requirements

I implemented “No Alpha Blending” by creating a separate drawing layer every time the user put the brush down. I also save what the canvas used to look like. I draw onto the separate layer rather than the current canvas, without taking into account the alpha value. After each brushDragged() call, I blend these layers together, using the alpha value as the parameter for a weighted average. When the brush is up, I stop caching the old canvas and the separate drawing layer, so they're available again when the brush goes down.

I created a new special brush that mimics a flame effect. I got the idea for this by researching how to create a fire effect. This implementation creates a noise array that scrolls infinitely. To get the fire effect, every pixel is averaged with it’s neighbors to slowly draw the color upwards. I then subtract the noise array value at that index from this average, so the color dims. (I found a basic explanation of this effect in a few papers online). The brush also “burns away” any other color on the canvas. This looks best without the Alpha-Blending fix. I used a QTimer to handle the animation here.

Bugs:

I noticed that when changing the settings, if you “click” on a different part of the line to change the alpha/red/green/blue value, the number of that value wasn’t actually changed. The slider needs to be actually dragged to change the value. Very weird. I think it’s in the demo too though.

I ran the memory analyzer and it looks like the issues it picked up on were external (in the main function, realloc function, etc).
