
## Console Based Game of Snake 
https://user-images.githubusercontent.com/24780/174604200-24085e3e-3077-4c44-8295-aa6f2c305355.mp4

---
### Planning
1. What is the game of snake? 
   - A player controls an agent (the snake)
   - The agent needs to reach the apple (food) in order to grow
   - The agent should grow 1 segment per food collected
   - The agent should not be allowed to run into itself or walls
   - The player will be rewarded a score for each food collected
   - The speed will increase as more food is collected
   - If no food is collected for an extended time the agent will die
   - Eating food quicker yields a higher scoring bonus
   - The player wins if the agent grows large enough to occupy the entire playfield
2. What are the technical aspects?
   - Tack agent data (location, length, hunger, time since last eaten)
   - Accept user input without delaying the game or feeling unnatural
   - render segments count, score, speed, playfield, random food positions, snake segments
   - detect agent collision with walls or self
   - allow playing again or exiting the game gracefuly
3. Can I keep it simple?
   - Render the graphics in ASCII in the console
   - Use a simple data structure for the playfield and snake segments
   - Calculate position from a single offset in one dimensional array, (position = y * width + x)

### Challenges
1. [Player input](#player-input)
2. Grow the snake and [render segments](#render-snake-properly) following each other
3. [Random Food Positions](#random-food-positioning)
4. [COLOR!](#color)
5. [Render speed](#rendering-got-slow-once-color-was-added) after adding color
6. Semi impressive [Demo Play](#title-screen-and-demo) / Title Screen

---
<br>

### Player Input
 * Needs to be able to capture multiple possibilities of keyboard input without blocking execution.
   - This is accomplished utilizing Windows API calls to ```GetAsyncKeyState``` which returns 0 if a key has not been pressed since it was last called, a negative number if it is currently being pressed and 1 if it has been pressed since the last call.
   - using this information, I was able to create a small function that would loop through the keys I wanted to poll prior to the render phase. This could have also been done using conio.h, however it is less reliable and can vary widely. Another popular solution is to spawn a second thread that calls a callback function when input is received. This was an interresting lesson to learn.

### Render Snake Properly
 * The snake should continually move in the direction it was last commanded to travel, additionally if the snake eats food it should grow exactly 1 additional segment.
   - I challenged myself to come up with a simple/effective solution to this. What I came up with... was an additional data structure the same size as the map, that would hold the ```head number```. This number would determine weather a snake segment would be drawn or not. Any number above 0 means the space is occupied by a segment and should be drawn. The current head position and number of segments is set each time the snake moves. When rendering, each of these numbers is decreased by 1.
   - This results in the ability to simply track the number of segments the snake should be, and writing that number to the corresponding position each update. When a food is eaten this will result in the current number increasing by one leaving the additional segments for the appropriate number of updates.

### Random Food Positioning
 * Should spawn within the playable area.
 * should not spawn on an area currently occupied by a snake segment.

   - These two requirements... as simple as they are, were an initial overlook I made. In the beginning I was simply generating a random number within the range of the playable area and making it reprisent a food. 
   - After playing for a while I realized that the food could spawn in positions that were technically in the margin, or spawn ontop of the snake itself. 
   - **To solve this**, I went with the very simple solution of grabbing a random position (properly within the bounds), testing to see if the contents of that position were a blank space or not... and doing this until a suitable position is found.. **This does have a problem**, as the game progresses the number of valid positions for the food becomes less and less. I will improve this in the future via selecting only from the positions that are still valid rather than the whole game-board. This will also make testing for a win condition much easier. Than checking if the snake segments length is equal to the area of the gameboard.... (who actually completes a game of snake?). 

### COLOR!
 * Nobody wants to play a game of snake without any color!
   - For this... I took to creating some variables that held the ANSI escape sequences for various colors.. this would then be used within printf to set/change/unset the color at various points in rendering the playfield. When it comes to the console and colors... I am unaware of another way to change the color of text characters. I could have utilized the background and foreground colors for indivitual sections to define the playfield, however I was already drawing ASCII characters for this also.

### Rendering got slow once color was added...
 * A game should be smoothly animated to allow immersive gameplay. I rand into a challenge where dynamically changing the color of the text while rendering left me rendering sections of known text and in some cases single characters to ensure their color would be correct and not have a negative effect on other characters...
   - To fix this... I simply created a ```screen``` buffer and utilized ```sprintf_s``` to create the fully formed output, and then outputting the entire screen buffer in one procedure. This is a form of a back buffer or double buffering. 

### Title screen and Demo
 * I wanted the player to be able to open the game and not be shoved right into play mode... The player should be prompted to do something to cause play to begin. Although I also did not just want a simple (press a direction get to start) message, as this seemed too boring.
   - When the game begins a `demo` flag is set and this triggers the game to render the welcome screen and a routine that will pick a direction that would cause the snake to go toward the food... if the direction it picked was the exact opposite of the direction it was already going or it would land them in a wall or on themselves it would attempt to pick a new direction up to 10 times. This turned out to be a pretty simple and effective AI for the intro demo. 
   - Once a player begins to control the snake the game is reset and scoring is allowed to begin.
---
<br>

### Conclusion and the Future
 * I learned quite a lot in the journey of creating a simple console version of snake. I have a much bigger appreciation for being able to define pixels or tiles within a game screen, having the control over colors and freedom to define areas of a buffer as zones is a bit more intuative than dealing with ASCII characters alone. 
   - I plan to add sound and a configurable ```high scores``` (online/offline) tracking for those who want to see how they compare to everyone else, or just their friends/family... (yes I know it is just a simple snake game and doesnt really need this.. its about learning).. :)
   - I am also working on a half float and machine learning algorithm implementation within the [Linux](/SnakeGame/Linux/) version of the game. This is a large learning opportunity for many future projects.
   
<br>

---

[Home](/README.md)