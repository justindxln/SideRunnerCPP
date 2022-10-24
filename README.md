# SideRunnerCPP
Unreal Engine 4 project for myself to learn the engine and C++.
I started with a simple endless runner game based off of a tutorial and then extended the features on my own from there (acknowledgements below)

Extra features I have added so far are:
  - Stored level blueprints in an array instead of manually linking each as a separate variable
  - Made the game destroy old levels as new ones are spawned
  - Offsetted camera to show more on the right side
  - Fixed level spawn trigger box triggering twice
  - Added double jump
  - Added double jump cooldown limitation
  - Removed instant death and added player HP and damage types (wall does over time damage, spikes do instant damage)
  - Added main menu UI
  - Added settings menu UI
  - Allowed player speed, wall speed, double jump, double jump cooldown to be customisable
  - Added SaveGame and made player settings persistent
  - Added HUD UI
  - Added HP bar and HP Text to HUD
  - Added distance indicator to HUD (Timeline blink and spin animations, dynamic color based on distance)
  - Added double jump indicator to HUD (cooldown progress, glow image when double jump is available)
  - Added game over menu UI
  - Added scoring system based on how long the player survives, the closer the player stays to the wall the faster the score increases
  - Added arcade style persistent high scores and player names
  - Added a powerups systems with health and shield pickups
  - Added a test mode where a separate library of levels are spawned
  - Added dynamic HP Bar color


Planned extra features include:
  - More environmental hazards and damage types
  - Status effects
  - Destructible environment


References I've used for this project include:
  - https://docs.unrealengine.com/4.26/en-US/ProgrammingAndScripting/ProgrammingWithCPP/CPPTutorials/UMG/
  - https://www.youtube.com/watch?v=SOjZTmOMGcY
  - https://unrealcpp.com/
  - https://www.youtube.com/watch?v=4M2v2SYJDJk
  - https://docs.unrealengine.com/4.27/en-US/
 
