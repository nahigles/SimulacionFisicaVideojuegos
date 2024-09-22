#include "Vector3D.h"

Vector3D Vector3D::Normalizado()
{
	const float modulo = this->Modulo();
	return Vector3D(this->_x/modulo, this->_y/modulo, this->_z/modulo);
}

float Vector3D::Modulo()
{
	return sqrt(pow(this->_x, 2)+ pow(this->_y, 2)+ pow(this->_z, 2));
}

float Vector3D::ProductoEscalar(const Vector3D& vect)
{
	return this->_x * vect._x + this->_y * vect._y + this->_z * vect._z;
}

Vector3D Vector3D::ProductoVectorial(const Vector3D& vect)
{
	const float x = this->_y * vect._z - vect._y * this->_z;
	const float y = -(this->_x * vect._z - vect._x * this->_z);
	const float z = this->_x * vect._y - vect._x * this->_y;
	return Vector3D(x, y, z);
}
