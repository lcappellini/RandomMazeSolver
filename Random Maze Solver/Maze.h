#pragma once
#include <vector>
#include <algorithm>
#include <functional>
#include <array>
#include "Line.h"
#include "Rect.h"
#include "Path.h"
#include <opencv2/opencv.hpp>
#include <random>
#include <fstream>
#include <limits>

class Maze {
    struct Cell {
        bool top = false;
        bool right = false;
        bool bottom = false;
        bool left = false;
    };

    static inline const cv::Scalar colors[16] = {
        cv::Scalar(31, 119, 180), // Blue
        cv::Scalar(44, 160,  44), // Green
        cv::Scalar(214,  39,  40), // Red
        cv::Scalar(255, 127,  14), // Orange
        cv::Scalar(148, 103, 189), // Purple
        cv::Scalar(23, 190, 207), // Cyan
        cv::Scalar(227, 119, 194), // Pink
        cv::Scalar(188, 189,  34), // Yellow-green
        cv::Scalar(0,  85, 255), // Strong blue
        cv::Scalar(0, 180, 255), // Orange-yellow
        cv::Scalar(0, 200, 130), // Emerald
        cv::Scalar(180,  50, 180), // Magenta
        cv::Scalar(255,  80,  80), // Light blue
        cv::Scalar(80,  80, 220), // Red-violet
        cv::Scalar(50, 200, 200), // Turquoise
        cv::Scalar(50, 120, 220)  // Deep orange
    };

public:
    std::vector<std::vector<Cell>> cells;
    Point start_p;
    Rect end_region;
    int16_t end_region_w;
    int16_t end_region_h;
    int16_t w;
    int16_t h;
    int16_t cw;
    int16_t ch;

    int cols = 32;
    int rows = 24;

    Maze(int16_t w, int16_t h) {
        cw = w / cols;
        ch = h / rows;
        this->w = cw * cols;
        this->h = ch * rows;
        end_region_w = cw / 4;
        end_region_h = ch / 4;
        createMaze();
        start_p = { 1, 1 };
        end_region = { (int16_t)(this->h - end_region_h - 1), (int16_t)(this->w - end_region_w - 1), (int16_t)(this->h - 1), (int16_t)(this->w - 1) };
    }

    Point move(Point p1, Point p2) const {
        //muri esterni
        if (p2.x < 0)
            p2.x = -p2.x;
        if (p2.y < 0)
            p2.y = -p2.y;
        if (p2.x >= cols * cw)
            p2.x = 2 * cols * cw - p2.x;
        if (p2.y >= rows * ch)
            p2.y = 2* rows * ch - p2.y;

        int c1x = p1.x / cw;
        int c2x = p2.x / cw;
        int c1y = p1.y / ch;
        int c2y = p2.y / ch;

        if (c1x != c2x && c1y != c2y) {
            return p1;
        }

        // cambio cella orizzontalmente
        if (c1x != c2x) {
            const Cell& c = cells[c1y][c1x];
            if (p2.x > p1.x && c.right) {
                int wallx = (c1x + 1) * cw;
                if (p2.x == wallx) {
                    p2.x--;
                }
                else {
                    p2.x = 2 * wallx - p2.x;
                }
            }
            else if (p2.x < p1.x && c.left) {
                int wallx = c1x * cw;
                if (p2.x == wallx) {
                    p2.x++;
                }
                else {
                    p2.x = 2 * wallx - p2.x;
                }
            }
        }

        // cambio cella verticalmente
        if (c1y != c2y) {
            const Cell& c = cells[c1y][c1x];
            if (p2.y > p1.y && c.bottom) {
                int wally = (c1y + 1) * ch;
                if (p2.y == wally) {
                    p2.y--;
                }
                else {
                    p2.y = 2 * wally - p2.y;
                }

            }
            else if (p2.y < p1.y && c.top) {
                int wally = c1y * ch;
                if (p2.y == wally) {
                    p2.y++;
                }
                else {
                    p2.y = 2 * wally - p2.y;
                }
            }
        }

        return p2;
    }

    void drawMaze() {
        cv::Mat image(h + 1, w + 1, CV_8UC3, cv::Scalar(0, 0, 0));
        drawCells(image);
        cv::imwrite("maze.png", image);
        cv::imshow("ITER", image);
        cv::waitKey(0);
    }

