
struct Button_State {
	bool is_down;
	bool changed;
};

enum {
	BUTTON_UP,
	BUTTON_DOWN,
	BUTTON_W,
	BUTTON_S,
	BUTTON_LEFT,
	BUTTON_RIGHT,
	BUTTON_LMOUSE,
	BUTTON_ESCAPE,
	BUTTON_ENTER,
	ZERO,
	ONE,
	TWO,
	THREE,
	FOUR,
	FIVE,
	SIX,
	SEVEN,
	EIGHT,
	NINE,

	BUTTON_COUNT, // Should be the last item
};

struct Input {
	Button_State buttons[BUTTON_COUNT];
};