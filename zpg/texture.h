#ifndef TEXTURE_H_
#define TEXTURE_H_

/*! \class Texture
\brief T��da popisuj�c� texturu.

\author Tom� Fabi�n
\version 0.9
\date 2012
*/
class Texture
{
public:
	//! V�choz� konstruktor.
	/*!
	Inicializuje v�echny slo�ky na hodnotu nula.
	*/
	Texture();

	//! Specializovan� konstruktor.
	/*!
	Inicializuje texturu ze zadan�ho obr�zku.

	\param image obr�zek na�ten� pomoc� OpenCV.
	*/
	Texture( cv::Mat & image );

	//! Destruktor.
	/*!
	Uvoln� v�echny alokovan� zdroje.
	*/
	~Texture();		

	//! Vr�t� ���ku textury v pixelech.
	/*!	
	\return ���ku textury v pixelech.
	*/
	int width() const;

		//! Vr�t� v��ku textury v pixelech.
	/*!	
	\return V��ka textury v pixelech.
	*/
	int height() const;

	//! Vr�t� ukazatel na pole pixel� form�tu 8UC4.
	/*!	
	D�lka ��dku je \a width pixel� a po�et ��dku je roven \a height pixel�.

	\return V��ka textury v pixelech.
	*/
	unsigned char * get_data() const;

	//! Vr�t� texel o relativn�ch sou�adnic�ch \a u a \a v, kde \f$(u,v)\in\left<0,1\right>^2\f$.
	/*!	
	Hodnota barvy texelu je vypo�tena bilin�rn� interpolac�.

	\return Barva texelu.
	*/
	Color4 get_texel( const float u, const float v );

protected:

private:
	int pixel_size_; //*!< Velikost jednoho pixelu v bytech, mus� se shodovat s Color4. */
	int row_size_; //*!< D�lka jednoho ��dku obrazu v bytech. */
	int width_; //*!< ���ka obrazu v pixelech. */
	int height_; //*!< V��ka obrazu v pixelech. */

	unsigned char * data_; //*!< Pole pixel� form�tu 8UC4. */
};

/*! \fn Texture * LoadTexture( const char * file_name )
\brief Na�te texturu z obrazov�ho souboru \a file_name.
\param file_name �pln� cesta k obrazov�mu souboru v�etn� p��pony.
\param flip 0 vertik�ln� nebo 1 horizont�ln� flip obrazu
\param single_channel vynut� na�ten� jednokan�lov� obrazu.
*/
Texture * LoadTexture( const char * file_name, const int flip = -1, const bool single_channel = false );

#endif
