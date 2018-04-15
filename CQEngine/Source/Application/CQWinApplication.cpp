#include "CQWinApplication.h"
#include "CQRenderer.h"
#include "CQGLProgram.h"
#include "CQTimeStamp.h"
#include "CQResLoader.h"
#include "CQDebug.h"

USING_NS_CQ

/////////////////////// TMP //////////////////
CQWglContext context;

const char *vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"\n"
"uniform mat4 transform;\n"
"void main()\n"
"{\n"
"   gl_Position = transform * vec4(aPos, 1.0f);\n"
"}\0";
const char *fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.2f, 0.2f, 1.0f);\n"
"}\n\0";

/////////////////////// TMP //////////////////

bool CQWinApp::isExit_ = false;

CQWinApp::CQWinApp(const char *_title, int _xPos, int _yPos, int _width, int _height)
	:
	appName_(_title),
	winWidth_(_width),
	winHeight_(_height),
	hInstance_(0),
	hWnd_(0)
{}

CQWinApp::~CQWinApp()
{
	puts("release CQWinApp");
	__destroyWnd();
}

void CQWinApp::run()
{
	MSG msg = {};

	__createWnd();

	/////////////////////// TMP //////////////////
	// program
	CQGLProgram program;
	program.attachNativeShader(vertexShaderSource, CQGLProgram::SHADER_VERTEX);
	program.attachNativeShader(fragmentShaderSource, CQGLProgram::SHADER_PIXEL);
	program.genProgram();

	// texture
	CQResLoader::ImgData *img = nullptr;
	char *path = "D:/work/CQEngine/CQEngine_git/CQEngine/CQEngine/res/img.jpg";
	unsigned int texture;

	img = CQResLoader::shareLoader()->loadImgDataSync(path);
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img->width_, img->height_, 0, GL_RGB, GL_UNSIGNED_BYTE, img->data_);

	// array of structures
	float vertices[] = {
		-0.5f, -0.5f, 0.0f,		0.0f,0.0f, // left  bottom
		0.5f, -0.5f, 0.0f,		1.0f,0.0f, // right bottom
		0.0f,  0.5f, 0.0f,		0.5f,1.0f, // top middle  
	};

#define VERTEX_POS_INDEX		0
#define VERTEX_TEXCOORD0_INDEX	1

#define VERTEX_POS_SIZE			3
#define VERTEX_TEXCOORD0_SIZE	3

#define VERTEX_POS_OFFSET		0
#define VERTEX_TEXCOORD0_OFFSET	3

#define VERTEX_ATTRIB_SIZE		VERTEX_POS_SIZE + \
								VERTEX_TEXCOORD0_SIZE

	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// pos
	glVertexAttribPointer(VERTEX_POS_INDEX, VERTEX_POS_SIZE, GL_FLOAT, GL_FALSE, VERTEX_ATTRIB_SIZE * sizeof(float), (void*)VERTEX_POS_OFFSET);
	glEnableVertexAttribArray(VERTEX_POS_INDEX);

	// uv
	glVertexAttribPointer(VERTEX_TEXCOORD0_INDEX, VERTEX_TEXCOORD0_SIZE, GL_FLOAT, GL_FALSE, VERTEX_ATTRIB_SIZE * sizeof(float), (void*)VERTEX_TEXCOORD0_OFFSET);
	glEnableVertexAttribArray(VERTEX_TEXCOORD0_INDEX);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


	
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	float angle = 0.f;
	/////////////////////// TMP //////////////////

	CQTimeStamp timeStamp;
	while (!isExit_)
	{
		if (timeStamp.getElapsedSecond() > 1/120.0f)
		{
			timeStamp.tick();

			/////////////////////// TMP //////////////////
			glClearColor(1.0f, 1.f, 0.f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			program.load();

			//tmat4x4<T>& rotate(value_type angle, tvec3<T> const & v)
			Matrix4 mat(1.0f);
			Vector3 v(0.0f, 1.0f, 0.0f);
			Matrix4 trans = rotate(mat, ++angle, v);
			program.setMatrix("transform", trans);
			glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
			glDrawArrays(GL_TRIANGLES, 0, 3);

			context.update();
			/////////////////////// TMP //////////////////

			while (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			Sleep(1);
		}
	}

	/////////////////////// TMP //////////////////
	program.load();
	CQResLoader::shareLoader()->unloadImgData(img);
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	/////////////////////// TMP //////////////////

	return ;
}

void CQWinApp::__createWnd()
{
	/////////////////////// TMP //////////////////
	if (gladLoadGL()) {
		// you need an OpenGL context before loading glad
		printf("I did load GL with no context!\n");
		exit(-1);
	}
	/////////////////////// TMP //////////////////

	hInstance_ = GetModuleHandle(NULL);
	WNDCLASSEX wc = {};

	WCHAR wszAppName[256];
	memset(wszAppName, 0, sizeof(wszAppName));
	MultiByteToWideChar(CP_ACP, 0, appName_, strlen(appName_) + 1, wszAppName,
		sizeof(wszAppName) / sizeof(wszAppName[0]));

	wc.cbSize = sizeof(WNDCLASSEX);
	/*
	* CS_HREDRAW | CS_VREDRAW | CS_OWNDC
	* CS_HREDRAW : window's width change or coordinate x change.(WM_PAINT)
	* CS_VREDRAW : window's height change or coordinate y change.(WM_PAINT)
	* CS_OWNDC : for Pixel Format.
	*/
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = wndProc;
	wc.cbClsExtra = wc.cbWndExtra = NULL;
	wc.hInstance = hInstance_;
	wc.hbrBackground = (HBRUSH)GetStockObject(COLOR_WINDOW + 1);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = wszAppName;
	wc.hIcon = wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);

	RegisterClassEx(&wc);

	hWnd_ = CreateWindowEx(NULL,
		wc.lpszClassName,
		wc.lpszClassName,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		winWidth_,
		winHeight_,
		NULL,
		NULL,
		hInstance_,
		NULL);

	/////////////////////// TMP ////////////////// 
	context.hWnd_ = hWnd_;
	context.create();

	if (!gladLoadGL()) {
		printf("Something went wrong!\n");
		exit(-1);
	}

	context.printGLInfo();
	/////////////////////////////////////////
	ShowWindow(hWnd_, SW_SHOW);
	UpdateWindow(hWnd_);
}

LRESULT CALLBACK CQWinApp::wndProc(HWND _hWnd, UINT _nMsg, WPARAM _wParam, LPARAM _lParam)
{
	switch (_nMsg)
	{
	case WM_CREATE:
	{


	}
	break;
	case WM_COMMAND:
	{

	}
	break;
	case WM_PAINT:
	{
		
	}
		break;
	case WM_LBUTTONDOWN:
	{
		
	}
	break;
	case WM_LBUTTONUP:
	{

	}
	break;
	case WM_RBUTTONDOWN:
	{

	}
	break;
	case WM_RBUTTONUP:
	{

	}
	break;
	case WM_MOUSEMOVE:
	{

	}
	break;
	case WM_LBUTTONDBLCLK:

		break;
	case WM_MOUSEWHEEL:
	{

	}
	break;
	case WM_DESTROY:
	{
		isExit_ = true;
		PostQuitMessage(0);
	}
		break;
	}
	return DefWindowProc(_hWnd, _nMsg,
		_wParam, _lParam);
}

void CQWinApp::__destroyWnd()
{

}

///
CQAppI *g_app = CQ_NEW(CQWinApp,"CQEngine",0,0,800,600);

