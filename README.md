# CPU_RayTracer
Implementing a naive ray tracer running on the CPU

I am currently following along the "Ray Tracing in one weekdend" tutoral : 
https://raytracing.github.io/books/RayTracingInOneWeekend.html

Notes :

CMake is used to build the Visual Studio solution (so windows project).

The used coordiante system is Left Handed 
X Foward
Y Right
Z Up

The maths are done using Microsoft's DirectXMath library, this means I am using XMVECTOR and XMMATRIX structs witch are SIMD accelerated.

Tracy (version 10) is used tokeep track of computation times


