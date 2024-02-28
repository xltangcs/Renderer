project "Renderer"  --modify
   kind "ConsoleApp"
   language "C++"
   cppdialect "C++17"
   staticruntime "off"

   files 
   { 
      "src/**.h", 
      "src/**.cpp",
   }

   includedirs
   {
      "src",
      "../vendor/imgui",
      "../vendor/glfw/include",
      "../vendor/glad/include",
      "../vendor/stb_image",
      "../vendor/glm",
   }

   links 
	{ 
		"glfw",
		"glad",
		"imgui",
      "opengl32.lib",
	}


   targetdir ("../bin/" .. outputdir .. "/%{prj.name}")
   objdir ("../bin-int/" .. outputdir .. "/%{prj.name}")

   filter "system:windows"
      systemversion "latest"
      defines { "PLATFORM_WINDOWS" }

   filter "configurations:Debug"
      defines { "DEBUG" }
      runtime "Debug"
      symbols "On"

   filter "configurations:Release"
      defines { "RELEASE" }
      runtime "Release"
      optimize "On"
      symbols "On"

   filter "configurations:Dist"
      defines { "DIST" }
      runtime "Release"
      optimize "On"
      symbols "Off"