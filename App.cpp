#include <GLFW/glfw3.h>
#include <vector>
#include <iostream>
#include <list>

#define Screen_Width 590
#define Screen_Height 590

bool LeftMousePressed = false;
bool SKeyPressed = false;
bool EKeyPressed = false;
bool StartPathfinding = false;

// To store coordinates
struct Point
{
    float x, y;

    void Add(float X, float Y)
    {
        x = X; y = Y;
    }

    bool operator==(const Point& o) const {
        return x == o.x && y == o.y;
    }

    bool operator<(const Point& o)  const {
        return x < o.x || (x == o.x && y < o.y);
    }
};


struct Quad
{
    float X, X1, Y, Y1;

    Quad()
    {
        X = 0; X1 = 0; Y = 0; Y1 = 0;
    }

    Quad(float x, float x1, float y, float y1)
    {
        X = x; X1 = x1; Y = y; Y1 = y1;
    }

    void SetQuad(float x, float x1, float y, float y1)
    {
        X = x; X1 = x1; Y = y; Y1 = y1;
    }

    bool operator==(const Quad& o) const {
        return X == o.X && X1 == o.X1 && Y == o.Y && Y1 == o.Y1;
    }

    bool operator<(const Quad& o)  const {
        return X < o.X || (X == o.X && Y < o.Y);
    }
};


class Node
{
public:
    
    bool Obstacle;
    float GlobalGoal;				
    float LocalGoal;				
    Quad m_Quad;
    Point Position;
    bool Initialized;
    bool Clicked;
    bool Visited;
    Node* Parent;
    std::vector<Node*> Neighbours;	
    
    Node()
    {
        m_Quad.SetQuad(0, 0, 0, 0);
        Obstacle = false;
        Position.x = 0; Position.y = 0;
        GlobalGoal = 0; LocalGoal = 0;
        Initialized = false;
        Clicked = false;
        Visited = false;
        Parent = nullptr;
    }
};

class Map
{
private:

    const int size = 20 , Rows = 20, Cols = 20;
    Point Scale;
    Point Cursor;
    Node* Nodes;
    Node* Start;
    Node* End;

public:

    Map()
    {
        Scale.x = 0; Scale.y = 0;

        Cursor.x = 0; Cursor.y = 0;

        Nodes = new Node[Rows * Cols];

        Start = nullptr;

        End = nullptr;

        for (int x = 0; x < Rows; x++)
        {
            for (int y = 0; y < Cols; y++)
            {
                Nodes[y * Rows + x].Position.x = x;

                Nodes[y * Rows + x].Position.y = y;

                Nodes[y * Rows + x].Obstacle = false;
                Nodes[y * Rows + x].Parent = nullptr;
                Nodes[y * Rows + x].Visited = false;

                if (y > 0)
                    Nodes[y * Rows + x].Neighbours.push_back(&Nodes[(y - 1) * Rows + (x + 0)]);
                if (y < Cols - 1)
                    Nodes[y * Rows + x].Neighbours.push_back(&Nodes[(y + 1) * Rows + (x + 0)]);
                if (x > 0)
                    Nodes[y * Rows + x].Neighbours.push_back(&Nodes[(y + 0) * Rows + (x - 1)]);
                if (x < Rows - 1)
                    Nodes[y * Rows + x].Neighbours.push_back(&Nodes[(y + 0) * Rows + (x + 1)]);

            }
        }

    }

    float distance(Node* a, Node* b)
    {
        // Pythagoras theorem
        return sqrtf((a->Position.x - b->Position.x) * (a->Position.x - b->Position.x)
            + (a->Position.y - b->Position.y) * (a->Position.y - b->Position.y));
    }

