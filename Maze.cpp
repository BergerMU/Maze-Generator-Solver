// Maze Generator - Berger

#include <iostream>
#include <cstdlib>
#include <vector>
#include <cctype>
#include <string>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
using namespace std;

void imageSave(vector<vector<string>> maze)
{
    // Initializing Variables
    int scale = 20;
    int scaled_size = maze.size() * scale;
    vector<unsigned char> image(scaled_size * scaled_size);
    for (int y = 0; y < maze.size(); y++)
    {
        for (int x = 0; x < maze.size(); x++)
        {
            // Converts paths to black and walls to white and gray for the pathways
            unsigned char value = (maze[y][x] == "   ")   ? 0
                                  : (maze[y][x] == " S ") ? 80
                                  : (maze[y][x] == " E ") ? 80
                                  : (maze[y][x] == " * ") ? 80
                                  : (maze[y][x] == " . ") ? 130
                                                          : 255;
            for (int yy = 0; yy < scale; yy++)
            {
                // Scales the image up
                for (int xx = 0; xx < scale; xx++)
                {
                    image[(y * scale + yy) * scaled_size + (x * scale + xx)] = value;
                }
            }
        }
    }
    stbi_write_png("Maze.png", scaled_size, scaled_size, 1, image.data(), scaled_size);
    cout << "Maze saved to Maze.png" << endl;
}

int askMazeSize()
{
    // Initalizing variables and getting user input
    string input;
    cout << "Enter a odd number for your maze size that is at least 5: ";
    getline(cin, input);
    int digit;
    bool isdig = true;

    // Checks if each character is a digit or not
    for (char d : input)
    {
        if (!isdigit(d))
        {
            isdig = false;
            break;
        }
    }

    if (isdig)
    {
        digit = stoi(input);

        // Ensures mazes are at least size 5 or larger
        if (digit < 5)
        {
            cout << "Too small to generate maze" << endl;
        }

        // Handles even or odd input
        else if (digit % 2 == 0)
        {
            cout << "Error: Please enter an odd number" << endl;
        }
        else
        {
            return digit;
        }
    }

    // Handles non-digit inputs
    else
    {
        cout << "Not a valid number" << endl;
    }
    return askMazeSize();
}

void displayMaze(vector<vector<string>> maze)
{
    if (maze.size() > 45)
    {
        cout << "Maze too large to display in terminal" << endl;
    }
    else
    {
        // Display Board
        for (int y = 0; y < maze.size(); y++)
        {
            for (int x = 0; x < maze.size(); x++)
            {
                cout << maze[y][x];
            }
            cout << endl;
        }
    }
}

vector<vector<string>> solveMaze(vector<vector<string>> maze)
{
    // Find entrance and exit of the maze
    int start_x, start_y, exit_x, exit_y;

    for (int y = 0; y < maze.size(); y++)
    {
        for (int x = 0; x < maze[0].size(); x++)
        {
            if (maze[y][x] == " S ")
            {
                start_x = x;
                start_y = y;
            }
            else if (maze[y][x] == " E ")
            {
                exit_x = x;
                exit_y = y;
            }
        }
    }

    vector<pair<int, int>> previous_locations;
    vector<int> checking_order = {0, 1, 2, 3};

    while (start_x != exit_x || start_y != exit_y)
    {
        bool moved = false;

        for (int i = 0; i < checking_order.size() && !moved; i++)
        {
            int direction = checking_order[i];
            switch (direction)
            {
            // Right
            case 0:
                if (start_x + 1 < maze[0].size() && (maze[start_y][start_x + 1] == " E " || maze[start_y][start_x + 1] == "   "))
                {
                    start_x += 1;
                    moved = true;
                    checking_order = {1, 0, 3, 2};
                }
                break;
            // Down
            case 1:
                if (start_y + 1 < maze.size() && (maze[start_y + 1][start_x] == " E " || maze[start_y + 1][start_x] == "   "))
                {
                    start_y += 1;
                    moved = true;
                    checking_order = {2, 1, 0, 3};
                }
                break;
            // Left
            case 2:
                if (start_x - 1 >= 0 && (maze[start_y][start_x - 1] == " E " || maze[start_y][start_x - 1] == "   "))
                {
                    start_x -= 1;
                    moved = true;
                    checking_order = {3, 2, 1, 0};
                }
                break;
            // Up
            case 3:
                if (start_y - 1 >= 0 && (maze[start_y - 1][start_x] == " E " || maze[start_y - 1][start_x] == "   "))
                {
                    start_y -= 1;
                    moved = true;
                    checking_order = {0, 3, 2, 1};
                }
                break;
            }
        }
        if (moved)
        {
            maze[start_y][start_x] = " * ";
            previous_locations.push_back({start_x, start_y});
        }
        else if (!previous_locations.empty())
        {
            maze[start_y][start_x] = " . ";
            start_y = previous_locations.back().second;
            start_x = previous_locations.back().first;
            maze[start_y][start_x] = " . ";
            previous_locations.pop_back();
        }
        if (start_y == exit_y && start_x == exit_x)
        {
            break;
        }
    }
    return maze;
}

