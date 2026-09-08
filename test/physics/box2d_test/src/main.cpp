#include <SDL3/SDL.h>
#include <box2d/box2d.h>
#include <glm/glm.hpp>
#include "imgui.h"
#include "implot.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_sdlrenderer3.h"
#include <cmath>
#include <cstdio>
#include <vector>
#include <algorithm>

struct BodyInfo {
    b2BodyId bodyId{};
    float halfWidth{};
    float halfHeight{};
};

static constexpr int WindowWidth = 1280;
static constexpr int WindowHeight = 720;
static constexpr float FixedDeltaTime = 1.0f / 60.0f;
static constexpr float WorldWidth = 25.6f;
static constexpr float WorldHeight = 14.4f;

static void PlotBox(const char* label, const BodyInfo& bodyInfo) {
    if (!b2Body_IsValid(bodyInfo.bodyId)) return;

    const b2Vec2 position = b2Body_GetPosition(bodyInfo.bodyId);
    const float angle = b2Rot_GetAngle(b2Body_GetRotation(bodyInfo.bodyId));
    const float cosAngle = std::cos(angle);
    const float sinAngle = std::sin(angle);

    const float localPoints[4][2] = {
        {-bodyInfo.halfWidth, -bodyInfo.halfHeight},
        {bodyInfo.halfWidth, -bodyInfo.halfHeight},
        {bodyInfo.halfWidth, bodyInfo.halfHeight},
        {-bodyInfo.halfWidth, bodyInfo.halfHeight}
    };

    double x[5];
    double y[5];

    for (int i = 0; i < 4; ++i) {
        const float localX = localPoints[i][0];
        const float localY = localPoints[i][1];
        x[i] = position.x + localX * cosAngle - localY * sinAngle;
        y[i] = position.y + localX * sinAngle + localY * cosAngle;
    }

    x[4] = x[0];
    y[4] = y[0];

    ImPlot::PlotLine(label, x, y, 5);
}

static void PlotVelocity(const BodyInfo& bodyInfo) {
    if (!b2Body_IsValid(bodyInfo.bodyId)) return;

    const b2Vec2 position = b2Body_GetPosition(bodyInfo.bodyId);
    const b2Vec2 velocity = b2Body_GetLinearVelocity(bodyInfo.bodyId);

    double x[2] = { position.x, position.x + velocity.x };
    double y[2] = { position.y, position.y + velocity.y };

    ImPlot::PlotLine("Velocity", x, y, 2);
}

static b2WorldId CreateWorld(std::vector<BodyInfo>& bodies, BodyInfo& ground) {
    b2WorldDef worldDef = b2DefaultWorldDef();
    worldDef.gravity = { 0.0f, -9.8f };

    b2WorldId worldId = b2CreateWorld(&worldDef);

    b2BodyDef groundBodyDef = b2DefaultBodyDef();
    groundBodyDef.position = { WorldWidth * 0.5f, 0.5f };

    ground.bodyId = b2CreateBody(worldId, &groundBodyDef);
    ground.halfWidth = WorldWidth * 0.5f;
    ground.halfHeight = 0.5f;

    b2Polygon groundPolygon = b2MakeBox(ground.halfWidth, ground.halfHeight);
    b2ShapeDef groundShapeDef = b2DefaultShapeDef();
    groundShapeDef.material.friction = 0.6f;
    groundShapeDef.material.restitution = 0.1f;

    b2CreatePolygonShape(ground.bodyId, &groundShapeDef, &groundPolygon);

    for (int i = 0; i < 10; ++i) {
        b2BodyDef bodyDef = b2DefaultBodyDef();
        bodyDef.type = b2_dynamicBody;
        bodyDef.position = { 5.0f + static_cast<float>(i % 3) * 1.0f, 2.0f + static_cast<float>(i) * 1.0f };

        BodyInfo bodyInfo;
        bodyInfo.bodyId = b2CreateBody(worldId, &bodyDef);
        bodyInfo.halfWidth = 0.35f;
        bodyInfo.halfHeight = 0.35f;

        b2Polygon polygon = b2MakeBox(bodyInfo.halfWidth, bodyInfo.halfHeight);
        b2ShapeDef shapeDef = b2DefaultShapeDef();
        shapeDef.density = 1.0f;
        shapeDef.material.friction = 0.4f;
        shapeDef.material.restitution = 0.2f;

        b2CreatePolygonShape(bodyInfo.bodyId, &shapeDef, &polygon);
        bodies.push_back(bodyInfo);
    }

    return worldId;
}

static void ResetWorld(b2WorldId& worldId, std::vector<BodyInfo>& bodies, BodyInfo& ground) {
    if (b2World_IsValid(worldId)) b2DestroyWorld(worldId);
    bodies.clear();
    worldId = CreateWorld(bodies, ground);
}

