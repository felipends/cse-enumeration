# cse-enumeration
Enumerates all possible instances for an given number of slots for the CSE problem.

Usage:

> This project depends on google OR-Tools.

Build the project:

```bash
mkdir build && cd build
cmake ..
make
```

Run it passing the number of slots (e.g., 6):

```bash
./enumeration 6
``` 

The result should include something like:

```latex
Instances for 5 professors % Identifier of instances' type
0 % Objective function value
3 4 5 % Professors allocated to this slot
3 4 5 
3 4 5 
1 2 5 
1 2 5 
1 2 5 
```