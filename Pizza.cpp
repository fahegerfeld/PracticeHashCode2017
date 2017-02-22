//
// Created by bennatura on 2/22/17.
//

#include <fstream>
#include <sstream>
#include <iostream>
#include "Pizza.h"

static void print(BoolMatrix & used)
{
    for (size_t i = 0; i < used.size(); ++i)
    {
        for (size_t j = 0; j < used[i].size(); ++j)
        {
            if (used[i][j])
            {
                std::cout << "X";
            }
            else
            {
                std::cout << "O";
            }
        }
        std::cout << std::endl;
    }
}

Pizza::Pizza(char * file) {
    std::fstream filestream(file);

    std::string line;
    getline(filestream, line);
    std::stringstream ss(line);

    ss >> _rows;
    ss >> _columns;
    ss >> _min_content;
    ss >> _max_content;

    _cells.reserve(_rows);

    for(size_t i = 0; i < _rows; i++)
    {
        std::vector<Content> cur_row;
        getline(filestream,line);

        for(size_t j = 0; j < line.length(); j++)
        {
            Content const content = line[j] == 'T' ? Content::T : Content::M;
            cur_row.push_back(content);
        }

        _cells.push_back(cur_row);
    }
}

void Pizza::print_cells() {
    for(size_t i = 0; i < _cells.size(); i++)
    {
        for(size_t j = 0; j < _cells[i].size(); j++)
        {
            std::cout << _cells[i][j];
        }
        std::cout << std::endl;
    }
}

int Pizza::get_score() {

    bool cell_used[_rows][_columns];

    for(size_t i = 0; i < _rows; i++)
    {
        for(size_t j = 0; j < _columns; j++)
        {
            cell_used[i][j] = false;
        }
    }

    int score = 0;

    std::cout << "#Slices: " << _slices.size() << std::endl;

    for(size_t i = 0; i < _slices.size(); i++)
    {
        Slice const slice = _slices[i];

        //test if slice is legit

        if(std::max(slice.row1, slice.row2) >= _rows or std::max(slice.column1, slice.column2) >= _columns)
        {
            return SLICES_OUT_OF_BOUNDS;
        }

        std::vector<unsigned> contents(2, 0);

        for(unsigned row = slice.row1; row <= slice.row2; row++)
        {
            for(unsigned column = slice.column1; column <= slice.column2; column++)
            {
                if(cell_used[row][column])
                {
                    return SLICES_OVERLAP;
                }

                cell_used[row][column] = true;
                contents[_cells[row][column]]++;
            }
        }

        if(contents[0] + contents[1] > _max_content or std::min(contents[0],contents[1]) < _min_content)
        {
            return SLICES_WRONG_CONTENT;
        }

        score += slice.size();
    }

    return score;
}

void Pizza::write_submission_file(char const * output_filename)
{
    std::ofstream output_file;
    output_file.open(output_filename);

    output_file << _slices.size();

    for(size_t i = 0; i < _slices.size(); i++)
    {
        Slice const slice = _slices[i];
        output_file << std::endl << slice.row1 << " " << slice.column1 << " " << slice.row2 << " " << slice.column2;
    }

    output_file.close();
    /*std::string file_string(output_filename);
    system(("gedit " + file_string).c_str());*/
}

void Pizza::run_algorithm() {
    /*greedy algorithm, just adds new legal slices if possible*/
    bool cell_used[_rows][_columns];
    for(size_t i = 0; i < _rows; i++)
    {
        for(size_t j = 0; j < _columns; j++)
        {
            cell_used[i][j] = false;
        }
    }

    for(unsigned i = 0; i < _rows;i++)
    {
        for(unsigned k = i; k < std::min(_rows,i+_max_content+1);k++)
        {
            for(unsigned j = 0; j < _columns; j++)
            {
                for(unsigned l = j; l < std::min(_columns,j+_max_content+1); l++)
                {
                    //test if slice is legit
                    bool this_slice_not_used = true;
                    std::vector<unsigned> contents(2, 0);

                    for(unsigned row = i; row <= k; row++)
                    {
                        for(unsigned column = j; column <= l; column++)
                        {
                            if(cell_used[row][column]!=false)
                            {
                                this_slice_not_used = false;
                                goto TEST;
                            }
                            contents[_cells[row][column]]++;
                        }
                    }
                    if(contents[0]+contents[1] > _max_content or std::min(contents[0],contents[1]) < _min_content)
                    {
                        this_slice_not_used = false;
                    }
                   TEST:               //add if possible
                    if(this_slice_not_used)
                    {
                        _slices.push_back(Slice(i,j,k,l));
                        for(unsigned row = i; row <= k; row++)
                        {
                            for(unsigned column = j; column <= l; column++)
                            {
                                cell_used[row][column] = true;
                            }
                        }
                    }
                }
            }
        }
    }
}


