#include <rcade.h>
#include <emscripten.h>
#include <cmath>
#include <cstdlib>
#include <ctime>

// Game constants
const int CANVAS_WIDTH = 336;
const int CANVAS_HEIGHT = 262;
const int PADDLE_WIDTH = 8;
const int PADDLE_HEIGHT = 40;
const int PADDLE_MARGIN = 10;  // Distance from edge
const int BALL_SIZE = 6;
const float PADDLE_SPEED = 3.0f;
const float INITIAL_BALL_SPEED = 2.5f;

// Game state
struct GameState {
    float paddle1Y;
    float paddle2Y;
    float ballX;
    float ballY;
    float ballVelX;
    float ballVelY;
    int score1;
    int score2;
    bool gameStarted;
    // Event-based paddle movement state
    bool paddle1MovingUp;
    bool paddle1MovingDown;
    bool paddle2MovingUp;
    bool paddle2MovingDown;
} game;

// Global RCade objects
rcade::Canvas* canvas;
rcade::Input* input;

void resetBall() {
    game.ballX = CANVAS_WIDTH / 2;
    game.ballY = CANVAS_HEIGHT / 2;

    // Random angle between -45 and 45 degrees, either left or right
    float angle = (rand() % 90 - 45) * 3.14159f / 180.0f;
    float direction = (rand() % 2 == 0) ? 1.0f : -1.0f;

    game.ballVelX = direction * INITIAL_BALL_SPEED * cos(angle);
    game.ballVelY = INITIAL_BALL_SPEED * sin(angle);
}

void handleInputEvent(const rcade::InputEvent& event) {
    // Start game on any button press (using PRESS event for immediate response)
    if (!game.gameStarted && event.eventType == rcade::InputEventType::PRESS) {
        game.gameStarted = true;
        return;  // Don't process paddle movement on game start
    }

    // Only handle INPUT_START and INPUT_END events for paddle movement
    if (event.eventType != rcade::InputEventType::INPUT_START &&
        event.eventType != rcade::InputEventType::INPUT_END) {
        return;
    }

    // Handle paddle movement events
    if (event.type == "button") {
        bool pressed = event.pressed;

        if (event.player == 1) {
            if (event.button == "UP") {
                game.paddle1MovingUp = pressed;
            } else if (event.button == "DOWN") {
                game.paddle1MovingDown = pressed;
            }
        } else if (event.player == 2) {
            if (event.button == "UP") {
                game.paddle2MovingUp = pressed;
            } else if (event.button == "DOWN") {
                game.paddle2MovingDown = pressed;
            }
        }
    }
}

void updateGame() {
    if (!game.gameStarted) return;

    // Update paddle 1 (left) based on event-driven state
    if (game.paddle1MovingUp && game.paddle1Y > 0) {
        game.paddle1Y -= PADDLE_SPEED;
    }
    if (game.paddle1MovingDown && game.paddle1Y < CANVAS_HEIGHT - PADDLE_HEIGHT) {
        game.paddle1Y += PADDLE_SPEED;
    }

    // Update paddle 2 (right) based on event-driven state
    if (game.paddle2MovingUp && game.paddle2Y > 0) {
        game.paddle2Y -= PADDLE_SPEED;
    }
    if (game.paddle2MovingDown && game.paddle2Y < CANVAS_HEIGHT - PADDLE_HEIGHT) {
        game.paddle2Y += PADDLE_SPEED;
    }

    // Update ball
    game.ballX += game.ballVelX;
    game.ballY += game.ballVelY;

    // Ball collision with top/bottom
    if (game.ballY <= 0 || game.ballY >= CANVAS_HEIGHT - BALL_SIZE) {
        game.ballVelY = -game.ballVelY;
        game.ballY = game.ballY <= 0 ? 0 : CANVAS_HEIGHT - BALL_SIZE;
    }

    // Ball collision with paddles
    // Left paddle
    if (game.ballX <= PADDLE_MARGIN + PADDLE_WIDTH &&
        game.ballY + BALL_SIZE >= game.paddle1Y &&
        game.ballY <= game.paddle1Y + PADDLE_HEIGHT) {
        game.ballVelX = fabs(game.ballVelX);
        float hitPos = (game.ballY + BALL_SIZE/2 - game.paddle1Y) / PADDLE_HEIGHT;
        game.ballVelY = (hitPos - 0.5f) * 4.0f;
    }

    // Right paddle
    if (game.ballX + BALL_SIZE >= CANVAS_WIDTH - PADDLE_WIDTH - PADDLE_MARGIN &&
        game.ballY + BALL_SIZE >= game.paddle2Y &&
        game.ballY <= game.paddle2Y + PADDLE_HEIGHT) {
        game.ballVelX = -fabs(game.ballVelX);
        float hitPos = (game.ballY + BALL_SIZE/2 - game.paddle2Y) / PADDLE_HEIGHT;
        game.ballVelY = (hitPos - 0.5f) * 4.0f;
    }

    // Score points
    if (game.ballX <= 0) {
        game.score2++;
        resetBall();
    }
    if (game.ballX >= CANVAS_WIDTH - BALL_SIZE) {
        game.score1++;
        resetBall();
    }
}

