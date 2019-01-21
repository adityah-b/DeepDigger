# DeepDigger
A Real-Time Single Player Arcade Game Developed for an ARM LPC1768 Microcontroller. 

Click the following link to see a demo:
https://www.youtube.com/watch?v=0iLFEUAH8IU&feature=share

## Overview
In Deep Digger, players are deployed onto a foreign planet where they are tasked with exploring the terrain for minerals. Using a robot, players can drill through the surface to collect minerals, earning points depending on the rarity of the mineral collected. To beat the game, the player must accumulate a certain quantity of minerals all the while not exceeding the assigned spending limit and keeping the fuel tank from depleting completely. When the game ends, the display will be cleared and will output a message depending on whether the player died due to an empty fuel tank or was able to collect all the required quantities of minerals.

## Control of I/O and Peripheral Devices
In Deep Digger, players can use the joystick to traverse through the map and will automatically collect minerals if its position matches that of the mineral’s. Based on user input from the joystick, the robot can move/dig along the left, right, and down directions - provided that the terrain beneath it is solid, and the terrain ahead of it contains either dirt or a mineral. The LEDs will be used to indicate the fuel status of the robot, with all LEDs ON indicating a full tank. The INT0 push button will be used as a toggle switch allowing the robot to move upwards to reach the surface and refill its tank. To refill the tank, the joystick must be pressed when the robot is on the pixel denoted by ‘F’, decrementing the player’s points tally based on the amount of fuel purchased. 

## Tasks
The game is implemented using 4 tasks of equal priority (updateDisplay, moveRobot, updateFuelStatus, and buyFuel) in addition to the Main Task. The updateDisplay task will loop through the 2D char array and set the colour and character of each pixel based on the contents of the respective array element. It then waits until an action has been performed (signaled using a semaphore) before redrawing the two pixels which changed. The moveRobot task waits for updateDisplay to signal that the screen has been refreshed before polling the joystick to determine if the user wants to move the robot. The updateFuelStatus and buyFuel tasks are interdependent as well since they rely on the other to signal completion before they can run. This mutual exclusion is also enforced using semaphores. 
