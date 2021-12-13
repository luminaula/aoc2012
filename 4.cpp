
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

class Board;

struct Cell {
    long value;
    std::vector<long *> indices;
    Board *parent;
};

class Board {
  private:
    uint32_t m_width, m_height;
    std::vector<long> m_count_width;
    std::vector<long> m_count_height;

  public:
    bool done;
    size_t m_id;
    std::vector<Cell> m_data;
    Board(int width, int height, std::vector<int> board_data, size_t id) : m_width(width), m_height(height), m_id(id) {
        done = false;
        m_count_width.resize(width, width);
        m_count_height.resize(height, height);

        for (int i = 0; i < m_height; i++) {
            for (int j = 0; j < m_width; j++) {
                Cell cell;
                cell.value = board_data[i * m_width + j];
                cell.parent = this;
                cell.indices.push_back(&m_count_height[i]);
                cell.indices.push_back(&m_count_width[j]);
                m_data.push_back(std::move(cell));
            }
        }
    }

    void print() {
        for (int i = 0; i < m_height; i++) {
            for (int j = 0; j < m_width; j++) {
                std::cout << m_data[i * m_width + j].value << " ";
            }
            std::cout << std::endl;
        }
    }

    long get_sum() {
        long sum = 0;
        for (auto &cell : m_data) {
            sum += cell.value;
        }
        return sum;
    }

    std::vector<Cell> &get_data() { return m_data; }
};

class Bingo {
  private:
    std::string m_input_data;
    std::vector<Board> m_boards;
    std::vector<int> m_numbers;
    std::vector<std::vector<Cell *>> indices;
    int board_width;
    int board_height;

  public:
    void parse_input() {
        std::string::size_type s;
        size_t largest_number = 0;
        size_t num_boards = 0;
        s = m_input_data.find('\n');
        std::string numbers = m_input_data.substr(0, s);
        m_input_data = m_input_data.substr(s + 2);
        std::istringstream ss(numbers);
        std::string val;

        while (std::getline(ss, val, ',')) {
            m_numbers.push_back(std::atoi(val.c_str()));
            if (largest_number < m_numbers.back()) {
                largest_number = m_numbers.back();
            }
        }

        ss = std::istringstream(m_input_data);

        std::vector<int> board_data;
        board_width = 0, board_height = 0;
        while (std::getline(ss, val, '\n')) {
            if (val.empty()) {
                m_boards.emplace_back(board_width, board_height, board_data, num_boards++);
                board_data = std::vector<int>();
                board_height = 0;
                continue;
            }
            board_width = 0;
            std::istringstream number_stream(val);
            std::string cell_value;
            while (std::getline(number_stream, cell_value, ' ')) {
                if (cell_value.empty()) {
                    continue;
                }
                board_width++;
                board_data.push_back(std::atoi(cell_value.c_str()));
                if (largest_number < board_data.back()) {
                    largest_number = board_data.back();
                }
            }
            board_height++;
        }
        m_boards.emplace_back(board_width, board_height, board_data, num_boards++);
        indices.resize(largest_number + 1);
    }

    void read_input(std::filesystem::path &path) {
        std::ifstream file(path, std::ios::ate);
        size_t file_size = file.tellg();
        m_input_data.resize(file_size);
        file.seekg(std::ios::beg);
        file.read(m_input_data.data(), file_size);
        file.close();
    }

    void create_indice_array() {
        for (auto &board : m_boards) {
            for (auto &cell : board.m_data) {
                cell.parent = &board;
                indices[cell.value].push_back(&cell);
            }
        }
    }

    void play() {
        bool part1 = false;
        int boards_finished = 0;
        for (auto num : m_numbers) {
            for (auto cell : indices[num]) {
                *cell->indices[0] -= 1;
                *cell->indices[1] -= 1;
                cell->value = 0;
                if (*cell->indices[0] == 0 || *cell->indices[1] == 0) {
                    if (!part1) {
                        //cell->parent->print();
                        std::cout << cell->parent->get_sum() * num << std::endl;
                        part1 = true;
                    }
                    if (!cell->parent->done) {
                        boards_finished++;
                        if (boards_finished == m_boards.size()) {
                            for (auto &board : m_boards) {
                                if (!board.done) {
                                    //board.print();
                                    std::cout << cell->parent->get_sum() * num << std::endl;
                                }
                            }
                        }
                        cell->parent->done = true;
                    }
                }
            }
        }
    }

    Bingo(std::filesystem::path path) {
        read_input(path);
        parse_input();
        create_indice_array();
        play();
    }
};

int main(int argc, char **argv) {
    Bingo bing(std::filesystem::current_path() / "input4");

    return 0;
}