int main(int argc, char** argv) {
    (void)argc;
    (void)argv;

    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS)) {
        std::printf("SDL_Init failed: %s\n", SDL_GetError());
        return -1;
    }

    SDL_Window* window = SDL_CreateWindow("SDL3 + Box2D + ImPlot", WindowWidth, WindowHeight, SDL_WINDOW_RESIZABLE);

    if (window == nullptr) {
        std::printf("SDL_CreateWindow failed: %s\n", SDL_GetError());
        SDL_Quit();
        return -1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);

    if (renderer == nullptr) {
        std::printf("SDL_CreateRenderer failed: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImPlot::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    ImGui::StyleColorsDark();

    if (!ImGui_ImplSDL3_InitForSDLRenderer(window, renderer)) {
        std::printf("ImGui_ImplSDL3_InitForSDLRenderer failed\n");
        ImPlot::DestroyContext();
        ImGui::DestroyContext();
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    if (!ImGui_ImplSDLRenderer3_Init(renderer)) {
        std::printf("ImGui_ImplSDLRenderer3_Init failed\n");
        ImGui_ImplSDL3_Shutdown();
        ImPlot::DestroyContext();
        ImGui::DestroyContext();
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    std::vector<BodyInfo> bodies;
    BodyInfo ground;
    b2WorldId worldId = CreateWorld(bodies, ground);

    bool running = true;
    bool paused = false;
    bool showVelocity = true;
    float simulationTime = 0.0f;
    double accumulator = 0.0;

    Uint64 previousTime = SDL_GetTicksNS();

    std::vector<float> timeHistory;
    std::vector<float> velocityHistory;

    constexpr int MaxHistory = 300;

    timeHistory.reserve(MaxHistory);
    velocityHistory.reserve(MaxHistory);

    while (running) {
        const Uint64 currentTime = SDL_GetTicksNS();
        const double frameTime = static_cast<double>(currentTime - previousTime) / 1000000000.0;
        previousTime = currentTime;
        accumulator += std::min(frameTime, 0.25);

        SDL_Event event;

        while (SDL_PollEvent(&event)) {
            ImGui_ImplSDL3_ProcessEvent(&event);

            if (event.type == SDL_EVENT_QUIT) running = false;
            if (event.type == SDL_EVENT_KEY_DOWN && event.key.key == SDLK_ESCAPE && !event.key.repeat) running = false;
            if (event.type == SDL_EVENT_KEY_DOWN && event.key.key == SDLK_SPACE && !event.key.repeat) paused = !paused;
        }

        ImGui_ImplSDL3_NewFrame();
        ImGui_ImplSDLRenderer3_NewFrame();
        ImGui::NewFrame();

        if (!paused) {
            while (accumulator >= FixedDeltaTime) {
                b2World_Step(worldId, FixedDeltaTime, 4);
                accumulator -= FixedDeltaTime;
                simulationTime += FixedDeltaTime;
            }
        }

        float velocity = 0.0f;

        if (!bodies.empty() && b2Body_IsValid(bodies[0].bodyId)) {
            const b2Vec2 linearVelocity = b2Body_GetLinearVelocity(bodies[0].bodyId);
            velocity = std::sqrt(linearVelocity.x * linearVelocity.x + linearVelocity.y * linearVelocity.y);
        }

        timeHistory.push_back(simulationTime);
        velocityHistory.push_back(velocity);

        if (timeHistory.size() > MaxHistory) {
            timeHistory.erase(timeHistory.begin());
            velocityHistory.erase(velocityHistory.begin());
        }

        ImGui::Begin("Box2D Physics");

        ImGui::Text("Simulation Time: %.2f s", simulationTime);
        ImGui::Text("Bodies: %zu", bodies.size());
        ImGui::Text("FPS: %.1f", io.Framerate);
        ImGui::Text("Frame Time: %.3f ms", io.Framerate > 0.0f ? 1000.0f / io.Framerate : 0.0f);

        ImGui::Separator();

        if (ImGui::Button(paused ? "Resume" : "Pause")) paused = !paused;

        ImGui::SameLine();

        if (ImGui::Button("Step")) {
            b2World_Step(worldId, FixedDeltaTime, 4);
            simulationTime += FixedDeltaTime;
        }

        ImGui::SameLine();

        if (ImGui::Button("Reset")) {
            ResetWorld(worldId, bodies, ground);
            simulationTime = 0.0f;
            accumulator = 0.0;
            timeHistory.clear();
            velocityHistory.clear();
        }

        ImGui::Checkbox("Show Velocity", &showVelocity);

        ImGui::Separator();

        if (ImPlot::BeginPlot("Physics World", ImVec2(-1.0f, 500.0f))) {
            ImPlot::SetupAxes("X (m)", "Y (m)");
            ImPlot::SetupAxesLimits(0.0, WorldWidth, 0.0, WorldHeight, ImGuiCond_Once);

            PlotBox("Ground", ground);

            for (size_t i = 0; i < bodies.size(); ++i) {
                char label[32];
                std::snprintf(label, sizeof(label), "Body %zu", i);
                PlotBox(label, bodies[i]);
            }

            if (showVelocity) {
                for (const BodyInfo& body : bodies) PlotVelocity(body);
            }

            ImPlot::EndPlot();
        }

        ImGui::End();

        ImGui::Begin("Velocity History");

        if (ImPlot::BeginPlot("Velocity", ImVec2(-1.0f, 300.0f))) {
            ImPlot::SetupAxes("Time (s)", "Velocity (m/s)");

            if (!timeHistory.empty()) ImPlot::PlotLine("Body 0", timeHistory.data(), velocityHistory.data(), static_cast<int>(timeHistory.size()));

            ImPlot::EndPlot();
        }

        ImGui::End();

        ImGui::Render();

        SDL_SetRenderDrawColor(renderer, 20, 20, 25, 255);
        SDL_RenderClear(renderer);

        ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), renderer);

        SDL_RenderPresent(renderer);
    }

    if (b2World_IsValid(worldId)) b2DestroyWorld(worldId);

    ImGui_ImplSDLRenderer3_Shutdown();
    ImGui_ImplSDL3_Shutdown();

    ImPlot::DestroyContext();
    ImGui::DestroyContext();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
