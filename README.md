# WeekendRaytracer
![Test Render](https://github.com/astrand130/Weekend-Raytracer/blob/master/_media/Media1.png?raw=true)

Check https://github.com/astrand130/Weekend-Raytracer/releases for pre-compiled builds

## How to Build
1. Clone this repository to your' PC
2. Open the local folder with CMake as the source directory
3. Check/Uncheck optional features and provide a working path to their dependencies
4. Generate the project with your' IDE/Compiler of choice
5. Compile the Generated project

## Dependencies/Requirements
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
- [X] Denoiser Implimentation
- [ ] Dynamic Camera (Needs fixing)
- [ ] Defocus
- [ ] Random Scene
- [ ] Scene Loader
- [ ] Scene Creator

## Known issues
* Planes don't appear outside indirect lighting
* Glasss lacks fresnel
* Dynamic camera has math issues
* Has only been tested on Windows x64 with MSVC
