/*
 * Matrix.hpp
 *
 *  Created on: 9 Feb 2017
 *      Author: Thomas
 */

#ifndef MATRIX_HPP_
#define MATRIX_HPP_

#include <array>
#include <cstdint>
#include <initializer_list>
#include <iostream>
#include <vector>

template<std::size_t W, std::size_t H, class T = int>
class Matrix
{
public:
	static_assert(std::is_arithmetic<T>::value, "Value T must be integral T or a floating-point T.");

	Matrix(T defaultValue = 0);

	Matrix(const std::initializer_list<T>& list);

	Matrix(const std::initializer_list<std::initializer_list<T>>& list);

	Matrix(const std::array<std::array<T, W>, H>& array);

	Matrix(const Matrix<W, H, T>& other);

	std::array<T, W> at(std::size_t height) const;

	T at(std::size_t height, std::size_t width) const;

	std::array<T, W>& operator[](const std::size_t idx);

	const Matrix<W, H, T>& operator=(const Matrix<W, H, T>& other);

	/*
	 * Can't assign a matrix of a different size, this returned.
	 */
	template<std::size_t aWidth, std::size_t aHeight, class aT>
	const Matrix operator=(const Matrix<aWidth, aHeight, aT>& other);

	/*
	 * Compares 2 matricies. Always returns false by comparing matricies with
	 * different sizes.
	 */
	template<std::size_t aWidth, std::size_t aHeight, class aT>
	bool operator==(const Matrix<aWidth, aHeight, aT>& other);

	/*
	 *Returns the amount of elements in the matrix.
	 */
	inline std::size_t size() const
	{
		return W * H;
	}

	inline std::size_t getWidth() const
	{
		return W;
	}

	inline std::size_t getHeight() const
	{
		return H;
	}

	/************************************************************
	 * Matrix functions.
	 ************************************************************/

	Matrix<H, W, T> transpose() const;

	Matrix<H, W, T> Invert() const;

	Matrix<W, H, T> Swap(unsigned long row1, unsigned long row2) const;

	Matrix<W, H, T> Add(unsigned long from, unsigned long to, double multiplier) const;

	bool appr(const Matrix<W,H,T>& a, const Matrix<W,H,T>& b, double precision);

	Matrix<W, H, T> Identity() const;

	std::size_t getHighestRow(const unsigned short i);

	Matrix<H, W*2, T> hConcat(const Matrix<H, W*2, T>& other) const;

	template<class aT = T>
	Matrix<W, H, T> pointWiseMultiply(const T value);

	std::string to_string() const;

	/************************************************************
	 * Scalar operations.
	 ************************************************************/

	Matrix<W, H, T>& operator*=(const T& scalar);

	Matrix<W, H, T> operator*(const T& scalar) const;

	template<class aT = T>
	Matrix<W, H, T>& operator/=(const aT& scalar);

	template<class aT = T>
	Matrix<W, H, T> operator/(const aT& scalar) const;

	/************************************************************
	 * Pointwise operations.
	 ************************************************************/

	Matrix<W, H, T>& operator+=(const T& value);

	Matrix<W, H, T> operator+(const T& value);

	template<class aT = T>
	Matrix<W, H, T>& operator-=(const aT& value);

	template<class aT = T>
	Matrix<W, H, T> operator-(const aT& value) const;

	/************************************************************
	 * Matrix operations.
	 ************************************************************/
	template<std::size_t aWidth, std::size_t aHeight>
	Matrix<aWidth, H, T>
	operator*(const Matrix<aWidth, aHeight, T>& other) const;

	Matrix<W, H, T>&
	operator+=(const Matrix<W, H, T>& other);

	Matrix<W, H, T>
	operator+(const Matrix<W, H, T>& other);

	Matrix<W, H, T>&
	operator-=(const Matrix<W, H, T>& other);

	Matrix<W, H, T> operator-(Matrix<W, H, T>& other);

	virtual ~Matrix();

private:
	std::array<std::array<T, W>, H> innerMatrix;
};

template<std::size_t W, std::size_t H, class T>
Matrix<W, H, T>::Matrix(T defaultValue)
{
	for (std::size_t i = 0; i < H; ++i)
	{
		innerMatrix.at(i).fill(defaultValue);
	}
}

template<std::size_t W, std::size_t H, class T>
Matrix<W, H, T>::Matrix(const std::initializer_list<T>& list)
{
	if (list.size() != W * H)
	{
		throw std::runtime_error("Error: Can't initialize Matrix, "
				"because initalizer list size does "
				"not match template size.");
	}

	auto it = list.begin();
	for (size_t i = 0; i < H; ++i)
	{
		for (size_t j = 0; j < W; ++j)
		{
			innerMatrix.at(i % W).at(j) = *it;
			++it;
			++i;
		}
	}
}

