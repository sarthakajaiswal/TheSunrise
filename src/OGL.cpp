/* 
    TODO: 
    Add count of each type and increment/decrement in their constructors
    to verify all objects created are destroyed at last 
*/ 

#include "../headers/OGL.hpp" 

#define STB_IMAGE_IMPLEMENTATION
#include "../headers/stb_image.h"

#include <al\al.h> 
#include <al\alc.h>

// ====================== GLOBAL CONSTANTS ================== 
const int WIN_WIDTH = 800; 
const int WIN_HEIGHT = 600;

// ====================== GLOBAL VARIABLES ================== 
// window dimentions 
int winWidth, winHeight; 

// full screen related 
BOOL gbFullscreen = FALSE; 
HWND ghwnd = NULL; 
DWORD dwStyle; 
WINDOWPLACEMENT wpPrev; 
UINT uMsg; 

// variables related with file io 
char gszLogFileName[] = "Log.txt"; 
FileHandler logFile(gszLogFileName, "w"); 

// active window related variables 
BOOL gbActiveWindow = FALSE; 

// exit keypress related 
BOOL gbEscapeKeyIsPressed = FALSE; 

// OpenGL related global variables 
HDC ghdc = NULL; 
HGLRC ghrc = NULL; 

// ====================== GLOBAL VARIABLES ================= 
mat4 projectionMatrix; 
mat4 viewMatrix; 

Stack<vmath::mat4> matrixStack; 

// timer related variables 
LARGE_INTEGER startTime, currentTime, freq; 
float mainTimer = 0.0f; 

// audio related variables 
ALuint audioBuffer, audioSource; 

// ====================== Scene VARIABLES ================== 
// TestScene testScene; 
IntroScene introScene; 
Scene1 scene1; 
// Scene2 scene2; 
// OutroScene outroScene; 
// TestScene testScene; 

enum Scene CurrentScene = INTRO_SCENE; 

// ==================== ENTRY-POINT FUNCTION ================ 
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow) 
{
    // local function declarations 
    int initialize(void); 
    void display(void); 
    void update(void); 
    void uninitialize(void); 
    void toggleFullscreen(void); 

    // variable declarations 
    WNDCLASSEX wndClass; 
    HWND hwnd; 
    MSG msg; 
    TCHAR szAppName[] = TEXT("RTR6"); 
    TCHAR szWindowTitle[] = TEXT("Template"); 
    BOOL bDone = FALSE; 

    // create a log file 
    logFile.log("***********************************************************\n" \
                "                       THE SUNRISE                         \n" \
                "***********************************************************\n\n"
            ); 

    // window class initialization 
    wndClass.cbSize = sizeof(WNDCLASSEX); 
    wndClass.cbClsExtra = 0; 
    wndClass.cbWndExtra = 0; 
    wndClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC; 
    wndClass.lpfnWndProc = WndProc; 
    wndClass.hInstance = hInstance; 
    wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); 
    wndClass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(ID_MYICON)); 
    wndClass.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(ID_MYICON)); 
    wndClass.hCursor = LoadCursor(NULL, IDC_ARROW); 
    wndClass.lpszClassName = szAppName; 
    wndClass.lpszMenuName = NULL; 

    // Registration of window class 
    RegisterClassEx(&wndClass); 

    // centering of window related 
    int centeredWindowX = GetSystemMetrics(SM_CXSCREEN)/2 - WIN_WIDTH/2; 
    int centeredWindowY = GetSystemMetrics(SM_CYSCREEN)/2 - WIN_HEIGHT/2; 

    // create window from registered class 
    hwnd = CreateWindowEx(
        WS_EX_APPWINDOW, 
        szAppName, 
        szWindowTitle, 
        WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE, 
        centeredWindowX, 
        centeredWindowY, 
        WIN_WIDTH, 
        WIN_HEIGHT, 
        NULL, 
        NULL, 
        hInstance, 
        NULL 
    ); 
    if(hwnd == NULL) 
    {
        logFile.log("Window creation failed\n"); 
    } 

    ghwnd = hwnd; 

    // Show window 
    ShowWindow(hwnd, iCmdShow); 

    // update window 
    UpdateWindow(hwnd); 

    // initialize 
    int result = initialize(); 
    if(result != 0) 
    {
        logFile.log("initialize() failed\n"); 
        DestroyWindow(hwnd); 
        hwnd = NULL; 
    } 
    else 
    {
        logFile.log("initialize() completed successfully\n"); 
    } 

    // set this window as foreground window 
    SetForegroundWindow(hwnd); 
    SetFocus(hwnd); 

    QueryPerformanceFrequency(&freq); 
    float one_divided_by_freq = 1.0f/(float)freq.QuadPart; 

    QueryPerformanceCounter(&startTime); 

    alSourcePlay(audioSource); 
    assert(alGetError() == AL_NO_ERROR); 

    toggleFullscreen(); 

    // game loop 
    while(bDone == FALSE) 
    {
        if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) 
        {
            if(msg.message == WM_QUIT) 
            {
                bDone = TRUE; 
            } 
            else 
            {
                TranslateMessage(&msg); 
                DispatchMessage(&msg); 
            } 
        } 
        else 
        {
            if(gbActiveWindow == TRUE) 
            {
                if(gbEscapeKeyIsPressed == TRUE) 
                {
                    bDone = TRUE; 
                } 

                // render 
                display(); 

                // update 
                update(); 

                // calculate main timer 
                QueryPerformanceCounter(&currentTime); 
                mainTimer = (float)(currentTime.QuadPart - startTime.QuadPart) * one_divided_by_freq; 
            } 
        } 
    } 

    // uninitialize 
    uninitialize(); 
    
    return ((int)msg.wParam); 
} 

