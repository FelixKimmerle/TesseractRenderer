#pragma once
#include <vector>
#include <ostream>
#include <algorithm>
#include <iostream>
#include "Vector.hpp"

template <typename T>
class Matrix;
template <typename T>
std::ostream &operator<<(std::ostream &out, const Matrix<T> &p_Matrix);

template <typename T>
class Matrix
{
private:
    std::vector<std::vector<T>> m_Data;
    unsigned int m_uiWidth;
    unsigned int m_uiHeight;

public:
    Matrix(unsigned int p_uiWidth, unsigned int p_uiHeight);
    Matrix(const std::vector<std::vector<T>> &p_Data);
    Matrix(const Vector<T> &vec);
    ~Matrix();
    std::vector<T> &operator[](unsigned int index);
    friend std::ostream &operator<<<>(std::ostream &out, const Matrix<T> &p_Matrix);
    Matrix<T> operator*(const Matrix<T> &p_Other) const; //Cross Product
    void operator*=(const Matrix<T> &p_Other);           //Cross Product
    unsigned int GetWidth() const;
    unsigned int GetHeight() const;
    Matrix<T> &operator=(const std::vector<std::vector<T>> &p_Data);
    Vector<T> AsVector();
};

template <typename T>
Matrix<T>::Matrix(unsigned int p_uiWidth, unsigned int p_uiHeight) : m_uiWidth(p_uiWidth), m_uiHeight(p_uiHeight)
{
    m_Data.resize(p_uiWidth, std::vector<T>(p_uiHeight, 0));
}

template <typename T>
Matrix<T>::Matrix(const std::vector<std::vector<T>> &p_Data) : m_Data(p_Data)
{
    m_uiWidth = m_Data.size();
    if (m_uiWidth != 0)
    {
        m_uiHeight = m_Data[0].size();
    }
    else
    {
        m_uiHeight = 0;
    }
}

template <typename T>
Matrix<T>::Matrix(const Vector<T> &vec)
{
    m_uiHeight = 1;
    m_uiWidth = vec.GetLength();
    m_Data.resize(m_uiWidth, std::vector<T>(m_uiHeight, 0));
    for (size_t i = 0; i < m_uiWidth; i++)
    {
        m_Data[i][0] = vec[i];
    }
}

template <typename T>
Matrix<T>::~Matrix()
{
}

template <typename T>
std::vector<T> &Matrix<T>::operator[](unsigned int index)
{
    return m_Data[index];
}
template <typename T>
unsigned int Matrix<T>::GetWidth() const
{
    return m_uiWidth;
}
template <typename T>
unsigned int Matrix<T>::GetHeight() const
{
    return m_uiHeight;
}

template <typename T>
Matrix<T> &Matrix<T>::operator=(const std::vector<std::vector<T>> &p_Data)
{
    m_Data = p_Data;
    m_uiWidth = m_Data.size();
    if (m_uiWidth != 0)
    {
        m_uiHeight = m_Data[0].size();
    }
    else
    {
        m_uiHeight = 0;
    }
    return *this;
}

template <typename T>
Matrix<T> Matrix<T>::operator*(const Matrix<T> &p_Other) const
{
    if (m_uiHeight == p_Other.m_uiWidth)
    {
        Matrix res(m_uiWidth, p_Other.m_uiHeight);
        for (unsigned int x = 0; x < m_uiWidth; x++)
        {
            std::vector<float> row(p_Other.m_uiHeight);
            for (unsigned int y = 0; y < p_Other.m_uiHeight; y++)
            {
                for (unsigned int k = 0; k < p_Other.m_uiWidth; k++)
                {
                    row[y] += m_Data[x][k] * p_Other.m_Data[k][y];
                }
                res.m_Data[x] = row;
            }
        }
        return res;
    }
    else
    {
        std::cerr << "Matrices are not compatible" << std::endl;
    }
    return Matrix(0, 0);
}

template <typename T>
void Matrix<T>::operator*=(const Matrix<T> &p_Other)
{
    if (m_uiHeight == p_Other.m_uiWidth)
    {
        std::vector<std::vector<float>> res(m_uiWidth, std::vector<float>(p_Other.m_uiHeight, 0));
        for (unsigned int x = 0; x < m_uiWidth; x++)
        {
            std::vector<float> row(p_Other.m_uiHeight);
            for (unsigned int y = 0; y < p_Other.m_uiHeight; y++)
            {
                for (unsigned int k = 0; k < p_Other.m_uiWidth; k++)
                {
                    row[y] += m_Data[x][k] * p_Other.m_Data[k][y];
                }
                res[x] = row;
            }
        }
        m_uiHeight = p_Other.m_uiHeight;
        m_Data = res;
    }
    else
    {
        std::cerr << "Matrices are not compatible" << std::endl;
    }
}

template <typename T>
Vector<T> Matrix<T>::AsVector()
{
    if (m_uiHeight == 1)
    {
        Vector<T> vec(m_uiWidth);
        for (size_t i = 0; i < m_uiWidth; i++)
        {
            vec[i] = m_Data[i][0];
        }
        return vec;
    }
    std::cerr << "Matrix is not a vector" << std::endl;
    return Vector<T>(0);
}

template <typename T>
std::ostream &operator<<(std::ostream &out, const Matrix<T> &p_Matrix)
{
    out << "[Matrix " << p_Matrix.m_uiWidth << "x" << p_Matrix.m_uiHeight << "]" << std::endl;
    for (unsigned int x = 0; x < p_Matrix.m_uiWidth; x++)
    {
        for (unsigned int y = 0; y < p_Matrix.m_uiHeight; y++)
        {
            out << p_Matrix.m_Data[x][y] << ", ";
        }
        out << std::endl;
    }
    return out;
}
