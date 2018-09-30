#ifndef __LINEAR_EQUATION__
#define __LINEAR_EQUATION__

#include <initializer_list>

#include "matrix.hpp"

class LinearEquation {
    private:
      Matrix<double> m_coef;
    public:
      LinearEquation() = default;
      LinearEquation(std::initializer_list<std::vector<double>> list) : m_coef(std::move(list)) {}

      Matrix<double>& coef() { return m_coef; }

      /**
       * @brief Solve a function with Gaussian method
       *
       * @return solution
       */
      Matrix<double> solve();
};

#endif
