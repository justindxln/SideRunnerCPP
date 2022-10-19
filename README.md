# SideRunnerCPP
Unreal Engine 4 project for myself to learn the engine and C++.
I started with a simple endless runner game based off of a tutorial and then extended the features on my own from there (acknowledgements below)

The current extra features I have added so far are:
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