    void Solve_AStar()
    {
        // Reset states
        for (int x = 0; x < Rows; x++)
        {
            for (int y = 0; y < Cols; y++)
            {
                Nodes[y * Rows + x].Visited = false;
                Nodes[y * Rows + x].GlobalGoal = INFINITY;
                Nodes[y * Rows + x].LocalGoal = INFINITY;
                Nodes[y * Rows + x].Parent = nullptr;	
            }
        }

        
        Node* Current = Start;
        Start->LocalGoal = 0.0f;
        Start->GlobalGoal = distance(Start, End);

        std::list<Node*> listNotTestedNodes;
        listNotTestedNodes.push_back(Start);

        while (!listNotTestedNodes.empty() && Current != End)
        {
            // Sort Untested nodes by global goal
            listNotTestedNodes.sort([](const Node* lhs, const Node* rhs) { return lhs->GlobalGoal < rhs->GlobalGoal; });
           
            while (!listNotTestedNodes.empty() && listNotTestedNodes.front()->Visited)
            {
                listNotTestedNodes.pop_front();
            }

       
            if (listNotTestedNodes.empty())
            {
                break;
            }

            Current = listNotTestedNodes.front();
            Current->Visited = true; // Explored


            // Check neighbours...
            for (auto nodeNeighbour : Current->Neighbours)
            {
                if (!nodeNeighbour->Visited && nodeNeighbour->Obstacle == 0)
                {
                    listNotTestedNodes.push_back(nodeNeighbour);
                }

                // Calculate the neighbours potential lowest parent distance
                float PossibleLowerGoal = Current->LocalGoal + distance(Current, nodeNeighbour);

                if (PossibleLowerGoal < nodeNeighbour->LocalGoal)
                {
                    nodeNeighbour->Parent = Current;
                    nodeNeighbour->LocalGoal = PossibleLowerGoal;

                    // Adding heuristic + Local goal
                    nodeNeighbour->GlobalGoal = nodeNeighbour->LocalGoal + distance(nodeNeighbour, End);
                }
            }
        }
    }

    void DrawWorld()
    {
        Scale.x = 0; Scale.y = 0;

        float x = 0, y = 0;

        for (int i = 0; i < Rows; i++)
        {
            for (int j = 0; j < Cols; j++)
            {
                Scale.x = y * size; Scale.y = x * size;

                if (Nodes[j * Rows + i].Initialized == false)
                {
                    Nodes[j * Rows + i].m_Quad.SetQuad(Scale.x, Scale.x + size, Scale.y, Scale.y + size);

                    Nodes[j * Rows + i].Initialized = true;

                }

                if (Start == &Nodes[j * Rows + i])
                {
                    glColor4f(0, 1, 0, 1);
                    glBegin(GL_QUADS);
                    glVertex2f(Scale.x, Scale.y);
                    glVertex2f(Scale.x, Scale.y + size);
                    glVertex2f(Scale.x + size, Scale.y + size);
                    glVertex2f(Scale.x + size, Scale.y);
                    glEnd();
                }


                if (End == &Nodes[j * Rows + i])
                {
                    glColor4f(1, 0, 0, 1);
                    glBegin(GL_QUADS);
                    glVertex2f(Scale.x, Scale.y);
                    glVertex2f(Scale.x, Scale.y + size);
                    glVertex2f(Scale.x + size, Scale.y + size);
                    glVertex2f(Scale.x + size, Scale.y);
                    glEnd();
                }

                if (!Nodes[j * Rows + i].Clicked && Start != &Nodes[j * Rows + i] && End != &Nodes[j * Rows + i])
                {
                    glColor4f(0.3, 0.3, 0.3, 1);
                    glBegin(GL_QUADS);
                    glVertex2f(Scale.x, Scale.y);
                    glVertex2f(Scale.x, Scale.y + size);
                    glVertex2f(Scale.x + size, Scale.y + size);
                    glVertex2f(Scale.x + size, Scale.y);
                    glEnd();
                }

                if (Nodes[j * Rows + i].Clicked && Start != &Nodes[j * Rows + i] && End != &Nodes[j * Rows + i])
                {
                    glColor4f(0, 0, 1, 1);
                    glBegin(GL_QUADS);
                    glVertex2f(Scale.x, Scale.y);
                    glVertex2f(Scale.x, Scale.y + size);
                    glVertex2f(Scale.x + size, Scale.y + size);
                    glVertex2f(Scale.x + size, Scale.y);
                    glEnd();
                }

                y += 1.5;
            }

            x += 1.5;
            y = 0;
        }

       
    }
   
    bool CheckCollision(float Px, float Py, int& Index)
    {
        for (int itr = 0; itr < Rows * Cols; itr++)
        {
            // if collides
            if (Px >= Nodes[itr].m_Quad.X && Px <= Nodes[itr].m_Quad.X1
                && Py >= Nodes[itr].m_Quad.Y && Py <= Nodes[itr].m_Quad.Y1)
            {
                Index = itr;
                return false;
            }
        }

        // else
        return true;
    }

    void SetCursorPosition(GLFWwindow* window)
    {
        double X, Y;
        glfwGetCursorPos(window, &X, &Y);

        Cursor.x = X; Cursor.y = Y;
    }
    static void cursorPositionCallBack(GLFWwindow* window, double Xpos, double Ypos)
    {
        // For tracking Mouse Position
        //std::cout << "X : " << Xpos << " , Y : " << Ypos << std::endl;
    }

