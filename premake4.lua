newoption
{
    trigger = 'tests',
    description = 'Build with unit tests',
}

newoption
{
    trigger = 'benchmark',
    description = 'Build with benchmars',
}

newoption
{
    trigger = 'static',
    description = 'Build static library'
}

newoption
{
    trigger = 'docs',
    description = 'Build library documentation'
}

solution 'metasystem'
    configurations { 'Release', 'Debug' }
    location 'build'
    
    project 'umof'
        targetname 'umof'
        targetdir 'lib'
        language 'C++'
        
        if _OPTIONS['static'] then
            kind 'StaticLib'
            defines { 'UMOF_STATIC' }
        else
            kind 'SharedLib'
        end
        
        files
        {
            'include/**.h',
            'src/**.cpp'
        }
        
        includedirs { "include" }
        defines { 'UMOF_LIBRARY' }
        
        if _OPTIONS['docs'] then
            postbuildcommands { 'doxygen ../doxyfile' }
        end
        
        configuration 'Debug'
            targetsuffix 'd'
            defines { '_DEBUG' }
            flags { 'Symbols' }
            
        configuration 'Release'
            defines { 'NDEBUG' }
            flags { 'Optimize' }
            
        configuration 'gmake'
            buildoptions { '-std=c++11' }

    if _OPTIONS['tests'] then
        project 'test'
            targetname 'umof-tests'
            targetdir 'bin'
            language 'C++'
            kind 'ConsoleApp'
            
            if _OPTIONS['static'] then
                defines { 'UMOF_STATIC' }
            end
            
            files
            {
                'test/unit/**.h',
                'test/unit/**.cpp'
            }
            
            libdirs { 'lib' }
            includedirs { 'include' }
            
            configuration 'Debug'
                targetsuffix 'd'
                links { 'umofd' }
                defines { '_DEBUG' }
                flags { 'Symbols' }
            
            configuration 'Release'
                links { 'umof' }
                defines { 'NDEBUG' }
                flags { 'Optimize' }
                
            configuration 'gmake'
                buildoptions { '-std=c++11' }
            
    end
    
    if _OPTIONS['benchmark'] then
        project 'benchmark'
            targetdir 'bin'
            targetname 'umof-benchmark'
            language 'C++'
            kind 'ConsoleApp'
            
            if _OPTIONS['static'] then
                defines { 'UMOF_STATIC' }
            end
            
            files { 'test/bench/**.cpp' }
            links { 'umof', 'cpgf' }
            includedirs { 'include' }
            
            configuration 'Debug'
                links { 'celerod', 'campd', 'qt5cored' }
                targetsuffix 'd'
                defines { '_DEBUG' }
                flags { 'Symbols' }
            
            configuration 'Release'
                links { 'celero', 'camp', 'qt5core' }
                defines { 'NDEBUG' }
                flags { 'Optimize' }
                
            configuration 'gmake'
                buildoptions { '-std=c++11' }
    end
    
    
    