// callback function 
LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) 
{
    // function declarations 
    void toggleFullscreen(void); 
    void resize(int, int); 
    void uninitialize(void); 

    extern bool doImGuiCapturedEvent; 

    // code
	if (ImGuiManager::IsEnabled() && ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam))
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
    
    switch(CurrentScene) 
    {
        case INTRO_SCENE: 
            introScene.eventCallback(hwnd, uMsg, wParam, lParam);
            break; 

        case SCENE_1: 
            scene1.eventCallback(hwnd, uMsg, wParam, lParam);
            break; 

        // case SCENE_2: 
        //     scene2.eveneCallback(hwnd, uMsg, wParam, lParam);
        //     break;
            
        // case OUTRO_SCENE: 
        //     outroScene.eveneCallback(hwnd, uMsg, wParam, lParam);
        //     break;

        // case TEST_SCENE: 
        //     testScene.eventCallback(hwnd, uMsg, wParam, lParam); 
        //     break; 
            
        default: 
            break; 
    } 

    switch(uMsg) 
    {
        case WM_CREATE: 
            ZeroMemory((void*)&wpPrev, sizeof(WINDOWPLACEMENT)); 
            wpPrev.length = sizeof(WINDOWPLACEMENT); 
            break; 

        case WM_SETFOCUS: 
            gbActiveWindow = TRUE; 
            break; 

        case WM_KILLFOCUS: 
            gbActiveWindow = FALSE; 
            break; 

        case WM_ERASEBKGND: 
            return (0); 

        case WM_SIZE: 
            resize(LOWORD(lParam), HIWORD(lParam)); 
            break; 

        case WM_KEYDOWN: 
            switch(wParam) 
            {
                case VK_ESCAPE: 
                    gbEscapeKeyIsPressed = TRUE; 
                    break; 

                default: 
                    break; 
            } 
            break; 

        case WM_CHAR: 
            switch(wParam) 
            {
                case 'F': 
                case 'f': 
                    if(gbFullscreen == FALSE) 
                    {
                        toggleFullscreen(); 
                        gbFullscreen = TRUE; 
                    } 
                    else 
                    {
                        toggleFullscreen(); 
                        gbFullscreen = FALSE; 
                    } 
                    break; 
                
                default: 
                    break; 
            } 
            break; 

        case WM_CLOSE: 
            uninitialize(); 
            break; 

        case WM_DESTROY: 
            PostQuitMessage(0); 
            break; 

        default: 
            break; 
    } 

    return (DefWindowProc(hwnd, uMsg, wParam, lParam)); 
} 

