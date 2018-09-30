#include "linear_equation.hpp"

#include <stdexcept>

using namespace std;

Matrix<double> LinearEquation::solve() {
    auto row = m_coef.row(), col = m_coef.col();

    Matrix<double> result(row, 1);

    for(int i = 0; i < row - 1; ++i) {
        if(m_coef.at(i, i) == 0 || m_coef.at(i + 1, i) == 0)
            continue;
        auto coef = m_coef.at(i + 1, i) / m_coef.at(i, i);
        for(int r = i + 1; r < row; ++r) {
            for(int c = 0; c < col; ++c)
                m_coef.at(r, c) -= m_coef.at(i, c) * coef;
        }
    }

    if(m_coef.at(row - 1, row - 1) == 0)
        throw invalid_argument("no solution");

    result[row - 1][0] = m_coef[row - 1][row] / m_coef[row - 1][row - 1];

    for(int i = row - 2; i >= 0; --i) {
        double sum = 0;
        for(int j = i + 1; j < col - 1; ++j)
            sum += m_coef[i][j] * result[j][0];
        result[i][0] = (m_coef[i][row] - sum) / m_coef[i][i];
    }

    return result;
}
