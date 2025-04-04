//Copyright(C) 2025 Lost Empire Entertainment
//This program comes with ABSOLUTELY NO WARRANTY.
//This is free software, and you are welcome to redistribute it under certain conditions.
//Read LICENSE.md for more information.

//main log macro
#define WRITE_LOG(type, msg) std::cout << "[KALAKIT_WINDOWUTILS | " << type << "] " << msg << "\n"

//log types
#if KALAUTILS_DEBUG
	#define LOG_DEBUG(msg) WRITE_LOG("DEBUG", msg)
#else
	#define LOG_DEBUG(msg)
#endif
#define LOG_SUCCESS(msg) WRITE_LOG("SUCCESS", msg)
#define LOG_ERROR(msg) WRITE_LOG("ERROR", msg)

#include <iostream>

#include "windowutils.hpp"

using std::to_string;

namespace KalaKit
{
	DebugType WindowUtils::GetDebugType()
	{
		return debugType;
	}
	void WindowUtils::SetDebugType(DebugType newDebugType)
	{
		debugType = newDebugType;
	}

	bool WindowUtils::GetWindowFocusRequiredState()
	{
		return isWindowFocusRequired;
	}
	void WindowUtils::SetWindowFocusRequiredState(bool newWindowFocusRequiredState)
	{
		isWindowFocusRequired = newWindowFocusRequiredState;
	}

	void WindowUtils::SetWindowTitle(const string& title)
	{
		if (debugType == DebugType::DEBUG_ALL
			|| debugType == DebugType::DEBUG_WINDOW_TITLE)
		{
			LOG_DEBUG("New window title: " << title << "");
		}

		SetWindowTextA(window, title.c_str());
	}

	void WindowUtils::SetWindowState(WindowState state)
	{
		if (debugType == DebugType::DEBUG_ALL
			|| debugType == DebugType::DEBUG_WINDOW_TITLE)
		{
			string type{};
			if (state == WindowState::WINDOW_RESET) type = "WINDOW_RESET";
			else if (state == WindowState::WINDOW_MINIMIZED) type = "WINDOW_MINIMIZED";
			else if (state == WindowState::WINDOW_MAXIMIZED) type = "WINDOW_MAXIMIZED";

			LOG_DEBUG("New window type: " << type << "");
		}

		switch (state)
		{
		case WindowState::WINDOW_RESET:
			ShowWindow(window, SW_RESTORE);
			break;
		case WindowState::WINDOW_MINIMIZED:
			ShowWindow(window, SW_MINIMIZE);
			break;
		case WindowState::WINDOW_MAXIMIZED:
			ShowWindow(window, SW_MAXIMIZE);
			break;
		}
	}

	bool WindowUtils::IsWindowBorderless()
	{
		LONG style = GetWindowLong(window, GWL_STYLE);
		return (style & WS_OVERLAPPEDWINDOW);
	}
	void WindowUtils::SetWindowBorderlessState(bool newWindowBorderlessState)
	{
		if (debugType == DebugType::DEBUG_ALL
			|| debugType == DebugType::DEBUG_WINDOW_BORDERLESS_STATE)
		{
			string type = newWindowBorderlessState ? "true" : "false";
			LOG_DEBUG("New window borderless state: " << type << "");
		}

		if (!newWindowBorderlessState)
		{
			//save original style and placement
			originalStyle = GetWindowLong(window, GWL_STYLE);
			GetWindowPlacement(window, &originalPlacement);

			//set style to borderless
			SetWindowLong(window, GWL_STYLE, WS_POPUP);

			//resize to full monitor
			MONITORINFO mi = { sizeof(mi) };
			GetMonitorInfo(MonitorFromWindow(window, MONITOR_DEFAULTTONEAREST), &mi);
			SetWindowPos(
				window,
				HWND_TOP,
				mi.rcMonitor.left, mi.rcMonitor.top,
				mi.rcMonitor.right - mi.rcMonitor.left,
				mi.rcMonitor.bottom - mi.rcMonitor.top,
				SWP_FRAMECHANGED
				| SWP_NOOWNERZORDER
				| SWP_SHOWWINDOW);

			isWindowBorderless = true;
		}
		else
		{
			//restore original style
			SetWindowLong(window, GWL_STYLE, originalStyle);

			//restore previous size/position
			SetWindowPlacement(window, &originalPlacement);
			SetWindowPos(
				window,
				nullptr,
				0,
				0,
				0,
				0,
				SWP_NOMOVE
				| SWP_NOSIZE
				| SWP_FRAMECHANGED
				| SWP_NOOWNERZORDER
				| SWP_NOZORDER
				| SWP_SHOWWINDOW);

			isWindowBorderless = true;
		}
	}

