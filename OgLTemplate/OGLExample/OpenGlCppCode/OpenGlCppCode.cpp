// OpenGlCppCode.cpp: определяет экспортированные функции для приложения DLL.
//

#include "stdafx.h"
#include <windows.h>
#include <gl\GL.h>
#include <exception>

#define CPP_EXPORTS_API
#ifdef CPP_EXPORTS_API
#define CPP_API extern "C" __declspec(dllexport)
#else
#define CPP_API __declspec(dllimport)
#endif
static PIXELFORMATDESCRIPTOR pfdScreen3D = {
	sizeof(PIXELFORMATDESCRIPTOR),  // size of this pfd
	1,                            // version number
	PFD_DRAW_TO_WINDOW |          // support window
	PFD_SUPPORT_OPENGL           // support OpenGL
	| PFD_DOUBLEBUFFER
	,
	PFD_TYPE_RGBA,                  // RGBA type
	24,                             // 24-bit color depth
	0, 0, 0, 0, 0, 0,               // color bits ignored
	0,                              // no alpha buffer
	0,                              // shift bit ignored
	0,                              // no accumulation buffer
	0, 0, 0, 0,                     // accum bits ignored
	32,		                        // 32-bit z-buffer      
	0,                              // no stencil buffer
	0,                              // no auxiliary buffer
	PFD_MAIN_PLANE,                 // main layer
	0,                              // reserved
	0, 0, 0                         // layer masks ignored
};
CPP_API int InitOpenGL(int handle)
{
	auto ctx = GetDC((HWND) handle);

	PIXELFORMATDESCRIPTOR pxfd = pfdScreen3D;
	int pixformatnumber = ::ChoosePixelFormat((HDC) ctx, &pxfd);
	if (!pixformatnumber)
	{
		auto er=GetLastError();
		exit(er);// Можно выдать сообщение об ошибке, используя GetLastError()
	}
		
	::DescribePixelFormat((HDC) ctx, pixformatnumber, sizeof(PIXELFORMATDESCRIPTOR), &pxfd);
	//		DoubleBufferOff=false;
	if (!::SetPixelFormat((HDC) ctx, pixformatnumber, &pxfd)) exit(-1);// Можно выдать сообщение об ошибке, используя GetLastError();
	void* OpenGLContext = wglCreateContext((HDC) ctx);
	if (!OpenGLContext)exit(-1);// Можно выдать сообщение об ошибке, используя GetLastError();
	ReleaseDC((HWND) handle, ctx);
	return  (int) OpenGLContext;
}

CPP_API void Render(int handle, int oglcontext, int w, int h, int type)
{
	
	if (oglcontext == NULL)return;
	auto ctx = GetDC((HWND) handle);
	::wglMakeCurrent((HDC) ctx, (HGLRC) oglcontext);
	::glViewport(0, 0, w, h);
	::glDrawBuffer(GL_BACK);

	::glMatrixMode(GL_PROJECTION);
	::glLoadIdentity();
	::glOrtho(0, w, 0, h, -1, 1);
	//		::glOrtho(minp.x(),maxp.x(),minp.y(),maxp.y(),minp.z(),maxp.z());
	::glMatrixMode(GL_MODELVIEW);
	::glLoadIdentity();
	::glClearColor(1, 0 , 1, 1);

	::glClearDepth(1.0);

	::glDepthFunc(GL_LEQUAL);

	::glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	::glBegin(GL_LINES);
	::glColor3f(1, 0, 0);
	if (type)
	{
		::glVertex2d(0, 0);
		::glVertex2d(w,h);
		::glVertex2d(0, h);
		::glVertex2d(w, 0);
	}
	else
	{
		::glVertex2d(0, h/2);
		::glVertex2d(w, h/2);
		::glVertex2d(w/2, h);
		::glVertex2d(w/2, 0);
	}
	::glEnd();
	::glFlush();
	::glFinish();
	::SwapBuffers((HDC) ctx);
	::wglMakeCurrent(0, 0);
	ReleaseDC((HWND) handle, ctx);
}
