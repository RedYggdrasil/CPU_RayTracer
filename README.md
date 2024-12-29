# CPU_RayTracer
A naïve **ray tracer** running on the CPU, using a single thread.

This raytracer was made following the ["_Ray Tracing in one weekdend_" Book 1](https://raytracing.github.io/books/RayTracingInOneWeekend.html). **CMake** is used to build the _Visual Studio_ solution.

The used coordiante system is Left Handed 
- X Foward
- Y Right
- Z Up

The maths are done using Microsoft's **DirectXMath** library, this means I am using _XMVECTOR_ and _XMMATRIX_ structs witch are **SIMD** accelerated.

[Tracy](https://github.com/wolfpld/tracy) (version 0.10) is used tokeep track of computation times

<div align="center">
  <img src="https://quentin-noguier.fr/Curriculum_Vitae/images/CPU_Raytracer/Result1080.png" alt="Link to resulting rendered image" style="width: 90%;">
</div>


## CPU Raytracer Branches
CPU Raytracer contains two  branches.

- ```main``` is the main initial developpement branch.

- ```ray_not_loaded_recursive``` is a working in progress branch to track SIMD related performance issues, this should be ignored.


## Build the CPU Raytracer from sources
To build the CPU Raytracer from these sources, you need to : 

0. Git clone the complete solution.
1. Generate the visual studio solution.
2. Build the project.
3. Copy the _Assets_ folder in the build directory.


## Build Redsap Rush from sources
To build Redsap Rush from these sources, you need to : 

0. Git clone the complete solution.
1. Generate the visual studio solution.
2. Build the project.

### 0. Git clone the complete solution
**_In addition to DirectXMath, CPU Raytracer depend on [Tracy](https://github.com/wolfpld/tracy) for profiling._**\
Tracy is included into the visual studio project and built with _CPU Raytracer_. **_To ensure your project is valid. please git clone recursively, as tracy is linked via git submodule._**

### 1. Generate the visual studio solution
Generating the visual studio solution can be by either :
#### 1.a. Using the .bat files
Simply run **RegenerateAll.bat** at the project root. this will automatically run _CleanBuild.bat_ and then _Generate_CMake.bat_.

#### 1.b. Running Cmake in command line
Write these lines in your CLI with the current context being the project's root directory.
```
mkdir "Build"
cd Build
cmake ..
```

### 2. Build the project
Run the newly created ```./Build/CPU_RayTracer.sln``` and build the **App** project in **Debug** or **Release** for the **x64** platform.