template<std::size_t W, std::size_t H, class T>
Matrix<W, H, T>::Matrix(const std::initializer_list<std::initializer_list<T>>& list)
{
	if (list.size() != H)
	{
		throw std::runtime_error("Error: Can't initialize Matrix, "
				"because initalizer list size does "
				"not match template size.");
	}

	auto it = list.begin();
	for (size_t i = 0; i < list.size(); ++i)
	{
		if (it->size() != W)
		{
			throw std::runtime_error("Error: Can't initialize Matrix, "
					"because initalizer list size does "
					"not match template size.");
		}

		auto itt = (*it).begin();
		for (size_t j = 0; j < (*it).size(); ++j)
		{
			innerMatrix.at(i).at(j) = *itt;
			++itt;
		}
		++it;
	}
}

template<std::size_t W, std::size_t H, class T>
Matrix<W, H, T>::Matrix(const std::array<std::array<T, W>, H>& array) :
		innerMatrix(array)
{

}

template<std::size_t W, std::size_t H, class T>
Matrix<W, H, T>::Matrix(const Matrix<W, H, T>& other) :
		innerMatrix(other.innerMatrix)
{
	//        std::cout << __PRETTY_FUNCTION__ << std::endl;
}

template<std::size_t W, std::size_t H, class T>
std::array<T, W> Matrix<W, H, T>::at(std::size_t height) const
{
	return innerMatrix.at(height);
}

template<std::size_t W, std::size_t H, class T>
T Matrix<W, H, T>::at(std::size_t height, std::size_t width) const
{
	return innerMatrix.at(height).at(width);
}

template<std::size_t W, std::size_t H, class T>
std::array<T, W>& Matrix<W, H, T>::operator[](const std::size_t idx)
{
	return innerMatrix.at(idx);
}

template<std::size_t W, std::size_t H, class T>
const Matrix<W, H, T>& Matrix<W, H, T>::operator=(const Matrix<W, H, T>& other)
{
	std::cout << __PRETTY_FUNCTION__ << std::endl;

	innerMatrix = other.innerMatrix;

	return *this;
}

template<std::size_t W, std::size_t H, class T>
template<std::size_t aWidth, std::size_t aHeight, class aT>
const Matrix<W, H, T> Matrix<W, H, T>::operator=(const Matrix<aWidth, aHeight,
		aT>& other)
{
	std::cout << __PRETTY_FUNCTION__ << std::endl;

	return *this;
}

template<std::size_t W, std::size_t H, class T>
template<std::size_t aWidth, std::size_t aHeight, class aT>
bool Matrix<W, H, T>::operator==(const Matrix<aWidth, aHeight, aT>& other)
{
	std::cout << __PRETTY_FUNCTION__ << std::endl;
	if (&other == this)
	{
		return true;
	}

	return W == aWidth && H == aHeight && innerMatrix == other.innerMatrix;
}

template<std::size_t W, std::size_t H, class T>
std::ostream& operator<<(std::ostream& stream, const Matrix<W, H, T>& other)
{
	return stream << other.to_string();
}

template<std::size_t W, std::size_t H, class T>
Matrix<H, W, T> Matrix<W, H, T>::transpose() const
{
	Matrix<H, W, T> temp = Matrix<H, W, T>();
	for (std::size_t i = 0; i < H; ++i)
	{
		for (std::size_t j = 0; j < W; ++j)
		{
			temp[j][i] = innerMatrix[i][j];
		}
	}
	return temp;
}

template<std::size_t W, std::size_t H, class T>
template<class aT>
Matrix<W, H, T> Matrix<W, H, T>::pointWiseMultiply(const T value)
{
	for (std::size_t i = 0; i < H; ++i)
	{
		for (std::size_t j = 0; j < W; ++j)
		{
			innerMatrix[j][i] = value * innerMatrix[i][j];
		}
	}
	return *this;
}

template<std::size_t W, std::size_t H, class T>
std::string Matrix<W, H, T>::to_string() const
{
	std::string temp = "[Matrix]<" + std::to_string(W) + "," + std::to_string(H) + ">\n{\n";
	for (std::size_t i = 0; i < H; ++i)
	{
		for (std::size_t j = 0; j < W; ++j)
		{
			temp += std::to_string(innerMatrix[i][j]) + ",";
		}
		temp += "\n";
	}
	temp += "}";
	return temp;
}


