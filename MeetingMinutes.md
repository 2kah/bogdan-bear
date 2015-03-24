#Meeting Minutes


## Mins for meeting on 09/03/12 ##
> ## Meeting 19 (Bogdan Bear) ##

Attended By: All

Progress

Adam
  * Linux build

Harshna
  * Sound

James
  * Rockets in networking, with 2 players
  * Player connects/disconnnects

Joe
  * Reloading, goal, shooting from turret

Mark
  * Corrected repo
  * Set up control path for physics

Group Programming
  * Monday, 11am

Networking
  * Progress - Rockets, players
  * Needed - Platform



## Mins for meeting on 02/03/12 ##
> ## Meeting 19 (Bogdan Bear) ##

Attended By : All

Next Composer's Meeting
  * 12pm Tuesday

Progress
Mark
  * Signals for player walking (kinematic controls - should tell you when player has jumped)
  * Boost signals tracker (for deleting objects)

Adam
  * Linux build
  * 32 Bit Binary...64Bit system..can't build 64 bit binaries

Joe
  * Goal implemented

Harshna
  * Sounds done

James
  * Unordered map, network IDs etc.
  * Iterates through networked players

No points if more than one person in the goal

Gantt Chart
  * About one month behind

Networking
  * Tower regeneration event (parameters come from synced RNG) - Happens in correct order relative to other events

Group Programming
  * Monday 10:30am

## Mins for meeting on 29/02/12 ##
> ## Meeting 19 (Bogdan Bear) ##

Attended by: All

Meeting with Andrew

Progress Reports
James
  * Networking - Infrastructure in place
Adam
  * Linux Build - Almost working on an early revision
Joe
  * Goal - Physics problem & reloading
Harshna
  * Sounds
Mark
  * General networking information

Composers Meeting
  * uesday 12pm

Deadlines
  * 2 next week
  * MPI - End of Easter
  * AS & HPC - 2 weeks after Easter

Week dedicated to Games Project - Last week of Easter

Report binding
Splitting up report
Presentation (everyone talks if possible), think about transitions

Group Programming
  * 5:30pm Thursday



> ## Mins for meeting on 10/02/12 ##
> > ## Meeting 18 (Bogdan Bear) ##

Attended by: All

Progress Reports

Adam
  * Worked on tower regeneration/ player physics for turrets
Joseph
  * Turrets
James
  * Tidying up networking
  * Researching replica manager
Harshna
  * Sound

Gantt Chart Progress
  * Behind on:
    * Goal
    * Networking
    * Ogre tidy up
    * Console
    * Tower Regeneration

  * Nearly there:
    * Meta shapes
    * Turrets
    * Ogre tidy up

Linux Build
  * Blocked
  * Not linking with Boost

Group Programming Session
  * Sunday, 11am

Meetings
  * Composers - Tuesday, 12pm
  * Andrew - Wednesday, 11am

Animation
  * Needs research before modelling







> ## Mins for meeting on 03/02/12 ##
> > ## Meeting 17 (Bogdan Bear) ##

Attended by: All (5 members now)

Progress Reports

Mark
  * Ogre cleanup
  * Networking (with James)
Harshna
  * Sound
Joseph
  * 4 Turrets and sorted problems with aiming
  * Working on entering turrets
James
  * Networking - Has synchronised explosions
Adam
  * Fixed running faster when diagonal

Turret Graphics
  * Turret should have graphic with player inside

Camera
  * First person
  * Possibly implement third person camera

Group Coding Session
  * Useful for networking issues - James & Mark
  * Better for later on in the project
  * Google Hangout
    * Next Session: Weds 2pm

Beta Progress
  * Game controller
  * Networking
  * Tower regeneration
  * Goal
  * Turrets

Game Names
  * Themes
    * Refactor
    * Tower
    * Regeneration
    * Reconstruction
    * Destruction
    * Renewal
    * Ascent
    * Digital
    * Virtual
    * Abstract
    * Structure
    * Frantic
    * Control
    * Power
    * Electro
    * Dynamic
    * Nexus
    * Flux
    * Energetic
    * Pixel

