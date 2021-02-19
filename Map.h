#pragma once

bool LeftMousePressed = false;
bool SKeyPressed = false;
bool EKeyPressed = false;
bool StartPathfinding = false;
bool QuitApp = false;

class Map
{
private:

    const int size = 20, Rows = round(Screen_Height * 0.0336), Cols = round(Screen_Width * 0.0336);
    Point Scale;
    Point Cursor;
    Node* Nodes;
    Node* Start;
    Node* End;
    Point Choices[2];
    int Algorithm;

public:

    Map()
    {
        Scale.x = 0; Scale.y = 0; Algorithm = 0;

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

        SetChoice(0, "A* SEARCH", Screen_Width * 0.15, Screen_Height * 0.20);
        SetChoice(1, "DIJKSTRA", Screen_Width * 0.75, Screen_Height * 0.20);
    }

    void ResetKeys()
    {
        LeftMousePressed = false;
        SKeyPressed = false;
        EKeyPressed = false;
        StartPathfinding = false;
    }

    float distance(Node* a, Node* b)
    {
        // Pythagoras theorem
        return sqrtf((a->Position.x - b->Position.x) * (a->Position.x - b->Position.x)
            + (a->Position.y - b->Position.y) * (a->Position.y - b->Position.y));
    }

    void SetChoice(int Index, const char* Name, float x, float y)
    {
        Choices[Index].Add(x, y);
        Choices[Index].SetLength(strlen(Name));
    }

    void FindPath()
    {
        // Reset Navigation Graph - default all node states
        for (int x = 0; x < Rows * Cols; x++)
        {
            Nodes[x].Visited = false;
            Nodes[x].GlobalGoal = INFINITY;
            Nodes[x].LocalGoal = INFINITY;
            Nodes[x].Parent = nullptr;	// No parents

        }

        // Setup starting conditions
        Node* nodeCurrent = Start;
        Start->LocalGoal = 0.0f;
        Start->GlobalGoal = distance(Start, End);

        // Add start node to not tested list - this will ensure it gets tested.
        // As the algorithm progresses, newly discovered nodes get added to this
        // list, and will themselves be tested later
        std::list<Node*> listNotTestedNodes;
        listNotTestedNodes.push_back(Start);

        // if the not tested list contains nodes, there may be better paths
        // which have not yet been explored. However, we will also stop 
        // searching when we reach the target - there may well be better
        // paths but this one will do - it wont be the longest.
        while (!listNotTestedNodes.empty() && nodeCurrent != End)// Find absolutely shortest path // && nodeCurrent != nodeEnd)
        {
            // Sort Untested nodes by global goal, so lowest is first
            listNotTestedNodes.sort([](const Node* lhs, const Node* rhs) { return lhs->GlobalGoal < rhs->GlobalGoal; });

            // Front of listNotTestedNodes is potentially the lowest distance node. Our
            // list may also contain nodes that have been visited, so ditch these...
            while (!listNotTestedNodes.empty() && listNotTestedNodes.front()->Visited)
                listNotTestedNodes.pop_front();

            // ...or abort because there are no valid nodes left to test
            if (listNotTestedNodes.empty())
                break;

            nodeCurrent = listNotTestedNodes.front();
            nodeCurrent->Visited = true; // We only explore a node once


            // Check each of this node's neighbours...
            for (auto nodeNeighbour : nodeCurrent->Neighbours)
            {
                // ... and only if the neighbour is not visited and is 
                // not an obstacle, add it to NotTested List
                if (!nodeNeighbour->Visited && nodeNeighbour->Obstacle == 0)
                    listNotTestedNodes.push_back(nodeNeighbour);

                // Calculate the neighbours potential lowest parent distance
                float fPossiblyLowerGoal = nodeCurrent->LocalGoal + distance(nodeCurrent, nodeNeighbour);

                // If choosing to path through this node is a lower distance than what 
                // the neighbour currently has set, update the neighbour to use this node
                // as the path source, and set its distance scores as necessary
                if (fPossiblyLowerGoal < nodeNeighbour->LocalGoal)
                {
                    nodeNeighbour->Parent = nodeCurrent;
                    nodeNeighbour->LocalGoal = fPossiblyLowerGoal;

                    // The best path length to the neighbour being tested has changed, so
                    // update the neighbour's score. The heuristic is used to globally bias
                    // the path algorithm, so it knows if its getting better or worse. At some
                    // point the algo will realise this path is worse and abandon it, and then go
                    // and search along the next best path.

                    if (GetSelectedAlgorithm() == 1)
                    {
                        nodeNeighbour->GlobalGoal = nodeNeighbour->LocalGoal + distance(nodeNeighbour, End);
                    }

                    else if (GetSelectedAlgorithm() == 2)
                    {
                        nodeNeighbour->GlobalGoal = nodeNeighbour->LocalGoal;
                    }
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
                    glColor4f(0.4, 0.4, 0.4, 1);
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

                if (Nodes[j * Rows + i].Visited && Start != &Nodes[j * Rows + i] && End != &Nodes[j * Rows + i])
                {
                    glColor4f(0.4, 0.5, 0.7, 1);
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

    bool CheckBlockCollision(float Px, float Py, int& Index)
    {
        for (int itr = 0; itr < 2; itr++)
        {
            // if collides
            if (Px >= Choices[itr].x && Px <= Choices[itr].x + Choices[itr].length * 14.5
                && Py >= Choices[itr].y - 20 && Py <= Choices[itr].y)
            {
                Index = itr + 1;
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

    float GetCursorPositionX() const
    {
        return Cursor.x;
    }

    float GetCursorPositionY() const
    {
        return Cursor.y;
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
                FindPath();

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

            else
            {
                // Reset Navigation Graph - default all node states
                for (int x = 0; x < Rows * Cols; x++)
                {
                    Nodes[x].Visited = false;
                    Nodes[x].GlobalGoal = INFINITY;
                    Nodes[x].LocalGoal = INFINITY;
                    Nodes[x].Parent = nullptr;	// No parents

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

        if (Button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
        {
            LeftMousePressed = false;

        }
    }

    int GetSelectedAlgorithm() const
    {
        return Algorithm;
    }

    void InputAlgoChoice(int Choice)
    {
        if (Algorithm == 0 && LeftMousePressed)
        {
            Algorithm = Choice;
        }

    }

    static void KeyboardCallBack(GLFWwindow* window, int Key, int scancode, int action, int mods)
    {
        if (Key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        {
            QuitApp = true;
        }

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
