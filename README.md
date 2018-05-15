![logo](logo.png?raw=true "logo")

# rasterix
*The swiss army knife for geospatial raster data.*


Rasterix is a **cross-platform** utility built around the **GDAL** library and the **Qt** framework designed to process geospatial raster data. It can perform most of the tasks already implemented in various GDAL command line utilities, but using a friendly graphical user interface.

Its plugin based architecture makes it extensible and future proof.

At the moment, rasterix can perform the following tasks:

   *   **Display** raster files with pan and zoom capabilities;
   *   **Convert** files to different formats;
   *   **Resize** rasters while preserving georeferenced metadata;
   *   **Reproject** rasters (with warp) to different projections / coordinate systems;
   *   Extract **Contour maps** from raster bands;
   *   Override **no data** values;
   *   Compute **height maps** with different methods;
   
More plugins / capabilities will be implemented in the future, so please stay tuned.

![Imgur](https://i.imgur.com/fOzbeGr.jpg)

# Downloads
Vist the Releases section [here](https://github.com/mogasw/rasterix/releases/) for pre-built binary packages for Windows (.zip), Linux (.AppImage) and macOS (.dmg). These are all 64 bit binaries.

# Compilation instructions

## Compilers
Rasterix has been compiled successfully with GCC 7+, MinGW-64 5, and MSVC 2017. Other compilers / versions *may* work but were not tested by us.

## Dependencies
   *   **GDAL** version 2.2.x or newer and its dependencies;
   *   **Qt** version 5.9 or newer;
   
The project is based on the **qmake** system. A .pro file is provided so that the whole project can be compiled using **Qt Creator**.

Before attempting to compile, some configuration is required. In particular, please review the file **gdal.pri** to ensure the GDAL library path is correctly set for your compiler.

