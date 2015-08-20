What is this?
----
An opinionated c++11 game engine. Evolved from the internal game engine used by Ironclad Games (Sins of a Solar Empire, Sins of a Dark Age)

Used by
---
- https://github.com/jseward/solar_tools
- https://github.com/jseward/peon
- secret project(s)

Features
---
- consistent archiving system that supports reading/writing to binary/json/packets/etc.
- platform abstraction of everything (ex. file io, mouse input, keyboard input)
- directory change watching. all data files support being reloaded on the fly.
- flexible setting system for user settings
- text localization.
- scalable fonts.
- rendering meshes with shaders.
- window ui layer with support for common controls: buttons, text entry, etc. 

Internals
----
- no global variables and no singletons.
- uses c++11.
- uses stl containers mostly, but provides some custom containers useful for games.
	- fixed_vector (std::vector with a fixed size)
	- linked_list (custom list class where memory allocation is controlled much easier)
- minimal dependencies (see below)

Platforms supported
---
- Direct3D9
- Win32
- more to come

Coding Style
------------
- picked to conform to std library and c++ in general
	- all types and variables lowercase with underscores
	- macros and enum values all uppercase with underscores
	- template parameters are mixed case
	- open brackets start on same line
	
Dependencies
---
- https://github.com/cppformat/cppformat
- https://github.com/miloyip/rapidjson
- directx9 sdk
- bm_font

How to use?
---
Easiest to look at peon and solar_tools for examples. Add the appropriate _buildbuild_x files to your project that will build solar into your app.

String Formatting
-----------------
- use https://github.com/cppformat/cppformat internally
- python format syntax
    https://docs.python.org/2/library/string.html#formatstrings

Tips and Tricks
------------
- containers/container_helpers.h has lots of helpful one line functions for doing common container ops.
- convert a container (ex. std::vector<int>) into a comma seperated std::string using build_container_string() found in strings/string_build.h

Preprocessor macros
--------------------
	SOLAR__JSON_ARCHIVE_READER_NO_ALERT_UNUSED_VALUES
	disables alerts that json values read from the file are being unused by the code. this is potentially expensive due to all the string tracking.