vector<vector<string>> generateMaze(int size)
{
    // Initializing blank maze and randomly selecting the start and end points
    vector<vector<string>> maze(size, vector<string>(size, "███"));
    srand(time(NULL));

    int odd_or_even = rand() % 2;
    if (odd_or_even == 0)
    {
        maze[1][0] = " S ";
        maze[size - 2][size - 1] = " E ";
    }
    else
    {
        maze[size - 2][0] = " S ";
        maze[1][size - 1] = " E ";
    }

    // Switches location of maze generation starting point to make it feel more random
    int start = rand() % 3;
    int spot_x, spot_y;
    if (start == 0)
    {
        // Places the maze generation starting point roughly in the center
        if ((size / 2) % 2 == 0)
        {
            spot_x = size / 2 + 3;
            spot_y = size / 2 - 3;
        }
        else
        {
            spot_x = (size / 2);
            spot_y = (size / 2);
        }
    }
    else if (start == 1)
    {
        // Places the maze generation starting point in the bottom left
        spot_x = 1;
        spot_y = size - 2;
    }
    else if (start == 2)
    {
        // Places the maze generation starting point in the top right
        spot_x = size - 2;
        spot_y = 1;
    }
    else
    {
        // Places the maze generation starting point in the top right
        spot_x = size - 2;
        spot_y = size - 2;
    }

    // Initializing loop variables
    bool generated = false;
    vector<int> previous_locations;

    // Loops until fully generated
    while (!generated)
    {
        // Checks if there is still room to create pathways in the maze
        if (spot_y + 2 < size && maze[spot_y + 2][spot_x] == "███" ||
            spot_y - 2 > 0 && maze[spot_y - 2][spot_x] == "███" ||
            spot_x + 2 < size && maze[spot_y][spot_x + 2] == "███" ||
            spot_x - 2 > 0 && maze[spot_y][spot_x - 2] == "███")
        {
            // Randomly chooses a direction to go
            int direction = rand() % 4;
            switch (direction)
            {
            // Down
            case 0:
                if (spot_y + 2 < size && maze[spot_y + 2][spot_x] == "███")
                {
                    maze[spot_y][spot_x] = "   ";
                    maze[spot_y + 1][spot_x] = "   ";
                    maze[spot_y + 2][spot_x] = "   ";
                    spot_y += 2;
                    break;
                }
            // Up
            case 1:
                if (spot_y - 2 > 0 && maze[spot_y - 2][spot_x] == "███")
                {
                    maze[spot_y][spot_x] = "   ";
                    maze[spot_y - 1][spot_x] = "   ";
                    maze[spot_y - 2][spot_x] = "   ";
                    spot_y -= 2;
                    break;
                }
            // Right
            case 2:
                if (spot_x + 2 < size && maze[spot_y][spot_x + 2] == "███")
                {
                    maze[spot_y][spot_x] = "   ";
                    maze[spot_y][spot_x + 1] = "   ";
                    maze[spot_y][spot_x + 2] = "   ";
                    spot_x += 2;
                    break;
                }
            // Left
            case 3:
                if (spot_x - 2 > 0 && maze[spot_y][spot_x - 2] == "███")
                {
                    maze[spot_y][spot_x] = "   ";
                    maze[spot_y][spot_x - 1] = "   ";
                    maze[spot_y][spot_x - 2] = "   ";
                    spot_x -= 2;
                    break;
                }
            }
            // Adds the location of each new direction to a vector to backtrack later
            previous_locations.push_back(spot_x);
            previous_locations.push_back(spot_y);
        }

        // Checks if there aren't any more locations and starts to backtrack
        else if (!previous_locations.empty())
        {
            spot_y = previous_locations.back();
            previous_locations.pop_back();
            spot_x = previous_locations.back();
            previous_locations.pop_back();
        }

        // Exits the loop when there aren't any more locations to backtrack to
        if (previous_locations.empty())
        {
            generated = true;
        }
    }
    return maze;
}

int getValidatedInput(int min, int max)
{
    string input;
    int digit;
    bool valid = false;

    while (!valid)
    {
        getline(cin, input);
        valid = true;

        // Check if input is a valid digit
        for (char d : input)
        {
            if (!isdigit(d))
            {
                valid = false;
                break;
            }
        }

        if (valid)
        {
            digit = stoi(input);

            // Check if the digit is within the allowed range
            if (digit < min || digit > max)
            {
                valid = false;
                cout << "Please enter a number between " << min << " and " << max << ": ";
            }
        }
        else
        {
            cout << "Invalid input. Please enter a number: ";
        }
    }

    return digit;
}

void menu(vector<vector<string>> maze)
{
    while (true)
    {
        cout << "What would you like to do?" << endl;
        cout << "(1) Save" << endl;
        cout << "(2) Generate new Maze" << endl;
        cout << "(3) Solve" << endl;
        cout << "(4) Exit" << endl;

        int choice = getValidatedInput(1, 4);
        system("clear");

        switch (choice)
        {
        // Save
        case 1:
            displayMaze(maze);
            imageSave(maze);
            menu(maze);
            break;
        // New maze
        case 2:
            // Generate new maze
            maze = generateMaze(askMazeSize());
            displayMaze(maze);
            break;
        // Solve
        case 3:
            maze = solveMaze(maze);
            displayMaze(maze);
            cout << "Maze Solved" << endl;
            break;
        // Exit
        case 4:
            cout << "Bye" << endl;
            exit(0);
        default:
            cout << "Invalid option. Please try again." << endl;
            break;
        }
    }
}

int main()
{
    system("clear");

    // Title screen
    cout << "Maze Generator - Berger" << endl;
    cout << "-------------------------" << endl;

    // Gets user input and generates appropriate maze
    int size = askMazeSize();
    vector<vector<string>> maze = generateMaze(size);
    system("clear");
    displayMaze(maze);
    menu(maze);
}