#pragma once

// To store coordinates
struct Point
{
    float x, y;
    int length;

    Point()
    {
        x = 0.0f; y = 0.0f; length = 0;
    }

    void SetLength(int length)
    {
        this->length = length;
    }

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
    float GlobalGoal;				// Distance to goal so far
    float LocalGoal;				// Distance to goal if we took the alternative route
    Quad m_Quad;
    Point Position;
    bool Initialized;
    bool Clicked;
    bool Visited;
    Node* Parent;
    std::vector<Node*> Neighbours;	// Connections to neighbours

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

