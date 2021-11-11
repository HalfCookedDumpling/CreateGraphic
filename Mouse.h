#ifndef __MOUSE__
#define __MOUSE__

class Mouse {

	private:

	double x;

	double y;

	double xTemp;

	double yTemp;

	int lDown;

	int rDown;

	Mouse ();

	~Mouse ();

	Mouse (Mouse *other);

	public:

	static Mouse *getMouse ();

	int getX () const;

	int getY () const;

	int getXTemp () const;

	int getYTemp () const;

	bool lButtonPressed ();

	bool rButtonPressed ();

	void setX (int x);

	void setY (int y);

	void setXTemp (int x);

	void setYTemp (int y);

	void set (int x, int y);

	void setXYTemp (int x, int y);

	void pressLButton ();

	void pressRButton ();

	void releaseLButton ();

	void releaseRButton ();

};

#endif
