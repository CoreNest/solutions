#include <iostream>
#include <chrono>
#include <cmath>
#if _WIN32
#include <windows.h>
#endif

#include <vector>
#include <functional>

using namespace std;

class Screen
{
public:
        Screen()
        {
#ifdef _WIN32
                _hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
#endif
                hideCursor();
                saveCursorPosition();
        }

        void clear()
        {
                for (int i = 0; i < height; i++)
                {
                        for (int j = 0; j < width; j++)
                        {
                                _screen[i][j] = 'x';
                        }
                }
        }

        void draw(char value, float x, float y)
        {
                int xPos = lround(x);
                int yPos = lround(y);
                if (xPos < width && yPos < height)
                {
                        _screen[yPos][xPos] = value;
                }
        }

        void flush()
        {
                restoreCursorPosition();

                for (int i = 0; i < height; i++)
                {
                        for (int j = 0; j < width; j++)
                        {
                                if (_screen[i][j] != 'x')
                                {
                                        setRedFont();
                                }
                                else
                                {
                                        setWhiteFont();
                                }
                                cout << _screen[i][j];
                        }
                        cout << "\n";
                }
        }

private:
        void hideCursor()
        {
#ifdef _WIN32
                CONSOLE_CURSOR_INFO cursorInfo;
                GetConsoleCursorInfo(_hConsole, &cursorInfo);
                cursorInfo.dwSize = 1;
                cursorInfo.bVisible = FALSE;
                SetConsoleCursorInfo(_hConsole, &cursorInfo);
#else
                cout << "\33[?25l"; // disable cursor
#endif
        }

        void saveCursorPosition()
        {
#ifdef _WIN32
                CONSOLE_SCREEN_BUFFER_INFO coninfo;
                GetConsoleScreenBufferInfo(_hConsole, &coninfo);
                _prevPos = coninfo.dwCursorPosition;
#else
                cout << "\33[s";
#endif
        }

        void restoreCursorPosition()
        {
#ifdef _WIN32
                SetConsoleCursorPosition(_hConsole, _prevPos);
#else
                cout << "\338";
#endif
        }

        void setRedFont()
        {
#ifdef _WIN32
                WORD attributes = FOREGROUND_RED;
                SetConsoleTextAttribute(_hConsole, attributes);
#else
                cout << "\33[31m"; // red color
#endif
        }

        void setWhiteFont()
        {
#ifdef _WIN32
                WORD attributes = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
                SetConsoleTextAttribute(_hConsole, attributes);
#else
                cout << "\33[37m"; // white color
#endif
        }

        static constexpr int width{60};
        static constexpr int height{20};
        char _screen[height][width];
#ifdef _WIN32
        HANDLE _hConsole;
        COORD _prevPos;
#endif
};

struct Position
{
        float x;
        float y;
};

using Pixel = char;

bool moveA_B(Position &a, chrono::milliseconds deltaTime)
{
        if (a.x >= 40)
        {
                a.x = 40;
                return true;
        }
        else
        {
                static float speed_x = 40. / 2000.;
                a.x += speed_x * deltaTime.count();
                return false;
        }
}

bool moveB_C(Position &a, chrono::milliseconds deltaTime)
{
        if (a.y >= 10)
        {
                a.y = 10;
                return true;
        }
        static float speed_y = 10. / 3000.;
        a.y += speed_y * deltaTime.count();
        return false;
}

bool moveC_A(Position &a, chrono::milliseconds deltaTime)
{
        if (a.x <= 0 && a.y <= 0)
        {
                a.x=0;
                a.y=0;//punkty początkowe są znane więc ustalenie ich pomoże przy błędach zaokrąfleń spowodowanymi operacjami na flotach
                return true;
        }
        static float speed_y = -10. / 1000.;
        static float speed_x = -40. / 1000.;
        a.x += speed_x * deltaTime.count();
        a.y += speed_y * deltaTime.count();

        return false;
}

class Engine
{
public:
        void update(chrono::milliseconds deltaTime)
        {
                if (remake != 5 * 3) // 3-liczba funkci
                {
                        if (remake % 3 == 0)
                                remake += moveA_B(position, deltaTime);
                        else if (remake % 3 == 1)
                                remake += moveB_C(position, deltaTime);
                        else
                                remake += moveC_A(position, deltaTime);
                }
        }

        void render(Screen &screen)
        {
                screen.draw(pixel, position.x, position.y);
        }

private:
        int remake;
        Pixel pixel{'y'};
        Position position{0.0f, 0.0f};

        std::chrono::_V2::system_clock::time_point s = chrono::high_resolution_clock::now();
};

int main()
{
        Engine engine;
        Screen screen;

        auto startTime = chrono::high_resolution_clock::now();
        while (1)
        {
                auto now = chrono::high_resolution_clock::now();
                auto deltaTime = chrono::duration_cast<chrono::milliseconds>(now - startTime);
                if (deltaTime >= 20ms)
                {
                        screen.clear();
                        engine.update(deltaTime);
                        engine.render(screen);
                        screen.flush();
                        startTime = now;
                }
        }

        return 0;
}