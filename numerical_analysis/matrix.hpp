#ifndef __MATRIX__
#define __MATRIX__

#include <vector>
#include <ostream>
#include <initializer_list>

template <class T>
class Matrix {
  private:
    using RowT = std::vector<T>;
    using SelfT = Matrix<T>;
  private:
    std::vector<RowT> elem;
  private:
    bool operable(const SelfT &o) const {
        return row() == o.row() && col() == o.col();
    }

    template <class Callable>
    void forEachWith(const SelfT &other, Callable &&call) {
        auto row = this->row(), col = this->col();
        for(int i = 0; i < row; ++i) {
            for(int j = 0; j < col; ++j)
                call(elem[i][j], other[i][j]);
        }
    }

    template <class Callable>
    void forEachWith(T coef, Callable &&call) {
        auto row = this->row(), col = this->col();
        for(int i = 0; i < row; ++i) {
            for(int j = 0; j < col; ++j)
                call(elem[i][j], coef);
        }
    }
  public:
    Matrix() = default;
    Matrix(int dim) : elem(dim, RowT(dim)) {}
    Matrix(int row, int col) : elem(row, RowT(col)) {}
    Matrix(std::initializer_list<std::vector<T>> list) : elem(std::move(list)) {}

    Matrix(const SelfT&) = default;
    Matrix(SelfT&&) = default;

    void swap(SelfT &other) { std::swap(elem, other.elem); }

    int col() const { return elem[0].size(); }
    int row() const { return elem.size(); }

    SelfT transpose() const;

    T determinate() const { return determinate(0, 0); }

    bool operator==(const SelfT&) const;

    RowT &      operator[](std::size_t i) { return elem[i]; }
    const RowT &operator[](std::size_t i) const { return elem[i]; }

    T &at(std::size_t i, std::size_t j) { return elem[i][j]; }
    const T &at(std::size_t i, std::size_t j) const { return elem[i][j]; }

    SelfT& operator=(const SelfT&) = default;

#define DEPLOY_OPERATOR(op, operand)                         \
    SelfT &operator op##=(operand other) {                   \
        forEachWith(other, [](T a, T b) { return a op b; }); \
        return *this;                                        \
    }                                                        \
    SelfT operator op(operand other) const {                 \
        auto        copy = *this;                            \
        return copy op## = other;                            \
    }

    DEPLOY_OPERATOR(+, const SelfT&);
    DEPLOY_OPERATOR(-, const SelfT&);
    DEPLOY_OPERATOR(*, const SelfT&);
    DEPLOY_OPERATOR(*, T);
};

template <class T>
Matrix<T> Matrix<T>::transpose() const {
    auto row = this->row(), col = this->col();
    Matrix<T> res(col, row);
    for(int i = 0; i < row;++i) {
        for(int j = 0; j < col;++j)
            res[j][i] = elem[i][j];
    }
    return res;
}

template <class T>
bool Matrix<T>::operator==(const Matrix<T> &other) const {
    auto row = this->row(), col = this->col();
    if(row != other.row() || col != other.col())
        return false;
    for(int i = 0; i < row; ++i) {
        for(int j = 0; j < col; ++j)
            if(elem[i][j] != other[i][j])
                return false;
    }
    return true;
}

template <class T>
std::ostream& operator<<(std::ostream &os, const Matrix<T> &m) {
    os << "[\n";
    auto row = m.row(), col = m.col();
    for(int i = 0; i < row;++i) {
        os << "  [";
        auto &&line = m[i];
        for(int j = 0; j < col; ++j)
            os << line[j] << ", ";
        os << "],\n";
    }
    return os << ']';
}

#endif
