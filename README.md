# WeekendRaytracer
### A Raytracer written in C99 based on the book "Ray Tracing in One Weekend" by Peter Shirley

![Test Render](https://github.com/astrand130/Weekend-Raytracer/blob/master/_media/Media1.png?raw=true)

Check https://github.com/astrand130/Weekend-Raytracer/releases for pre-compiled builds

Yes... this is taking me more than a weekend... There are bugs and I'm not good at math...

## How to Build
1. Clone this repository to your' PC
2. Open the local folder with CMake as the source directory
3. Check/Uncheck optional features and provide a working path to their dependencies
4. Generate the project with your' IDE/Compiler of choice
5. Compile the generated project
6. Marvel at a static image of spheres
7. Report the inevitable bugs to the Github Issues page
8. Watch in awe as nothing gets fixed

## Pre-Requisites
* A C99 Compatible Compiler (at-least recent MSVC's level of support)
* OpenMP support (Optional)
* A Image Viewer that can open and is associated with .HDR images by default (Recomended)
* Open Image Denoise https://github.com/OpenImageDenoise/oidn (Optional)

## Progress
- [X] Write Image
- [X] Vector Math
- [X] Rays
- [X] Multi-threading
- [X] Spheres
- [X] Generic Shape
- [X] Scenes
- [ ] Plane  (Needs fixing)
- [X] Anti-aliasing
- [X] Diffuse
- [X] Generic Material
- [X] Metal
- [ ] Glass (Needs fixing)
- [X] Emission
- [X] Denoiser Implimentation
- [X] Tone-mapping
- [ ] Dynamic Camera (Needs fixing)
- [ ] Defocus
- [ ] Random Scene
- [ ] Scene Loader
- [ ] Scene Creator
- [ ] Triangle Meshes
- [ ] Textures

## Known issues
* Planes don't appear outside of indirect lighting... for some reason
* Glasss lacks fresnel... and pretty much all physical acuracy
* Dynamic camera yeets into another plane of existence
* Has only been tested on Windows x64 with MSVC

## Original Book/Author
https://twitter.com/Peter_shirley

https://www.amazon.com/Ray-Tracing-Weekend-Minibooks-Book-ebook/dp/B01B5AODD8
