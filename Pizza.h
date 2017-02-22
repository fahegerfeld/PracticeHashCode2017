//
// Created by bennatura on 2/22/17.
//

#ifndef PRACTICEHASHCODE2017_PIZZA_H
#define PRACTICEHASHCODE2017_PIZZA_H

#include <vector>
#include <assert.h>

typedef std::vector<bool> BoolVector;
typedef std::vector<BoolVector> BoolMatrix;

const int SLICES_WRONG_CONTENT = -1;
const int SLICES_OVERLAP = -2;
const int SLICES_OUT_OF_BOUNDS = -3;

enum Content{T = 0, M =1};

struct Slice{

    Slice (unsigned row1_, unsigned column1_, unsigned row2_, unsigned column2_)
    : row1(row1_), column1(column1_), row2(row2_), column2(column2_)
    {}

    Slice(Slice const & slice)
    :
        row1(slice.row1),
        column1(slice.column1),
        row2(slice.row2),
        column2(slice.column2)
    {}

    unsigned row1;
    unsigned column1;
    unsigned row2;
    unsigned column2;

    void print() const
    {
        std::cout << "[" << row1 << ", " << row2 << "] x [" << column1 << ", " << column2 << "]" << std::endl;
    }

    void assign_cells(bool value, BoolMatrix & used) const
    {
        for (size_t i = row1; i <= row2; ++i)
        {
            for (size_t j = column1; j <= column2; ++j)
            {
                used[i][j] = value;
            }
        }
    }

    unsigned width() const
    {
        assert(row2 >= row1);
        return row2 - row1 + 1;
    }
    unsigned height() const
    {
        assert(column2 >= column1);
        return column2 - column1 + 1;
    }

    unsigned size() const
    {
        return width() * height();
    }
};

inline std::ostream& operator<< (std::ostream& stream, const Content& content)
{
    stream << ((content == Content::T) ? "T" : "M");
    return stream;
}

class Pizza {
public:
    Pizza(char * file);
    void print_cells();
    int get_score();
    void run_algorithm();
    void run_algorithm_falko();
    void write_submission_file(char const *);
    bool legit_slice(Slice & slice, BoolMatrix & used);
    bool can_extend_slice(Slice & slice);
    bool extend_slice(Slice & slice, BoolMatrix & used);
    void optimize_slice(Slice & slice, BoolMatrix & used);
private:

    unsigned _rows;
    unsigned _columns;
    unsigned _min_content;
    unsigned _max_content;
    std::vector<std::vector<Content> > _cells;

    std::vector<Slice> _slices;
};


#endif //PRACTICEHASHCODE2017_PIZZA_H