void toggleFullscreen(void) 
{
    // variable declarations 
    MONITORINFO mi; 

    // code 
    if(gbFullscreen == FALSE) 
    {
        dwStyle = GetWindowLong(ghwnd, GWL_STYLE); 
        if(dwStyle & WS_OVERLAPPEDWINDOW) 
        {
            ZeroMemory((void*)&mi, sizeof(MONITORINFO)); 
            mi.cbSize = sizeof(MONITORINFO); 

            if(GetWindowPlacement(ghwnd, &wpPrev) && GetMonitorInfo(MonitorFromWindow(ghwnd, MONITORINFOF_PRIMARY), &mi))
            {
                SetWindowLong(ghwnd, GWL_STYLE, dwStyle & ~WS_OVERLAPPEDWINDOW); 
                SetWindowPos(
                    ghwnd, 
                    HWND_TOP, 
                    mi.rcMonitor.left, 
                    mi.rcMonitor.top, 
                    mi.rcMonitor.right-mi.rcMonitor.left, 
                    mi.rcMonitor.bottom-mi.rcMonitor.top, 
                    SWP_NOZORDER | SWP_FRAMECHANGED
                ); 
            } 
        } 
     
        // ShowCursor(FALSE); 
    } 
    else 
    {
        SetWindowPlacement(ghwnd, &wpPrev); 
        SetWindowLong(ghwnd, GWL_STYLE, dwStyle | WS_OVERLAPPEDWINDOW); 
        SetWindowPos(ghwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOOWNERZORDER | SWP_FRAMECHANGED | SWP_NOZORDER); 

        ShowCursor(TRUE); 
    } 
} 

int initialize(void) 
{
    // code 
    void printGLInfo(void); 
    void resize(int, int); 
    void uninitialize(void); 
    BOOL initializeAudio(void); 

    // variable declarations 
    PIXELFORMATDESCRIPTOR pfd; 
    int iPixelFormatIndex = 0; 
    GLenum glewResult; 

    // code 
    logFile.log("INITIALIZING.....\n"); 
    // pixel format descriptor initialization 
    ZeroMemory((void*)&pfd, sizeof(PIXELFORMATDESCRIPTOR)); 
    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR); 
    pfd.nVersion = 1; 
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER; 
    pfd.iPixelType = PFD_TYPE_RGBA; 
    pfd.cColorBits = 32; 
    pfd.cRedBits = 8; 
    pfd.cGreenBits = 8; 
    pfd.cBlueBits = 8; 
    pfd.cAlphaBits = 8; 
    pfd.cDepthBits = 32; 

    // get DC 
    ghdc = GetDC(ghwnd); 
    if(ghdc == NULL) 
    {
        logFile.log("GetDC() failed\n"); 
        return (-1); 
    } 

    // get matching pixel format index using HDC and PFD 
    iPixelFormatIndex = ChoosePixelFormat(ghdc, &pfd); 
    if(iPixelFormatIndex == 0) 
    {
        logFile.log("ChoosePixelFormat() failed\n"); 
        return (-2); 
    } 

    // select the pixel format of found index 
    if(SetPixelFormat(ghdc, iPixelFormatIndex, &pfd) == FALSE) 
    {
        logFile.log("SetPixelFormat() failed\n"); 
        return (-3); 
    } 

    // Create a rendering context using HDC, PFD and chosen PixelFormatIndex
    ghrc = wglCreateContext(ghdc); 
    if(ghrc == NULL) 
    {
        logFile.log("wglCreateContext()\n"); 
        return (-4); 
    } 

    // make this rendering context as current context 
    if(wglMakeCurrent(ghdc, ghrc) == FALSE) 
    {
        logFile.log("wglMakeCurrent() failed\n"); 
        return (-5); 
    } 

    // initialize glew 
    glewResult = glewInit(); 
    if(glewResult != GLEW_OK) 
    {
        logFile.log("glewInit() failed\n"); 
        return (-6); 
    } 

    // print gl info 
    // printGLInfo();  

    introScene.initialize(); 
    scene1.initialize(); 

    // scene initialization 
    switch(CurrentScene) 
    {
        // case INTRO_SCENE: 
        //     introScene.initialize(); 
        //     break; 

        // case SCENE_1:  
        //     scene1.initialize(); 
        //     break; 

        // case SCENE_2: 
        //     scene2.initialize(); 
        //     break; 

        // case OUTRO_SCENE: 
        //     outroScene.initialize(); 
        //     break; 

        // case TEST_SCENE: 
        //     testScene.initialize(); 
        //     break; 

        default: 
            break; 
    } 

    if(initializeAudio() == FALSE)
    {
        logFile.log("Failed to initialize audio\n"); 
        return (-7); 
    } 
    else 
    {
        logFile.log("Audio Succesfully initialized\n"); 
    } 

    // depth related code 
    glClearDepth(1.0); 
    glEnable(GL_DEPTH_TEST); 
    glDepthFunc(GL_LEQUAL); 

    // tell opengl to clear the screen 
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); 

	// initialize ImGUI 
	if (ImGuiManager::IsEnabled() && !ImGuiManager::Initialize(ghwnd))
	{
		logFile.log("ImGui initialization failed\n");
		return (-1);
	}

    projectionMatrix = mat4::identity();

    // warm-up resize 
    resize(WIN_WIDTH, WIN_HEIGHT); 

    logFile.log("INITIALIZATION SUCCESSFULL.....\n");
    return (0); 
} 

