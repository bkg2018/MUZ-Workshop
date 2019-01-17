#  Structuration de GenerateListing

## Processus `AssembleFile(string file, ErrorList& msg)`

Appelle `GenerateFileListing(0, msg)`

--> `GenerateFileListing(int file, ErrorList& msg)`
	Ecrit nom du fichier file
	Pour chaque CodeLine :
		Appelle` GenerateCodeLineListing(codeline, msg)` (écrit listing de la ligne)
			 si (codeline.assembled) 
				 Ecrit les lignes reçues de `buildListingLines(codeline, m_status.allcodelisting)`
				 	Remplit un vecteur avec chaque ligne, créées par des appels de `buildOneListingLine()`
			sinon
				Ecrit une ligne reçue de `buildOneListingLine()`
		Ecrit si besoin les warnings/errors
		si (codeline.includefile > codeline.file)
			Appelle `GenerateFileListing(codeline.includefile, msg)` (appel récursif)
			Ecrit nom du fichier file (indique le retour)
		
## Transposition pour `GetListing(ErrorList& msg)`

Appelle `GenerateFileListing(0, msg, listinglines)`

--> `GenerateFileListing(int file, ErrorList& msg, std::vector<ListingLine> & listing)`
    Enregistre nom du fichier file
    Pour chaque CodeLine :
        Appelle` GenerateCodeLineListing(codeline, msg, listing)` (enregistre listing de la ligne)
            si (codeline.assembled)
            	enregistre les lignes reçues de `buildListingLineStructures(codeline, m_status.allcodelisting)`
            		Remplit un vecteur avec chaque ligne, créées par des appels de `buildOneListingLineStructure()`
	    sinon
	    	enreigistre une ligne reçue de `buildOneListingLineStructure()`
	enregistre si besoin les warnings/errors
	si (codeline.includefile > codeline.file)
		Appelle `GenerateFileListing(codeline.includefile, msg, listing)` (appel récursif)
		enregistre une ligne avec nom du fichier file (indique le retour)
	
