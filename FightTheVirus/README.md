### Premise
The FPS game follows the journey of the main protagonist being shrunken down to a miniature size and inserted into the human body to fight off an unnamed virus, essentially becoming an overpowered T-Cell. There are three different types of NPCs: Passive, Cooperative, and Aggressive. The Passive & Cooperative NPCs are red and white blood cells respectively, with aggressive NPCs being the unnamed virus.

Some Viruses are pre-spawned in at each level, and in a battle with the white and red cells upon entering each area of the map. In the final area of the map there exists a replication center for viruses that continuously spawns new ones that can then traverse throughout the map and track down to the players current location.

The final goal of the player is to reach the replication center and eradicate any viruses that exist.

---

### Planning Phase
1. This was a group project during one of the courses within my program. We brainstormed many ideas, primarily how the game should look and feel, What type of controls should be available to the player, what kind of power-ups/environmental buffs; would be accessible to the player, and the types/abilities of NPCs (friendly and enemy).

    - **Basics**:
        1. WASD-based character movement
        2. Multiple shooting modes
        3. Power-ups that confer bonuses on the player
<br><br>
    - **Unique Pick-ups**:
        1. Health regen
        2. Triple shot modifier
        3. Laser shot modifier
        4. Basic projectile shot modifier
    <br><br>
    - **Jump Pads**: Allows for traversal between some rooms, and will unlock after a certain amount of enemies have been defeated in the area. Jump pads can also be used to get a better vantage point on enemies and as such will be restricted by a countdown timer after their use.
    - **Teleporters**: Similarly, some progression is gated through teleporters, and they will only unlock after a certain amount of mobs have been defeated. The teleporters are essentially veins that allow you to move between areas of the body.
    - **Virus Reproduction**: The main area of virus reproduction comes in the form of the final area, the replication center, where a majority of enemies originate from. In addition, viruses can convert the passive and cooperative mobs to enemy viruses.

    - **NPC Types**
        1. **Conversion Virus** - Seeks red cells to convert
        2. **Melee Virus** - Attacks at close range
        3. **Ranged Virus** - Shoots projectiles with an arc projectile movement
        4. **White cell** *cooperative NPC* - Attacks Viruses
        5. **Red cell** *passive NPC* - Runs away from viruses
        6. **Reproduction Virus** - *Boss NPC* endlessly spawns random virus cells and can only be defeated with a basic attack. 
<br><br>
    - **Level Design**
        1. Should feel as if the player is traversing through one connected playfield rather than several "levels". The player should get the sense they are traversing through the body.
        2. Jump pads should provide mobility to otherwise in-accessible areas of a level.

    ---

### Challenges
1. [Multiple developers](#working-in-a-group) on single project
2. [Singular level](#level-streaming) design with multiple "levels"
3. [Sound effects and music](#its-so-quiet)
4. [Large number of entities](#anyone-lagging)

---
<br>

### Working in a group
* While working in a group on any type of project contains challenges... A multi-faceted gane development environment stems passed the typical communication, scheduling, responsibilities issues of any group project.
    1. Everyone is working on their own branch of the code.
    2. Some people are creating art assets, some are creating NPC systems, some people are working on AI or NPC spawning systems, others are working on power-ups/triggerables.
    3. Many aspects of design, depend on other works having been completed and merged to everyone, to allow designing levels with placeholders that can be updated on the creators branch and merged later to provide final functionality.
    4. Everyone needs to be able to work on their own section of the code within their own branch of the code while pulling in changes to other's section of code within your own branch. 

* These factors were tackled with very well done upfront planning. We developed a codebase that had everyones sections separate, and all placeholder entities or definitions were available for use in level design... however their base functionality was basic debug rendering/output. Being able to communicate on a bi-weekly basis with each team allowed quickly addressing any missing needs and ensuring everyone was on the same page before moving forward. Although version-control software can become a pull request/ merge nightmare... in the end we were able to accomplish our goal with little or no conflict resolution required.

### Level Streaming
* It was very important that each level designer was able to work on their levels separately, with all of the tools exposed to them (spawn points, triggerables, power-ups, teleporters, etc). While they were updating their levels, it was also important that it would not cause conflict with other designers maps. 
    - To solve this, we decided to have the levels created separately, however the final master level that the game would start on... would stream each of the individual levels into a section of this master level. This did require some tweaking in the end because some levels were larger than the space originally defined for their stream section, or were created with very different light levels/ light types. This was really the only thing that had to be done after the final merge.

### It's So Quiet
* Sound can make or break a game... effects should not be too loud or too anoying to hear repetatively. The player should also not feel like they need to bring their own soundtrack to the game, lest they find themselves sitting in awkward silence.
    - We addressed this in two ways... as none of us working on the project had much experience with creating music or sound effects. We utilized a combination of free assets, recorded/created sound effects, and reaching out to a very musically talented friend to record some original tracks that could be looped for game music. In the end the audio is probably not going to win any awards, but functioned very well.
### Anyone Lagging?
* A game that spawns a potentially endless number of enemy entities... who's AI tells them to seek out the nearest target... could get out of hand very quickly and could cause some major issues.
    - We had a couple different ideas that came together to form a powerful solution. First, the spawn points had a set number of enemies available to spawn. The enemies would spawn as a random type, and while the spawner was still active any enemies that had been eliminated would be recycled. These same enemies would be reset and reused for each level (the final level uses multiple spawn points that allows for a very large number of enemies to safely be re-used until the boss has been eliminated). Second, the AI for the enemies had a limited polling distance based on simple rays rather than polling the entire entity list over and over... They would "watch" on their rays and when triggered ATTACK! furthermore the enemy NPC would immediately "see" any entity that had attacked it. These allowed for a large network of NPCs that were able to carry out useful tasks quickly without causing a huge hit on memory or cpu.
    
---   
# Conclusion & Future
This was a fantastic project and we all had a lot of fun working on it, learning from eachother and encouraging/inspiring eachother with ideas or even taking on a little extra when a team member was in need or going through a rough patch in their personal life. (this was right at the height of COVID-19). After we had finished I had wanted to make the friendly NPCs follow you and head toward enemies that you started to attack or had attacked you. Also I wanted to have a health bar rather than a 100/100 text bar. Lastly I thought that the power-up icons should be used to show which weapon you are using and what alternate weapon you currently have. Those changes have since been added to this refined version of our original game, and I do think there is potention for more polish and expansion. 

### References
* [Original project BitBucket](https://bitbucket.org/douglasmcmann/project_repo)
* [Original project WiKi](https://bitbucket.org/douglasmcmann/project_repo/wiki)
