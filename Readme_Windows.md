# CG Projects Readme (Windows)



## Prerequisites

1. Install **MS Visual Studio 2019 Community Edition** and make sure you select the **Desktop develoment with C++** option. Download from here:<br>
https://visualstudio.microsoft.com/vs/ 
2. Download and install **CMake** from here, but make sure you download a **stable release**:<br>
https://cmake.org/download/
<br>

## Project Build:
#### Step 1: CMake Configuration
- Start CMake GUI and select the **CG_Projects** folder as the *SOURCE CODE* directory.
- Create a second build folder and select it as the *BUILD BINARIES* directory.
- Press the **Configure Button** to select the desired IDE code generator --> **Visual Studio 16 2019** and plattform **x64**.
- Check for any configure errors (in red) and solve them :-)
- Press the **Generate Button** to generate the selected IDE code.
- If needed, you can change some CMake define values (in red). This is usually not necessary.
- Press the **Generate Button** as many times as needed, until all CMake define values turn black.


#### Step 2: Visual Studio Build
- Open the Visual Studio solution file **CG_Projects.sln**, either by navigating to the *BUILD BINARIES* directory or by pressing the **Open Project Button** in the CMake GUI.
- In Visual Studio use the **Build Solution Menu** to build all CG sub-projects.
- To build/start just a single project, select the **Set As Startup Project** item from the right mouse button pop-up menu.
  