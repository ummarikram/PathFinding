#pragma once

// For hover animation played on Text
void CheckHover(Shader& shader, const int& HoverBlock, const bool& isHovering)
{
    RenderText(shader, "CHOOSE PATHFINDING ALGORITHM !", 430.0f, 670.0f, 1.0f, glm::vec3(0.4f, 0.6f, 0.9f));

    // if hovered on first block which is of bubble sort
    if (!isHovering && HoverBlock == 1)
    {
        // Display Bubble Sort as Red
        RenderText(shader, "A* SEARCH", 150.0f, 570.0f, 1.0f, glm::vec3(1.0f, 0.0f, 0.0f));
        RenderText(shader, "DIJKSTRA", 950.0f, 570.0f, 1.0f, glm::vec3(0.0f, 0.0f, 0.0f));
        
    }

    // Same check for other blocks
    else if (!isHovering && HoverBlock == 2)
    {
        RenderText(shader, "A* SEARCH", 150.0f, 570.0f, 1.0f, glm::vec3(0.0f, 0.0f, 0.0f));
        RenderText(shader, "DIJKSTRA", 950.0f, 570.0f, 1.0f, glm::vec3(1.0f, 0.0f, 0.0f));
 
    }

    // if no hovering
    else
    {
        RenderText(shader, "A* SEARCH", 150.0f, 570.0f, 1.0f, glm::vec3(0.0f, 0.0f, 0.0f));
        RenderText(shader, "DIJKSTRA", 950.0f, 570.0f, 1.0f, glm::vec3(0.0f, 0.0f, 0.0f));
    
    }
}