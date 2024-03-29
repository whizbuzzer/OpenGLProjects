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
## #pragma once vs #ifndef

- Both of these are __include guards__; the prevent duplicate declarations and make sure that the header file is compiled only once.

- using #ifndef makes the compiler check if a unique value (in this case HEADERFILE_H) is defined. Then if it's not defined, it defines it and continues to the rest of the page. When the code is included again, the first ifndef fails, resulting in a blank file. This prevents double declaration of any identifiers such as types, enums and static variables.

- [Further information](https://en.wikipedia.org/wiki/Include_guard)

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

- Layouts, inputs, outputs should be declared in the __SAME ORDER__ in all shaders!!! Although not mandatory, it will prevent you from pulling out your hair

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

- **Mipmap** is a collection of copies of the original texture which are subsequently twice as small than the last one. The concept behind it is that the farther an object goes, the less noticeable its texture becomes. So, using lower resolution textures on faraway objects not only does not affect much the viewing experience, but also prevents production of visible artifacts on faraway objects as well as saves memory.

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

---
## Lighting

- Basic lighting in OpenGL comes in [5 types](https://learnopengl.com/Lighting/Basic-Lighting):
    1. **Ambient:**
        * Even when a surface is not facing a light source, it will still have some color due to light reflecting off other surfaces
        * Algorithms used to approximately simulate this "light scattering" are called "global illumination algorithms
        * Global illumination algorithms however are complicated and expensive to compute
        * We can therefore use Ambient lighting instead to simulate light scattering in a much simpler manner by just multiplying an ambient constant with the color of that surface
    2. **Diffuse:**
        * In this lighting, intensity of a color on a surface is inversely proportional to the angle formed between the light source and the surface.
        * To calculate this angle, we need the position of the light source and the a way to know the slope of the surface
        * Traditionally, slope of the surface is represented by a normal vector
        * Normal vectors are unit vectors of length 1 that help us calculate how light should act on a certain surface
        * With a light position and a normal, we can take the dot product between the two to get an "intensity" scaling factor with which we can control the light intensity. This is because dot product would represent the cosine of the light source-surface angle
        * Normals come in 2 main types. Choosing on depends on your mesh and art style:
        1. **Face normals:** 
            * These are perpendicular to the face/surface of an object and defines the direction of that face
            * They provide "flat shading" (blockier/more blocky look)
        2. **Vertex normals:**
            * These are normals which poke out of vertices and define how virtual light will bounce off a mesh
            * They define a smoothing for the underlying mesh
            * They are perpendicular to the plane created by all vertices adjacent to the vertex that they poke out of
    3. **Specular:**
        * This lighting uses [Phong Shading](https://en.wikipedia.org/wiki/Phong_shading)
        * This lighting is similar to diffuse in the sense that a "gradient" of lighting is formed on the object surfaces
        * What makes this lighting different from diffuse is that it also considers reflective properties of the object surfaces and the view direction (direction from which we look at the object)

---
## Lighting Maps

- Just like lighting types, [lighting maps](https://learnopengl.com/Lighting/Lighting-maps) come in 3 types:
    1. **Diffuse map:**
        * Used to highlight base material colors of the texture
    2. **Specular map:**
        * Used to control reflectivity of an object's surface
        * Specular maps are black and white textures which tell a program how and where specular light should appear on a texture and how strong it should be at that point; lighter a pixel on a specular map is, the more light you will get at that point, and the darker a pixel on a specular map is, the less light you will get at that point.

---
## Types of Light Sources

- In real life, if $$lightIntensity=i$$ and $$distanceFromLightSource = d$$ then we have $$i = {1 \over d ^ 2}$$ This is also called the _inverse square law_

- In computer graphics though, we have more complicated equations to better control the properties of light e.g., [Phong Shading](https://en.wikipedia.org/wiki/Phong_shading) and [Goraud Shading](https://en.wikipedia.org/wiki/Gouraud_shading)

- The equation used in this project is $$i = {1 \over a \cdot d^2 + b \cdot d + 1}$$ The quadratic term `a` seems to control the light intensity closest to the light source and the linear term `b` seems to control the darkness intensity further away from the light source

- [Light sources](https://learnopengl.com/Lighting/Light-casters) come in 3 main types:
    1. **Point:**
        * Illuminates the environment omnidirectionally/in all directions
        * Light intensity however gradually decreases/withers with distance
    2. **Directional:**
        * Easiest of the three lightings to implement
        * Showers the object in parellel light rays
        * Mimics a light source infinitely far away. The further a light source goes from the object, the lesser the adjacent light rays coming from it seem to diverge from each other, and at infinity, the adjacent light rays would be perfectly parellel to each other
    3. **Spot:**
        * Only lights a conic area like a flashlight/disc lamp
        * It is defined by an inner angle and an outer angle. Inner angle controls central light intensity while outer angle controls border darkness intensity. Together, they give the lighting a nice gradient. Using only one of them makes the light look like a cut-out
        * Always have the outer angle be greater than the inner angle otherwise the lighting will cast a shadow over where the light is supposed to be and light where the shadow is supposed to be

---
## Mesh class

- Mesh in OpenGL is a data structure/set which contains generally 3 properties:
    1. Vertices
    2. Indices of those vertices
    3. Textures

---
## Model Loading

- `.png` and `.jpg` files are usually the standard when it comes to loading images in the Graphics industry, but when it come to 3D models, we have multiple different file formats to use/choose from, many of which are proprietary, such as:
    1. `.stl`
    2. `.obj`
    3. `.fbx`
    4. `.dae`
    5. `.3ds`
    6. `.iges`
    7. `.x3d`
    8. `.3mf`
    9. `.step`
    10. `.igs`
    11. `.stp`
    12. `.vrml`
    13. `.blend`

- For our project, we will use `.gltf` file format as it is made by the __KHRONOS Group__, who made OpenGL

- [__GLTF__](https://godotengine.org/article/we-should-all-use-gltf-20-export-3d-assets-game-engines/) uses the [__JSON__](https://github.com/nlohmann/json) file structure

- JSON (Javasript Object Notation) files are configuration files that work like dictionaries within dictionaries

- A simplified view of one of the main branches of the JSON tree and the one we care the most about:
    1. **buffers:**
        * Present at the top, it stores data
        * Parent of *bufferViews*
    2. **bufferViews:**
        * Child of *buffer*
        * Tells us which parts of *buffer* we should read i.e. points to the location where data of interest(DOI) is present
        * Parent of *accessors*
    3. **accessors:**
        * Tells us what datatypes we should look for in the location pointed by *bufferViews* e.g. GLuint or vec3
        * vec3 though is the datatype of vertices as well as normals
        * Parent of *attributes*
    4. **attributes:**
        * Tells us which *accessors* are part of which vertex attributes aka positions, normals, texCoords, etc.
    5. **primitives:**
        * Locates accessors of indices of our mesh as indices are crucial to constructing our mesh
        * Parent of *attributes* and *materials*
    6. **materials:**
        * Parent of textures. Not relevant when materials alrady have PBR (Physics Based Rendering) data, which they usually do
    7. **meshes:**
        * Parent of *primitives*
    8. **nodes:**
        * Parent of *meshes*
    9. **scenes:**
        * The setup of the entire environment with models, lighting and everything
        * Parent of *nodes*

- Expressing model rotations as quaternions helps avoid gimbal lock. Quaternions are represented as: $$a + b \cdot i + c \cdot j + d \cdot k$$ Where `a, b, c and d` are real numbers and `i, j and k` are basis vectors/elements which are imaginary numbers such that $$i^2 = j^2 = k^2 = -1$$

