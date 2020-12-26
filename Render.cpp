#include <vector>
#include <string>

const int infinity = 0;

internal void clear_screen(u32 color)
{
	unsigned int* pixel = (u32*)render_state.memory;
	for (int y = 0; y < render_state.height; y++)
	{
		for (int x = 0; x < render_state.width; x++)
		{
			*pixel++ = color;
		}
	}
}

internal void draw_rect_in_pixels(int x0, int y0, int x1, int y1, u32 color)
{
	x0 = clamp(0, x0, render_state.width);
	x1 = clamp(0, x1, render_state.width);
	y0 = clamp(0, y0, render_state.height);
	y1 = clamp(0, y1, render_state.height);

	for (int y = y0; y < y1; y++)
	{
		u32* pixel = (u32*)render_state.memory + x0 + y*render_state.width;
		for (int x = x0; x < x1; x++)
		{
			*pixel++ = color;
		}
	}
}

global_variable float render_scale = 0.01f;

internal void draw_rect(float x, float y, float size_x, float size_y, u32 color) //Origin bottom left of screen
{
	x *= render_state.width * render_scale;
	y *= render_state.height * render_scale;
	size_x *= render_state.width * render_scale;
	size_y *= render_state.height * render_scale;

	int x0 = x;
	int x1 = x + size_x;
	int y0 = y;
	int y1 = y + size_y;

	draw_rect_in_pixels(x0, y0, x1, y1, color);
}

internal void draw_node(float x, float y, float half_size_x, float half_size_y, u32 color) //helper function to printNode()
{

	// Change to pixels
	int x0 = x - half_size_x;
	int x1 = x + half_size_x;
	int y0 = y - half_size_y;
	int y1 = y + half_size_y;

	draw_rect_in_pixels(x0, y0, x1, y1, color);
}

internal void printNode(float x, float y) //Origin center of the screen
{
	u32 colorList[] = {0x32a858, 0x32a883, 0x32a8a6, 0xffffff};
	for (int i = 4; i >= 1; i--)
	{
		if (i == 4)
		{
			draw_node(x, y, 19, 19, colorList[3]);
		}
		else
			draw_node(x, y, 6 * i, 6 * i, colorList[i-1]);
	}
}

//NODE CLASS
class Node
{
private:
	double y;
	double x;
	int id;
	double x0;
	double x1;
	double y0;
	double y1;
	int NodeDist;
	int parent;
	std::vector<int> neighbors;
	bool visited = false;
public:
	//Constructors
	Node()
	{
		x = 0;
		y = 0;
		id = -1;
		x0 = x + 19;
		x1 = x - 19;
		y0 = y + 19;
		y1 = y - 19;
		NodeDist = infinity;
		parent = -1;
		std::vector<int> neighbors;
		id = -1;
		visited = false;
	}
	Node(double mx, double my, int idx, bool tf, int NodeDist1, int parentNode, std::vector<int> vect)
	{
		x = mx;
		y = my;
		id = idx;
		x0 = x - 19;
		x1 = x + 19;
		y0 = y - 19;
		y1 = y + 19;
		visited = tf;
		NodeDist = NodeDist1;
		parent = parentNode;
		neighbors = vect;
	}
	//Accessors
	double getX()
	{
		return x;
	}
	double getY()
	{
		return y;
	}
	int getID()
	{
		return id;
	}
	double getX0()
	{
		return x0;
	}
	double getY0()
	{
		return y0;
	}
	double getX1()
	{
		return x1;
	}
	double getY1()
	{
		return y1;
	}
	bool getVisited()
	{
		return visited;
	}
	int getNodeDist()
	{
		return NodeDist;
	}
	int getParent()
	{
		return parent;
	}
	std::vector<int> getNeighbors()
	{
		return neighbors;
	}
	int getNeighbor(int i)
	{
		return neighbors[i];
	}
	//Mutators
	void setX(double mx)
	{
		x = mx;
	}
	void setY(double my)
	{
		y = my;
	}
	void setID(int idx)
	{
		id = idx;
	}
	void print()
	{
		printNode(x, y);
	}
	bool inHitbox(double mx, double my)
	{
		if (mx > x0 && mx < x1 && my > y0 && my < y1)
			return true;
		return false;
	}
	void setBounds(double x0z, double y0z, double x1z, double y1z)
	{
		x0 = x0z;
		y0 = y0z;
		x1 = x1z;
		y1 = y1z;
	}
	void setVisited(bool tf)
	{
		visited = tf;
	}
	void setNodeDist(int NodeDist1)
	{
		NodeDist = NodeDist1;
	}
	void setParent(int parentNode)
	{
		parent = parentNode;
	}
	void addNeighbors(int node)
	{
		neighbors.push_back(node);
	}
	void sortNeighbors()
	{
		bool isSorted = false;
		while (!isSorted)
		{
			isSorted = true;
			for (int i = 0; i < neighbors.size() - 1; i++)
			{
				if (neighbors[i] > neighbors[i+1])
				{
					std::swap(neighbors[i], neighbors[i + 1]);
					isSorted = false;
				}
			}
		}
	}
};



