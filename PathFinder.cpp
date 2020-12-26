#include <cmath>
#include <vector>
#include <string>
#include <algorithm>
#include <shellapi.h>

#define is_down(b) input->buttons[b].is_down
#define pressed(b) (input->buttons[b].is_down && input->buttons[b].changed)
#define released(b) (!input->buttons[b].is_down && input->buttons[b].changed)

const int infinty = 0;

bool LMouseClick = false;
bool RMouseClick = false;
bool numClick = false;

float posy = 500;
float posx = 500;
int count = 0;

bool placeNodes = false;
bool addEdges = false;
bool selectStart = false;
bool selectEnd = false;
bool calculate = false;
bool doDijkstra = true;
bool finished = false;
bool Menu = true;
bool clearBut = false;
bool getWeight = false;

double mx;
double my;

//Edge variables

std::vector<Node*> Master;

std::vector<Node*> tempPair;

std::vector<Node*> pair1 = {};
std::vector<Node*> pair2 = {};

std::vector<Node*> queue;

std::vector<int> path;

Node* start;
Node* end;

static int tick = 2;
static int tick2 = 2;

Button ExitAll(1810, 50, 4);
Button Restart(1600, 50, 4);

void MakeNode(bool &click, double mx, double my)
{
	if (click)
	{
		Node* newNode = new Node;
		newNode->setID(count);
		newNode->setX(mx);
		newNode->setY(my);
		newNode->setVisited(false);
		newNode->setBounds(mx - 19, 1040 - my - 19, mx + 19, 1040 - my + 19);
		Master.push_back(newNode);
		count++;
	}
	click = false;
	Sleep(100);
}

double slope;

Node* getNode(int num)
{
	for (int n = 0; n < Master.size(); n++)
		if (Master[n]->getID() == num)
			return Master[n];
}

void MakeEdge(Node* node1, Node* node2)
{
	double x1 = node1->getX();
	double y1 = node1->getY();
	double x2 = node2->getX();
	double y2 = node2->getY();

	slope = (y2 - y1) / (x2 - x1);

	double f_i;

	if (slope <= 1 && slope >= -1)
	{
		for (int i = (int)x1; i <= (int)x2; i++)
		{
			f_i = ((slope) * (i - x1)) + y1;
			draw_node(i, f_i, 6, 6, 0xffffff);
		}
		for (int i = (int)x1; i <= (int)x2; i++)
		{
			f_i = ((slope) * (i - x1)) + y1;
			draw_node(i, f_i, 5, 5, 0x000000);
		}
		if (path.size() > 0)
			for (int j = 0; j < path.size() - 1; j++)
				if ((node1->getID() == path[j] && node2->getID() == path[j + 1]) || (node2->getID() == path[j] && node1->getID() == path[j + 1]))
				{
					for (int k = (int)x1; k <= (int)x2; k++)
					{
						f_i = ((slope) * (k - x1)) + y1;
						draw_node(k, f_i, 6, 6, 0xfcb103);
					}
					for (int k = (int)x1; k <= (int)x2; k++)
					{
						f_i = ((slope) * (k - x1)) + y1;
						draw_node(k, f_i, 5, 5, 0x000000);
					}
				}
	}
	else
	{
		if (node2->getY() > node1->getY())
		{
			Node* temp = node1;
			node1 = node2;
			node2 = temp;
			//node1 has a larger y
		}
		slope = (x2 - x1) / (y2 - y1);
		if (node2->getX() > node1->getX())
		{
			for (int i = (int)y2; i <= (int)y1; i++)
			{
				f_i = (slope) * (i - y2) + x2;
				draw_node(f_i, i, 6, 6, 0xffffff);
			}
			for (int i = (int)y2; i <= (int)y1; i++)
			{
				f_i = (slope) * (i - y2) + x2;
				draw_node(f_i, i, 5, 5, 0x000000);
			}
			if (path.size() > 0)
				for (int j = 0; j < path.size() - 1; j++)
					if ((node1->getID() == path[j] && node2->getID() == path[j + 1]) || (node2->getID() == path[j] && node1->getID() == path[j + 1]))
					{
						for (int k = (int)y2; k <= (int)y1; k++)
						{
							f_i = (slope) * (k - y2) + x2;
							draw_node(f_i, k, 6, 6, 0xfcb103);
						}
						for (int k = (int)y2; k <= (int)y1; k++)
						{
							f_i = (slope) * (k - y2) + x2;
							draw_node(f_i, k, 5, 5, 0x000000);
						}
					}
		}
		else
		{
			for (int i = (int)y1; i <= (int)y2; i++)
			{
				f_i = (slope) * (i - y1) + x1;
				draw_node(f_i, i, 6, 6, 0xffffff);
			}
			for (int i = (int)y1; i <= (int)y2; i++)
			{
				f_i = (slope) * (i - y1) + x1;
				draw_node(f_i, i, 5, 5, 0x000000);
			}
			if (path.size() > 0)
				for (int j = 0; j < path.size() - 1; j++)
					if ((node1->getID() == path[j] && node2->getID() == path[j + 1]) || (node2->getID() == path[j] && node1->getID() == path[j + 1]))
					{
						for (int k = (int)y1; k <= (int)y2; k++)
						{
							f_i = (slope) * (k - y1) + x1;
							draw_node(f_i, k, 6, 6, 0xfcb103);
						}
						for (int k = (int)y1; k <= (int)y2; k++)
						{
							f_i = (slope) * (k - y1) + x1;
							draw_node(f_i, k, 5, 5, 0x000000);
						}
					}
		}
	}
}

