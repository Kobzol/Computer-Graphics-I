#ifndef OMNI_LIGHT_H_
#define OMNI_LIGHT_H_

/*! \def NO_OMNI_LIGHTS
\brief Poèet všesmìrových svìtel.
*/
#define NO_OMNI_LIGHTS 1

/*! \struct OmniLight
\brief Všesmìrový zdroj svìtla.

\author Tomáš Fabián
\version 1.0
\date 2011-2013
*/
struct OmniLight
{
public:
	Vector3 position; /*!< Poloha zdroje [ws]. */

	Vector3 ambient; /*!< RGB barva prostøedí \f$\left<0, 1\right>^3\f$. */
	Vector3 diffuse; /*!< RGB barva rozptylu \f$\left<0, 1\right>^3\f$. */
	Vector3 specular; /*!< RGB barva odrazu \f$\left<0, 1\right>^3\f$. */

	float occlusion; /*!< Míra zakratí zdroje (0 - pøímá viditelnost zdroje, 1 - zdroj zcela zakryt).  */
	
	//float attenuation_; /*!< Útlum. */

	//! Výchozí konstruktor.
	/*!
	Inicializuje všechny složky zdroje na hodnotu nula.
	*/
	OmniLight() { }

	OmniLight(const Vector3& position): position(position)
	{

	}

	//! Obecný konstruktor.
	/*!
	Inicializuje zdroj podle zadaných hodnot parametrù.

	\param position poloha zdroje [ws].
	\param ambient barva prostøedí.	
	\param diffuse barva rozptylu.
	\param specular barva odrazu.
	*/
	OmniLight( const Vector3 & position, const Vector3 & ambient,
		const Vector3 & diffuse, const Vector3 & specular );

	//void Print();
};

#endif
