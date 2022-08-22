# PageReplacementSim
### Description: 
This is a terminal program written in C++ made to simulate three page replacement algorithms; first in first out, random, and least recently used replacement.
when executing the program as it goes through the three algorithms the program also collects the data of the the number of page faults and page replacements
aswell as the time taken to execute a given algorithm. <br/><br/>
*Note: This program does not simulate the secondary memory along with the virtual memory, only the primary memory* <br/>
### How to run:
***Important, the executing command requires two integer values from user first int, page size must be a number of power of 2
between 256 (2^8) and 8192 (2^13) bytes inclusively. Second int the physical memory size is also a
power of 2 between 4 MB (2^22) and 64 MB (2^26). Examples shown below.*** <br/><br/>
**Windows:** <br/>
&nbsp;To Compile: <br/>
``` .\build.bat ```<br/>
&nbsp;To Run: <br/>
```.\pagetable.exe 256 4```<br/>
**Linux:** <br/>
&nbsp;To Compile: <br/>
```make```<br/>
&nbsp;To Run: <br/>
```./PageReplacementSim 256 4```<br/>
