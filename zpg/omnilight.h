#ifndef OMNI_LIGHT_H_
#define OMNI_LIGHT_H_

/*! \def NO_OMNI_LIGHTS
\brief Po�et v�esm�rov�ch sv�tel.
*/
#define NO_OMNI_LIGHTS 1

/*! \struct OmniLight
\brief V�esm�rov� zdroj sv�tla.

\author Tom� Fabi�n
\version 1.0
\date 2011-2013
*/
struct OmniLight
{
public:
	Vector3 position; /*!< Poloha zdroje [ws]. */

	Vector3 ambient; /*!< RGB barva prost�ed� \f$\left<0, 1\right>^3\f$. */
	Vector3 diffuse; /*!< RGB barva rozptylu \f$\left<0, 1\right>^3\f$. */
	Vector3 specular; /*!< RGB barva odrazu \f$\left<0, 1\right>^3\f$. */

	float occlusion; /*!< M�ra zakrat� zdroje (0 - p��m� viditelnost zdroje, 1 - zdroj zcela zakryt).  */
	
	//float attenuation_; /*!< �tlum. */

	//! V�choz� konstruktor.
	/*!
	Inicializuje v�echny slo�ky zdroje na hodnotu nula.
	*/
	OmniLight() { }

	OmniLight(const Vector3& position): position(position)
	{

	}

	//! Obecn� konstruktor.
	/*!
	Inicializuje zdroj podle zadan�ch hodnot parametr�.

	\param position poloha zdroje [ws].
	\param ambient barva prost�ed�.	
	\param diffuse barva rozptylu.
	\param specular barva odrazu.
	*/
	OmniLight( const Vector3 & position, const Vector3 & ambient,
		const Vector3 & diffuse, const Vector3 & specular );

	//void Print();
};

#endif