Networking
  * Players - (To server) where they shot from, when in turret
  * Rocket collides and explodes when server tells it to
  * Player position - server might not necessarily know where players always are
  * Turrets - Under authority of server
  * Tower synchronisation - Limit refreshes
  * Easier for player to keep control over shooting, position and platform creation

(Mark has table of server/client setup)



> ## Mins for meeting on 31/01/12 ##
> > ## Meeting 16 (Bogdan Bear) ##

Attended by: All (5 members now)

Progress update with Andrew

Networking Update
  * Issues - Build issues, getting 2 game instances synchronised, communication
  * Progress/Queries - Getting 2 game instances synchronised

Plan Update
  * Rescheduled plan
  * Beta deadline: Networking, linux build, tower regeneration, turrets, goal, ogre tidy, console
  * Texturing reassigned - Joseph and Harshna

Music Update
  * Suggestion: Now time to arrange second meeting & demo with composers (Around beta deadline, 13th Feb?)
  * Does VS have a built in sound manager?

Testing Discussion
  * Play/Load testing (Linux build is important for this)
  * COGs LAN

Report Discussion
  * Suggestion: Spend one meeting discussing report entirely

Presentation Discussion
  * Good for everyone to talk about the section they concentrated on
  * Tees?





> ## Mins for meeting on 23/01/12 ##
> > ## Meeting 15 (Bogdan Bear) ##

Attended by: Adam, Mark, Harshna, Joseph

Progress report
  * Mark
    * Continuing on with tower physics
  * Callum
    * -
  * Joseph
    * Drawings and research for basic lighting
  * Harshna
    * Technical drawings
    * Research for sound engines
  * James
    * -
  * Adam
    * Player controls
    * Restructuring the schedule for Beta/Gamma deadlines - Priority is playable game.

Sound Effects

  * Priority
    * Player shooting
    * Exploding energy ball
    * Goal captured - 'victory' alert
    * Reload
    * Platform created
    * Goal lost
    * Game loss/victory

  * Low Priority
    * Destroying platforms
    * Opening turret doors
    * Turrets moving
    * Turret ejecting player
    * Turret fires

Needed (for playable game and restructuring plan):
  * Player control
  * Tower physics
  * Turret behaviour
  * Tower regeneration
  * Platform creation
  * --Networking--

Sound Engines
  * Consider FMOD, IrrKlang, OpenAL
  * 3D Sound

Group Programming Session
  * Sunday, 2-5pm

Meeting times for new timetable
  * Changed to Friday 11am
  * Progress Reports -> Tuesdays


---



> ## Mins for meeting on 10/01/12 ##
> > ## Meeting 14 (Bogdan Bear) ##

attended by: all

Progress report
  * Mark
    * Is everyone working on the new one
      * All bar Callum
  * Callum
    * None
  * Harshna & Joe
    * Concept art
    * Handed tower over
  * James
    * Issues with Cmake
    * Racknet interpolation
    * Racknet header tags
  * Adam
    * working with Mark on planning
    * Character control
      * Switching to kinematic

The Plan
  * On FB group

  * By today:
    * Wanted the tower and character working
    * Meeting with Andrew next week
      * Have artwork done
      * Have networking done
      * Have basic tower Regen
      * Bowl in

  * Week after this (after review)
    * Technical drawings
    * Some animation by James
    * Mark does some Ogre tidy-up
    * Adam work on platform physics
    * Mark and Adam to work on a console

  * Week after this
    * Tower Regen working, not finished but to a good standard
    * Harshna and Joe to work on lighting

  * February
  * Week after next
    * Up to at least 6 players
    * Harshna and Joe on Turret behaviour

  * Week after this
    * Have the modelling done
    * Objective behaviour
    * Basic sound for Harshna and Joe, what and when
    * Beta deadline


> Art
    * Looked at Harshna and Joe's concept art
    * Bowl
    * Talked about lighting
    * Talked about Character designs
      * ASCII faces

**Turret Tangent
  * Number of turrets?
    * Belong to a team? or neutral?**

  * Players start up at the top, in the sir of get exploded off to the top

  * falling, have some random spin in it, or with small movement: postponed

  * Music
    * Meet up
      * have an art and music meeting on Monday



  * Back story
    * Tournament?

  * Task allocation for next Tuesday
    * Networking
      * Messaging
    * Tower physics
    * Bowl in
    * Concept art