void MakePathEdge(int node1, int node2)
{
	double x1 = getNode(node1)->getX();
	double y1 = getNode(node1)->getY();
	double x2 = getNode(node2)->getX();
	double y2 = getNode(node2)->getY();

	slope = (y2 - y1) / (x2 - x1);

	double f_i;

	if (slope <= 1 && slope >= -1)
	{
		for (int i = (int)x1; i <= (int)x2; i++)
		{
			f_i = ((slope) * (i - x1)) + y1;
			draw_node(i, f_i, 6, 6, 0xfcb103);
		}
		for (int i = (int)x1; i <= (int)x2; i++)
		{
			f_i = ((slope) * (i - x1)) + y1;
			draw_node(i, f_i, 5, 5, 0x000000);
		}
	}
	else
	{
		if (getNode(node2)->getY() > getNode(node1)->getY())
		{
			int temp = node1;
			node1 = node2;
			node2 = temp;
			//node1 has a larger y
		}
		slope = (x2 - x1) / (y2 - y1);
		if (getNode(node2)->getX() > getNode(node1)->getX())
		{
			for (int i = (int)y2; i <= (int)y1; i++)
			{
				f_i = (slope) * (i - y2) + x2;
				draw_node(f_i, i, 6, 6, 0xfcb103);
			}
			for (int i = (int)y2; i <= (int)y1; i++)
			{
				f_i = (slope) * (i - y2) + x2;
				draw_node(f_i, i, 5, 5, 0x000000);
			}
		}
		else
		{
			for (int i = (int)y1; i <= (int)y2; i++)
			{
				f_i = (slope) * (i - y1) + x1;
				draw_node(f_i, i, 6, 6, 0xfcb103);
			}
			for (int i = (int)y1; i <= (int)y2; i++)
			{
				f_i = (slope) * (i - y1) + x1;
				draw_node(f_i, i, 5, 5, 0x000000);
			}
		}
	}
}

void MakeEdgeMainMenu(Button node1, Button node2)
{
	double x1 = node1.getX();
	double y1 = node1.getY();
	double x2 = node2.getX();
	double y2 = node2.getY();

	slope = (y2 - y1) / (x2 - x1);

	double f_i;

	if (slope <= 1 && slope >= -1)
	{
		for (int i = (int)x1; i <= (int)x2; i++)
		{
			f_i = ((slope) * (i - x1)) + y1;
			draw_node(i, f_i, 6, 6, 0xffffff);
		}
		for (int i = (int)x1; i <= (int)x2; i++)
		{
			f_i = ((slope) * (i - x1)) + y1;
			draw_node(i, f_i, 5, 5, 0x000000);
		}
		if (path.size() > 0)
			for (int j = 0; j < path.size() - 1; j++)
				if ((node1.getID() == path[j] && node2.getID() == path[j + 1]) || (node2.getID() == path[j] && node1.getID() == path[j + 1]))
				{
					for (int k = (int)x1; k <= (int)x2; k++)
					{
						f_i = ((slope) * (k - x1)) + y1;
						draw_node(k, f_i, 6, 6, 0xfcb103);
					}
					for (int k = (int)x1; k <= (int)x2; k++)
					{
						f_i = ((slope) * (k - x1)) + y1;
						draw_node(k, f_i, 5, 5, 0x000000);
					}
				}
	}
	else
	{
		if (node2.getY() > node1.getY())
		{
			Button temp = node1;
			node1 = node2;
			node2 = temp;
			//node1 has a larger y
		}
		slope = (x2 - x1) / (y2 - y1);
		if (node2.getX() > node1.getX())
		{
			for (int i = (int)y2; i <= (int)y1; i++)
			{
				f_i = (slope) * (i - y2) + x2;
				draw_node(f_i, i, 6, 6, 0xffffff);
			}
			for (int i = (int)y2; i <= (int)y1; i++)
			{
				f_i = (slope) * (i - y2) + x2;
				draw_node(f_i, i, 5, 5, 0x000000);
			}
			if (path.size() > 0)
				for (int j = 0; j < path.size() - 1; j++)
					if ((node1.getID() == path[j] && node2.getID() == path[j + 1]) || (node2.getID() == path[j] && node1.getID() == path[j + 1]))
					{
						for (int k = (int)y2; k <= (int)y1; k++)
						{
							f_i = (slope) * (k - y2) + x2;
							draw_node(f_i, k, 6, 6, 0xfcb103);
						}
						for (int k = (int)y2; k <= (int)y1; k++)
						{
							f_i = (slope) * (k - y2) + x2;
							draw_node(f_i, k, 5, 5, 0x000000);
						}
					}
		}
		else
		{
			for (int i = (int)y1; i <= (int)y2; i++)
			{
				f_i = (slope) * (i - y1) + x1;
				draw_node(f_i, i, 6, 6, 0xffffff);
			}
			for (int i = (int)y1; i <= (int)y2; i++)
			{
				f_i = (slope) * (i - y1) + x1;
				draw_node(f_i, i, 5, 5, 0x000000);
			}
			if (path.size() > 0)
				for (int j = 0; j < path.size() - 1; j++)
					if ((node1.getID() == path[j] && node2.getID() == path[j + 1]) || (node2.getID() == path[j] && node1.getID() == path[j + 1]))
					{
						for (int k = (int)y1; k <= (int)y2; k++)
						{
							f_i = (slope) * (k - y1) + x1;
							draw_node(f_i, k, 6, 6, 0xfcb103);
						}
						for (int k = (int)y1; k <= (int)y2; k++)
						{
							f_i = (slope) * (k - y1) + x1;
							draw_node(f_i, k, 5, 5, 0x000000);
						}
					}
		}
	}
}

Node* addEdge(double mx, double my)
{
	for (int i = 0; i < Master.size(); i++)
	{
		if ((mx > getNode(i)->getX0()) && (mx < getNode(i)->getX1()) && (my > getNode(i)->getY0()) && (my < getNode(i)->getY1()))
			if (getNode(i)->getX0() != getNode(i)->getX1() && getNode(i)->getY0() != getNode(i)->getY1())
				return getNode(i);
	}
}

std::vector<std::vector<int>> weights;

void recordWeight(Node node1, Node node2, std::vector<std::vector<int>>& weights, Input* input);

//Sort
void sort(std::vector<Node*> &queue, std::vector<Node*> Master) //Bubble sort
{
	bool isSorted = false;
	while (!isSorted)
	{
		isSorted = true;
		for (int i = 0; i < queue.size() - 1; i++)
		{
			if (queue[i]->getNodeDist() > queue[i + 1]->getNodeDist())
			{
				std::swap(queue[i], queue[i + 1]);
				isSorted = false;
			}
		}
	}
}

