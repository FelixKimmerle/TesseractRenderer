#pragma once

#include <ostream>
#include <vector>

template <typename T>
class Vector;
template <typename T>
std::ostream &operator<<(std::ostream &out, const Vector<T> &p_Vector);

template <typename T>
class Vector
{
private:
    std::vector<T> m_Data;
    unsigned int m_uiLength;

public:
    Vector(unsigned int p_uiLength);
    Vector();
    Vector(const std::vector<T> &p_Data);
    ~Vector();
    T &operator[](unsigned int index);
    T operator[](unsigned int index) const;
    friend std::ostream &operator<<<>(std::ostream &out, const Vector<T> &p_Vector);
    unsigned int GetLength() const;
    void operator/=(const T &value);
    void Resize(unsigned int size);
    void Append(const T &value);
};

template <typename T>
Vector<T>::Vector(unsigned int p_uiLength) : m_uiLength(p_uiLength)
{
    m_Data.resize(m_uiLength, 0);
}

template <typename T>
Vector<T>::Vector() : m_uiLength(0)
{
    m_Data.resize(0);
}

template <typename T>
Vector<T>::Vector(const std::vector<T> &p_Data) : m_Data(p_Data)
{
    m_uiLength = m_Data.size();
}
template <typename T>
Vector<T>::~Vector()
{
}
template <typename T>
void Vector<T>::Resize(unsigned int size)
{
    m_uiLength = size;
    m_Data.resize(m_uiLength, 0);
}

template <typename T>
void Vector<T>::Append(const T &value)
{
    m_Data.push_back(value);
    m_uiLength++;
}

template <typename T>
T &Vector<T>::operator[](unsigned int index)
{
    return m_Data[index];
}

template <typename T>
void Vector<T>::operator/=(const T &value)
{
    T v = ((T)1) / value;
    for (size_t i = 0; i < m_uiLength; i++)
    {
        m_Data[i] = m_Data[i] * v;
    }
}

template <typename T>
T Vector<T>::operator[](unsigned int index) const
{
    return m_Data[index];
}

template <typename T>
unsigned int Vector<T>::GetLength() const
{
    return m_uiLength;
}

template <typename T>
std::ostream &operator<<(std::ostream &out, const Vector<T> &p_Vector)
{
    out << "[Vector " << p_Vector.m_uiLength << "]" << std::endl;
    for (unsigned int x = 0; x < p_Vector.m_uiLength; x++)
    {
        out << p_Vector.m_Data[x] << ", ";
        out << std::endl;
    }
    return out;
}