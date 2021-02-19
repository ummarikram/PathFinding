#pragma once

// For hover animation played on Text
void CheckHover(Shader& shader, const int& HoverBlock, const bool& isHovering)
{
    RenderText(shader, "CHOOSE PATHFINDING ALGORITHM !", Screen_Width * 0.33, Screen_Height * 0.93, float(Screen_Width) / float(1280), glm::vec3(0.4f, 0.6f, 0.9f));

    // if hovered on first block which is of bubble sort
    if (!isHovering && HoverBlock == 1)
    {
        // Display Bubble Sort as Red
        RenderText(shader, "A* SEARCH", Screen_Width * 0.15, Screen_Height * 0.80, float(Screen_Width) / float(1280), glm::vec3(1.0f, 0.0f, 0.0f));
        RenderText(shader, "DIJKSTRA", Screen_Width * 0.75, Screen_Height * 0.80, float(Screen_Width) / float(1280), glm::vec3(0.0f, 0.0f, 0.0f));
        
    }

    // Same check for other blocks
    else if (!isHovering && HoverBlock == 2)
    {
        RenderText(shader, "A* SEARCH", Screen_Width * 0.15, Screen_Height * 0.80, float(Screen_Width) / float(1280), glm::vec3(0.0f, 0.0f, 0.0f));
        RenderText(shader, "DIJKSTRA", Screen_Width * 0.75, Screen_Height * 0.80, float(Screen_Width) / float(1280), glm::vec3(1.0f, 0.0f, 0.0f));
 
    }

    // if no hovering
    else
    {
        RenderText(shader, "A* SEARCH", Screen_Width * 0.15, Screen_Height * 0.80, float(Screen_Width) / float(1280), glm::vec3(0.0f, 0.0f, 0.0f));
        RenderText(shader, "DIJKSTRA", Screen_Width * 0.75, Screen_Height * 0.80, float(Screen_Width) / float(1280), glm::vec3(0.0f, 0.0f, 0.0f));
    
    }
}