#ifndef _FILE_HANDLER_HPP 
#define _FILE_HANDLER_HPP 

// standard headers 
#include <Windows.h> 

#include <cstdio> 
#include <cstdlib> 
#include <iostream> 

#include <sys/types.h>
#include <sys/stat.h>
#include <string.h> 
#include <errno.h>
#include <stdarg.h>

class FileHandler 
{
    private: 
        FILE* filePtr; 

    public: 
        FileHandler(const char* filePath, const char* mode); 
        ~FileHandler(); 

        void log(const char* fmt, ...); 
        
        static char* fileToString(const char* filePath); 
        static char* readBinaryFile(const char* pFilename, int& size); 
}; 

#endif /* _FILE_HENDLER_HPP */ 