void renderGame() {
    // Clear canvas
    canvas->clear("#1a1a2e");

    // Draw center line
    canvas->setLineDash(5, 5);
    canvas->beginPath();
    canvas->moveTo(CANVAS_WIDTH / 2, 0);
    canvas->lineTo(CANVAS_WIDTH / 2, CANVAS_HEIGHT);
    canvas->stroke("#444");
    canvas->clearLineDash();

    // Draw paddles with margin from edges
    canvas->fillRect(PADDLE_MARGIN, game.paddle1Y, PADDLE_WIDTH, PADDLE_HEIGHT, "#eee");
    canvas->fillRect(CANVAS_WIDTH - PADDLE_WIDTH - PADDLE_MARGIN, game.paddle2Y, PADDLE_WIDTH, PADDLE_HEIGHT, "#eee");

    // Draw ball
    canvas->fillRect(game.ballX, game.ballY, BALL_SIZE, BALL_SIZE, "#eee");

    // Draw scores
    char score1Str[16], score2Str[16];
    snprintf(score1Str, sizeof(score1Str), "%d", game.score1);
    snprintf(score2Str, sizeof(score2Str), "%d", game.score2);

    canvas->fillText(score1Str, 100, 40, "24px monospace", "#eee", "center");
    canvas->fillText(score2Str, 236, 40, "24px monospace", "#eee", "center");

    // Draw instructions if game not started
    if (!game.gameStarted) {
        canvas->fillText("Press any button to start", CANVAS_WIDTH / 2, 150,
                        "12px monospace", "#eee", "center");
        canvas->fillText("P1: UP/DOWN  P2: UP/DOWN", CANVAS_WIDTH / 2, 170,
                        "12px monospace", "#eee", "center");
    }
}

void gameLoop() {
    updateGame();
    renderGame();
}

int main() {
    srand(time(NULL));

    // Initialize game state
    game.paddle1Y = CANVAS_HEIGHT / 2 - PADDLE_HEIGHT / 2;
    game.paddle2Y = CANVAS_HEIGHT / 2 - PADDLE_HEIGHT / 2;
    game.score1 = 0;
    game.score2 = 0;
    game.gameStarted = false;
    game.paddle1MovingUp = false;
    game.paddle1MovingDown = false;
    game.paddle2MovingUp = false;
    game.paddle2MovingDown = false;
    resetBall();

    // Initialize RCade SDK
    canvas = new rcade::Canvas(CANVAS_WIDTH, CANVAS_HEIGHT);
    input = new rcade::Input();

    // Register event handlers for input events
    // Use INPUT_START and INPUT_END to track when buttons are pressed/released
    input->onInputEvent(rcade::InputEventType::INPUT_START, handleInputEvent);
    input->onInputEvent(rcade::InputEventType::INPUT_END, handleInputEvent);
    input->onInputEvent(rcade::InputEventType::PRESS, handleInputEvent);

    // Start game loop at 60 FPS
    emscripten_set_main_loop(gameLoop, 60, 1);

    return 0;
}