---


> ## Mins for meeting on 21/12/11 ##
> > ## Meeting 13 (Bogdan Bear) ##

attended by: all

Progress report:
  * Harshna & Joe
    * Working on collision stuff
    * close to finishing

  * Mark
    * Plan needs fleshing out

  * Callum
    * No progress

Alpha
  * Missing Alpha by approx 1 day
  * everyone should work on what they were previously working on until Mark finishes the re-factor plan

To Do:
  * Joe & Harshna
    * Physics and collisions
  * Callum
    * Manual objects

> James
    * Networking
  * Mark
    * Re-factor plan


---


> ## Mins for meeting on 05/12/11 ##
> > ## Meeting 12 (Bogdan Bear) ##


attended by: all

Progress report:
  * Mark
    * Not much since Thursday

  * James
    * Not much since Thursday

  * Adam
    * Integrated Tower code

  * Callum
    * On with Manual objects

  * Joe and Harshna
    * Solid tower block with physics

Discussion on how to create the tower:
  * Start with Manual objects
    * Use Maya for all but the tower
    * There are drawbacks to this

  * We still need to hide the geometry
    * Fridge light

  * For Alpha this doesn't matter too much


Music:
  * Should hear back about this on Thursday

To do:
  * Sketching ideas for modelling
    * By Christmas
    * Character design
      * More solid ideas
    * Tower design

Alpha Approaching
  * Push back 1 week into Christmas - Wed 21t
  * Player with physics
    * Currently a ninja
  * Tower with explosions
  * Networking
    * build racknet

Next deadline:
  * Monday
  * Harshna & Joe
    * Work towards tower
  * Adam
    * Work on physics
  * All
    * drawing




---



> ## Mins for meeting on 21/11/11 ##
> > ## Meeting 11 (Bogdan Bear) ##

attended by: all

Progress report:
  * Mark
    * Main loop is working
    * Spinning Ogre head

  * James
    * Looked at Fmod and Irrklang
    * Irrklang works fine, doesn't output an executable
    * Fmod did some old tutorials

  * Callum
    * Had a look at some Racknet tutorials, no integration
    * Basic Ogre tutorial

  * Harshna and Joe
    * Made tower prototype

  * Adam
    * Bullet tutorial done
    * Should be fairly straightforward to integrate

  * Firmly Defining Alpha
    * Destructible tower
      * Not have rockets
        * Player generated explosion
    * Have player that can move in 3D environment
      * Effected by Physics
      * Generate Explosions
      * Jump
    * Networking
      * Have server program, which will run the entire simulation
      * Min 2 clients that can connect
      * Both players should be able to interact with each other and the tower
      * Players can move about
      * Players can see each
    * "Bowl" in place
    * Have games engine working
    * Have cubes moving in 3D
    * Have a tower


  * Music
    * Playlists that Mark and Harshna have made
    * When and how much
      * During the Game
        * Yes how much
        * Need to work on art style?
    * We should all listen to the sound track

  * Task Distribution
    * Meet at 28/11/11 at 12:00
    * Harshna & Joe - Tower physics - destructible?
    * Mark & Adam - Player and physics
    * Mark - API for adding objects
      * Will be used for integration
    * James & Callum - Racknet
      * Generate an event when ever something happens - server outputs this to clients
      * How to send things, how to receive, how to make a client, how to make a server
      * Integrated?
    * Music and backstory for all




---



> # All of the meeting minutes #

> ## Mins for meeting on 15/11/11 ##
> > ## Meeting 10 (Bogdan Bear) ##

attended by: all

