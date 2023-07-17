# Important Notes!

- OpenGL cannot be used on its own. OpenGL does not know anything about windows, mouse events, key presses, fonts, etc. All it knows is rendering 2D and 3D graphics. So, to do anything/work with OpenGL (i.e. create an OpenGL context), you need another library. This is where GLFW (Graphics Library FrameWork) library comes into the picture. GLFW creates those contexts to work with OpenGL as required.

- If OpenGL is fire, then GLFW is a torch to carry that fire and use it as it suites you.

- OpenGL context is an instance/state of the OpenGL API

- OpenGL profile is a mode to operate in. "Core" mode means only modern methods/methods of current version will be used. "Compatibility" mode means that methods of current as well as past versions will be used.

- Add the suffix "f" after every float value (e.g. `1.0f`) so that the compiler does not mistake it for a double.

---
## Glad (OpenGL Loader-Generator):

- When working with OpenGL, the developer needs to retrieve "function pointers" for each OpenGL function from the graphics driver at runtime. This process can be platform-specific and cumbersome, involving various platform-specific APIs and extensions, if it were to be done manually. Glad streamlines this process by simplifying code and reducing platform-specific dependencies, allowing developers to focus on the actual OpenGL rendering logic.

---
## Graphics Pipeline:

- Vertex data[] -> Vertex Shader -> Shape Assembly -> Geometry Shader -> Rasterization -> Fragment Shader -> Tests and blending
https://www.khronos.org/opengl/wiki/Rendering_Pipeline_Overview

- Sending data between CPU and GPU is slow, so you have to send it in big batches called "Buffers" (not to be confused between front and back buffers)

- Binding in OpenGL is a way to make a certain object the current object (kind of like turning an object global); whenever a function modifies the binding, the bound object gets modified (probably because OpenGL stuff LOVES using references)

---
## Shaders and GLSL (OpenGL Shading Language):

- Shaders are small isolated programs that rest on the GPU.

- Shaders are run for each specific section of the graphics pipeline.

- Shaders are not allowed to communicate with each other and only communicate with their inputs and outputs

- Shaders are user-defined programs designed to run on some stage of a graphics processor. Shaders provide certain code for certain programmable stages of the rendering pipeline. They run on the GPU. They can also be used in a slightly more limited form, on-GPU computation.
https://www.khronos.org/opengl/wiki/Shader

- Shaders tell the GPU what to do with the data that we send in. Sadly, OpenGL does not provide defaults for Vertex and Fragment Shaders, so you gotta write your own. Two most commonly used shaders are *Vertex* and *Fragment/Pixel* shaders.

Vertex | Fragment/Pixel
---|---
Corner points of your polygon | Pixels generated between corner points of a polygon
Operates on [data pertaning to vertices](https://www.khronos.org/opengl/wiki/Vertex_Shader#Inputs) | Operates on [data pertaining to pixels](https://www.khronos.org/opengl/wiki/Fragment_Shader)
Will create only the wireframe | Will fill up the faces of the wireframe object created with vertices and generate a proper polygon
Provides input to the fragment shader to fill the faces with pixels | Takes input from vertex shader to fill the faces with pixels

- Shaders are NOT written in C. They are instead written in a C-like language called GLSL.

- GLSL is tailored for use with graphics and provides useful features specifically targeted at matrix and vector manipulation.

- [gl_position](https://registry.khronos.org/OpenGL-Refpages/gl4/html/gl_Position.xhtml)

---
## Uniform:

- This is another method of getting inputs and outputs, this time outside of a shader.

- Uniforms are global shader variables declared with `uniform` storage qualifier.

- They can be accessed even from within the main method of a program without using a VAO.

- NEVER DECLARE UNIFORMS IF YOU ARE NOT GOING TO USE THEM.

- Uniforms can be assigned values only after the shaders are activated.

- [Various methods can be used to assign values to a uniform depending on use-case](https://registry.khronos.org/OpenGL-Refpages/gl4/html/glUniform.xhtml)

---
## Usages of glBufferData:

1. STREAM: data store contents will be modified once, and used at most a few times
2. STATIC: data store contents will be modified once, and used many times
3. DYNAMIC: data store contents will be modified many times, and used many times

---
## Natures of usage of glBufferData:

1. DRAW: data store contents are modified by the application, and used as the source for GL drawing and image specifiation commands
2. READ: data store contents are modified by reading data from the GL, and used to return that data when acquired by the application
3. COPY: data store contents are modified by reading data from the GL, and used as the source for GL drawing and image specification commands (READ+DRAW)

---
## Vertex Buffer Objects (VBO), Vertex Array Objects (VAO) and Element Array Buffer Objects (EBO):

- VBO is an array of references to vertices, but if we currently have only one vertices object, then only 1 VBO is enough

- VAO is used to point OpenGL to VBOs which it cannot find by itself. It stores pointers to one or more VBOS and tells OpenGL how to interpret them. It also helps quickly switch between different VBOs

- EBO is an array of indices pointing to vertices. It used to draw more complex objects. OpenGL generates objects with triangles. EBO tells OpenGL the order in which the vertices should be processed and helps avoid creation/processing of duplicate vertices.

---
- Vertex attribute is a way of communicating with a vertex shader from the outside

## Vertex attribute pointer:

- glVertexAttribPointer() takes 5 arguments:
    1. Starting index
    2. Number of vertices
    3. OpenGL equivalent of datatype of vertex values
    4. Specifies whether fixed-point data should normalized (matters only if we have integers)
    5. Stride=amount of byte-offset data between each vertex (Data is read as an array of bytes, so offset=number of bytes occupied by a single vertex (normally equal to 3 * sizeof(datatype))
    6. Pointer to the starting index in the array in the buffer. Usually initialized as 0 of a "void pointer" datatype

---
## Void pointer:

- Declared as `void*`

- Can point to any variable that is not declared with **const** or **volatile**

- Cannot be dereferenced unless it is typecasted to another type

- Can also point to a non-member function or a static member function, but not to a non-static member function.

- Static methods need to be called with the scope resolution operator (::) instead of (.) to differentiate between static and non-static methods

---
## Textures

- Textures need to assigned width, height and number of channels

- Textures are stored in texture units which generally hold upto 16 textures

- [Read this on color channels and pixel datatypes](https://www.khronos.org/opengl/wiki/Image_Format)

---
## 3D

- OpenGL restricts coordinates to normalised coordinates. To bypass this, we can scale(contract/expand) over coordinates using matrices (Transformation matrix!)

- To achieve this, we will use the GLM (OpenGL Mathematics) Library

- [**Types of Coordinates**](https://learnopengl.com/Getting-started/Coordinate-Systems):
    1. **Local Coordinates:**
        * These are object specific. Each object is located at the origin of its local coordinate system
    2. **World Coordinates:**
        * The world coordinate system. It contains locations of other objects w.r.t. the world
        * Obtained by multiplying *local coordinates* with the *Model Matrix*
    3. **View Coordinates:**
        * This is the coordinate system of the camera
        * Obtained by multiplying *World coordinates* with the *View matrix*
    4. **Clip coordinates:**
        * These are the perspective coordinates. They delete all coordinates that fall outside the normalised range [-1.0, 1.0] and determine which coordinates will be rendered on the screen
        * Obtained by multiplying *View Coordinates* with *Projection Matrix*
    5. **Screen Coordinates:**
        * These are *Clip coordinates* transformed to the coordinate range defined by the `glViewPort`
        * These are the coordinates sent to the rasterizer to be turned into fragments