    void UpdateMap(GLFWwindow* window)
    {
        if (LeftMousePressed)
        {
            LeftMousePressed = false;

            int Index = 0;

            bool IsCollison = CheckCollision(Cursor.x, Cursor.y, Index);

            if (!IsCollison && !Nodes[Index].Clicked)
            {
                Nodes[Index].Clicked = true;
                Nodes[Index].Obstacle = true;
            }
            else if (!IsCollison && Nodes[Index].Clicked)
            {
                Nodes[Index].Clicked = false;
                Nodes[Index].Obstacle = false;
            }
        }

        else if (SKeyPressed)
        {

            SKeyPressed = false;

            int Index = 0;

            bool IsCollison = CheckCollision(Cursor.x, Cursor.y, Index);

            if (!IsCollison && !Nodes[Index].Clicked)
            {
                Nodes[Index].Clicked = true;

                if (Start != nullptr)
                {
                    Start->Clicked = false;
                }

                Start = &Nodes[Index];
            }

            else if (!IsCollison && Nodes[Index].Clicked && Start == &Nodes[Index])
            {
                Nodes[Index].Clicked = false;

                Start = nullptr;
            }

        }

        else if (EKeyPressed)
        {
            EKeyPressed = false;

            int Index = 0;

            bool IsCollison = CheckCollision(Cursor.x, Cursor.y, Index);

            if (!IsCollison && !Nodes[Index].Clicked)
            {
                Nodes[Index].Clicked = true;

                if (End != nullptr)
                {
                    End->Clicked = false;
                }

                End = &Nodes[Index];
            }

            else if (!IsCollison && Nodes[Index].Clicked && End == &Nodes[Index])
            {
                Nodes[Index].Clicked = false;

                End = nullptr;
            }

        }

        else if (StartPathfinding)
        {
            if (Start != nullptr && End != nullptr)
            {
               
                Solve_AStar();

                Node* TraceBack = End;

                while (TraceBack->Parent != nullptr)
                {
                    // Draw Ray
                    glColor4f(1, 1, 0, 1);
                    glLineWidth(5);
                    glBegin(GL_LINES);

                    glVertex2f(((TraceBack->m_Quad.X1 - TraceBack->m_Quad.X) / 2) + TraceBack->m_Quad.X,
                        ((TraceBack->m_Quad.Y1 - TraceBack->m_Quad.Y) / 2) + TraceBack->m_Quad.Y);
                    glVertex2f(((TraceBack->Parent->m_Quad.X1 - TraceBack->Parent->m_Quad.X) / 2) + TraceBack->Parent->m_Quad.X,
                        ((TraceBack->Parent->m_Quad.Y1 - TraceBack->Parent->m_Quad.Y) / 2) + TraceBack->Parent->m_Quad.Y);
                    glEnd();
                    // Set next node to this node's parent
                    TraceBack = TraceBack->Parent;
                }

            }

        }
        
    }

    static void mouseButtonCallBack(GLFWwindow* window, int Button, int action, int mods)
    {
        if (Button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
        {
            LeftMousePressed = true;
            
        }

    }

    static void KeyboardCallBack(GLFWwindow* window, int Key, int scancode, int action, int mods)
    {
        if (Key == GLFW_KEY_S && action == GLFW_PRESS)
        {
            SKeyPressed = true;

        }

        if (Key == GLFW_KEY_E && action == GLFW_PRESS)
        {
            EKeyPressed = true;

        }

        if (Key == GLFW_KEY_ENTER && action == GLFW_PRESS)
        {
            StartPathfinding = true;
        }

    }
};


int main(int argc, char** argv)
{
    GLFWwindow* window;


    /* Initialize the library */
    if (!glfwInit())
        return -1;


    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(Screen_Width, Screen_Height, "Path Finding", NULL, NULL);

    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    // Start window at center
    glfwSetWindowPos(window, Screen_Width - 100, Screen_Height - 450);
    
    Map World;

    glfwSetMouseButtonCallback(window, World.mouseButtonCallBack);
    glfwSetCursorPosCallback(window, World.cursorPositionCallBack);
    glfwSetKeyCallback(window, World.KeyboardCallBack);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        // White Background
        glClearColor(1, 1, 1, 0);

        // To draw
        glLoadIdentity();
        glOrtho(0, Screen_Width, Screen_Height, 0, 100, -100);

        World.DrawWorld();
        World.SetCursorPosition(window);
        World.UpdateMap(window);
        
        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}