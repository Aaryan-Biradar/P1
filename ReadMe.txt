Project 1: CUinSPACE Rocket Status

Authors: Aaryan Biradar, Anushka Tankala Student Number(s): 101340583, 101339108

Description: This program manages a collection of rocket subsystems, allowing users to add, modify, filter, and monitor the status of various subsystems. Each subsystem's status is stored as a compacted byte, with specific bits representing power, data, activity, error, performance, and resource levels. The program supports bitwise operations for efficient status management and includes a console-based menu for user interaction.

Compilation: To compile the program, use the following command in your terminal:

gcc -o rocket_status main.c subsys.c subsys_collection.c

./rocket_status

Credits:

Developed by Aaryan Biradar (101340583), Anushka Tankala (101339108)
Referenced the following resources for assistance:
Bitwise operations: https://www.geeksforgeeks.org/bitwise-operators-in-c-cpp/
String manipulation: https://cplusplus.com/reference/cstring/