    void drawMaze(Path& path) {
        cv::Mat image(h + 1, w + 1, CV_8UC3, cv::Scalar(0, 0, 0));

        const cv::Scalar& color = colors[0];
        for (size_t i = 1; i < path.points.size(); i++) {
            Point& p1 = path.points[i - 1];
            Point& p2 = path.points[i];
            cv::line(
                image,
                cv::Point(p1.x, p1.y),
                cv::Point(p2.x, p2.y),
                color,
                1,
                cv::LINE_8
            );
        }
        drawCells(image);

        cv::imwrite("maze.png", image);
    }

private:
    void drawCells(cv::Mat& image) {
        image.at<cv::Vec3b>((int)start_p.y, (int)start_p.x) = cv::Vec3b(0, 0, 255);

        for (int y = 0; y < rows; ++y) {
            for (int x = 0; x < cols; ++x) {
                const Cell& cell = cells[y][x];
                const int x0 = x * cw;
                const int x1 = (x + 1) * cw;
                const int y0 = y * ch;
                const int y1 = (y + 1) * ch;

                if (cell.top)
                    cv::line(image, cv::Point(x0, y0), cv::Point(x1, y0), cv::Scalar(255, 255, 255), 1, cv::LINE_8);
                if (cell.bottom)
                    cv::line(image, cv::Point(x0, y1), cv::Point(x1, y1), cv::Scalar(255, 255, 255), 1, cv::LINE_8);
                if (cell.left)
                    cv::line(image, cv::Point(x0, y0), cv::Point(x0, y1), cv::Scalar(255, 255, 255), 1, cv::LINE_8);
                if (cell.right)
                    cv::line(image, cv::Point(x1, y0), cv::Point(x1, y1), cv::Scalar(255, 255, 255), 1, cv::LINE_8);
            }
        }

        cv::rectangle(image, cv::Point(end_region.left, end_region.top), cv::Point(end_region.right, end_region.bottom), cv::Scalar(0, 0, 255), -1);
    }


    int saveMaze(const std::string& filename) const {
        std::ofstream file(filename);
        file << rows << ' ' << cols << '\n';
        for (int y = 0; y < rows; ++y) {
            for (int x = 0; x < cols; ++x) {
                const Cell& cell = cells[y][x];
                file << cell.top << ' ' << cell.right << ' ' << cell.bottom << ' ' << cell.left << '\n';
            }
        }
        return 0;
    }

    int loadMaze(const std::string& filename) {
        std::ifstream file(filename);
        file >> rows >> cols;
        cells.assign(rows, std::vector<Cell>(cols));
        for (int y = 0; y < rows; ++y) {
            for (int x = 0; x < cols; ++x) {
                Cell& cell = cells[y][x];

                if (!(file >> cell.top
                    >> cell.right
                    >> cell.bottom
                    >> cell.left)) {
                    return 1;
                }
            }
        }
        return 0;
    }    

    void createMaze() {
        if (!loadMaze("maze.dat"))
            return;

        cells.assign(rows, std::vector<Cell>(cols));
        for (auto& row : cells)
            for (auto& cell : row)
                cell = { true, true, true, true }; // tutti i muri presenti all'inizio

        std::vector<std::vector<bool>> visited(rows, std::vector<bool>(cols, false));

        std::random_device rd;
        std::mt19937 rng(rd());

        // Recursive Backtracking
        std::function<void(int, int)> visit = [&](int x, int y) {
            visited[y][x] = true;

            std::array<std::pair<int, int>, 4> neighbors = { {
                { 0, -1 }, // sopra
                { 1,  0 }, // destra
                { 0,  1 }, // sotto
                {-1,  0 }  // sinistra
            } };

            std::shuffle(neighbors.begin(), neighbors.end(), rng);

            for (const auto& [dx, dy] : neighbors) {
                const int nx = x + dx;
                const int ny = y + dy;

                if (nx < 0 || nx >= cols || ny < 0 || ny >= rows)
                    continue;
                if (visited[ny][nx])
                    continue;

                // rimuovi il muro tra le due celle
                if (dx == 1) {
                    cells[y][x].right = false;
                    cells[ny][nx].left = false;
                }
                else if (dx == -1) {
                    cells[y][x].left = false;
                    cells[ny][nx].right = false;
                }
                else if (dy == 1) {
                    cells[y][x].bottom = false;
                    cells[ny][nx].top = false;
                }
                else if (dy == -1) {
                    cells[y][x].top = false;
                    cells[ny][nx].bottom = false;
                }

                visit(nx, ny);
            }
            };

        visit(0, 0);
        saveMaze("maze.dat");
    }

};