template<std::size_t W, std::size_t H, class T>
Matrix<W, H, T> Matrix<W, H, T>::Swap(unsigned long row1, unsigned long row2) const
{
	Matrix<W, H, T> temp;
	temp = (*this);
	std::array<T, W> tempArray;

	tempArray = temp[row1];
	temp[row1] = temp[row2];
	temp[row2] = tempArray;

	return temp;
}
template<std::size_t W, std::size_t H, class T>
Matrix<W, H, T> Matrix<W, H, T>::Add(unsigned long from, unsigned long to, double multiplier) const
{
	std::cout << "Add() from:" << from << " to: " << to << " multiplier: " << multiplier << std::endl;
	Matrix<W, H, T> temp;
	temp = (*this);

	for (std::size_t j = 0; j < W; ++j)
	{
		temp[to][j] = (temp[from][j] * multiplier) + temp[to][j]; // temp[to][j] + (innerMatrix[from][j] * -multiplier);
	}

	return temp;
}

template<std::size_t W, std::size_t H, class T>
bool Matrix<W, H, T>::appr(const Matrix<W,H,T>& a, const Matrix<W,H,T>& b, double precision)
{
	double diffX = a.at(0, 0) - b.at(0, 0);
	double diffY = a.at(1, 0) - b.at(1, 0);
	if (diffX < 0)
		diffX *= -1;
	if (diffY < 0)
		diffY *= -1;
	return (diffX < precision && diffY < precision);
}
template<std::size_t W, std::size_t H, class T>
Matrix<W, H, T> Matrix<W, H, T>::Identity() const
{
	Matrix<W, H, T> temp;
	for (std::size_t i = 0; i < H; ++i)
	{
		for (std::size_t j = 0; j < W; ++j)
		{
			temp[i][j] = (i == j);
		}
	}
	return temp;
}

template<std::size_t W, std::size_t H, class T>
std::size_t Matrix<W, H, T>::getHighestRow(const unsigned short i)
{
	std::size_t row = 0;
	double max = 0;
	for (std::size_t j = i; j < H; ++j)
	{
		double tempValue = (*this)[j][i];
		if (tempValue < 0)
		{
			tempValue = -tempValue;
		}
		if (tempValue >= max)
		{
			max = tempValue;
			row = j;
		}
		if ((*this)[j][i] == 1 || (*this)[j][i] == -1)
		{
			row = j;
			max = 1;
			break;
		}
	}
	return row;
}

template<std::size_t W, std::size_t H, class T>
Matrix<H, W*2, T> Matrix<W, H, T>::hConcat(const Matrix<H, W*2, T>& other) const
{
	Matrix<H, W * 2, T> temp;
	for (std::size_t i = 0; i < H; ++i)
	{
		for (std::size_t j = 0; j < W; ++j)
		{
			temp[i][j] = innerMatrix[i][j];
		}
		for (std::size_t j = 0; j < W; ++j)
		{
			temp[i][j + W] = other.at(i, j);
		}
	}
	return temp;
}
template<std::size_t W, std::size_t H, class T>
Matrix<H, W, T> Matrix<W, H, T>::Invert() const
{
	const std::size_t size = H;
	Matrix<size, size, double> inputMatrix;
	bool vierkant = true;
	if (H != W)
	{
		vierkant = false;
		inputMatrix = ((*this) * (*this).transpose());
	}
	else
	{
		inputMatrix = (*this);
	}
	Matrix<size, size, T> iI = inputMatrix.Identity();

	//(5)
	for (std::size_t i = 0; i < size; ++i)
	{
		std::cout << std::endl << "i: " << i << std::endl;
		//(1) vind de correcte bovenste rij.
		//std::cout << "(1) vind beste row" << std::endl;
		std::size_t row = inputMatrix.getHighestRow(i);

		//(2) swap (1) met de bovenste rij
		//std::cout << "(2) swap" << std::endl;
		inputMatrix = inputMatrix.Swap(i, row);
		iI = iI.Swap(i, row);

		//(3) bovenste rij * 1 / t[?]
		double multiplier = inputMatrix[i][i];
		//std::cout << "(3 rij " << i << " * 1 / " << multiplier << std::endl;
		for (std::size_t j = 0; j < size; ++j)
		{
			inputMatrix[i][j] = inputMatrix[i][j] * (1 / multiplier);
			iI[i][j] = iI[i][j] * (1 / multiplier);
		}

		//(4) maak overige eerste element 0
		//std::cout << "(4) maak overige elementen 0" << std::endl;

		for (std::size_t j = 0; j < size; ++j)
		{
			if (j != i)
			{
				double multiplier = -inputMatrix[j][i];
				inputMatrix = inputMatrix.Add(i, j, multiplier);
				iI = iI.Add(i, j, multiplier);
			}
		}
	}

	Matrix<H, W, T> outputMatrix;
	if (!vierkant)
	{
		outputMatrix = ((*this).transpose() * iI);
	}
	else
	{
		outputMatrix = iI;
	}
	std::cout << "outputMatrix: " << outputMatrix << std::endl;
	std::cout << "inputMatrix: " << inputMatrix << std::endl;

	return outputMatrix;
}







