//Copyright(C) 2025 Lost Empire Entertainment
//This program comes with ABSOLUTELY NO WARRANTY.
//This is free software, and you are welcome to redistribute it under certain conditions.
//Read LICENSE.md for more information.

//main log macro
#define WRITE_LOG(type, msg) std::cout << "[KALAKIT_INPUTUTILS | " << type << "] " << msg << "\n"

//log types
#if KALAUTILS_DEBUG
	#define LOG_DEBUG(msg) WRITE_LOG("DEBUG", msg)
#else
	#define LOG_DEBUG(msg)
#endif
#define LOG_SUCCESS(msg) WRITE_LOG("SUCCESS", msg)
#define LOG_ERROR(msg) WRITE_LOG("ERROR", msg)

#include <iostream>

#include "inpututils.hpp"
#include "windowutils.hpp"

using std::to_string;
using std::next;

namespace KalaKit
{
	bool InputUtils::IsKeyHeld(Key key)
	{
		bool isKeyDown = keyHeld[key];

		DebugType type = WindowUtils::GetDebugType();
		if (type == DebugType::DEBUG_ALL
			|| type == DebugType::DEBUG_KEY_HELD)
		{
			//only print if key is down
			if (isKeyDown)
			{
				string keyName = ToString(key);
				LOG_DEBUG("Key '" << keyName << "' is down");
			}
		}

		return isKeyDown;
	}

	bool InputUtils::IsKeyPressed(Key key)
	{
		bool wasKeyPressed = keyPressed[key];

		DebugType type = WindowUtils::WindowUtils::GetDebugType();
		if (type == DebugType::DEBUG_ALL
			|| type == DebugType::DEBUG_KEY_PRESSED)
		{
			//only print if key was pressed
			if (wasKeyPressed)
			{
				string keyName = ToString(key);
				LOG_DEBUG("Pressed key '" << keyName << "'.");
			}
		}

		return wasKeyPressed;
	}

	bool InputUtils::IsComboPressed(initializer_list<Key> keys)
	{
		//cannot proceed if only one key is assigned
		if (keys.size() < 2) return false;

		auto it = keys.begin();
		for (size_t i = 0; i < keys.size() - 1; i++)
		{
			if (!IsKeyHeld(*it++)) return false;
		}

		bool wasComboPressed = IsKeyPressed(*it);

		DebugType type = WindowUtils::GetDebugType();
		if (type == DebugType::DEBUG_ALL
			|| type == DebugType::DEBUG_COMBO_PRESSED)
		{
			//only print if combo was pressed
			if (wasComboPressed)
			{
				string comboString{};

				for (auto k = keys.begin(); k != keys.end(); ++k)
				{
					comboString += ToString(*k);
					if (next(k) != keys.end())
					{
						comboString += " + ";
					}
				}

				LOG_DEBUG("Pressed combo '" << comboString << "'.");
			}
		}

		//true as soon as last key is pressed
		return wasComboPressed;
	}

	bool InputUtils::IsMouseKeyDoubleClicked()
	{
		bool wasDoubleClicked =
			keyPressed[Key::MouseLeft]
			|| keyPressed[Key::MouseRight];

		DebugType type = WindowUtils::GetDebugType();
		if (type == DebugType::DEBUG_ALL
			|| type == DebugType::DEBUG_DOUBLE_CLICKED)
		{
			//only print if double clicked
			if (wasDoubleClicked)
			{
				LOG_DEBUG("Double clicked left mouse button.");
			}
		}

		return wasDoubleClicked;
	}

	bool InputUtils::IsMouseDragging()
	{
		bool isHoldingDragKey =
			keyHeld[Key::MouseLeft]
			|| keyHeld[Key::MouseRight];

		bool isDragging =
			isHoldingDragKey
			&& (mouseDelta.x != 0
				|| mouseDelta.y != 0);

		DebugType type = WindowUtils::GetDebugType();
		if (type == DebugType::DEBUG_ALL
			|| type == DebugType::DEBUG_IS_MOUSE_DRAGGING)
		{
			//only print if dragging
			if (isDragging)
			{
				LOG_DEBUG("Dragging mouse while holding left mouse button.");
			}
		}

		return isDragging;
	}

	POS InputUtils::GetMousePosition()
	{
		DebugType type = WindowUtils::GetDebugType();
		if (type == DebugType::DEBUG_ALL
			|| type == DebugType::DEBUG_MOUSE_POSITION)
		{
			static POS lastPos = { -9999, -9999 };

			//only print if mouse moved
			if (mousePosition.x != lastPos.x
				|| mousePosition.y != lastPos.y)
			{
				LOG_DEBUG("Mouse position: ("
					<< mousePosition.x << ", "
					<< mousePosition.y << ")");

				lastPos.x = mousePosition.x;
				lastPos.y = mousePosition.y;
			}
		}

		return mousePosition;
	}
	void InputUtils::SetMousePosition(POS newMousePosition)
	{
		mousePosition.x = newMousePosition.x;
		mousePosition.y = newMousePosition.y;
	}

