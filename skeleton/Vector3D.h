#pragma once
#include <cmath>

class Vector3D
{
protected:
	float _x;
	float _y;
	float _z;

public:
	// Constructora
	Vector3D(float x = 0, float y = 0, float z = 0) { _x = x; _y = y; _z = z; }

	// Getter
	inline float x() const { return _x; };
	inline float y() const{ return _y; };
	inline float z() const { return _z; };

	// Normalizar
	Vector3D Normalizado();

	// Modulo
	float Modulo();

	// Producto escalar
	float ProductoEscalar(const Vector3D& vect);

	// Producto vectorial
	Vector3D ProductoVectorial(const Vector3D& vect);

	// Sobrecarga de operadores
	// SUMA
	Vector3D operator +(const Vector3D& vect)
	{
		return Vector3D(this->_x + vect._x, this->_y + vect._y,this->_z + vect._z);
	}
	// RESTA
	Vector3D operator -(const Vector3D& vect)
	{
		return Vector3D(this->_x - vect._x, this->_y - vect._y, this->_z - vect._z);
	}
	//  ASIGNACION
	Vector3D operator =(const Vector3D& vect)
	{
		this->_x = vect._x;
		this->_y = vect._y;
		this->_z = vect._z;

		return *this;
	}
	// MULTIPLICACION
	Vector3D operator *(const int& num)
	{
		return Vector3D(this->_x*num, this->_y*num, this->_z*num);
	}
};

