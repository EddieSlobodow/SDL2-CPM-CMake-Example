#define SDL_MAIN_HANDLED true
#include <iostream>
#include <algorithm>
#include <string>
#include "ChessCLI.h"
using namespace std;
#include <SDL.h>
#include <SDL_image.h>
//#include <SDL_mixer.h>
//#include <SDL_ttf.h>
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_sdlrenderer.h"
#include <fstream>
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#ifdef __EMSCRIPTEN__
EM_JS(int, canvas_get_width, (), {
return canvas.width;
});

EM_JS(int, canvas_get_height, (), {
return canvas.height;
});
#endif


int main(int argc, char* argv[]) {
    // Unused argc, argv
    (void) argc;
    (void) argv;

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0) {
        printf("Error: %s\n", SDL_GetError());
        return -1;
    }

    auto width = 1280;
    auto height = 720;
#ifdef __EMSCRIPTEN__
    width = canvas_get_width();
    height = canvas_get_height();
#endif

    // Setup window
    SDL_WindowFlags window_flags = (SDL_WindowFlags) (SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN);
    SDL_Window *window = SDL_CreateWindow("Dear ImGui SDL2+SDL_Renderer example", SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED, width, height, window_flags);

    if (!window) {
        std::cout << "Window could not be created!" << std::endl
                  << "SDL_Error: " << SDL_GetError() << std::endl;
        abort();
    }

    // Setup SDL_Renderer instance
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        SDL_Log("Error creating SDL_Renderer!");
        abort();
    }
    //SDL_RendererInfo info;
    //SDL_GetRendererInfo(renderer, &info);
    //SDL_Log("Current SDL_Renderer: %s", info.name);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Setup Platform/Renderer backends
    ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer_Init(renderer);

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != NULL);

    // Our state
    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    // Main loop
    bool done = false;

    // Declare rect of square
    SDL_Rect squareRect;

    // Square dimensions: Half of the min(SCREEN_WIDTH, SCREEN_HEIGHT)
    squareRect.w = std::min(width, height) / 2;
    squareRect.h = std::min(width, height) / 2;

    // Square position: In the middle of the screen
    squareRect.x = width / 2 - squareRect.w / 2;
    squareRect.y = height / 2 - squareRect.h / 2;

    // load the imgs here
    // the great wall
    SDL_Surface* whitePawnSrf = IMG_Load("Chess_plt60.png");
    SDL_Texture* whitePawnTxt = SDL_CreateTextureFromSurface(renderer, whitePawnSrf);
    SDL_Surface* blackPawnSrf = IMG_Load("Chess_pdt60.png");
    SDL_Texture* blackPawnTxt = SDL_CreateTextureFromSurface(renderer, blackPawnSrf);
    SDL_Surface* whiteRookSrf = IMG_Load("Chess_rlt60.png");
    SDL_Texture* whiteRookTxt = SDL_CreateTextureFromSurface(renderer, whiteRookSrf);
    SDL_Surface* blackRookSrf = IMG_Load("Chess_rdt60.png");
    SDL_Texture* blackRookTxt = SDL_CreateTextureFromSurface(renderer, blackRookSrf);
    SDL_Surface* whiteKnightSrf = IMG_Load("Chess_nlt60.png");
    SDL_Texture* whiteKnightTxt = SDL_CreateTextureFromSurface(renderer, whiteKnightSrf);
    SDL_Surface* blackKnightSrf = IMG_Load("Chess_ndt60.png");
    SDL_Texture* blackKnightTxt = SDL_CreateTextureFromSurface(renderer, blackKnightSrf);
    SDL_Surface* whiteBishopSrf = IMG_Load("Chess_blt60.png");
    SDL_Texture* whiteBishopTxt = SDL_CreateTextureFromSurface(renderer, whiteBishopSrf);
    SDL_Surface* blackBishopSrf = IMG_Load("Chess_bdt60.png");
    SDL_Texture* blackBishopTxt = SDL_CreateTextureFromSurface(renderer, blackBishopSrf);
    SDL_Surface* whiteQueenSrf = IMG_Load("Chess_qlt60.png");
    SDL_Texture* whiteQueenTxt = SDL_CreateTextureFromSurface(renderer, whiteQueenSrf);
    SDL_Surface* blackQueenSrf = IMG_Load("Chess_qdt60.png");
    SDL_Texture* blackQueenTxt = SDL_CreateTextureFromSurface(renderer, blackQueenSrf);
    SDL_Surface* whiteKingSrf = IMG_Load("Chess_klt60.png");
    SDL_Texture* whiteKingTxt = SDL_CreateTextureFromSurface(renderer, whiteKingSrf);
    SDL_Surface* blackKingSrf = IMG_Load("Chess_kdt60.png");
    SDL_Texture* blackKingTxt = SDL_CreateTextureFromSurface(renderer, blackKingSrf);
    SDL_Surface* aSrf = IMG_Load("a.png");
    SDL_Texture* aTxt = SDL_CreateTextureFromSurface(renderer, aSrf);
    SDL_Surface* bSrf = IMG_Load("b.png");
    SDL_Texture* bTxt = SDL_CreateTextureFromSurface(renderer, bSrf);
    SDL_Surface* cSrf = IMG_Load("c.png");
    SDL_Texture* cTxt = SDL_CreateTextureFromSurface(renderer, cSrf);
    SDL_Surface* dSrf = IMG_Load("d.png");
    SDL_Texture* dTxt = SDL_CreateTextureFromSurface(renderer, dSrf);
    SDL_Surface* eSrf = IMG_Load("e.png");
    SDL_Texture* eTxt = SDL_CreateTextureFromSurface(renderer, eSrf);
    SDL_Surface* fSrf = IMG_Load("f.png");
    SDL_Texture* fTxt = SDL_CreateTextureFromSurface(renderer, fSrf);
    SDL_Surface* gSrf = IMG_Load("g.png");
    SDL_Texture* gTxt = SDL_CreateTextureFromSurface(renderer, gSrf);
    SDL_Surface* hSrf = IMG_Load("h.png");
    SDL_Texture* hTxt = SDL_CreateTextureFromSurface(renderer, hSrf);
    SDL_Surface* oneSrf = IMG_Load("1.png");
    SDL_Texture* oneTxt = SDL_CreateTextureFromSurface(renderer, oneSrf);
    SDL_Surface* twoSrf = IMG_Load("2.png");
    SDL_Texture* twoTxt = SDL_CreateTextureFromSurface(renderer, twoSrf);
    SDL_Surface* threeSrf = IMG_Load("3.png");
    SDL_Texture* threeTxt = SDL_CreateTextureFromSurface(renderer, threeSrf);
    SDL_Surface* fourSrf = IMG_Load("4.png");
    SDL_Texture* fourTxt = SDL_CreateTextureFromSurface(renderer, fourSrf);
    SDL_Surface* fiveSrf = IMG_Load("5.png");
    SDL_Texture* fiveTxt = SDL_CreateTextureFromSurface(renderer, fiveSrf);
    SDL_Surface* sixSrf = IMG_Load("6.png");
    SDL_Texture* sixTxt = SDL_CreateTextureFromSurface(renderer, sixSrf);
    SDL_Surface* sevenSrf = IMG_Load("7.png");
    SDL_Texture* sevenTxt = SDL_CreateTextureFromSurface(renderer, sevenSrf);
    SDL_Surface* eightSrf = IMG_Load("8.png");
    SDL_Texture* eightTxt = SDL_CreateTextureFromSurface(renderer, eightSrf);

    std::string notation;
    bool newInput = false;
    bool invalidMove = false;
    bool resetBoard = false;
    int rowCounter = 0;
    ofstream gameLog("C:\\repos\\SDL2-CPM-CMake-Example\\gamelog.txt");
    if (!gameLog.is_open()) std::cerr << "Error opening the file" << std::endl;

    Game* game = new Game(FEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR"));
    Teams winner = Teams::NONE;
    //Game game(FEN("k7/8/1K6/2Q5/8/8/8/8"));

    Teams tomove = Teams::WHITE;


    // Event loop
    while (!done) {
        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
        SDL_Event event;

            while (SDL_PollEvent(&event)) {
                ImGui_ImplSDL2_ProcessEvent(&event);
                if (event.type == SDL_QUIT)
                    done = true;
                if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE &&
                    event.window.windowID == SDL_GetWindowID(window))
                    done = true;
            }

            // Start the Dear ImGui frame
            ImGui_ImplSDLRenderer_NewFrame();
            ImGui_ImplSDL2_NewFrame();
            ImGui::NewFrame();

            // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
            //if (show_demo_window)
            //ImGui::ShowDemoWindow(&show_demo_window);

            // 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
            {
                static float f = 0.0f;

                ImGui::Begin(
                        "Hello, world!");                          // Create a window called "Hello, world!" and append into it.
                static char textBuffer[256] = "";
                bool enterPressed = ImGui::InputText("Enter a move", textBuffer, sizeof(textBuffer),
                                                     ImGuiInputTextFlags_EnterReturnsTrue);

                if (enterPressed) {
                    // Do something when Enter is pressed
                    printf("Enter pressed! Text: %s\n", textBuffer);
                    notation = textBuffer;
                    newInput = true;
                    textBuffer[0] = '\0';
                    // Here, you can store the textBuffer in a variable or perform any other actions.
                }

                if (tomove == Teams::WHITE) ImGui::Text("White to move");
                else ImGui::Text("Black to move");  // Display some text (you can use a format strings too)

                if (winner == Teams::BLACK) ImGui::Text("Black Wins!");
                if (winner == Teams::WHITE) ImGui::Text("White Wins!");

                if (invalidMove) ImGui::Text("Invalid Move!");
                if (ImGui::Button("Reset Game")) resetBoard = true;
                //ImGui::SameLine();

                ImGui::End();
            }

            // 3. Show another simple window.
            if (show_another_window) {
                ImGui::Begin("Another Window",
                             &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
                ImGui::Text("Hello from another window!");
                if (ImGui::Button("Close Me"))
                    show_another_window = false;
                ImGui::End();
            }


            // Rendering
            ImGui::Render();

            SDL_SetRenderDrawColor(renderer, (Uint8) (clear_color.x * 255), (Uint8) (clear_color.y * 255),
                                   (Uint8) (clear_color.z * 255), (Uint8) (clear_color.w * 255));
            SDL_RenderClear(renderer);

            // todo: add your game logic here to be drawn before the ui rendering

            // present ui on top of your drawings
            ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());

            int height, width;
            SDL_GetWindowSize(window, &width, &height);
            int boardSize = 9;
            int size = height / boardSize;
            int counter = 0;

            if (newInput) {
                if (game->AttemptMoves(interpretMove(game->getTeamPieces(tomove), discoverMove(notation), tomove),
                                      tomove)) {
                    if (tomove == Teams::WHITE) tomove = Teams::BLACK;
                    else tomove = Teams::WHITE;
                    invalidMove = false;
                    gameLog << notation << "  ";
                    rowCounter++;
                    if (rowCounter % boardSize-1 == 0) gameLog << endl;
                } else {
                    invalidMove = true;
                }
            }

            //printboard
            for (int row = 0; row < boardSize; row++) {
                for (int collum = 0; collum < boardSize; collum++) {
                    SDL_Rect r = {collum * size, ((boardSize-1) - row) * size, size, size};
                    if (row == boardSize-1 || collum == boardSize-1){
                        SDL_SetRenderDrawColor(renderer, (Uint8) (clear_color.x * 255), (Uint8) (clear_color.y * 255),
                                              (Uint8) (clear_color.z * 255), (Uint8) (clear_color.w * 255));
                    } else {
                        if (counter % 2 == 0) SDL_SetRenderDrawColor(renderer, 247, 235, 190, 0xFF);
                        else SDL_SetRenderDrawColor(renderer, 171, 125, 79, 0xFF);
                    }
                    SDL_RenderFillRect(renderer, &r);

                    Square square = {collum, row};
                    SDL_Texture* defaultTexture = nullptr;
                    if (row == boardSize-1){
                        switch (collum){
                            case 0: defaultTexture = aTxt;
                                break;
                            case 1: defaultTexture = bTxt;
                                break;
                            case 2: defaultTexture = cTxt;
                                break;
                            case 3: defaultTexture = dTxt;
                                break;
                            case 4: defaultTexture = eTxt;
                                break;
                            case 5: defaultTexture = fTxt;
                                break;
                            case 6: defaultTexture = gTxt;
                                break;
                            case 7: defaultTexture = hTxt;
                        }
                    }
                    if (collum == boardSize-1){
                        switch (row){
                            case 0: defaultTexture = oneTxt;
                                break;
                            case 1: defaultTexture = twoTxt;
                                break;
                            case 2: defaultTexture = threeTxt;
                                break;
                            case 3: defaultTexture = fourTxt;
                                break;
                            case 4: defaultTexture = fiveTxt;
                                break;
                            case 5: defaultTexture = sixTxt;
                                break;
                            case 6: defaultTexture = sevenTxt;
                                break;
                            case 7: defaultTexture = eightTxt;
                        }
                    }

                    switch (game->getPieceType(square)) {
                        case PieceType::PAWN:
                            if (game->getPieceTeam(square) == 0) SDL_RenderCopy(renderer, whitePawnTxt, NULL, &r);
                            else SDL_RenderCopy(renderer, blackPawnTxt, NULL, &r);
                            break;
                        case PieceType::ROOK:
                            if (game->getPieceTeam(square) == 0) SDL_RenderCopy(renderer, whiteRookTxt, NULL, &r);
                            else SDL_RenderCopy(renderer, blackRookTxt, NULL, &r);
                            break;
                        case PieceType::KNIGHT:
                            if (game->getPieceTeam(square) == 0) SDL_RenderCopy(renderer, whiteKnightTxt, NULL, &r);
                            else SDL_RenderCopy(renderer, blackKnightTxt, NULL, &r);
                            break;
                        case PieceType::BISHOP:
                            if (game->getPieceTeam(square) == 0) SDL_RenderCopy(renderer, whiteBishopTxt, NULL, &r);
                            else SDL_RenderCopy(renderer, blackBishopTxt, NULL, &r);
                            break;
                        case PieceType::KING:
                            if (game->getPieceTeam(square) == 0) SDL_RenderCopy(renderer, whiteKingTxt, NULL, &r);
                            else SDL_RenderCopy(renderer, blackKingTxt, NULL, &r);
                            break;
                        case PieceType::QUEEN:
                            if (game->getPieceTeam(square) == 0) SDL_RenderCopy(renderer, whiteQueenTxt, NULL, &r);
                            else SDL_RenderCopy(renderer, blackQueenTxt, NULL, &r);
                            break;
                        default:
                            SDL_RenderCopy(renderer, defaultTexture, NULL, &r);
                    }

                    //if (row < 4)
                    //SDL_RenderCopy(renderer, whitePawnTxt, NULL, &r);
                    //else SDL_RenderCopy(renderer, blackPawnTxt, NULL, &r);


                    counter++;
                }
            }

            if (newInput) {
                winner = game->getWinner();
                newInput = false;
            }
        if (resetBoard){
            tomove = Teams::WHITE;
            winner = Teams::NONE;
            delete game;
            game = new Game(FEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR"));
            resetBoard = false;
            invalidMove = false;
            gameLog << std::endl << std::endl;
        }

            SDL_RenderPresent(renderer);

            SDL_Delay(0);
    }

    // Cleanup
    ImGui_ImplSDLRenderer_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
    gameLog.close();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}