const char* letters[][7] = {
	" 00",
	"0  0",
	"0  0",
	"0000",
	"0  0",
	"0  0",
	"0  0",

	"000",
	"0  0",
	"0  0",
	"000",
	"0  0",
	"0  0",
	"000",

	" 000",
	"0",
	"0",
	"0",
	"0",
	"0",
	" 000",

	"000",
	"0  0",
	"0  0",
	"0  0",
	"0  0",
	"0  0",
	"000",

	"0000",
	"0",
	"0",
	"000",
	"0",
	"0",
	"0000",

	"0000",
	"0",
	"0",
	"000",
	"0",
	"0",
	"0",

	" 000",
	"0",
	"0",
	"0 00",
	"0  0",
	"0  0",
	" 000",

	"0  0",
	"0  0",
	"0  0",
	"0000",
	"0  0",
	"0  0",
	"0  0",

	"000",
	" 0",
	" 0",
	" 0",
	" 0",
	" 0",
	"000",

	" 000",
	"   0",
	"   0",
	"   0",
	"0  0",
	"0  0",
	" 000",

	"0  0",
	"0  0",
	"0 0",
	"00",
	"0 0",
	"0  0",
	"0  0",

	"0",
	"0",
	"0",
	"0",
	"0",
	"0",
	"0000",

	"00 00",
	"0 0 0",
	"0 0 0",
	"0   0",
	"0   0",
	"0   0",
	"0   0",

	"00  0",
	"0 0 0",
	"0 0 0",
	"0 0 0",
	"0 0 0",
	"0 0 0",
	"0  00",

	"0000",
	"0  0",
	"0  0",
	"0  0",
	"0  0",
	"0  0",
	"0000",

	" 000",
	"0  0",
	"0  0",
	"000",
	"0",
	"0",
	"0",

	" 000 ",
	"0   0",
	"0   0",
	"0   0",
	"0 0 0",
	"0  0 ",
	" 00 0",

	"000",
	"0  0",
	"0  0",
	"000",
	"0  0",
	"0  0",
	"0  0",

	" 000",
	"0",
	"0 ",
	" 00",
	"   0",
	"   0",
	"000 ",

	"00000",
	"  0",
	"  0",
	"  0",
	"  0",
	"  0",
	"  0",

	"0  0",
	"0  0",
	"0  0",
	"0  0",
	"0  0",
	"0  0",
	" 00",

	"0   0",
	"0   0",
	"0   0",
	"0   0",
	"0   0",
	" 0 0",
	"  0",

	"0   0 ",
	"0   0",
	"0   0",
	"0 0 0",
	"0 0 0",
	"0 0 0",
	" 0 0 ",

	"0   0",
	"0   0",
	" 0 0",
	"  0",
	" 0 0",
	"0   0",
	"0   0",

	"0   0",
	"0   0",
	" 0 0",
	"  0",
	"  0",
	"  0",
	"  0",

	"0000",
	"   0",
	"  0",
	" 0",
	"0",
	"0",
	"0000",

	"",
	"",
	"",
	"",
	"",
	"",
	"0",

	"   0",
	"  0",
	"  0",
	" 0",
	" 0",
	"0",
	"0",
};

internal void
draw_text(const char* text, float x, float y, float size, u32 color) {
	float half_size = size * .5f;
	float original_y = y;

	while (*text) {
		if (*text != 32) {
			const char** letter;
			if (*text == 47) letter = letters[27];
			else if (*text == 46) letter = letters[26];
			else letter = letters[*text - 'A'];
			float original_x = x;

			for (int i = 0; i < 7; i++) {
				const char* row = letter[i];
				while (*row) {
					if (*row == '0') {
						draw_rect(x, y, half_size, half_size, color);
					}
					x += .33 * size;
					row++;
				}
				y -= .5 * size;
				x = original_x;
			}
		}
		text++;
		x += size * 2.2;
		y = original_y;
	}
}