template<std::size_t W, std::size_t H, class T>
Matrix<W, H, T>& Matrix<W, H, T>::operator*=(const T& scalar)
{
	for (std::size_t i = 0; i < H; ++i)
	{
		for (std::size_t j = 0; j < W; ++j)
		{
			innerMatrix.at(i).at(j) *= scalar;
		}
	}
	return *this;
}

template<std::size_t W, std::size_t H, class T>
Matrix<W, H, T> Matrix<W, H, T>::operator*(const T& scalar) const
{
	Matrix<W, H, T> temp(*this);
	return temp *= scalar;
}

template<std::size_t W, std::size_t H, class T>
template<class aT>
Matrix<W, H, T>& Matrix<W, H, T>::operator/=(const aT& scalar)
{
	for (std::size_t i = 0; i < H; ++i)
	{
		for (std::size_t j = 0; j < W; ++j)
		{
			innerMatrix[i][j] /= scalar;
		}
	}
	return *this;
}

template<std::size_t W, std::size_t H, class T>
template<class aT>
Matrix<W, H, T> Matrix<W, H, T>::operator/(const aT& scalar) const
{
	Matrix<W, H, T> temp = *this;
	return temp /= scalar;
}

template<std::size_t W, std::size_t H, class T>
Matrix<W, H, T>& Matrix<W, H, T>::operator+=(const T& value)
{
	for (std::size_t i = 0; i < H; ++i)
	{
		for (std::size_t j = 0; j < W; ++j)
		{
			innerMatrix[i][j] += value;
		}
	}
	return *this;
}

template<std::size_t W, std::size_t H, class T>
Matrix<W, H, T> Matrix<W, H, T>::operator+(const T& value)
{
	Matrix<W, H, T> temp = *this;
	return temp += value;
}

template<std::size_t W, std::size_t H, class T>
template<class aT>
Matrix<W, H, T>& Matrix<W, H, T>::operator-=(const aT& value)
{
	for (std::size_t i = 0; i < H; ++i)
	{
		for (std::size_t j = 0; j < W; ++j)
		{
			innerMatrix[i][j] -= value;
		}
	}
	return *this;
}

template<std::size_t W, std::size_t H, class T>
template<class aT>
Matrix<W, H, T> Matrix<W, H, T>::operator-(const aT& value) const
{
	Matrix<W, H, T> temp = *this;
	return temp -= value;
}

template<std::size_t W, std::size_t H, class T>
template<std::size_t aWidth, std::size_t aHeight>
Matrix<aWidth, H, T> Matrix<W, H, T>::operator*(const Matrix<aWidth, aHeight, T>& other) const
{
	if (W != aHeight)
	{
		throw std::runtime_error("Error: Can't multiply matrixes, because "
				"their size doesn't allow "
				"that.");
	}

	Matrix<aWidth, aHeight, T> temp = other;
	Matrix<aWidth, H, T> tempMatrix = Matrix<aWidth, H, T>();

	for (std::size_t i = 0; i < H; ++i)
	{
		for (std::size_t j = 0; j < aWidth; ++j)
		{
			for (std::size_t k = 0; k < W; ++k)
			{
				tempMatrix[i][j] += innerMatrix[i][k] * temp[k][j];
			}
		}
	}
	return tempMatrix;
}

template<std::size_t W, std::size_t H, class T>
Matrix<W, H, T>& Matrix<W, H, T>::operator+=(const Matrix<W, H, T>& other)
{
	for (std::size_t i = 0; i < H; ++i)
	{
		for (std::size_t j = 0; j < W; ++j)
		{
			innerMatrix[i][j] += other.at(i, j);
		}
	}
	return *this;
}

template<std::size_t W, std::size_t H, class T>
Matrix<W, H, T> Matrix<W, H, T>::operator+(const Matrix<W, H, T>& other)
{
	Matrix<W, H, T> temp(*this);
	return temp += other;
}

template<std::size_t W, std::size_t H, class T>
Matrix<W, H, T>& Matrix<W, H, T>::operator-=(const Matrix<W, H, T>& other)
{
	for (std::size_t i = 0; i < H; ++i)
	{
		for (std::size_t j = 0; j < W; ++j)
		{
			innerMatrix[i][j] -= other.at(i, j);
		}
	}
	return *this;
}

template<std::size_t W, std::size_t H, class T>
Matrix<W, H, T> Matrix<W, H, T>::operator-(Matrix<W, H, T>& other)
{
	Matrix<W, H, T> temp(*this);
	return temp -= other;
}

template<std::size_t W, std::size_t H, class T>
Matrix<W, H, T>::~Matrix()
{
}

#endif /* MATRIX_HPP_ */
