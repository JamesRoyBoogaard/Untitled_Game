# Untitled Game Idea

## Premise

The setting is during the Anglo-Zulu war. A lone Zulu warrior has to fight off hoards of red-coats who have superior fire-power. What they don’t know, however, is this oak is not to be messed with.

Picture doom-slayer but from Africa. He is able to block and dodge bullets and can take troops out with one swing of his short spear.

The creation of this game is an exercise in learning Nvim, low level game development and C.

## Requirements

### Gameplay

#### Player<br>
colour = green.
form = square.
speed = 2 pixels per frame.
behaviour = The player will have a melee weapon which they can swing. This weapon kills enemies instantly. They players and all characters must have very tight, fast and consistent movement. 

#### Fighting <br>
The player can sustain 3 bullet from the enemy and there ranged attacks. The enemy dies instantly when hit within melee range by the player. 

#### Enemies <br>
colour = red.
form = square.
speed = half the players speed.
behaviour = Once that are within 60 pixels of the player they will stop and start shooting yellow bullets at the player. 

#### Levels <br>
The map is static and the enemies spawn from the sides randomly. There spawn rate ranges from 1 - 4 per second as the game continues.(to be dialed in as the game is tested) 

#### Menus <br>
There is a basic pause menu that is accessed by hitting 'm'. The menu is a toggle so to exit jsut press 'm' again.

#### Art <br>
The characters are represented by coloured blocks for now but will eventually be replaced by proper art, although that is not the priority for this project

### Performance

#### Frame Rate <br>
The frame rate for this game is a classic 30fps.
#### Graphics <br>

#### Tech stack <br>