internal void
draw_number(int number, float x, float y, float size, u32 color) {
	float half_size = size * .5f;

	bool drew_number = false;
	while (number || !drew_number) {
		drew_number = true;

		int digit = number % 10;
		number = number / 10;

		switch (digit) {
		case 0: {
			draw_rect(x - size, y - 1, half_size, 4.3f * size, color);
			draw_rect(x + size - .25, y - 1, half_size, 4.3f * size, color);
			draw_rect(x - .5, y + size * 2.f, half_size * 4, half_size, color);
			draw_rect(x - .5, y - size * 2.f, half_size * 4, half_size, color);
			x -= size * 4.f;
		} break;

		case 1: {
			draw_rect(x + size, y - 1, half_size, 4.5f * size, color);
			x -= size * 2.f;
		} break;

		case 2: {
			draw_rect(x, y + size * 2.f, 1.5f * size, half_size, color);
			draw_rect(x, y, 1.5f * size, half_size, color);
			draw_rect(x, y - size * 2.f, 1.5f * size, half_size, color);
			draw_rect(x + size, y + size - .17, half_size, half_size * 2.4, color);
			draw_rect(x - size + .5, y - size - .27, half_size, half_size * 3, color);
			x -= size * 4.f;
		} break;

		case 3: {
			draw_rect(x - half_size, y + size * 2.f, size + .5, half_size, color);
			draw_rect(x - half_size, y, size + .5, half_size, color);
			draw_rect(x - half_size, y - size * 2.f, size + .5, half_size, color);
			draw_rect(x + size, y - 1, half_size, 4.6f * size, color);
			x -= size * 4.f;
		} break;

		case 4: {
			draw_rect(x + size, y - 1, half_size, 4.5f * size, color);
			draw_rect(x - size + .25, y + size - .3, half_size, 2.2f * size, color);
			draw_rect(x - .25, y, half_size + .6, half_size, color);
			x -= size * 4.f;
		} break;

		case 5: {
			draw_rect(x, y + size * 2.f, 1.5f * size, half_size, color);
			draw_rect(x, y, 1.5f * size, half_size, color);
			draw_rect(x, y - size * 2.f, 1.5f * size, half_size, color);
			draw_rect(x - size + .5, y + size - .2, half_size, half_size * 2.5, color);
			draw_rect(x + size, y - size - .3, half_size, half_size * 3, color);
			x -= size * 4.f;
		} break;

		case 6: {
			draw_rect(x + half_size, y + size * 2.f, size, half_size, color);
			draw_rect(x + half_size, y, size * 1.35, half_size, color);
			draw_rect(x + half_size, y - size * 2.f, size, half_size, color);
			draw_rect(x - size + .5, y - 1, half_size, 4.5f * size, color);
			draw_rect(x + size + .25, y - size - .5, half_size / 1.5, half_size * 4, color);
			x -= size * 4.f;
		} break;

		case 7: {
			draw_rect(x + size - .25, y - 1, half_size, 4.3f * size, color);
			draw_rect(x - half_size, y + size * 2.f, size + .27, half_size, color);
			x -= size * 4.f;
		} break;

		case 8: {
			draw_rect(x + size - .45, y - 1, half_size, 4.3f * size, color);
			draw_rect(x - size - .25, y - 1, half_size, 4.3f * size, color);
			draw_rect(x - .7, y + size * 2.f, half_size * 3.6, half_size, color);
			draw_rect(x - .7, y - size * 2.f, half_size * 3.6, half_size, color);
			draw_rect(x - .7, y, half_size * 3.6, half_size, color);
			x -= size * 4.f;
		} break;

		case 9: {
			draw_rect(x - half_size, y + size * 2.f, size * 1.5, half_size, color);
			draw_rect(x - half_size, y, size * 1.5, half_size, color);
			draw_rect(x - half_size, y - size * 2.f, size * 1.5, half_size, color);
			draw_rect(x + size, y - 1, half_size, 4.5f * size, color);
			draw_rect(x - size, y + size - .5, half_size, half_size * 5, color);
			x -= size * 4.f;
		} break;
		}

	}
}



//BUTTON CLASS
class Button
{
private:
	bool currentlyPressed;
	bool clicked;
	bool hover;
	double x;
	double y;
	double x1 = x - 100;
	double x2 = x + 100;
	double y1 = y - 100;
	double y2 = y + 100;
	int ID;
public:
	//Constructors
	Button()
	{
		x = -1;
		y = -1;
		currentlyPressed = false;
		clicked = false;
		hover = false;
	}
	Button(double xb, double yb, int IDx)
	{
		x = xb;
		y = yb;
		ID = IDx;
		x1 = x - 100;
		x2 = x + 100;
		y1 = y - 40;
		y2 = y + 40;
		currentlyPressed = false;
		clicked = false;
		hover = false;
	}
	//Accessors
	bool getPressed()
	{
		return currentlyPressed;
	}
	bool getClicked()
	{
		return clicked;
	}
	bool getHover()
	{
		return hover;
	}
	double getX()
	{
		return x;
	}
	double getY()
	{
		return y;
	}
	int getID()
	{
		return ID;
	}
	//Mutators
	void setPressed(bool tf)
	{
		currentlyPressed = tf;
	}
	void setClicked(bool tf)
	{
		clicked = tf;
	}
	void setHover(bool tf)
	{
		hover = tf;
	}
	//Other
	bool inBounds(double mx, double my)
	{
		if (mx > x1)
			if (mx < x2)
				if (my > y1)
					if (my < y2)
					{
						return true;
					}
		return false;
	}
	void printButton() //Origin center of the screen
	{
		draw_node(x, y, 100, 40, 0xfc9a30);
		draw_node(x, y, 95, 35, 0x190e47);
	}
};