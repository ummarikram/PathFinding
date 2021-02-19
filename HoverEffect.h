#pragma once

// For hover animation played on Text
void CheckHover(Shader& shader, const int& HoverBlock, const bool& isHovering)
{
    float ScaleFactor = float(Screen_Width) / float(1280);
    Point AStarScaleFactor(Screen_Width * 0.15, Screen_Height * 0.80);
    Point DijkstraScaleFactor(Screen_Width * 0.75, Screen_Height * 0.80);

    RenderText(shader, "CHOOSE PATHFINDING ALGORITHM !", Screen_Width * 0.33, Screen_Height * 0.93, ScaleFactor, glm::vec3(0.4f, 0.6f, 0.9f));

    if (!isHovering && HoverBlock == 1)
    {
        // Display Bubble Sort as Red
        RenderText(shader, "A* SEARCH", AStarScaleFactor.x, AStarScaleFactor.y, ScaleFactor, glm::vec3(1.0f, 0.0f, 0.0f));
        RenderText(shader, "DIJKSTRA", DijkstraScaleFactor.x, DijkstraScaleFactor.y, ScaleFactor, glm::vec3(0.0f, 0.0f, 0.0f));
        
    }

    // Same check for other blocks
    else if (!isHovering && HoverBlock == 2)
    {
        RenderText(shader, "A* SEARCH", AStarScaleFactor.x, AStarScaleFactor.y, ScaleFactor, glm::vec3(0.0f, 0.0f, 0.0f));
        RenderText(shader, "DIJKSTRA", DijkstraScaleFactor.x, DijkstraScaleFactor.y, ScaleFactor, glm::vec3(1.0f, 0.0f, 0.0f));
 
    }

    // if no hovering
    else
    {
        RenderText(shader, "A* SEARCH", AStarScaleFactor.x, AStarScaleFactor.y, ScaleFactor, glm::vec3(0.0f, 0.0f, 0.0f));
        RenderText(shader, "DIJKSTRA", DijkstraScaleFactor.x, DijkstraScaleFactor.y, ScaleFactor, glm::vec3(0.0f, 0.0f, 0.0f));
    
    }
}