Agenda:
  * What we have done
    * Adam - 6 tutorials for Ogre
      * Directory for dll's
        * Put all dll's into here
        * Have zipped and in sync
    * Joe - Several tutorials and have a tower
    * Harshna - Same
    * Callum - 1 tutorial behind
    * Mark 6 Ogre tutorials (5 is a good tutorial)
      * Website for physics across machine read
      * http://gafferongames.com/game-physics/
      * Working code in repo for cmake that runs in Ogre console for shitty game.
    * James - On tutorial 3 for Ogre
      * Solution for console output for whole project

  * Note For OpenAL, have a look at higher lvl options

  * For the prototype
    * Tower that is generated and is physically there
    * Player that can move and 'jump'

  * What we need to get done for it
    * Main loop
    * Integrate with bullet

  * Tasks that we will do
    * Main loop
      * Can update Ogre when needed
      * Looks at Bullet when free
    * Integrate Bullet
    * Integrating the Tower the main loop and Bullet

  * Adam and Mark will get the main loop done
  * Callum Racknet integration
  * Joe and Harshna - Mark's Ogre test to work with tower
    * Look into instancing
  * James look into sound libraries - liaise with Adam and Mark
  * Everyone should do all of the basic Ogre tutorials, and many of the advanced ones.
  * Everyone should look at Bullet tutorials

  * Everything should be pushed for the prototype


  * Meeting for Monday 21st at 12:00

  * Agenda for next meeting
    * Discuss progress, in regard to about the prototype
    * Planning for Alpha
      * Define Alpha more
    * Task distribution for next meeting




---




> # All of the meeting minutes #

> ## Mins for meeting on 9/11/11 ##
> > ## Meeting 9 (Bogdan Bear) ##

attended by: all

Ogre:
  * Tutorials - everyone should be able to do them

  * Joe - rendering code pulled
  * Everyone should have a go at CMake

  * James - Player - this is simpler than most games (no inventory health etc)

  * Mark - Bullet into Ogre (?)

For Prototype:
  * Tower generation
    * How something is generated
    * Integrated with bullet

  * Input - should be straight forward

  * As well as these look at
    * Modelling
    * Create / destroy mechanics

  * James to look at ogre console output
  * All - look at ogre and cmake

For next time:
  * Harshna and Joe - Tower in ogre
  * Mark and Adam - Game ( Main loop that looks at input works out what happens etc)
  * James - Look at ogre output
  * Callum - Racknet and CMake - ping example

  * All - Ogre tutorial

Agenda for next meeting:
  * Discuss what we've done and explain
  * Work out next step




---



> ## Mins for meeting on 7/11/11 ##
> > ## Meeting 8 (Bogdan Bear) ##

attended by: all
Agenda:
  * Prototype discussion

Prototype discussion
  * Physics
  * Graphics
  * Player
  * Tower
  * Input

  * For just the prototype we'll focus on:
    * Game
    * Input
    * Tower
    * Player

  * Ogre issues
    * Joe is getting Josh to show him to install it
    * Should we change to 1.72

  * Assigning tasks
    * Game - Mark and Adam
    * Tower - Harshna and Joe
    * Player - James
    * Input - Callum

  * Mark will upload some header files
  * We should all work on getting Ogre working
  * Everything should be able to be pulled form the repo and used with CMake

Agenda for next time:
  * Discuss ogre
  * Discuss Tasks




---



> ## Mins for meeting on 2/11/11 ##
> > ## Meeting 7 (Bogdan Bear) ##

attended by: all
Agenda:
  * Proof of concept
  * Coding standards
  * Plans for next meeting
  * Approx time 1 hour
  * Potential for integration after the meeting

Coding Standards:
  * Brackets on their own line
  * Camel case for methods
  * Lower case for variables
  * Soft tabs for spaces
  * Operators have spaces
  * Commenting:
    * For every function there should be a comment for the auto-documenter

Proof of Concept:
  * Ogre:
    * Is evil
    * Use 1.73
  * Bullet
    * Fine
  * Ogre-Bullet integration
    * We won't bother
  * Racknet
    * Works, need code extract
  * OpenAL
    * Works, need code extract
  * Compilation
    * MSCV  for windows
    * GCC on Linux
    * Be careful with libraries
    * Whilst continually integrating check it works on Linux

For next meeting
  * All look at Ogre
  * Mark to look at the prototype design
  * Aim to get the prototype done in three weeks
  * We aim to have all of the used libraries in the repository for Monday meeting




---



> ## Mins for meeting on 29/10/11 ##
> > ## Meeting 6 (Bogdan Bear) ##

attended by: all
Agenda:
  * Engine
  * Proof of concept outlined
  * Tasks for next meeting

