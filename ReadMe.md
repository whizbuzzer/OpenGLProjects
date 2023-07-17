# OpenGL implementations

This project serves as my exposure to the [OpenGL framework](https://www.opengl.org/) in C++, and simultaneously to [GLFWG](https://www.glfw.org/) and [glad](https://glad.dav1d.de/).  of these libraries have already been loaded into this project, so that saves you the hassle of installing them.

---
## System info
Type | Info
---|---
Operating System | Windows 11 Pro
Processor | 11th Gen Intel(R) Core(TM) i5-1135G7 @ 2.40GHz   2.42 GHz
Graphics card | NVIDIA® T500 4GB GDDR6
Installed RAM | 16.0 GB (15.7 GB usable)
System type | 64-bit operating system, x64-based processor
Pen and touch | No pen or touch input is available for this display



---
## GLFW (Graphics Library FrameWork)

- GLFW provides us with the bare necessities required for rendering objects to the screen. OpenGL itself does not have rendering functionalities.

---
## GLAD

- [GLAD](https://www.khronos.org/opengl/wiki/OpenGL_Loading_Library#glad_(Multi-Language_GL/GLES/EGL/GLX/WGL_Loader-Generator)) is a [Loader-Generator](https://github.com/Dav1dde/glad) for the OpenGL framework.

- There are multiple versions of OpenGL. The location of most of its functions is not known at compile-time and needs to be queried at run-time. GLFW helps us at compile-time only. This is where glad comes into the picture. GLAD manages function pointers for OpenGL (OpenGL LOVES using pointers and references for its methods)

- Created the Triforce! 

![Triforce](./data/Triforce.png)

---
## GLM

- [GLM](https://glm.g-truc.net/0.9.9/index.html) is a mathematics library specifically made for mathematical operations in OpenGL projects, mainly stuff such Linear algebraic operation.

- Send in the memes!

![SadPepe](./data/spp_rotating.gif)