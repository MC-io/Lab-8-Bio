#include <iostream>
#include <vector>
#include <algorithm>
#include <math.h>
#include <fstream>

class SecondaryStruct
{
public:

    std::string seq;
    std::vector<std::vector<int>> matrix;
    std::vector<std::vector<bool>> up;
    std::vector<std::vector<bool>> right;
    std::vector<std::vector<bool>> diag;
    int best_score;
    int pos_best_i;
    int pos_best_j;

    int score_function(int i, int j)
    {
        if (seq[i] == 'A' && seq[j] == 'U') return -1;
        if (seq[i] == 'U' && seq[j] == 'A') return -1;
        if (seq[i] == 'C' && seq[j] == 'G') return -1;
        if (seq[i] == 'G' && seq[j] == 'C') return -1;
        return 0;
    }

    SecondaryStruct(std::string seq_)
    {
        std::ofstream file("graph.dot");
        file << "graph G {\n";
        this->seq = seq_;

        matrix = std::vector<std::vector<int>>(seq.size(), std::vector<int>(seq.size()));
        up = std::vector<std::vector<bool>>(seq.size(), std::vector<bool>(seq.size()));
        right = std::vector<std::vector<bool>>(seq.size(), std::vector<bool>(seq.size()));
        diag = std::vector<std::vector<bool>>(seq.size(), std::vector<bool>(seq.size()));

        for (int i = 0; i < seq.size(); i++)
        {
            file << i << " [label=\"" << this->seq[i] << "\"];\n";
        }

        for (int i = 0; i < seq.size(); i++)
        {
            matrix[i][i] = 0;
            if (i > 0) 
            {
                matrix[i][i - 1] = 0;                
                up[i][i - 1] = true;
                right[i][i - 1] = true;

            }
            if (i < seq.size() - 1) file << i << " -- " << i + 1 << ";\n";
        }

        int best_scr = 0;
        int best_i; 
        int best_j;
        for (int k = 2; k < seq.size(); k++)
        {
            int j = k;

            for (int i = 0; i < seq.size() - k; i++)
            {

                int alpha = this->score_function(i, j);
                int fourth = INT16_MAX;
                for (int kk = i + 1; kk < j; kk++)
                {
                    if (matrix[i][kk] + matrix[kk + 1][j] < fourth) fourth = matrix[i][kk] + matrix[kk + 1][j];
                }
                matrix[i][j] = std::min({matrix[i + 1][j], matrix[i][j - 1], matrix[i + 1][j - 1] + alpha, fourth});
                if (matrix[i][j] == matrix[i + 1][j])
                {
                    up[i][j] = true;
                }
                if (matrix[i][j] == matrix[i][j - 1])
                {
                    right[i][j] = true;
                }
                if (matrix[i][j] == matrix[i + 1][j - 1] + alpha)
                {
                    diag[i][j] = true;
                }

                if (best_scr < matrix[i][j])
                {
                    best_scr = matrix[i][j];
                    best_i = i;
                    best_j = j;
                }
                j++;
            }
        }
        this->best_score = best_scr;
        this->pos_best_i = best_i;
        this->pos_best_j = best_j;

        int x = 0;
        int y = seq.size() - 1;
        while (x < y)
        {
            if (diag[x][y])
            {
                file << x << " -- " << y << ";\n";
                file << x << " -- " << y << ";\n";
                x++;
                y--;
            }
            else if (right[x][y])
            {
                y--;
            }
            else if (up[x][y])
            {
                x++;
            }
            else break;
        }
        file << "\n}";
        file.close();
    }


};


void print_matrix(std::vector<std::vector<int>> matrix)
{
    for (int i = 0; i <matrix.size(); i++)
    {
        for (int j = 0; j < matrix.size(); j++)
        {
            std::cout << matrix[i][j] << ' ';
        }
        std::cout << '\n';
    }
}


int main()
{
    std::string seq = "AACUUGUCUUAGCUUUGCAGUCGAGUU";


    SecondaryStruct seco(seq);

    print_matrix(seco.matrix);

    std::ofstream out("result.txt");
    out << " \t-\t\t";
    for (int i = 0; i < seq.size(); i++)
    {
        out << seq[i] << "\t\t";
    }

    out << '\n';
    for (int i = 0; i < seco.matrix.size(); i++)
    {

        if (i == 0) out << "-";
        else out << seq[i - 1];
        for (int j = 0; j < seco.matrix[i].size(); j++)
        {
            if (seco.diag[i][j]) out << "\\\t";
            else out << "\t";
            if (seco.up[i][j]) out << "^\t";
            else out << "\t";
        }
        out << '\n';

        for (int j = 0; j < seco.matrix[i].size(); j++)
        {
            if (seco.right[i][j]) out << "<\t";
            else out << "\t";
            out << seco.matrix[i][j] << '\t';
        }
        out << '\n';
    }
    out << '\n';




    return 0;
}