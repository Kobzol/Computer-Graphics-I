#ifndef MATERIAL_H_
#define MATERIAL_H_

/*! \def NO_TEXTURES
\brief Maximální poèet textur pøiøazených materiálu.
*/
#define NO_TEXTURES 4

/*! \def IOR_AIR
\brief Index lomu vzduchu za normálního tlaku.
*/
#define IOR_AIR static_cast<float>( 1.000293 )

/*! \def IOR_AIR
\brief Index lomu vody.
*/
#define IOR_WATER static_cast<float>( 1.33 )

/*! \class Material
\brief Difuzní materiál.

\author Tomáš Fabián
\version 0.9
\date 2011-2012
*/
class Material
{
public:	
	//! Implicitní konstruktor.
	/*!
	Inicializuje všechny složky materiálu na výchozí matnì šedý materiál.
	*/
	Material();

	//! Specializovaný konstruktor.
	/*!
	Inicializuje materiál podle zadaných hodnot parametrù.

	\param name název materiálu.
	\param ambient barva prostøedí.
	\param diffuse barva rozptylu.
	\param specular barva odrazu.
	\param emission  barva emise.
	\param shininess lesklost.
	\param ior index lomu.
	\param textures pole ukazatelù na textury.
	\param no_textures délka pole \a textures. Maximálnì \a NO_TEXTURES - 1.
	*/
	Material( std::string & name, const Vector3 & ambient, const Vector3 & diffuse,
		const Vector3 & specular, const Vector3 & emission, const float reflectivity,
		const float shininess, const float ior,
		Texture ** textures = NULL, const int no_textures = 0 );

	//! Destruktor.
	/*!
	Uvolní všechny alokované zdroje.
	*/
	~Material();

	//void Print();

	//! Nastaví název materiálu.
	/*!	
	\param name název materiálu.
	*/
	void set_name( const char * name );

	//! Vrátí název materiálu.
	/*!	
	\return Název materiálu.
	*/
	std::string get_name() const;

	//! Nastaví texturu.
	/*!	
	\param slot èíslo slotu, do kterého bude textura pøiøazena. Maximálnì \a NO_TEXTURES - 1.
	\param texture ukazatel na texturu.
	*/
	void set_texture( const int slot, Texture * texture );

	//! Vrátí texturu.
	/*!	
	\param slot èíslo slotu textury. Maximálnì \a NO_TEXTURES - 1.
	\return Ukazatel na zvolenou texturu.
	*/
	Texture * get_texture( const int slot ) const;

public:
	Vector3 ambient; /*!< RGB barva prostøedí \f$\left<0, 1\right>^3\f$. */
	Vector3 diffuse; /*!< RGB barva rozptylu \f$\left<0, 1\right>^3\f$. */
	Vector3 specular; /*!< RGB barva odrazu \f$\left<0, 1\right>^3\f$. */

	Vector3 emission; /*!< RGB barva emise \f$\left<0, 1\right>^3\f$. */

	float shininess; /*!< Koeficient lesklosti (\f$\ge 0\f$). Èím je hodnota vìtší, tím se jeví povrch lesklejší. */

	float reflectivity; /*!< Koeficient odrazivosti. */
	float ior; /*!< Index lomu. */

	static const char kDiffuseMapSlot; /*!< Èíslo slotu difuzní textury. */
	static const char kSpecularMapSlot; /*!< Èíslo slotu spekulární textury. */
	static const char kNormalMapSlot; /*!< Èíslo slotu normálové textury. */
	static const char kOpacityMapSlot; /*!< Èíslo slotu transparentní textury. */

private:
	Texture * textures_[NO_TEXTURES]; /*!< Pole ukazatelù na RGBA 8UC4 textury. */
	/*
	slot 0 - diffuse map + alpha
	slot 1 - specular map + opaque alpha
	slot 2 - normal map
	slot 3 - transparency map
	*/
	
	std::string name_; /*!< Název plochy. */
};

#endif
