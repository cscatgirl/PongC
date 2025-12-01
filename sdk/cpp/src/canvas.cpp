#include "canvas.h"
#include <emscripten/bind.h>

namespace rcade {

Canvas::Canvas(int width, int height, const char* canvasId)
    : width_(width), height_(height), canvasId_(canvasId) {

    // Create canvas element and initialize 2D context
    EM_ASM({
        const body = document.body;
        const canvas = document.createElement('canvas');
        canvas.id = UTF8ToString($2);
        canvas.width = $0;
        canvas.height = $1;
        body.innerHTML = "";
        body.appendChild(canvas);

        const ctx = canvas.getContext('2d');
        window.gameCtx = ctx;
    }, width_, height_, canvasId);
}

void Canvas::clear(const char* color) {
    EM_ASM({
        const ctx = window.gameCtx;
        if (!ctx) return;
        ctx.fillStyle = UTF8ToString($0);
        ctx.fillRect(0, 0, $1, $2);
    }, color, width_, height_);
}

void Canvas::fillRect(float x, float y, float width, float height, const char* color) {
    EM_ASM({
        const ctx = window.gameCtx;
        if (!ctx) return;
        ctx.fillStyle = UTF8ToString($4);
        ctx.fillRect($0, $1, $2, $3);
    }, x, y, width, height, color);
}

void Canvas::strokeRect(float x, float y, float width, float height, const char* color) {
    EM_ASM({
        const ctx = window.gameCtx;
        if (!ctx) return;
        ctx.strokeStyle = UTF8ToString($4);
        ctx.strokeRect($0, $1, $2, $3);
    }, x, y, width, height, color);
}

void Canvas::fillText(const char* text, float x, float y,
                      const char* font, const char* color, const char* align) {
    EM_ASM({
        const ctx = window.gameCtx;
        if (!ctx) return;
        ctx.font = UTF8ToString($3);
        ctx.fillStyle = UTF8ToString($4);
        ctx.textAlign = UTF8ToString($5);
        ctx.fillText(UTF8ToString($0), $1, $2);
    }, text, x, y, font, color, align);
}

void Canvas::beginPath() {
    EM_ASM({
        const ctx = window.gameCtx;
        if (!ctx) return;
        ctx.beginPath();
    });
}

void Canvas::moveTo(float x, float y) {
    EM_ASM({
        const ctx = window.gameCtx;
        if (!ctx) return;
        ctx.moveTo($0, $1);
    }, x, y);
}

void Canvas::lineTo(float x, float y) {
    EM_ASM({
        const ctx = window.gameCtx;
        if (!ctx) return;
        ctx.lineTo($0, $1);
    }, x, y);
}

void Canvas::stroke(const char* color, float lineWidth) {
    EM_ASM({
        const ctx = window.gameCtx;
        if (!ctx) return;
        ctx.strokeStyle = UTF8ToString($0);
        ctx.lineWidth = $1;
        ctx.stroke();
    }, color, lineWidth);
}

void Canvas::setLineDash(float dash, float gap) {
    EM_ASM({
        const ctx = window.gameCtx;
        if (!ctx) return;
        ctx.setLineDash([$0, $1]);
    }, dash, gap);
}

void Canvas::clearLineDash() {
    EM_ASM({
        const ctx = window.gameCtx;
        if (!ctx) return;
        ctx.setLineDash([]);
    });
}

} // namespace rcade