//Sort Master
void sortMaster(std::vector<Node*> &Master) //Bubble sort
{
	bool isSorted = false;
	while (!isSorted)
	{
		isSorted = true;
		for (int i = 0; i < Master.size() - 1; i++)
		{
			if (Master[i]->getNodeDist() > Master[i + 1]->getNodeDist())
			{
				std::swap(Master[i], Master[i + 1]);
				isSorted = false;
			}
		}
	}
}

//CheckQ
bool checkQ(std::vector<Node*> queue, int neighbor)
{
	for (int i = 0; i < queue.size(); i++)
	{
		if (queue[i]->getID() == neighbor)
		{
			return true;
		}
	}
	return false;
}

//CheckDist
int checkDist(int neighbor, int current, std::vector<std::vector<int>> weights, std::vector<Node*> Master)
{
	int currentdist = getNode(current)->getNodeDist();
	int newdist = currentdist + weights[current][neighbor];
	if (newdist <= getNode(neighbor)->getNodeDist())
		return newdist;
	return getNode(neighbor)->getNodeDist();
}

//Dijkstra
int dijkstra(std::vector<std::vector<int>> weights, std::vector<Node*> &queue, Node* end, std::vector<Node*> &Master, int current)
{
	//get a current node that is in view
	std::vector<int> neighborList; 
	getNode(current)->sortNeighbors();
	for (int i = 0; i < getNode(current)->getNeighbors().size(); i++)
	{
		neighborList.push_back(getNode(current)->getNeighbors()[i]);
	}

	for (int i = 0; i < neighborList.size(); i++)
	{
		int neighbor = neighborList[i];

		if ((!checkQ(queue, neighbor)) && (getNode(neighbor)->getVisited() == false)) //NEW NEIGHBORS
		{
			int Dist = checkDist(neighbor, current, weights, Master);
			getNode(neighbor)->setVisited(true);
			getNode(neighbor)->setNodeDist(Dist);
			getNode(neighbor)->setParent(current);
			queue.push_back(getNode(neighbor));
		}

		else if (checkQ(queue, neighbor)) //OLD NEIGHBORS
		{
			int Dist = checkDist(neighbor, current, weights, Master);
			getNode(neighbor)->setNodeDist(Dist);
			if (Dist <= getNode(neighbor)->getNodeDist())
			{
				getNode(neighbor)->setParent(current);
			}
			draw_text("OLD", 9, 30, .5, 0xfcb103);
		}
	}

	//remove current from queue
	bool removeNode = true;

	for (int k = 0; k < neighborList.size(); k++)
	{
		if (getNode(getNode(current)->getNeighbor(k))->getVisited() == false)
			removeNode = false;
	}
	if (removeNode)
	{
		for (int i = 0; i < queue.size(); i++)
		{
			if (queue[i]->getID() == current)
			{
				for (int j = i; j < queue.size() - 1; j++)
				{
					queue[j] = queue[j + 1];
				}
			}
		}
		queue.pop_back();
	}

	//Sort
	sort(queue, Master);

	int newCurrent = queue[0]->getID();

	return newCurrent;
}

void highlight()
{
	for (int i = 0; i < path.size(); i++)
	{
		if (path[i] == start->getID())
			draw_node(getNode(path[i])->getX(), getNode(path[i])->getY(), 22, 22, 0x00d118);
		else if (path[i] == end->getID())
			draw_node(getNode(path[i])->getX(), getNode(path[i])->getY(), 22, 22, 0xd40000);
		else
			draw_node(getNode(path[i])->getX(), getNode(path[i])->getY(), 22, 22, 0xfcb103);
	}
}

static int URL = 1;

static int tempCounter;
std::vector<Button> ButtonList;

