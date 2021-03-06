#pragma once

#include <vector>
#include "../core/system.h"
#include "../video/window.h"
#include "gamepad.h"

/*
	[TODO] -- Non-gamepad inputs (racing wheels, arcade sticks, flight sticks, dance pads, guitar, drumkid, arcade pad are mentioned in xinput subtypes)
*/

namespace overdrive {
	namespace input {
		class Keyboard;
		class Mouse;
	}

	class Input:
		public core::System,
		public MessageHandler<video::Window::OnCreated>,
		public MessageHandler<video::Window::OnClosed>
	{
	public:
		Input();

		virtual void initialize() override;
		virtual void update() override;
		virtual void shutdown() override;

		const std::vector<input::Keyboard*>& getKeyboardList() const;
		const std::vector<input::Mouse*>& getMouseList() const;
		const std::vector<input::Gamepad>& getGamepadList() const;

		void operator()(const video::Window::OnCreated& created);
		void operator()(const video::Window::OnClosed& closed);

	private:
		std::vector<input::Keyboard*> mKeyboardList;
		std::vector<input::Mouse*> mMouseList;
		std::vector<input::Gamepad> mGamepadList;
	};

	namespace input {
		namespace detail {
			// [TODO]  These are platform-specific. Until there's a need for it (or someone else wants to do it for me) I'm leaving it as it is.
			void loadXInput();
			void unloadXInput();
		}
	}
}
