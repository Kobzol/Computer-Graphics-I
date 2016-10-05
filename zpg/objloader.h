#ifndef OBJ_LOADER_H_
#define OBJ_LOADER_H_

/*! \fn int LoadOBJ( const char * file_name, Vector3 & default_color, std::vector<Surface *> & surfaces, std::vector<Material *> & materials )
\brief Na�te geometrii z OBJ souboru \a file_name.
\note P�i exportu z 3ds max je nutn� nastavit syst�mov� jednotky na metry:
Customize -> Units Setup Metric (Meters)
System Unit Setup -> 1 Unit = 1,0 m a za�krtnout Respect System Units in File
\see 
\param file_name �pln� cesta k OBJ souboru v�etn� p��pony.
\param default_color v�choz� barva vertexu.
\param surfaces pole ploch, do kter�ho se budou ukl�dat na�ten� plochy.
\param materials pole materi�l�, do kter�ho se budou ukl�dat na�ten� materi�ly.
\param flip_yz rotace kolem osy x o + 90st.
*/
int LoadOBJ( const char * file_name, Vector3 & default_color,
	std::vector<Surface *> & surfaces, std::vector<Material *> & materials,
	const bool flip_yz = false );

#endif