//"MAIN FUNCTION"
internal void simulate(Input* input)
{
	clear_screen(0x1c2047);

	if (GetAsyncKeyState(VK_LBUTTON))
	{
		LMouseClick = true;
	}
	if (GetAsyncKeyState(VK_RBUTTON))
	{
		RMouseClick = true;
	}

	POINT cursorPos;
	GetCursorPos(&cursorPos);

	mx = (double)cursorPos.x;
	my = (double)cursorPos.y;

	if (Menu == true)
	{
		clear_screen(0x1c2047);
		draw_text("PATH FINDING WITH DIJKSTRAS ALGORITHM", 10, 92, 1, 0xfc9a30);
		draw_text("BY RAAFI RAHMAN", 42, 84, .5, 0xfc9a30);
		
		Button Start(970, 700, 0);
		ButtonList.push_back(Start);

		Button ExitMain(970, 300, 1);
		ButtonList.push_back(ExitMain);

		Button About(570, 500, 2);
		ButtonList.push_back(About);

		Button Contact(1370, 500, 3);
		ButtonList.push_back(Contact);

		MakeEdgeMainMenu(ExitMain, Start);
		MakeEdgeMainMenu(About, Start);
		MakeEdgeMainMenu(Start, Contact);
		MakeEdgeMainMenu(About, ExitMain);
		MakeEdgeMainMenu(ExitMain, Contact);
		MakeEdgeMainMenu(About, Contact);

		Start.printButton();
		ExitMain.printButton();
		About.printButton();
		Contact.printButton();
		printNode(970, 500);

		draw_text("START", 48, 69.5, .5, 0xffffff);
		draw_text("EXIT", 48.5, 30.2, .5, 0xffffff);
		draw_text("ABOUT", 26.7, 49.8, .5, 0xffffff);
		draw_text("CONTACT", 67.6, 49.8, .5, 0xffffff);

		if (LMouseClick)
		{
			//Start
			if (mx > 870)
			{
				if (mx < 1070)
				{
					if (my > 300)
					{
						if (my < 380)
						{
							clear_screen(0x1c2047);
							Sleep(100);
							Menu = false;
							placeNodes = true;
						}
					}
				}
			}
			//Exit
			if (mx > 870)
			{
				if (mx < 1070)
				{
					if (my > 700)
					{
						if (my < 780)
						{
							clear_screen(0x1c2047);
							Sleep(100);
							exit(0);
						}
					}
				}
			}
			//About
			if (mx > 470)
			{
				if (mx < 670)
				{
					if (my > 500)
					{
						if (my < 580)
						{
							ShellExecute(0, 0, "http://raafi101.pythonanywhere.com/", 0, 0, SW_SHOW);
							LMouseClick = false;
							Sleep(200);
						}
					}
				}
			}
			//Contact
			if (mx > 1270)
			{
				if (mx < 1470)
				{
					if (my > 500)
					{
						if (my < 580)
						{
							ShellExecute(0, 0, "http://raafi101.pythonanywhere.com/contact/", 0, 0, SW_SHOW);
							LMouseClick = false;
							Sleep(200);
						}
					}
				}
			}
			LMouseClick = false;
		}
	}

	else if (placeNodes == true) //Place Node phase
	{
		if (LMouseClick)
		{
			if (mx > 1710)
			{
				if (mx < 1910)
				{
					if (my > 940)
					{
						if (my < 1030)
						{
							exit(0);
						}
					}
				}
			}
			else if (mx > 1500)
			{
				if (mx < 1700)
				{
					if (my > 940)
					{
						if (my < 1030)
						{
							clearBut = true;
							if (clearBut)
							{
								placeNodes = true;
								int nodeDel = Master.size();
								for (int i = 0; i < nodeDel; i++)
								{
									Master.pop_back();
									count--;
								}
								tempPair.clear();
								pair1.clear();
								pair2.clear();
								queue.clear();
								path.clear();
								LMouseClick = false;
							}
						}
					}
				}
			}
		}
		if (!clearBut)
			MakeNode(LMouseClick, mx, 1040 - my);
		if (RMouseClick)
		{
			if (Master.size() > 0)
			{
				Master.pop_back();
				count--;
				RMouseClick = false;
			}
		}
		if (is_down(BUTTON_ENTER))
		{
			placeNodes = false;
			addEdges = true;
			Sleep(50);
		}
		clearBut = false;
	}

	else if (addEdges == true) //Add edges phase
	{
		if (LMouseClick)
		{
			if (mx > 1710)
			{
				if (mx < 1910)
				{
					if (my > 940)
					{
						if (my < 1030)
						{
							exit(0);
						}
					}
				}
			}
			if (mx > 1500)
			{
				if (mx < 1700)
				{
					if (my > 940)
					{
						if (my < 1030)
						{
							addEdges = false;
							placeNodes = true;
							int nodeDel = Master.size();
							for (int i = 0; i < nodeDel; i++)
							{
								Master.pop_back();
								count--;
							}
							tempPair.clear();
							pair1.clear();
							pair2.clear();
							queue.clear();
							path.clear();
						}
					}
				}
			}
		}
		int numOfNodes = Master.size(); //Number of Nodes for Dijkstra algorithm. You can get their ids
		weights.resize(numOfNodes, std::vector<int>(numOfNodes));
		if (tempPair.size() < 2)
		{
			if (tempPair.size() == 1)
				draw_text("ONE NODE SELECTED", 2, 88, .5, 0xffffff);
			if (is_down(BUTTON_ESCAPE))
			{
				tempPair.clear();
			}
			if (LMouseClick)
			{
				for (int i = 0; i < Master.size(); i++)
				{
					if (mx > getNode(i)->getX0())
					{
						if (mx < getNode(i)->getX1())
						{
							if (my > getNode(i)->getY0())
							{
								if (my < getNode(i)->getY1())
								{
									if (tempPair.size() == 1)
									{
										if ((getNode(i)->getX() != tempPair[0]->getX()) && (getNode(i)->getY() != tempPair[0]->getY()))
										{
											tempPair.push_back(getNode(i));
											Sleep(100);
										}
									}
									else
									{
										tempPair.push_back(getNode(i));
									}
								}
							}
						}
					}
				}
			}
			LMouseClick = false;
		}
		else
		{
			draw_text("TWO NODES SELECTED. ASSIGN WEIGHT...", 2, 88, .5, 0xffffff);
			getWeight = false;

			if (is_down(BUTTON_ESCAPE))
			{
				tempPair.clear();
				getWeight = false;
			}
			else if (is_down(ZERO))//0
			{
				weights[tempPair[0]->getID()][tempPair[1]->getID()] = 0;
				weights[tempPair[1]->getID()][tempPair[0]->getID()] = 0;
				tempCounter = 1;
			}
			else if (is_down(ONE))//1
			{
				weights[tempPair[0]->getID()][tempPair[1]->getID()] = 1;
				weights[tempPair[1]->getID()][tempPair[0]->getID()] = 1;
				tempCounter = 1;
			}
			else if (is_down(TWO))//2
			{
				weights[tempPair[0]->getID()][tempPair[1]->getID()] = 2;
				weights[tempPair[1]->getID()][tempPair[0]->getID()] = 2;
				tempCounter = 1;
			}
			else if (is_down(THREE))//3
			{
				weights[tempPair[0]->getID()][tempPair[1]->getID()] = 3;
				weights[tempPair[1]->getID()][tempPair[0]->getID()] = 3;
				tempCounter = 1;
			}
			else if (is_down(FOUR))//4
			{
				weights[tempPair[0]->getID()][tempPair[1]->getID()] = 4;
				weights[tempPair[1]->getID()][tempPair[0]->getID()] = 4;
				tempCounter = 1;
			}
			else if (is_down(FIVE))//5
			{
				weights[tempPair[0]->getID()][tempPair[1]->getID()] = 5;
				weights[tempPair[1]->getID()][tempPair[0]->getID()] = 5;
				tempCounter = 1;
			}
			else if (is_down(SIX))//6
			{
				weights[tempPair[0]->getID()][tempPair[1]->getID()] = 6;
				weights[tempPair[1]->getID()][tempPair[0]->getID()] = 6;
				tempCounter = 1;
			}
			else if (is_down(SEVEN))//7
			{
				weights[tempPair[0]->getID()][tempPair[1]->getID()] = 7;
				weights[tempPair[1]->getID()][tempPair[0]->getID()] = 7;
				tempCounter = 1;
			}
			else if (is_down(EIGHT))//8
			{
				weights[tempPair[0]->getID()][tempPair[1]->getID()] = 8;
				weights[tempPair[1]->getID()][tempPair[0]->getID()] = 8;
				tempCounter = 1;
			}
			else if (is_down(NINE))//9
			{
				weights[tempPair[0]->getID()][tempPair[1]->getID()] = 9;
				weights[tempPair[1]->getID()][tempPair[0]->getID()] = 9;
				tempCounter = 1;
			}

			if (tempCounter == 1)
			{
				getNode(tempPair[0]->getID())->addNeighbors(tempPair[1]->getID());
				getNode(tempPair[1]->getID())->addNeighbors(tempPair[0]->getID());
				tempCounter = 0;
				getWeight = true;
			}

			if (getWeight)
			{
				if (tempPair[0]->getX() > tempPair[1]->getX())
				{
					pair1.push_back(tempPair[1]);
					pair2.push_back(tempPair[0]);
				}
				else if (tempPair[1]->getX() > tempPair[0]->getX())
				{
					pair1.push_back(tempPair[0]);
					pair2.push_back(tempPair[1]);
				}
				tempPair.clear();
			}
		}
		if (is_down(BUTTON_ENTER))
		{
			addEdges = false;
			selectStart = true;
		}
	}

	else if (selectStart == true) //Select Start
	{
		if (LMouseClick)
		{
			if (mx > 1710)
			{
				if (mx < 1910)
				{
					if (my > 940)
					{
						if (my < 1030)
						{
							exit(0);
						}
					}
				}
			}
			if (mx > 1500)
			{
				if (mx < 1700)
				{
					if (my > 940)
					{
						if (my < 1030)
						{
							selectStart = false;
							placeNodes = true;
							int nodeDel = Master.size();
							for (int i = 0; i < nodeDel; i++)
							{
								Master.pop_back();
								count--;
							}
							tempPair.clear();
							pair1.clear();
							pair2.clear();
							queue.clear();
							path.clear();
						}
					}
				}
			}
		}
		for (int i = 0; i < Master.size(); i++)
			for (int j = 0; j < Master.size(); j++)
			{
				if (weights[i][j] != 0)
					if (weights[i][j] != 1)
						if (weights[i][j] != 2)
							if (weights[i][j] != 3)
								if (weights[i][j] != 4)
									if (weights[i][j] != 5)
										if (weights[i][j] != 6)
											if (weights[i][j] != 7)
												if (weights[i][j] != 8)
													if (weights[i][j] != 9)
														weights[i][j] = infinty;
			}
		if (LMouseClick)
		{
			for (int i = 0; i < Master.size(); i++)
			{
				if (mx > getNode(i)->getX0())
				{
					if (mx < getNode(i)->getX1())
					{
						if (my > getNode(i)->getY0())
						{
							if (my < getNode(i)->getY1())
							{
								start = getNode(i);
								selectStart = false;
								selectEnd = true;
								LMouseClick = false;
								Sleep(100);
							}
						}
					}
				}
			}
		}
	}

	else if (selectEnd == true)
	{
		if (LMouseClick)
		{
			if (mx > 1710)
			{
				if (mx < 1910)
				{
					if (my > 940)
					{
						if (my < 1030)
						{
							exit(0);
						}
					}
				}
			}
			if (mx > 1500)
			{
				if (mx < 1700)
				{
					if (my > 940)
					{
						if (my < 1030)
						{
							selectEnd = false;
							placeNodes = true;
							int nodeDel = Master.size();
							for (int i = 0; i < nodeDel; i++)
							{
								Master.pop_back();
								count--;
							}
							tempPair.clear();
							pair1.clear();
							pair2.clear();
							queue.clear();
							path.clear();
						}
					}
				}
			}
		}
		if (LMouseClick)
		{
			for (int i = 0; i < Master.size(); i++)
			{
				if (mx > getNode(i)->getX0())
				{
					if (mx < getNode(i)->getX1())
					{
						if (my > getNode(i)->getY0())
						{
							if (my < getNode(i)->getY1())
							{
								end = getNode(i);
								selectEnd = false;
								calculate = true;
								LMouseClick = false;
								Sleep(100);
							}
						}
					}
				}
			}
		}
	}

	else if (calculate == true)
	{
		if (LMouseClick)
		{
			if (mx > 1710)
			{
				if (mx < 1910)
				{
					if (my > 940)
					{
						if (my < 1030)
						{
							exit(0);
						}
					}
				}
			}
			if (mx > 1500)
			{
				if (mx < 1700)
				{
					if (my > 940)
					{
						if (my < 1030)
						{
							calculate = false;
							placeNodes = true;
							int nodeDel = Master.size();
							for (int i = 0; i < nodeDel; i++)
							{
								Master.pop_back();
								count--;
							}
							tempPair.clear();
							pair1.clear();
							pair2.clear();
							queue.clear();
							path.clear();
						}
					}
				}
			}
		}
		if (tick > 1)
		{
			getNode(start->getID())->setNodeDist(0);
			getNode(start->getID())->setParent(getNode(start->getID())->getID());
			queue.push_back(getNode(start->getID()));
			getNode(start->getID())->setVisited(true);
			tick -= 2;
		}

		int topQ;
		if (doDijkstra == true)
		{
			static int current1 = queue[0]->getID();
			sortMaster(Master);
			topQ = dijkstra(weights, queue, end, Master, current1);
			if (end->getID() == topQ)
				doDijkstra = false;
			else
			{
				doDijkstra = true;
				current1 = queue[0]->getID();
			}
		}
		else if (doDijkstra == false)
		{
			bool pathEnd = false;
			if (tick2 > 1)
			{
				path.push_back(end->getID());
				tick2 -= 2;
			}
			while (!pathEnd)
			{
				static int node = end->getID(); //Last node
				if (node == getNode(node)->getParent())
				{
					pathEnd = true;
				}
				else
				{
					path.push_back(getNode(node)->getParent());
					node = getNode(node)->getParent();
				}
			}
		}
	}

	//RENDERING STUFF
	if (pair1.size() > 0)
	{
		for (int i = 0; i < pair1.size(); i++)
		{
			MakeEdge(pair1[i], pair2[i]);
			slope = (pair2[i]->getY() - pair1[i]->getY()) / (pair2[i]->getX() - pair1[i]->getX());
			double f_i;
			if (slope <= 1 && slope >= -1)
				draw_number(weights[pair1[i]->getID()][pair2[i]->getID()], (pair1[i]->getX() + pair2[i]->getX()) / (render_state.width * render_scale * 2) - .35, (pair1[i]->getY() + pair2[i]->getY()) / (render_state.height * render_scale * 2) + 3, .5, 0xffffff);
			else if (slope > -9999 && slope < -1)
				draw_number(weights[pair1[i]->getID()][pair2[i]->getID()], (pair1[i]->getX() + pair2[i]->getX()) / (render_state.width * render_scale * 2) + 1, (pair1[i]->getY() + pair2[i]->getY()) / (render_state.height * render_scale * 2) + .75, .5, 0xffffff);
			else
				draw_number(weights[pair1[i]->getID()][pair2[i]->getID()], (pair1[i]->getX() + pair2[i]->getX()) / (render_state.width * render_scale * 2) - 1.5, (pair1[i]->getY() + pair2[i]->getY()) / (render_state.height * render_scale * 2) + .75, .5, 0xffffff);
		}
	}

	highlight();

	for (int i = 0; i < Master.size(); i++)
	{
		getNode(i)->print();
	}

	if (Master.size() >= 1)
		draw_text("A", getNode(0)->getX() / (render_state.width * render_scale) - .35, getNode(0)->getY() / (render_state.height * render_scale) + .65, .5, 0xffffff);
	if (Master.size() >= 2)
		draw_text("B", getNode(1)->getX() / (render_state.width * render_scale) - .35, getNode(1)->getY() / (render_state.height * render_scale) + .65, .5, 0xffffff);
	if (Master.size() >= 3)
		draw_text("C", getNode(2)->getX() / (render_state.width * render_scale) - .35, getNode(2)->getY() / (render_state.height * render_scale) + .65, .5, 0xffffff);
	if (Master.size() >= 4)
		draw_text("D", getNode(3)->getX() / (render_state.width * render_scale) - .35, getNode(3)->getY() / (render_state.height * render_scale) + .65, .5, 0xffffff);
	if (Master.size() >= 5)
		draw_text("E", getNode(4)->getX() / (render_state.width * render_scale) - .35, getNode(4)->getY() / (render_state.height * render_scale) + .65, .5, 0xffffff);
	if (Master.size() >= 6)
		draw_text("F", getNode(5)->getX() / (render_state.width * render_scale) - .35, getNode(5)->getY() / (render_state.height * render_scale) + .65, .5, 0xffffff);
	if (Master.size() >= 7)
		draw_text("G", getNode(6)->getX() / (render_state.width * render_scale) - .35, getNode(6)->getY() / (render_state.height * render_scale) + .65, .5, 0xffffff);
	if (Master.size() >= 8)
		draw_text("H", getNode(7)->getX() / (render_state.width * render_scale) - .35, getNode(7)->getY() / (render_state.height * render_scale) + .65, .5, 0xffffff);
	if (Master.size() >= 9)
		draw_text("I", getNode(8)->getX() / (render_state.width * render_scale) - .35, getNode(8)->getY() / (render_state.height * render_scale) + .65, .5, 0xffffff);
	if (Master.size() >= 10)
		draw_text("J", getNode(9)->getX() / (render_state.width * render_scale) - .35, getNode(9)->getY() / (render_state.height * render_scale) + .65, .5, 0xffffff);
	if (Master.size() >= 11)
		draw_text("K", getNode(10)->getX() / (render_state.width * render_scale) - .35, getNode(10)->getY() / (render_state.height * render_scale) + .65, .5, 0xffffff);
	if (Master.size() >= 12)
		draw_text("L", getNode(11)->getX() / (render_state.width * render_scale) - .35, getNode(11)->getY() / (render_state.height * render_scale) + .65, .5, 0xffffff);
	if (Master.size() >= 13)
		draw_text("M", getNode(12)->getX() / (render_state.width * render_scale) - .35, getNode(12)->getY() / (render_state.height * render_scale) + .65, .5, 0xffffff);
	if (Master.size() >= 14)
		draw_text("N", getNode(13)->getX() / (render_state.width * render_scale) - .35, getNode(13)->getY() / (render_state.height * render_scale) + .65, .5, 0xffffff);
	if (Master.size() >= 15)
		draw_text("O", getNode(14)->getX() / (render_state.width * render_scale) - .35, getNode(14)->getY() / (render_state.height * render_scale) + .65, .5, 0xffffff);
	if (Master.size() >= 16)
		draw_text("P", getNode(15)->getX() / (render_state.width * render_scale) - .35, getNode(15)->getY() / (render_state.height * render_scale) + .65, .5, 0xffffff);
	if (Master.size() >= 17)
		draw_text("Q", getNode(16)->getX() / (render_state.width * render_scale) - .35, getNode(16)->getY() / (render_state.height * render_scale) + .65, .5, 0xffffff);
	if (Master.size() >= 18)
		draw_text("R", getNode(17)->getX() / (render_state.width * render_scale) - .35, getNode(17)->getY() / (render_state.height * render_scale) + .65, .5, 0xffffff);
	if (Master.size() >= 19)
		draw_text("S", getNode(18)->getX() / (render_state.width * render_scale) - .35, getNode(18)->getY() / (render_state.height * render_scale) + .65, .5, 0xffffff);
	if (Master.size() >= 20)
		draw_text("T", getNode(19)->getX() / (render_state.width * render_scale) - .35, getNode(19)->getY() / (render_state.height * render_scale) + .65, .5, 0xffffff);
	if (Master.size() >= 21)
		draw_text("U", getNode(20)->getX() / (render_state.width * render_scale) - .35, getNode(20)->getY() / (render_state.height * render_scale) + .65, .5, 0xffffff);
	if (Master.size() >= 22)
		draw_text("V", getNode(21)->getX() / (render_state.width * render_scale) - .35, getNode(21)->getY() / (render_state.height * render_scale) + .65, .5, 0xffffff);
	if (Master.size() >= 23)
		draw_text("W", getNode(22)->getX() / (render_state.width * render_scale) - .35, getNode(22)->getY() / (render_state.height * render_scale) + .65, .5, 0xffffff);
	if (Master.size() >= 24)
		draw_text("X", getNode(23)->getX() / (render_state.width * render_scale) - .35, getNode(23)->getY() / (render_state.height * render_scale) + .65, .5, 0xffffff);
	if (Master.size() >= 25)
		draw_text("Y", getNode(24)->getX() / (render_state.width * render_scale) - .35, getNode(24)->getY() / (render_state.height * render_scale) + .65, .5, 0xffffff);
	if (Master.size() >= 26)
		draw_text("Z", getNode(25)->getX() / (render_state.width * render_scale) - .35, getNode(25)->getY() / (render_state.height * render_scale) + .65, .5, 0xffffff);

	if (Menu == false)
	{
		//Fixed Banner while running dijkstra
		draw_rect(0, 89, 100, 10, 0x190e47);
		draw_rect(0, 90, 100, 10, 0xad8c69);
		ExitAll.printButton();
		Restart.printButton();
		draw_text("EXIT", 92.4, 5.5, .5, 0xffffff);
		draw_text("CLEAR", 80.6, 5.5, .5, 0xffffff);
	}
	if (placeNodes == true) //Place Node phase
	{
		draw_text("LEFT CLICK TO ADD NODE. RIGHT CLICK TO REMOVE. PRESS ENTER TO FINISH ADDING NODES.", 2, 98, .5, 0x000000);
		//draw_number(1234567890, 30, 80, .5, 0xffffff);
	}
	if (addEdges == true) //Add Edges
	{
		draw_text("NOW ADD NEIGHBORS. CLICK FIRST NODE AND THEN THE SECOND NODE TO MAKE A CONNECTION. THEN CHOOSE A NUMBER FROM", 2, 98, .4, 0x000000);
		draw_text("ZERO TO NINE TO ADD THE WEIGHT. CLICK ESCAPE TO UNSELECT NODE. CLICK ENTER TO CONTINUE.", 2, 94, .4, 0x000000);
	}
	if (selectStart == true) //slect start
	{
		draw_text("LEFT CLICK ON STARTING NODE.", 2, 98, .5, 0x000000);
	}
	if (selectEnd == true) //select end
	{
		draw_text("LEFT CLICK ON ENDING NODE.", 2, 98, .5, 0x000000);
	}
	if (calculate == true) //calculate route
	{
		draw_text("HERE IS YOUR ROUTE.", 2, 98, .5, 0x000000);
		draw_text("ROUTE ORDER IS", 2, 94, .5, 0x000000);
		for (int i = 0; i < path.size(); i++)
		{
			int letternum = path[path.size() - i - 1];
			if (letternum == 0)
				draw_text("A", 18.5 + (i*2), 94, .5, 0x000000);
			else if (letternum == 1)
				draw_text("B", 18.5 + (i*2), 94, .5, 0x000000);
			else if (letternum == 2)
				draw_text("C", 18.5 + (i*2), 94, .5, 0x000000);
			else if (letternum == 3)
				draw_text("D", 18.5 + (i*2), 94, .5, 0x000000);
			else if (letternum == 4)
				draw_text("E", 18.5 + (i*2), 94, .5, 0x000000);
			else if (letternum == 5)
				draw_text("F", 18.5 + (i*2), 94, .5, 0x000000);
			else if (letternum == 6)
				draw_text("G", 18.5 + (i*2), 94, .5, 0x000000);
			else if (letternum == 7)
				draw_text("H", 18.5 + (i*2), 94, .5, 0x000000);
			else if (letternum == 8)
				draw_text("I", 18.5 + (i*2), 94, .5, 0x000000);
			else if (letternum == 9)
				draw_text("J", 18.5 + (i*2), 94, .5, 0x000000);
			else if (letternum == 10)
				draw_text("K", 18.5 + (i*2), 94, .5, 0x000000);
			else if (letternum == 11)
				draw_text("L", 18.5 + (i*2), 94, .5, 0x000000);
			else if (letternum == 12)
				draw_text("M", 18.5 + (i*2), 94, .5, 0x000000);
			else if (letternum == 13)
				draw_text("N", 18.5 + (i*2), 94, .5, 0x000000);
			else if (letternum == 14)
				draw_text("O", 18.5 + (i*2), 94, .5, 0x000000);
			else if (letternum == 15)
				draw_text("P", 18.5 + (i*2), 94, .5, 0x000000);
			else if (letternum == 16)
				draw_text("Q", 18.5 + (i*2), 94, .5, 0x000000);
			else if (letternum == 17)
				draw_text("R", 18.5 + (i*2), 94, .5, 0x000000);
			else if (letternum == 18)
				draw_text("S", 18.5 + (i*2), 94, .5, 0x000000);
			else if (letternum == 19)
				draw_text("T", 18.5 + (i*2), 94, .5, 0x000000);
			else if (letternum == 20)
				draw_text("U", 18.5 + (i*2), 94, .5, 0x000000);
			else if (letternum == 21)
				draw_text("V", 18.5 + (i*2), 94, .5, 0x000000);
			else if (letternum == 22)
				draw_text("W", 18.5 + (i*2), 94, .5, 0x000000);
			else if (letternum == 23)
				draw_text("X", 18.5 + (i*2), 94, .5, 0x000000);
			else if (letternum == 24)
				draw_text("Y", 18.5 + (i*2), 94, .5, 0x000000);
			else if (letternum == 25)
				draw_text("Z", 18.5 + (i*2), 94, .5, 0x000000);
			if (i != path.size() - 1)
				draw_text("/", 18.5 + (i*2) + 1, 94, .5, 0x000000);
		}
		/*
		if (getNode(0)->getNeighbors().size() == 0)
			draw_text("ZERO NEIGHBOR", 9, 45, .5, 0xfcb103);
		else if (getNode(0)->getNeighbors().size() == 1)
			draw_text("ONE NEIGHBOR", 9, 45, .5, 0xfcb103);
		else if (getNode(0)->getNeighbors().size() == 2)
			draw_text("TWO NEIGHBOR", 9, 45, .5, 0xfcb103);
		else if (getNode(0)->getNeighbors().size() == 3)
			draw_text("THREE NEIGHBOR", 9, 45, .5, 0xfcb103);
		else if (getNode(0)->getNeighbors().size() == 4)
			draw_text("FOUR NEIGHBOR", 9, 45, .5, 0xfcb103);
		else if (getNode(0)->getNeighbors().size() == 5)
			draw_text("FIVE NEIGHBOR", 9, 45, .5, 0xfcb103);
		else if (getNode(0)->getNeighbors().size() == 6)
			draw_text("SIX NEIGHBOR", 9, 45, .5, 0xfcb103);
		else if (getNode(0)->getNeighbors().size() == 7)
			draw_text("SEVEN NEIGHBOR", 9, 45, .5, 0xfcb103);
		else if (getNode(0)->getNeighbors().size() == 8)
			draw_text("EIGHT NEIGHBOR", 9, 45, .5, 0xfcb103);
		else if (getNode(0)->getNeighbors().size() == 9)
			draw_text("NINE NEIGHBOR", 9, 45, .5, 0xfcb103);
		else if (getNode(0)->getNeighbors().size() == 10)
			draw_text("TEN NEIGHBOR", 9, 45, .5, 0xfcb103);
		else if (getNode(0)->getNeighbors().size() == 18)
			draw_text("EIGHTEEN NEIGHBOR", 9, 45, .5, 0xfcb103);
		else if (getNode(0)->getNeighbors().size() == 19)
			draw_text("NINETEEN NEIGHBOR", 9, 45, .5, 0xfcb103);
		*/
	}
}