	POS InputUtils::GetMouseDelta()
	{
		DebugType type = WindowUtils::GetDebugType();
		if (type == DebugType::DEBUG_ALL
			|| type == DebugType::DEBUG_MOUSE_DELTA)
		{
			//only print if mouse moved
			if (mouseDelta.x != 0
				|| mouseDelta.y != 0)
			{
				LOG_DEBUG("Mouse delta: ("
					<< mouseDelta.x << ", "
					<< mouseDelta.y << ")");
			}
		}

		return mouseDelta;
	}
	void InputUtils::SetMouseDelta(POS newMouseDelta)
	{
		mouseDelta.x = newMouseDelta.x;
		mouseDelta.y = newMouseDelta.y;
	}

	POS InputUtils::GetRawMouseDelta()
	{
		DebugType type = WindowUtils::GetDebugType();
		if (type == DebugType::DEBUG_ALL
			|| type == DebugType::DEBUG_RAW_MOUSE_DELTA)
		{
			//only print if mouse moved
			if (rawMouseDelta.x != 0
				|| rawMouseDelta.y != 0)
			{
				LOG_DEBUG("Raw mouse delta: ("
					<< rawMouseDelta.x << ", "
					<< rawMouseDelta.y << ")");
			}
		}

		return rawMouseDelta;
	}
	void InputUtils::SetRawMouseDelta(POS newRawMouseDelta)
	{
		rawMouseDelta.x = newRawMouseDelta.x;
		rawMouseDelta.y = newRawMouseDelta.y;
	}

	int InputUtils::GetMouseWheelDelta()
	{
		DebugType type = WindowUtils::GetDebugType();
		if (type == DebugType::DEBUG_ALL
			|| type == DebugType::DEBUG_MOUSE_WHEEL_DELTA)
		{
			LOG_DEBUG("Mouse wheel delta: " << to_string(mouseWheelDelta) << "");
		}

		return mouseWheelDelta;
	}
	void InputUtils::SetMouseWheelDelta(int newMouseWheelDelta)
	{
		mouseWheelDelta = newMouseWheelDelta;
	}

	bool InputUtils::IsMouseVisible()
	{
		return isMouseVisible;
	}
	void InputUtils::SetMouseVisibility(bool newMouseVisibleState)
	{
		//ignores same state
		if (isMouseVisible == newMouseVisibleState) return;

		DebugType type = WindowUtils::GetDebugType();
		if (type == DebugType::DEBUG_ALL
			|| type == DebugType::DEBUG_MOUSE_VISIBILITY)
		{
			string type = newMouseVisibleState ? "true" : "false";
			LOG_DEBUG("Mouse wheel visibility: " << type << "");
		}

		isMouseVisible = newMouseVisibleState;

		if (isMouseVisible) while (ShowCursor(TRUE) < 0); //increment until visible
		else while (ShowCursor(FALSE) >= 0);              //decrement until hidden
	}

	bool InputUtils::IsMouseLocked()
	{
		return isMouseLocked;
	}
	void InputUtils::SetMouseLockState(bool newMouseLockState)
	{
		//ignores same state
		if (isMouseLocked == newMouseLockState) return;

		DebugType type = WindowUtils::GetDebugType();
		if (type == DebugType::DEBUG_ALL
			|| type == DebugType::DEBUG_MOUSE_LOCK_STATE)
		{
			string type = newMouseLockState ? "true" : "false";
			LOG_DEBUG("Mouse wheel lock state: " << type << "");
		}

		isMouseLocked = newMouseLockState;
		if (!isMouseLocked) ClipCursor(nullptr);
		else
		{
			LockCursorToCenter();

			//clip the cursor to the window to prevent it from leaving
			RECT rect{};
			GetClientRect(WindowUtils::window, &rect);
			ClientToScreen(WindowUtils::window, (POS*)&rect.left);
			ClientToScreen(WindowUtils::window, (POS*)&rect.right);
			ClipCursor(&rect);
		}
	}

	void InputUtils::ResetFrameInput()
	{
		//clear "pressed" keys after each frame
		for (auto& [key, pressed] : keyPressed)
		{
			pressed = false;
		}

		if (!IsMouseDragging())
		{
			mouseDelta = { 0, 0 };
			rawMouseDelta = { 0, 0 };
			mouseWheelDelta = 0;
		}
	}

	void InputUtils::LockCursorToCenter()
	{
		RECT rect{};
		GetClientRect(WindowUtils::window, &rect);

		POS center{};
		center.x = (rect.right - rect.left) / 2;
		center.y = (rect.bottom - rect.top) / 2;

		ClientToScreen(WindowUtils::window, &center);
		SetCursorPos(center.x, center.y);
	}

	string InputUtils::ToString(Key key)
	{
		return "";
	}
}