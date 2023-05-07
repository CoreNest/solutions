#pragma once
#include <array>
#include <iostream>
#include <algorithm>
#include <utility>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/RenderStates.hpp>

struct vec2
{
    float x, y;
    operator sf::Vector2f() const { return sf::Vector2f(x, y); }
    // operator sf::Vertex() const { return sf::Vector2f(x, y); }
};

struct triangle
{
    std::array<vec2, 3> points;
    sf::Color color = sf::Color::Red;
    operator sf::VertexArray() const
    {
        sf::VertexArray tri(sf::Triangles, 3);
        tri[0].position = points[0];
        tri[1].position = points[1];
        tri[2].position = points[2];
        return tri;
    }
    triangle(std::array<vec2, 3> &&a)
    {
        points = a;
    }

private:
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const
    {
        sf::VertexArray tri(sf::Triangles, 3);

        tri[0].position = points[0];
        tri[1].position = points[1];
        tri[2].position = points[2];
        
        tri[0].color =color;
        tri[1].color =color;
        tri[2].color =color;
        target.draw(tri, states);
    }
};

// Helper function to calculate the orientation of three points
int orientation(vec2 p1, vec2 p2, vec2 p3)
{
    double val = (p2.y - p1.y) * (p3.x - p2.x) -
                 (p2.x - p1.x) * (p3.y - p2.y);
    if (abs(val) < 1e-9)
    {
        return 0; // collinear
    }
    return (val > 0) ? 1 : 2; // clockwise or counterclockwise
}

// Helper function to check if two line segments intersect
bool do_lines_intersect(vec2 p1, vec2 q1, vec2 p2, vec2 q2)
{
    int o1 = orientation(p1, q1, p2);
    int o2 = orientation(p1, q1, q2);
    int o3 = orientation(p2, q2, p1);
    int o4 = orientation(p2, q2, q1);

    if (o1 != o2 && o3 != o4)
    {
        return true;
    }

    if (o1 == 0 && q1.x >=std::min(p1.x, p2.x) && q1.x <=std::max(p1.x, p2.x) &&
        q1.y >=std::min(p1.y, p2.y) && q1.y <=std::max(p1.y, p2.y))
    {
        return true;
    }

    if (o2 == 0 && q1.x >=std::min(p1.x, q2.x) && q1.x <=std::max(p1.x, q2.x) &&
        q1.y >=std::min(p1.y, q2.y) && q1.y <=std::max(p1.y, q2.y))
    {
        return true;
    }

    if (o3 == 0 && q2.x >=std::min(p2.x, p1.x) && q2.x <=std::max(p2.x, p1.x) &&
        q2.y >=std::min(p2.y, p1.y) && q2.y <=std::max(p2.y, p1.y))
    {
        return true;
    }

    if (o4 == 0 && q2.x >=std::min(p2.x, q1.x) && q2.x <=std::max(p2.x, q1.x) &&
        q2.y >=std::min(p2.y, q1.y) && q2.y <=std::max(p2.y, q1.y))
    {
        return true;
    }

    return false;
}

// Main function to check if two triangles are colliding
bool isColliding(const triangle &triangle1, const triangle &triangle2)
{
    // Step 1: Find the bounding boxes of both triangles
    vec2 tri1_min = {std::min(std::min(triangle1.points[0].x, triangle1.points[1].x), triangle1.points[2].x),
                     std::min(std::min(triangle1.points[0].y, triangle1.points[1].y), triangle1.points[2].y)};
    vec2 tri1_max = {std::max(std::max(triangle1.points[0].x, triangle1.points[1].x), triangle1.points[2].x),
                     std::max(std::max(triangle1.points[0].y, triangle1.points[1].y), triangle1.points[2].y)};
    vec2 tri2_min = {std::min(std::min(triangle2.points[0].x, triangle2.points[1].x), triangle2.points[2].x),
                     std::min(std::min(triangle2.points[0].y, triangle2.points[1].y), triangle2.points[2].y)};
    vec2 tri2_max = {std::max(std::max(triangle2.points[0].x, triangle2.points[1].x), triangle2.points[2].x),
                     std::max(std::max(triangle2.points[0].y, triangle2.points[1].y), triangle2.points[2].y)};
    // Step 2: Check if the bounding boxes overlap
    if (tri1_min.x > tri2_max.x || tri1_max.x < tri2_min.x ||
        tri1_min.y > tri2_max.y || tri1_max.y < tri2_min.y)
    {
        return false; // no overlap
    }

    // Step 3: Check if any line segment of the two triangles intersect
    if (do_lines_intersect(triangle1.points[0], triangle1.points[1], triangle2.points[0], triangle2.points[1]) || do_lines_intersect(triangle1.points[0], triangle1.points[1], triangle2.points[1], triangle2.points[2]) || do_lines_intersect(triangle1.points[0], triangle1.points[1], triangle2.points[2], triangle2.points[0]) ||
        do_lines_intersect(triangle1.points[1], triangle1.points[2], triangle2.points[0], triangle2.points[1]) || do_lines_intersect(triangle1.points[1], triangle1.points[2], triangle2.points[1], triangle2.points[2]) || do_lines_intersect(triangle1.points[1], triangle1.points[2], triangle2.points[2], triangle2.points[0]) ||
        do_lines_intersect(triangle1.points[2], triangle1.points[0], triangle2.points[0], triangle2.points[1]) || do_lines_intersect(triangle1.points[2], triangle1.points[0], triangle2.points[1], triangle2.points[2]) || do_lines_intersect(triangle1.points[2], triangle1.points[0], triangle2.points[2], triangle2.points[0]))
    {
        return true; // intersection detected
    }

    // Step 4: Check if one triangle is completely inside the other
    for (int i = 0; i < 3; i++)
    {
        bool is_inside_tri1 = true;
        bool is_inside_tri2 = true;
        for (int j = 0; j < 3; j++)
        {
            if (i != j)
            {
                if (orientation(triangle1.points[i], triangle1.points[j], triangle2.points[0]) == 2)
                {
                    is_inside_tri1 = false;
                }
                if (orientation(triangle2.points[i], triangle2.points[j], triangle1.points[0]) == 2)
                {
                    is_inside_tri2 = false;
                }
            }
        }
        if (is_inside_tri1 || is_inside_tri2)
        {
            return true; // one triangle is completely inside the other
        }
    }

    // If none of the above conditions are satisfied, the triangles are not colliding
    return false;
}