void printGLInfo(void) 
{
    // varible declarations 
    GLint numExtensions, i; 

    // code 
    // printf OpenGL Information 
    logFile.log("********************** OPENGL INFORMATION *********************\n"); 
    logFile.log("OpenGL vendor: %s\n", glGetString(GL_VENDOR)); 
    logFile.log("OpenGL renderer: %s\n", glGetString(GL_RENDERER)); 
    logFile.log("OpenGL Version: %s\n", glGetString(GL_VERSION)); 
    logFile.log("GLSL version: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION)); 

    // get number of extensions 
    glGetIntegerv(GL_NUM_EXTENSIONS, &numExtensions); 
    logFile.log("Number of extensions: %d\n", numExtensions); 

    // print opengl extensions 
    for(i = 0; i < numExtensions; ++i) 
    {
        logFile.log("%s\n", glGetStringi(GL_EXTENSIONS, i)); 
    } 

    logFile.log("****************************************************************\n"); 
} 

BOOL initializeAudio(void) 
{
    // Variable decalarations
    ALCdevice *audioDevice;
    ALCcontext *alContext;

    HRSRC hResource = NULL;
    HANDLE hAudioBuffer = NULL;

    LPVOID audioBufferData = NULL;
    DWORD sizeOfAudioDataInBytes = 0;

    // Code
    // Step 1 : Open Default audio device
    // Null means default audio device is yet to be opened
    audioDevice = alcOpenDevice(NULL);
    // Checks error for ALC function call above
    if(alcGetError(audioDevice) != ALC_NO_ERROR)
    {
        logFile.log("InitializeAudio(): alcOpenDevice() Failed. \n");
        return(FALSE);

    }
    // Step 2: Create OpenAL context
    alContext = alcCreateContext(audioDevice, NULL);
    if(alcGetError(audioDevice) != ALC_NO_ERROR)
    {
        logFile.log("InitializeAudio(): alcCreateContext() Failed. \n");
        return(FALSE);
    }

    // Step 3: This is similar to wglMakeCurrent() to make the above create context as a current context
    alcMakeContextCurrent(alContext);
    if(alcGetError(audioDevice) != ALC_NO_ERROR)
    {
        logFile.log("InitializeAudio(): alcMakeContextCurrent() Failed. \n");
        return(FALSE);
    }

    // Step 4: Create Audio buffer
    alGenBuffers(1, &audioBuffer); 
    if(alGetError() != AL_NO_ERROR)
    {
        logFile.log("InitializeAudio(): alGenBuffers() Failed. \n");
        return(FALSE);
    }
    // Step : 5 Load WAV resource data
    // Load WAV Resource
    // 5A : Find Resource, & get its handle
    hResource = FindResource(NULL, MAKEINTRESOURCE(ID_MUSIC), TEXT("WAVE"));
    if(hResource == NULL)
    {
        logFile.log("InitializeAudio(): FindResource() Failed. \n");
        return(FALSE);
    }
    // 5B : Use the resource handle to load it in memory
    // And returns handle to it
    hAudioBuffer = LoadResource(NULL, hResource);
    if(hAudioBuffer == NULL)
    {
        logFile.log("InitializeAudio(): LoadResource() Failed. \n");
        return(FALSE);
    }
    
    // 5C : Get startng byte-offset of this data in memory
    audioBufferData = LockResource(hAudioBuffer);
    if(audioBufferData == NULL)
    {
        logFile.log("InitializeAudio(): LockResource() Failed. \n");
        return(FALSE);
    }

    // 5D : Get the size of audio dAata
    sizeOfAudioDataInBytes = SizeofResource(NULL, hResource);
    if(sizeOfAudioDataInBytes == 0)
    {
        logFile.log("InitializeAudio(): LockResource() Failed. \n");
        return(FALSE);
    }
    logFile.log("\nSize returned by SizeOfResource() = %d\n", sizeOfAudioDataInBytes);

    // STEP 6
    // Load WAVE data into audio buffer
    // Size in bytes, frequency of audio data in hertz
    sizeOfAudioDataInBytes = sizeOfAudioDataInBytes - (sizeOfAudioDataInBytes%4); 
    alBufferData(audioBuffer, AL_FORMAT_STEREO16, audioBufferData, sizeOfAudioDataInBytes, 44100);
    // alBufferData(audioBuffer, AL_FORMAT_MONO16, audioBufferData, sizeOfAudioDataInBytes, 44100);
    if(alGetError() != AL_NO_ERROR)
    {
        logFile.log("InitializeAudio(): alBufferData() Failed. \n");
        return(FALSE);
    }

    // STEP 7
    // Create Audio Source
    alGenSources(1, &audioSource);
    if(alGetError() != AL_NO_ERROR)
    {
        logFile.log("InitializeAudio(): alGenSources() Failed. \n");
        return(FALSE);
    }


    // Step 8
    // Give the audio buffer to audio source
    alSourcei(audioSource, AL_BUFFER, audioBuffer);
    if(alGetError() != AL_NO_ERROR)
    {
        logFile.log("InitializeAudio(): alSourcei() Failed. \n");
        return(FALSE);
    }

    // These 2 are not needed now since win32 frees the memory
    // Unload WAV resource
    // UnlockResource(hResource);
    // FreeResource(hAudioBuffer);

    return (TRUE);
} 

void resize(int width, int height) 
{
    // code 
    if(height <= 0) 
    {
        height = 1; 
    } 

    winWidth = width; 
    winHeight = height; 

    glViewport(0, 0, (GLsizei)width, (GLsizei)height); 

    projectionMatrix = vmath::perspective(
        45.0f, 
        (GLfloat)width/(GLfloat)height, 
        0.1f, 
        50000.0f 
    ); 
} 

void display(void) 
{
    // code 
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

    if (ImGuiManager::IsEnabled())
	{
		// Start new ImGui frame
		ImGuiManager::NewFrame();
	}

    switch(CurrentScene) 
    {
        case INTRO_SCENE: 
            introScene.display(); 
            break; 

        case SCENE_1: 
            scene1.display(); 
            break; 

        // case SCENE_2: 
        //     scene2.display(); 
        //     break; 

        // case OUTRO_SCENE: 
        //     outroScene.display(); 
        //     break; 
        
        // case TEST_SCENE: 
        //     testScene.display(); 
        //     break; 

        default: 
            break; 
    } 

	// Render ImGui debug window and other UI elements
	if (ImGuiManager::IsEnabled())
	{
		ImGuiManager::RenderDebugWindow();
		ImGuiManager::Render();
	}

    // Swap buffers 
    SwapBuffers(ghdc); 
} 

void update(void) 
{
    // code 
    updateEvent(); 

    switch(CurrentScene) 
    {
        case INTRO_SCENE: 
            introScene.update(); 
            break; 

        case SCENE_1:  
            scene1.update(); 
            break; 

        // case SCENE_2: 
        //     scene2.update(); 
        //     break; 

        // case OUTRO_SCENE: 
        //     outroScene.update(); 
        //     break; 

        // case TEST_SCENE: 
        //     testScene.update(); 
        //     break; 

        default: 
            break; 
    } 
} 

void uninitialize(void) 
{
    // function declarations 
    void toggleFullscreen(void); 

    // code 
	// Cleanup ImGui
	if (ImGuiManager::IsEnabled())
	{
		ImGuiManager::Cleanup();
	}

    // if user is exitting in full screen then restore fullscreen back to normal 
    if(gbFullscreen == TRUE) 
    {
        toggleFullscreen(); 
        gbFullscreen = FALSE; 
    } 

    // uninitialize scenes 
    // introScene.uninitialize(); 

    // make HDC as current rendering context 
    if(wglGetCurrentContext() == ghrc) 
    {
        wglMakeCurrent(NULL, NULL); 
    } 

    // delete rendering cintext 
    if(ghrc) 
    {
        wglDeleteContext(ghrc); 
        ghrc = NULL; 
    } 

    // release DC 
    if(ghdc) 
    {
        ReleaseDC(ghwnd, ghdc); 
        ghdc = NULL; 
    } 

    // destroy window 
    if(ghwnd) 
    {
        DestroyWindow(ghwnd); 
        ghwnd = NULL; 
    } 

    // no need to close log file as destructor will be automatically called 
    logFile.log("Program terminated succesfully\n"); 
} 