void recordWeight(Node* node1, Node* node2, std::vector<std::vector<int>>& weights, Input* input)
{
	if (is_down(ZERO))//0
	{
		weights[node1->getID()][node2->getID()] = 0;
		weights[node2->getID()][node1->getID()] = 0;
	}
	else if (is_down(ONE))//1
	{
		weights[node1->getID()][node2->getID()] = 1;
		weights[node2->getID()][node1->getID()] = 1;
	}
	else if (is_down(TWO))//2
	{
		weights[node1->getID()][node2->getID()] = 2;
		weights[node2->getID()][node1->getID()] = 2;
	}
	else if (is_down(THREE))//3
	{
		weights[node1->getID()][node2->getID()] = 3;
		weights[node2->getID()][node1->getID()] = 3;
	}
	else if (is_down(FOUR))//4
	{
		weights[node1->getID()][node2->getID()] = 4;
		weights[node2->getID()][node1->getID()] = 4;
	}
	else if (is_down(FIVE))//5
	{
		weights[node1->getID()][node2->getID()] = 5;
		weights[node2->getID()][node1->getID()] = 5;
	}
	else if (is_down(SIX))//6
	{
		weights[node1->getID()][node2->getID()] = 6;
		weights[node2->getID()][node1->getID()] = 6;
	}
	else if (is_down(SEVEN))//7
	{
		weights[node1->getID()][node2->getID()] = 7;
		weights[node2->getID()][node1->getID()] = 7;
	}
	else if (is_down(EIGHT))//8
	{
		weights[node1->getID()][node2->getID()] = 8;
		weights[node2->getID()][node1->getID()] = 8;
	}
	else if (is_down(NINE))//9
	{
		weights[node1->getID()][node2->getID()] = 9;
		weights[node2->getID()][node1->getID()] = 9;
	}
	getWeight = true;
}

