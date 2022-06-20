
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
1. Player input
2. Grow the snake and render segments following each other
3. Random Food Positions
4. COLOR!
5. Render speed after adding color
6. Semi impressive Demo Play / Title Screen
