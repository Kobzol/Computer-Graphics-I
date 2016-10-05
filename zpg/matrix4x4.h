#ifndef MATRIX_H_
#define MATRIX_H_

/*! \class Matrix4x4
\brief Reálná matice 4x4 uspoøádaná po øádcích.

\author Tomáš Fabián
\version 0.9
\date 2014
*/
class Matrix4x4
{
public:
	//! Výchozí konstruktor.
	/*!
	Inicializace na matici identity.
	*/
	Matrix4x4();

	//! Výchozí konstruktor.
	/*!
	Inicializace matice zadanými hodnotami. První index oznaèuje øádek, druhý index patøí sloupci.
	*/
	Matrix4x4( const float m00, const float m01, const float m02, const float m03, 
		const float m10, const float m11, const float m12, const float m13, 
		const float m20, const float m21, const float m22, const float m23,
		const float m30, const float m31, const float m32, const float m33 );

	//! Transpozice matice.
	/*!
	Provede traspozici matice vzájemnou výmìnou øádkù a sloupcù.
	*/
	void Transpose();

	//! Euklidovská inverze matice.
	/*!
	Provede inverzi matice. Výsledek je správný jen za pøedpokladu, že se jedná o matici
	pøedstavující Euklidovské transformace zahrnující translaci,
	rotaci a reflexi. Taková transformace mìní pouze orientaci a pozici objektù, úhly a
	délky zùstávají zachovány.
	*/
	void EuclideanInverse();

	//! Nastaví zadaný prvek matice na novou hodnotu.
	/*!
	\param row øádek matice.
	\param column sloupec matice.
	\param value nová hodnota prvku matice.
	*/
	void set( const int row, const int column, const float value );

	//! Ukazatel na prvky matice.
	/*!
	\return Ukazatel na prvky matice.
	*/
	float * data();

	friend Matrix4x4 operator*( const Matrix4x4 & a, const Matrix4x4 & b );
	friend Vector3 operator*( const Matrix4x4 & a, const Vector3 & b );
	friend Vector3 operator*( const Matrix4x4 & a, const Vector4 & b );

	void Print() const;

private:
	union
	{
		/* Row-major
		m00 m01 m02 m03
		m10 m11 m12 m13
		m20 m21 m22 m23
		m30 m31 m32 m33
		*/

		struct
		{
			float m00_;
			float m01_;
			float m02_;
			float m03_;

			float m10_;
			float m11_;
			float m12_;
			float m13_;

			float m20_;
			float m21_;
			float m22_;
			float m23_;

			float m30_;
			float m31_;
			float m32_;
			float m33_;
		};

		float data_[4 * 4];
	};
};

typedef Matrix4x4 Matrix4;

#endif
