#ifndef RCADE_CANVAS_H
#define RCADE_CANVAS_H

#include <emscripten.h>
#include <emscripten/html5.h>
#include <string>
#include <functional>

namespace rcade {

/**
 * RCadeCanvas - Manages HTML5 canvas setup and 2D rendering context
 *
 * This class handles the creation and initialization of a canvas element
 * for RCade games, providing a simple interface for 2D graphics.
 */
class Canvas {
public:
    /**
     * Creates and initializes a canvas with the specified dimensions
     *
     * @param width Canvas width in pixels (default: 336)
     * @param height Canvas height in pixels (default: 262)
     * @param canvasId HTML id for the canvas element (default: "gameCanvas")
     */
    Canvas(int width = 336, int height = 262, const char* canvasId = "gameCanvas");

    /**
     * Get canvas width
     */
    int getWidth() const { return width_; }

    /**
     * Get canvas height
     */
    int getHeight() const { return height_; }

    /**
     * Clear the canvas with a color
     *
     * @param color CSS color string (e.g., "#1a1a2e", "rgb(255,0,0)")
     */
    void clear(const char* color = "#1a1a2e");

    /**
     * Draw a filled rectangle
     *
     * @param x X position
     * @param y Y position
     * @param width Rectangle width
     * @param height Rectangle height
     * @param color CSS color string
     */
    void fillRect(float x, float y, float width, float height, const char* color = "#eee");

    /**
     * Draw a stroked rectangle
     *
     * @param x X position
     * @param y Y position
     * @param width Rectangle width
     * @param height Rectangle height
     * @param color CSS color string
     */
    void strokeRect(float x, float y, float width, float height, const char* color = "#eee");

    /**
     * Draw text
     *
     * @param text Text to draw
     * @param x X position
     * @param y Y position
     * @param font CSS font string (e.g., "24px monospace")
     * @param color CSS color string
     * @param align Text alignment ("left", "center", "right")
     */
    void fillText(const char* text, float x, float y,
                  const char* font = "16px monospace",
                  const char* color = "#eee",
                  const char* align = "left");

    /**
     * Begin a new path for custom drawing
     */
    void beginPath();

    /**
     * Move to a point without drawing
     */
    void moveTo(float x, float y);

    /**
     * Draw a line to a point
     */
    void lineTo(float x, float y);

    /**
     * Stroke the current path
     */
    void stroke(const char* color = "#eee", float lineWidth = 1.0f);

    /**
     * Set line dash pattern
     *
     * @param dash Dash length
     * @param gap Gap length
     */
    void setLineDash(float dash, float gap);

    /**
     * Clear line dash pattern (solid line)
     */
    void clearLineDash();

private:
    int width_;
    int height_;
    std::string canvasId_;
};

} // namespace rcade

#endif // RCADE_CANVAS_H
