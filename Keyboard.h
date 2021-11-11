#ifndef __KEYBOARD__
#define __KEYBOARD__

class Keyboard {

	private:

		char arr[255];

		Keyboard ();

		~Keyboard ();

		Keyboard (const Keyboard &other);

	public:

		/* Gets the single instance of keyboard */

		static Keyboard * getKeyboard ();

		/* Sets a key to the pressed state */

		void press (char key);

		/* Sets a key to the unpressed state */

		void release (char key);

		/* Returns if a key is recently pressed or not */

		bool isPressed (char key);

		/* Returns if they key is pressed down at all */ 

		bool isDown (char key) const;

};

#endif