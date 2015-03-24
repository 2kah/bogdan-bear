# Aims #

The prototype will have a basic tower and player that can move around it.

# Components #
## Tower ##
Classes structure to manage the rings of the tower and keep track of which blocks still exist.
## Player ##
Class to keep track of player location, orientation and to forward movement commands to the physics.
## Game ##
Class/Main function to manage the whole game and update everything.
## Input ##
Class to process input and redirect it to locally controlled players.

# Outline #
## Tower ##
A class encapsulating all the behaviour of the tower.
### Members ###
  * Container of **Layers**
### Methods ###
```
CarveSphere(sphere)
```
Remove tower blocks from each layer that intersect with a given sphere. (Bullet probably handles the collision maths for this)

```
BuildPath(path)
```
Replace tower blocks from each layer that intersect with a given path. (Bullet probably handles the collision maths for this)

## TowerLayer ##
A class encapsulating all the behaviour of an individual rotating segment of the tower. (See my circles.py for maths on generating segments)
### Members ###
  * Initial radius (this is the distance of the layer's closest edge to the centre of the tower, which is 0 for the middle column)
  * Sequence of sub-divisions (e.g [12, 12, 24] for a layer with three rings, the first two divided into 12 segments, the third divided into 14)
  * Rotation speed (this is the speed the layer rotates at, negative/positive for clockwise/anticlockwise, zero for static)
  * Description of currently present blocks (some sort of index which maps a block index to alive/dead)
### Methods ###
```
CarveSphere(sphere)
```
Remove tower blocks in this layer that intersect with a given sphere. (Bullet probably handles the collision maths for this)

```
BuildPath(path)
```
Replace tower blocks in this layer that intersect with a given path. (Bullet probably handles the collision maths for this)

## Player ##
A class encapsulating all the state information and behaviour about a player.
### Members ###
  * position (this is pulled from Bullet after a frame of physics simulation has run)
  * velocity (this is pulled from Bullet after a frame of physics simulation has run)
  * orientation (this is the direction the player is facing)
  * pointer to **Weapon**
  * pointer to **Animation** (ignore this until we know how it works)

### Methods ###
```
Jump()
```
Check the player is on a floor and apply an upward impulse if they are.

```
Move(keyboard direction)
```
Apply a small impulse to the player in the keyboard direction relative to the direction they are facing.

```
Aim(mouse movement?)
```
Change the player's orientation based on the given mouse movement.

```
Shoot()
```
Ask the weapon to shoot, it may trigger an event indicating ammo/cooldown is not ready.

## Weapon ##
A class encapsulating all the state and behaviour of the player's weapon.
### Members ###
  * ammo count
  * cooldown remaining (between shots)
  * reload time remaining (before ammo count increases by one)

### Methods ###
```
Shoot(position, aim)
```
Ask the weapon to shoot from the given position, in the given direction. If this is not allowed due to cooldown or ammo then the relevant signal should be generated. (not sure about boost signals yet, so we could just print a message to console)

```
Reload()
```
Ask the weapon to reload. If this is not possible due to the weapon being full or other reasons, then the relevant signal should be generated. (not sure about boost signals yet, so we could just print a message to console)

**More info to come later.**

Add your content here.  Format your content with:
  * Text in **bold** or _italic_
  * Headings, paragraphs, and lists
  * Automatic links to other wiki pages