bool Pizza::legit_slice(Slice& slice, BoolMatrix & used)
{
    if (slice.size() <= _max_content) {
        for (size_t i = slice.row1; i <= slice.row2; ++i) {
            for (size_t j = slice.column1; j <= slice.column2; ++j) {
                if (used[i][j]) {
                    return false;
                }
            }
        }
        return true;
    }
    else
    {
        return false;
    }
}

bool Pizza::can_extend_slice(Slice& slice)
{
    unsigned size = slice.size();
    unsigned width = slice.width();
    unsigned height = slice.height();
    return (size + width <= _max_content or size + height <= _max_content);
}

bool Pizza::extend_slice(Slice& slice, BoolMatrix& used)
{
    if (can_extend_slice(slice))
    {
        Slice copy(slice);
        std::vector<unsigned> results(4,0);
        if (copy.row1 > 0)
        {
            --copy.row1;
            if (legit_slice(copy, used))
            {
                results[0] = copy.size();
            }
            ++copy.row1;

        }
        if (copy.row2 < _rows - 1)
        {
            ++copy.row2;
            if (legit_slice(copy, used))
            {
                results[1] = copy.size();
            }

            --copy.row2;

        }
        if (copy.column1 > 0)
        {
            --copy.column1;
            if (legit_slice(copy, used))
            {
                results[2] = copy.size();
            }

            ++copy.column1;

        }
        if (copy.column2 < _columns - 1)
        {
            ++copy.column2;
            if (legit_slice(copy, used))
            {
                results[3] = copy.size();
            }

            --copy.column2;

        }

        size_t best_idx = 0;
        size_t size = 0;
        for (size_t i = 0; i < 4; ++i)
        {
            if (results[i] > size)
            {
                best_idx = i;
                size = results[i];
            }
        }

        if (size == 0)
        {
            return false;
        }
        else
        {
            switch (best_idx)
            {
                case 0:
                    --slice.row1;
                    break;
                case 1:
                    ++slice.row2;
                    break;
                case 2:
                    --slice.column1;
                    break;
                case 3:
                    ++slice.column2;
                    break;
                default:
                    return false;
            }
            return true;
        }
    }

    return false;
}

void Pizza::optimize_slice(Slice & slice, BoolMatrix & used)
{
    //std::cout << "Before: " << slice.size() << std::endl;
    //slice.print();
    slice.assign_cells(false, used);
    //print(used);
    while (extend_slice(slice, used));
    slice.assign_cells(true, used);
    //std::cout << "After: " << slice.size() << std::endl;
    //slice.print();
    //print(used);

}

void Pizza::run_algorithm_falko()
{
    /*size_t num_tomatoes = 0;
    size_t num_mushrooms = 0;
    for (size_t i = 0; i < _rows; ++i)
    {
        for (size_t j = 0; j < _columns; ++j)
        {
            if (_cells[i][j] == T)
            {
                ++num_tomatoes;
            }
            else
            {
                ++num_mushrooms;
            }
        }
    }
    Content rare = num_tomatoes >= num_mushrooms ? M : T;*/
    
    BoolMatrix used(_rows);
    BoolVector init(_columns, false);
    for (size_t i = 0; i < used.size(); ++i)
    {
        used[i] = init;
    }

    run_algorithm();

    for (size_t i = 0; i < _slices.size(); ++i)
    {
        _slices[i].assign_cells(true, used);
    }

    //print(used);

    for (size_t i = 0; i < _slices.size(); ++i)
    {
        //std::cout << "Slice " << i << std::endl;
        optimize_slice(_slices[i], used);
    }

}


