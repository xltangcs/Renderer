-- premake5.lua
workspace "Renderer"
   architecture "x64"
   configurations { "Debug", "Release", "Dist" }
   startproject "Renderer"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

include "External.lua"
include "Renderer"