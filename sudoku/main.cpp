#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

constexpr int CELL_SIZE = 60;
constexpr int GRID_SIZE = 9;
constexpr int WINDOW_SIZE = CELL_SIZE * GRID_SIZE;

bool isValid(const std::vector<std::vector<int>>& sudokuboard, int row, int col, int num) {
	int startRow = row - row % 3;
	int startCol = col - col % 3;

	for (int i = 0; i < 9; i++) {
		// Check row and column in the same loop
		if (sudokuboard[row][i] == num || sudokuboard[i][col] == num) {
			return false;
		}

		// Check the 3ª3 subgrid
		int gridRow = startRow + i / 3;
		int gridCol = startCol + i % 3;
		if (sudokuboard[gridRow][gridCol] == num) {
			return false;
		}
	}

	return true;
}

std::vector<std::vector<int>> sudokuBoard = {
	  {5, 3, 0, 0, 7, 0, 0, 0, 0},
	  {6, 0, 0, 1, 9, 5, 0, 0, 0},
	  {0, 9, 8, 0, 0, 0, 0, 6, 0},
	  {8, 0, 0, 0, 6, 0, 0, 0, 3},
	  {4, 0, 0, 8, 0, 3, 0, 0, 1},
	  {7, 0, 0, 0, 2, 0, 0, 0, 6},
	  {0, 6, 0, 0, 0, 0, 2, 8, 0},
	  {0, 0, 0, 4, 1, 9, 0, 0, 5},
	  {0, 0, 0, 0, 8, 0, 0, 7, 9}
};



// Stores which cells are originally given (to prevent modifying them)
std::vector<std::vector<bool>> fixedCells(GRID_SIZE, std::vector<bool>(GRID_SIZE, false));

int selectedRow = -1, selectedCol = -1;

void markFixedCells() {
	for (int i = 0; i < GRID_SIZE; i++) {
		for (int j = 0; j < GRID_SIZE; j++) {
			if (sudokuBoard[i][j] != 0) {
				fixedCells[i][j] = true;
			}
		}
	}
}


// Function to draw the board
void drawBoard(sf::RenderWindow& window, sf::Font& font) {
    window.clear(sf::Color::White);
    sf::RectangleShape cell(sf::Vector2f(CELL_SIZE, CELL_SIZE));

    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            cell.setPosition(j * CELL_SIZE, i * CELL_SIZE);
            cell.setOutlineThickness(1);
            cell.setOutlineColor(sf::Color::Black);
            cell.setFillColor((i == selectedRow && j == selectedCol) ? sf::Color(200, 200, 255) : sf::Color::White);
            window.draw(cell);

            if (sudokuBoard[i][j] != 0) {
                sf::Text text;
                text.setFont(font);
                text.setString(std::to_string(sudokuBoard[i][j]));
                text.setCharacterSize(40);
                text.setFillColor(fixedCells[i][j] ? sf::Color::Black : sf::Color::Magenta);
                text.setPosition(j * CELL_SIZE + 20, i * CELL_SIZE + 10);
                window.draw(text);
            }
        }
    }

    // Draw bold lines for 3x3 blocks
    sf::RectangleShape thickLine(sf::Vector2f(WINDOW_SIZE, 3));
    thickLine.setFillColor(sf::Color::Black);
    for (int i = 1; i < 3; i++) {
        thickLine.setPosition(0, i * 3 * CELL_SIZE);
        window.draw(thickLine);
        thickLine.setSize(sf::Vector2f(3, WINDOW_SIZE));
        thickLine.setPosition(i * 3 * CELL_SIZE, 0);
        window.draw(thickLine);
        thickLine.setSize(sf::Vector2f(WINDOW_SIZE, 3)); // Reset width
    }

    window.display();
}

// Main function
int main() {
    sf::RenderWindow window(sf::VideoMode(WINDOW_SIZE, WINDOW_SIZE), "Sudoku Game");
    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        std::cerr << "Error loading font!" << std::endl;
        return 1;
    }

    markFixedCells(); // Identify fixed numbers at the start

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            // Mouse Click - Select a Cell
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    int x = event.mouseButton.x;
                    int y = event.mouseButton.y;
                    selectedCol = x / CELL_SIZE;
                    selectedRow = y / CELL_SIZE;
                }
            }

            // Keyboard Input - Modify Cell
            if (event.type == sf::Event::KeyPressed && selectedRow != -1 && selectedCol != -1) {
                if (!fixedCells[selectedRow][selectedCol]) { // Prevent modifying fixed cells
                    if (event.key.code >= sf::Keyboard::Num1 && event.key.code <= sf::Keyboard::Num9) {
                        int num = (event.key.code - sf::Keyboard::Num1) + 1;

                        // Validate before placing the number
                        if (isValid(sudokuBoard, selectedRow, selectedCol, num)) {
                            sudokuBoard[selectedRow][selectedCol] = num;
                        }
                        else {
                            // Temporarily show the invalid number
                            sudokuBoard[selectedRow][selectedCol] = num;
                            drawBoard(window, font); // Refresh the board
                            sf::sleep(sf::milliseconds(500)); // Show for 500ms
                            sudokuBoard[selectedRow][selectedCol] = 0; // Remove invalid number
                        }
                    }

                    if (event.key.code == sf::Keyboard::Backspace || event.key.code == sf::Keyboard::Delete) {
                        sudokuBoard[selectedRow][selectedCol] = 0; // Allow deleting a number
                    }
                }
            }
        }

        // Draw the board
        drawBoard(window, font);
    }

    return 0;
}








