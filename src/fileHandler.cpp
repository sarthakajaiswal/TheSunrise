#include "../headers/fileHandler.hpp" 
#include "../headers/global_header.hpp" // this should be here itself not in fileHandler.hpp to avoid recursive inclusion 

void uninitialize(void); 

FileHandler::FileHandler(const char* filePath, const char* mode) 
{
    this->filePtr = fopen(filePath, mode); 
} 

FileHandler::~FileHandler() 
{
    fclose(this->filePtr); 
    this->filePtr = NULL; 
} 

// TODO: HOW IT WORKS ? 
void FileHandler::log(const char* fmt, ...) 
{
    va_list args; 
    
    va_start(args, fmt); 
    vfprintf(filePtr, fmt, args); 
    fflush(filePtr); 
    va_end(args); 
} 

// TODO: write your own function 
char* FileHandler::fileToString(const char* filePath)
{
    FILE* filePointer = fopen(filePath, "r");
    if (!filePointer)
    {
        MessageBox(NULL, TEXT("Failed to open file"), TEXT("File Error"), MB_OK | MB_ICONERROR);
        uninitialize();
        return (NULL);
    }

    int size = 512;
    int index = 0;

    char* str = (char*)malloc(size);
    if (!str)
    {
        MessageBox(NULL, TEXT("Memory allocation failed"), TEXT("Memory Error"), MB_OK | MB_ICONERROR);
        uninitialize();
        return (NULL);
    }

    int c;
    while ((c = getc(filePointer)) != EOF)
    {
        if (index >= size - 1)
        {
            size += 512;
            char* temp = (char*)realloc(str, size);
            if (!temp)
            {
                free(str);
                MessageBox(NULL, TEXT("Memory allocation failed"), TEXT("Memory Error"), MB_OK | MB_ICONERROR);
                uninitialize();
                return (NULL);
            }
            str = temp;
        }

        str[index++] = (char)c;
    }

    str[index] = '\0';
    fclose(filePointer);

    return (str);
}

// TODO: write your own function 
char* FileHandler::readBinaryFile(const char* pFilename, int& size)
{
    FILE* f = NULL;

    errno_t err = fopen_s(&f, pFilename, "rb");

    if (!f) {
        char buf[256] = { 0 };
        strerror_s(buf, sizeof(buf), err);
        logFile.log("Error opening '%s': %s\n", pFilename, buf);
        exit(0);
    }

    struct stat stat_buf;
    int error = stat(pFilename, &stat_buf);

    if (error) {
        char buf[256] = { 0 };
        strerror_s(buf, sizeof(buf), err);
        logFile.log("Error getting file stats: %s\n", buf);
        return NULL;
    }

    size = stat_buf.st_size;

    char* p = (char*)malloc(size);
    assert(p);

    size_t bytes_read = fread(p, 1, size, f);

    if (bytes_read != size) {
        char buf[256] = { 0 };
        strerror_s(buf, sizeof(buf), err);
        logFile.log("Read file error file: %s\n", buf);
        exit(0);
    }

    fclose(f);

    return p;
}