Engine:
  * Ogre is cross platform
  * Unreal may not be as bad to develop for as first thought
  * XNA provides little more than Ogre

  * We decided on **Ogre - Bullet - Racknet - OpenAL**


Proof of Concept:
  * Goals
    * To have a pre-alpha quick and messy example to prove the concept and get some experience using the tools

  * Graphics (Ogre):
    * Ogre to render something
    * Create some geometry in Ogre
    * Create the tower geometry (not a necessity)
    * Make some models in a format Ogre will export
    * James is working on this


  * Networking (Racknet):
    * Connect two computers a client and a server
    * Synchronous input and output to both
    * Mark is working on this

  * Physics (Bullet):
    * Play with and investigate
    * Implement some things that may appear easy without the graphics
    * Harshna & Joe are working on this

  * Sounds (OpenAL):
    * Work out how it works
    * Play some sound effects
    * Play some music
    * Import music/sound effects
    * Callum will work on this

  * Compilation and Distribution:
    * Find a way to make the game cross platform (Windows and Linux)
    * Get a working "hello world" program
    * Making an installer, making sure this can work on lab machines
    * Adam will work on this

Next meeting agenda:
  * Proof of concept
  * Coding standards
  * Plans for next meeting
  * Approx time 1 hour
  * Potential for integration after the meeting



---



> # All of the meeting minutes #

> ## Mins for meeting on 28/10/11 ##
> > ## Meeting 5 (Bogdan Bear) ##

attended by: all
Agenda:
  * Write Paragraph
  * Decide on an engine
  * Work for next meeting

Write Paragraph:
  * Paragraph written
  * Issues with photograph on department page resolved

Decide on an engine:
  * Entered deadlock with two choices due to platform choices:
    * If we end up on Windows plan to use:
      * Unreal engine
      * XNA

  * If we end up on Linux we plan to use:
    * Ogre - Bullet - Racknet - OpenAL

  * We went through a number of choices and their pro's and con's:
    * Unreal - Can use the binary for it, has comprehensive networking, has fracture for destructible environments.

  * Cry Engine - Can use a binary for it, lacks destructible environments

  * Source - Cannot use without license

  * XNA - Complete freedom in it's use, and it has a large community behind it

  * Ogre - difficult to use, but has cutting edge features, used by past groups

  * Irrlicht - Quite easy to use, hard to spell

  * PhysX - Licence is no good

  * HavoK - Can make use of binary

  * Bullet - Free, uses similar back end to PhysX

  * Open **- Hard**

  * Racknet - widely used

  * OCE - Not seemingly used for games

  * Boost asio - Not seemingly used for games

  * Fmod - Does fancy stuff

  * OpenAL - Open source


For next meeting
  * Look into Unreal and XNA engines
  * Look at procedural generation
  * Concept art



---



> # All of the meeting minutes #

> ## Mins for meeting on 25/10/11 ##
> > ## Meeting 4 (Bogdan Bear) ##

attended by: all
Agenda:
  * Long term Goals
  * Results of Research
  * Concept Drawings
  * Tasks by next meeting

Long Term Goals:
  * Set deadlines for Alpha, Beta and Gamma
    * Meeting was held in week 2
    * Alpha deadline: 16th December, 7 weeks from meeting
      * Have games engine working
      * Cubes moving in 3D environment
      * Tower in place
      * "Destructible"
      * Physics working
        * Networking for minimum 2 players

  * Beta Deadline 13th February, 9 weeks from Alpha
    * Tower regeneration
    * Real-time destructibility / regeneration
    * Platform generation
    * Infrastructure in for graphics
    * Basic animated models
    * Rudimentary lighting
    * The whole game should be working, but only in a rudimentary way
    * networking for at least 6 players

  * Gamma Deadline 23rd March, 6 weeks from Beta
    * Game complete
    * Play testing and tweaking done
    * Fully formed graphics
    * Models done

  * Results of research
    * Engines
      * Unreal engine
        * Destructible demo
        * James has experience with creating assets for it
        * its pretty
        * Problems:
          * Technically strong, takes away some challenges
          * A non-issues according to Colin

  * Irlicht
    * Only a graphics engine

  * Ogre
  * Source
  * IdTech 3/4
  * Cry Engine

  * To do for next meeting
    * Decide Engine
      * Unreal (all look at thoroughly)
      * Source
      * IdTech 3/4
      * Cry Engine
      * Infinity Ward Engine
      * Geo Mod
      * Ogre - Bullet - racknet

  * Concept Art
  * Procedural thoughts

  * Agenda for next meeting
    * Discuss research
    * Decide on engine
    * Work for next meeting



