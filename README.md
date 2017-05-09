# Tanks

## Summary
Jump in the cockpit of a personalized tank and storm the front line of the battle in a 1v1 tank fight. Use strategy to determine when to spend your points, move, or even give up your turn. Fully customizable game settings allow the fight to be down and dirty in the desert or tactical and patient up in the mountain tops. What are you waiting for?

## Table of Contents:
* Team Members
* About
* Controls
* Other Controls
* Pointshop
* Update Log

## Team Members:
* Paul A. Fischer paulfischerproductions@gmail.com, pfischer@carthage.edu
* Donald Jervis

## About the Project
Tanks was created as a final project for Computer Science II at Carthage College. Ideas for the game were sourced from conversations with peers, other versions online, teacher recommendations, as well as a multitude of other games. This program was written first year of learning the language, thus the langauge may be crude. Tanks was written in Visual Stuido 15 using c++ and ncurses. Total time invested is 52 hours over the course of 5 days.

## Controls
**(A)** move left

**(D)** move right

**(left_arrow)** p1 = power down | p2 = power up

**(right_arrow)** p1 = power up   | p2 = power down

**(up_arrow)** = angle up

**(down_arrow)** = angle down

**(enter)(space)** = fire, +1 petrol

**(p)** = open pointshop

**(backspace)(delete)** = turn pass, + 2 petrol

**(q)** = quit

## Other Controls
Other controls consists of keys that allow the game to be debugged or demonstrated easier. m (money) allows the pointshop to be demonstrated. W (wind) shows the effect of wind on each player respectively. s (show) toggles the view of tools used to help debug. r (render) toggles between a colored block view and black and white ground smoothing view.

**(m)** = generate money

**(w)** = regenerate wind

**(s)** = toggle debugging features

**(r)** = toggle ground render

## Settings
**(b)** = back

**(/)(?)** = ? This effects the pointshop

**(p)** = starting gas toggle Ability to change starting gas between 0 and 15 for both players

**(h)** = starting health toggle Ability to change starting health between 1 and 5 for both players

**(w)** = wind toggle Turns wind on and off in the game

**(t)** = terrain type toggle Toggles between flat (desert), medium (forest), and high (mountain) terrains

**(1)** = assign player 1 nickname

**(2)** = assign player 2 nickname

**(3)** = assign player 1 tank color

**(4)** = assign player 2 tank color

# Pointshop
Players are rewarded 25 points if the other tank takes damage. They are also awarded 50 points for winning. As points accumulate they are saved and added up throughout the rounds allowing the players to make purchases in the pointshop. Health can be purchased until the user has 5. Petrol refills can be purchased and give a full tank of gas (toggle). Bombs that are purchased in the Pointshop are automatically equipped for the turn (unless skipping). Only one bomb may be purchased at a time.
**(b)** = back

**(l)** = large bomb

**(s)** = strong bomb

**(h)** = health

**(p)** = petrol

## Update Log (Simple)
**5/2/17; Paul A. Fischer**

-Basic controls, game over screen


**5/3/17; Paul A. Fischer**

-Created smooth ground, health, gas, out of gas, player specific controls, main menu page, credits page, settings page, commented, setting up setting infastructure, gas regeneration, bullet icon, tank icon, fixed movement, fixed health, added vertical check for bomb, added draw option, added winner to game, changed start angle, removed bullet tail, created winner, changes to .hpp's, started debugging menu, got petrol/health toggles to change, ground destruction

**5/4/17; Paul A. Fischer**

-Hard Reset to previous nights code

-Fixed ground bugs, tweaked groumd, fixed game over loop, added color to settings, fixed mainmenu, started infastructure for nicknames


**5/5/17; Paul A. Fischer**

-tank color, implamented color toggle, cleaned code, comments, hid cursor on all menus, nicknames finished, fixed bugs, added loop for credits, tweaked settings, tweaked ground spawn, added score, fixed gameover delay, created player score


**5/6/17; Paul A. Fischer**

-Fixed gameover bug, added pointshop, cleaned code, bugs and comments, changed points, recoded gasoline, implemented turn skip, pointshop working, recoded all health functions, ps tweaks, fixed gas refill bug, created way to tie/draw, created bombs in ps, added color to bombs, added bomb indicator


**5/7/17; Paul A. Fischer**

-Added toggle for wind, added toggle for terrain, reworked settings, added way to give yourself credits, aadded menu for updates, added wind infastructure, fixed nickname updating bug, changed hit detection, changed player spawns, created map selection, changed bullet/tank positioning, expanded debugging menu, large bomb radius added, added wind button, bug fixes, implemented working wind, improved debug feature


**5/8/17; Paul A. Fischer**

-Filled ground, added choice to change ground display, tweaked lives, movement, and ground class, recoded ground spawning, added movement limiter, updated log, removed 4 bugs, fixed bug to drive off screen, increated tank limits, created direct impact for tanks, fixed pointshop glitch, fixed wind indicator on debugging screen, added buttered toast, bug fixes, log change