	bool WindowUtils::IsWindowHidden()
	{
		return !IsWindowVisible(window);
	}
	void WindowUtils::SetWindowHiddenState(bool newWindowHiddenState)
	{
		if (debugType == DebugType::DEBUG_ALL
			|| debugType == DebugType::DEBUG_WINDOW_HIDDEN_STATE)
		{
			string type = newWindowHiddenState ? "true" : "false";
			LOG_DEBUG("New window hidden state: " << type << "");
		}

		ShowWindow(window, newWindowHiddenState ? SW_HIDE : SW_SHOW);
	}

	POINT WindowUtils::GetWindowPosition()
	{
		RECT rect{};
		GetWindowRect(window, &rect);

		POINT pos{};
		pos.x = rect.left;
		pos.y = rect.top;
		return pos;
	}
	void WindowUtils::SetWindowPosition(int width, int height)
	{
		if (debugType == DebugType::DEBUG_ALL
			|| debugType == DebugType::DEBUG_WINDOW_SET_POSITION)
		{
			string sizeX = to_string(width);
			string sizeY = to_string(height);

			LOG_DEBUG("New window position: (" << width << ", " << height);
		}

		SetWindowPos(
			window,
			nullptr,
			width,
			height,
			0,
			0,
			SWP_NOSIZE
			| SWP_NOZORDER
			| SWP_NOOWNERZORDER);
	}

	POINT WindowUtils::GetWindowFullSize()
	{
		RECT rect{};
		GetWindowRect(window, &rect);

		POINT size{};
		size.x = rect.right - rect.left;
		size.y = rect.bottom - rect.top;
		return size;
	}
	void WindowUtils::SetWindowFullSize(int width, int height)
	{
		if (debugType == DebugType::DEBUG_ALL
			|| debugType == DebugType::DEBUG_WINDOW_SET_FULL_SIZE)
		{
			string sizeX = to_string(width);
			string sizeY = to_string(height);

			LOG_DEBUG("New window full size: (" << width << ", " << height);
		}

		SetWindowPos(
			window,
			nullptr,
			0,
			0,
			width,
			height,
			SWP_NOMOVE
			| SWP_NOZORDER
			| SWP_NOOWNERZORDER);
	}

	POINT WindowUtils::GetWindowContentSize()
	{
		RECT rect{};
		GetClientRect(window, &rect);

		POINT size{};
		size.x = rect.right - rect.left;
		size.y = rect.bottom - rect.top;
		return size;
	}
	void WindowUtils::SetWindowContentSize(int width, int height)
	{
		if (debugType == DebugType::DEBUG_ALL
			|| debugType == DebugType::DEBUG_WIŃDOW_SET_CONTENT_SIZE)
		{
			string sizeX = to_string(width);
			string sizeY = to_string(height);

			LOG_DEBUG("New window content size: (" << width << ", " << height);
		}

		RECT rect = { 0, 0, width, height };
		AdjustWindowRect(
			&rect,
			GetWindowLong(
				window,
				GWL_STYLE),
			FALSE);

		SetWindowPos(
			window,
			nullptr,
			0,
			0,
			rect.right - rect.left,
			rect.bottom - rect.top,
			SWP_NOMOVE
			| SWP_NOZORDER
			| SWP_NOOWNERZORDER);
	}

	POINT WindowUtils::GetWindowMaxSize()
	{
		POINT point{};
		point.x = maxWidth;
		point.y = maxHeight;
		return point;
	}
	POINT WindowUtils::GetWindowMinSize()
	{
		POINT point{};
		point.x = minWidth;
		point.y = minHeight;
		return point;
	}
	void WindowUtils::SetMinMaxSize(
		int newMaxWidth,
		int newMaxHeight,
		int newMinWidth,
		int newMinHeight)
	{
		if (debugType == DebugType::DEBUG_ALL
			|| debugType == DebugType::DEBUG_WINDOW_SET_MINMAX_SIZE)
		{
			LOG_DEBUG("Set new max size: " << maxWidth << ", " << maxHeight
				<< ", min size:" << minWidth << ", " << minHeight);
		}

		maxWidth = newMaxWidth;
		maxHeight = newMaxHeight;
		minWidth = newMinWidth;
		minHeight = newMinHeight;
	}
}