/*
void sort(std::vector<Node*>& queue, std::vector<Node*> Master) //Bubble sort
{
	bool isSorted = false;
	while (!isSorted)
	{
		isSorted = true;
		for (int i = 0; i < queue.size() - 1; i++)
		{
			if (getNode(queue)i]->getID()]->getNodeDist() > getNode(queue)i + 1]->getID()]->getNodeDist())
			{
				std::swap(queue[i], queue[i + 1]);
				isSorted = false;
			}
		}
	}
}

bool checkQ(std::vector<Node*> queue, Node* neighbor)
{
	for (int i = 0; i < queue.size(); i++)
	{
		if (queue[i]->getID() == neighbor->getID())
		{
			return true;
		}
	}
	return false;
}

int checkDist(Node* neighbor, Node* current, std::vector<std::vector<int>> weights, std::vector<Node*> Master)
{
	int currentdist = current->getNodeDist();
	int newdist = currentdist + weights[current->getID()][neighbor->getID()];
	if (newdist < getNode(neighbor-)getID()]->getNodeDist())
		return newdist;
	return getNode(neighbor-)getID()]->getNodeDist();
}

int dijkstra() //make a 2d vector to hold onto 
{
	int static count2 = 0;
	//get a current node that is in view
	Node* current = queue[0];

	//Announce current
	std::vector<Node*> neighborList = getNode(current-)getID()]->getNeighbors();


	//take all new neighbors and add to queue and set node dist. Update old neighbors in queue iff new dist would be smaller than old dist
	for (int i = 0; i < neighborList.size(); i++)
	{
		Node* neighbor = getNode(current-)getID()]->getNeighbor(i);

		if (!checkQ(queue, neighbor))
			if (getNode(neighbor-)getID()]->getVisited() == false) //NEW NEIGHBORS
			{
				int Dist = checkDist(neighbor, current, weights, Master);
				neighbor->setVisited(true);
				getNode(neighbor-)getID()]->setVisited(true);
				neighbor->setNodeDist(Dist);
				getNode(neighbor-)getID()]->setNodeDist(Dist);
				neighbor->setParent(current->getID());
				getNode(neighbor-)getID()]->setParent(current->getID());
				queue.push_back(neighbor);
			}

			else if (checkQ(queue, neighbor))//OLD NEIGHBORS
			{
				int Dist = checkDist(neighbor, current, weights, Master);
				if (Dist < getNode(neighbor-)getID()]->getNodeDist())
				{
					neighbor->setParent(current->getID());
					neighbor->setNodeDist(Dist);
					getNode(neighbor-)getID()]->setParent(current->getID());
					getNode(neighbor-)getID()]->setNodeDist(Dist);
				}
				else
				{
					neighbor->setParent(neighbor->getParent());
					neighbor->setNodeDist(neighbor->getNodeDist());
				}
			}
	}

	//Sort
	sort(queue, Master);

	//remove current from queue
	for (int i = 0; i < queue.size(); i++)
	{
		if (queue[i] == current)
		{
			for (int j = i; j < queue.size(); j++)
			{
				queue[j] = queue[j + 1];
			}
		}
	}
	queue.pop_back();

	//Print out queue
	for (int i = 0; i < queue.size(); i++)
	{
		Node* node = getNode(queue)i]->getID()];
	}
	return count2++;
}
*/