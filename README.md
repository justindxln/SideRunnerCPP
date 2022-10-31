# SideRunnerCPP
This is an Unreal Engine 4 learning project for myself to dive into the Unreal Engine 4 C++ API and the Unreal Motion Graphics system
I started with a simple endless runner game tutorial and then extended the features on my own from there (acknowledgements below)


**CURRENT FEATURE EXTENSIONS:**

** Gameplay Systems **
- Level Generation: All levels are stored in Arrays in Game Mode instead of individual hardcoded references
- Level Generation: Test mode with special levels are accessible through entering "password" as player name
- Movement Mechanics: Player movement properties are adjustable and saved in settings menu
- Core Mechanics: Instant death is removed, and the player has depletable HP and Shield
- Core Mechanics: Different damage types are introduced, doing instant damage or damage over time
- Core Mechanics: Powerup pickups in the levels provide boosts to the player (health, healing, shield, speed boost, score boost, etc)
- Scoring: Player accumulates scores as they survive, the more risky they play by staying close to the wall, the higher the score multiplier
- Scoring: High scores are saved to the game, and displayed at the end of each run

** Menu UI **
- Main Menu: Player can enter a player name, start a new game, or go to Settings Menu
- Settings Menu: Player can adjust various game settings and go back to Main Menu. Settings are saved upon returning to Main Menu
- Game Over Menu: The final score is displayed along with high scores. Player can go back to Main Menu

** HUD UI **
- Player Status HUD: Player health bar with number text display, dynamic fill color depending on HP, and HP gain/loss animation
- Player Status HUD: Shield bar showing shield presence around HP bar
- Player Status HUD: Double Jump indicator light showing double jump availability and cooldown progress
- Player Status HUD: Status Icons Tray shows current active status effects and their remaining durations and/or stack amount
- Score HUD: Score Multiplier display with dynamic color and animation speed, plus "pop" animation when multiplier changes
- Score HUD: Current Score display

** Misc **
- Adjust camera position to show more on the side the player is facing
- Various edge case bug fixes and code architecture reworks to the original tutorial code
- Various aesthetic changes


Planned extra features include:
  - More environmental hazards and damage types
  - Destructible environment

Assets I've used for this project:
  - https://krukowski.itch.io/free-sci-fi-game-icons

References I've used for this project include:
  - https://docs.unrealengine.com/4.26/en-US/ProgrammingAndScripting/ProgrammingWithCPP/CPPTutorials/UMG/
  - https://www.youtube.com/watch?v=SOjZTmOMGcY
  - https://unrealcpp.com/
  - https://www.youtube.com/watch?v=4M2v2SYJDJk
  - https://docs.unrealengine.com/4.27/en-US/
  - https://joyrok.com/UMG-Layouts-Tips-and-Tricks
 
