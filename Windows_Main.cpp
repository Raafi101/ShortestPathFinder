#include <windows.h>
#include "utils.cpp"

global_variable bool running = true;

struct Render_State
{
	int height, width;
	void* memory;

	BITMAPINFO bitmap_info;
};

global_variable Render_State render_state;

#include "platform_common.cpp"
#include "Render.cpp"
#include "PathFinder.cpp"
#include "MainMenu.cpp"
#include <stdio.h>

LRESULT CALLBACK window_callback(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT result = 0;

	char str[150];

	switch (uMsg)
	{
		case WM_PAINT:
		{
			PAINTSTRUCT pen;
			HDC hdc = BeginPaint(hwnd, &pen);

			sprintf_s(str, "String");
			TextOut(hdc, 10, 10, str, strlen(str));
		}
		break;
		case WM_CLOSE:
		case WM_DESTROY:
		{
			running = false;
		}
		break;

		case WM_SIZE:
		{
			RECT rect;
			GetClientRect(hwnd, &rect);
			render_state.width = rect.right - rect.left;
			render_state.height = rect.bottom - rect.top;

			int size = render_state.width * render_state.height * sizeof(unsigned int);

			if (render_state.memory) 
				VirtualFree(render_state.memory, 0, MEM_RELEASE);
			render_state.memory = VirtualAlloc(0, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);


			render_state.bitmap_info.bmiHeader.biSize = sizeof(render_state.bitmap_info.bmiHeader);
			render_state.bitmap_info.bmiHeader.biWidth = render_state.width;
			render_state.bitmap_info.bmiHeader.biHeight = render_state.height;
			render_state.bitmap_info.bmiHeader.biPlanes = 1;
			render_state.bitmap_info.bmiHeader.biBitCount = 32;
			render_state.bitmap_info.bmiHeader.biCompression = BI_RGB;

		} break;
		case WM_CREATE:
		{

		} break;

		default:
		{
			result = DefWindowProc(hwnd, uMsg, wParam, lParam);
		}
	}
	return result;
}

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	//Need to make a window class
	WNDCLASS window_class = {};
	window_class.style = CS_HREDRAW | CS_VREDRAW;
	window_class.lpszClassName = "Window Class";
	window_class.lpfnWndProc = window_callback;

	//Register window class
	RegisterClass(&window_class);

	DWORD screenW = GetSystemMetrics(SM_CXSCREEN);
	DWORD screenH = GetSystemMetrics(SM_CYSCREEN);

	//actually create window (draw)

	HWND window = CreateWindow(window_class.lpszClassName, "Dijkstra", WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, screenW, screenH, 0, 0, hInstance, 0);
	
	ShowWindow(window, SW_MAXIMIZE);


	HDC hdc = GetDC(window);

	Input input = {};

	while(running)
	{
		//Input
		MSG message;

		for (int i = 0; i < BUTTON_COUNT; i++)
		{
			input.buttons[i].changed = false;
		}

		while (PeekMessage(&message, window, 0, 0, PM_REMOVE))
		{
			switch (message.message)
			{
				case WM_KEYUP:
				case WM_KEYDOWN:
				{
					u32 vk_code = (u32)message.wParam;
					bool is_down = ((message.lParam & (1 << 31)) == 0);

					switch (vk_code)
					{
						case VK_LBUTTON:
						{
							input.buttons[BUTTON_LMOUSE].is_down = is_down;
							input.buttons[BUTTON_LMOUSE].changed = true;
						}
						case VK_ESCAPE:
						{
							input.buttons[BUTTON_ESCAPE].is_down = is_down;
							input.buttons[BUTTON_ESCAPE].changed = true;
						}
						case VK_RETURN:
						{
							input.buttons[BUTTON_ENTER].is_down = is_down;
							input.buttons[BUTTON_ENTER].changed = true;
						}
						case 0x30:
						{
							input.buttons[ZERO].is_down = is_down;
							input.buttons[ZERO].changed = true;
						}
						case 0x31:
						{
							input.buttons[ONE].is_down = is_down;
							input.buttons[ONE].changed = true;
						}
						case 0x32:
						{
							input.buttons[TWO].is_down = is_down;
							input.buttons[TWO].changed = true;
						}
						case 0x33:
						{
							input.buttons[THREE].is_down = is_down;
							input.buttons[THREE].changed = true;
						}
						case 0x34:
						{
							input.buttons[FOUR].is_down = is_down;
							input.buttons[FOUR].changed = true;
						}
						case 0x35:
						{
							input.buttons[FIVE].is_down = is_down;
							input.buttons[FIVE].changed = true;
						}
						case 0x36:
						{
							input.buttons[SIX].is_down = is_down;
							input.buttons[SIX].changed = true;
						}
						case 0x37:
						{
							input.buttons[SEVEN].is_down = is_down;
							input.buttons[SEVEN].changed = true;
						}
						case 0x38:
						{
							input.buttons[EIGHT].is_down = is_down;
							input.buttons[EIGHT].changed = true;
						}
						case 0x39:
						{
							input.buttons[NINE].is_down = is_down;
							input.buttons[NINE].changed = true;
						}

					}
				}
				break;

				default:
				{
					TranslateMessage(&message);
					DispatchMessage(&message);
				}
			}
		}

		//Simulate
		simulate(&input);

		//Render
		StretchDIBits(hdc, 0, 0, render_state.width, render_state.height, 0, 0, render_state.width, render_state.height, render_state.memory, &render_state.bitmap_info, DIB_RGB_COLORS, SRCCOPY);
	}

}