---


> ## Mins for meeting on 21/10/11 ##
> > ## Meeting 3 (Bogdan Bear) ##

attended by: all

Names for game
  * Tower
  * Refactor - Going for this at present

Need to put up a blurb for the game

By the end of the game we aim to have networking support for 16 players
By the end of the game we aim to support at least 4 teams of 4 people
By the end we aim to support 1st person play, with the potential to have 3rd person as well

We discussed a number of ways to make the tower:
  * Voxels
  * Bigger Voxels
  * Lego blocs
  * Fake voxels
  * circle Fan out idea (we went with this one)


We decided to leave any descisions about jumping, with or without rokets, and gravity till we start playtesting

Goals for next meeting
  * Mark - make a google code and wiki
  * Callum - start the report on google docs, write the blurb for the game

  * For all look at
    * Procedural generation
    * Concept art
    * Character Design
    * Circle generation
    * Networking
    * Engines

In future we will aim to have meeting adgendas




---


> ## Mins for meeting on 17/10/11 ##
> > ## Meeting 2 (Bogdan Bear) ##

attended by: all

People have thouhgt on the 2 ideas "Mulder" and "Scully"

Harshna:
  * Scully
    * Canvas world
    * paint is alive
    * bad guy "Terps"
    * User is a paint brush
    * paints obstacles

  * Mulder
    * Lost in a book
    * For each chapter the difficultly increases
    * Author = bad guy
    * Each chapter has a different theme

  * Misc
    * Flying wiht the kinect
      * everyone is a plane
      * arms out
      * mosies to fire gun

  * Evil orphanage
    * sweet granny = bad guy
    * each level of orphange is a _level_
    * make paper weapons
    * cardboard for specials
    * scary

Mark:
  * Scully
    * Tank/Robot
      * drill arm
      * builder arm
        * can build raods
        * maike path for walking bombs
        * drill can destroy terrain
        * 2 player Helicopter

  * Mulder
    * terror atmospheric
    * set in a nightmare
    * procedurally generated
    * run from nightmare
    * co-op looks at mario ghost
    * multiple escape routes
    * climb dream tower towards consciousness
    * generate monstors randomly


James:
  * Scully
    * First person
    * Proceedurally generated map
    * build/ destroy

  * Mulder
    * Escher like
    * Non logical


Adam
  * Scully
    * random Proceedurally generated blocks
    * top down
    * Dotted line to see where they go
    * get to bad guy's base

  * Mulder
    * Webcrawler
      * type url, uses this to make the level
    * play through a web page

Callum
  * Scully
    * Light Lemmings
    * territory control with light bulbs
    * Monsters in the dark

  * Mulder
    * Procedurally generated platformer
    * colourful robots
    * each colour can do something




---


> ## Mins for meeting on 14/10/11 ##
> > ## Meeting 1 (Bogdan Bear) ##

attended by: all

From last meeting:
  * Come up with ideas for 2 games each

Maze game
  * can move up levels/floors
  * There are obstacles
  * is in a pyramid shape
  * king of the hill?
  * team multiplayer
    * time limit to make single player competative
    * different players have different abilities
  * Voice travels over a fixed distance
    * there is a danger to communication
    * muck about with audio timings (make it creepy)
    * randomly generated maze
    * customise characters?

Wii-mote Jousting

Kinect puzzle

Minecraft X lemmings
  * Isometric design
  * 2 player antagonistic

Tower defense/attack

Terrain deformation

Bright and colourful
  * not racing
  * platformer
    * remove colour

Create / destroy game
  * draw to create
    * technical

3 player game
  * 2 try to attack one another
  * 3rd tries to stop them


Ended with choosing 2 ideas to work on:
idea 1 "Mulder" Maze idea
idea 2 "Scully" Lemming X Minecraft

Next meeting Monday at 11
Setup Blog
put any ideas up on facebook