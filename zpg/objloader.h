#ifndef OBJ_LOADER_H_
#define OBJ_LOADER_H_

/*! \fn int LoadOBJ( const char * file_name, Vector3 & default_color, std::vector<Surface *> & surfaces, std::vector<Material *> & materials )
\brief Naète geometrii z OBJ souboru \a file_name.
\note Pøi exportu z 3ds max je nutné nastavit systémové jednotky na metry:
Customize -> Units Setup Metric (Meters)
System Unit Setup -> 1 Unit = 1,0 m a zaškrtnout Respect System Units in File
\see 
\param file_name úplná cesta k OBJ souboru vèetnì pøípony.
\param default_color výchozí barva vertexu.
\param surfaces pole ploch, do kterého se budou ukládat naètené plochy.
\param materials pole materiálù, do kterého se budou ukládat naètené materiály.
\param flip_yz rotace kolem osy x o + 90st.
*/
int LoadOBJ( const char * file_name, Vector3 & default_color,
	std::vector<Surface *> & surfaces, std::vector<Material *> & materials,
	const bool flip_yz = false );

#endif
