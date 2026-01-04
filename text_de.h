#ifndef _text_de_h_
#define _text_de_h_
#include "text.h"

class text_DE_type : public text_basic
{
public:
	text_DE_type () : text_basic("de", "deutsch") {}

	std::string help () const override { return std::string() +
		program_name + " - Wandelt STL-Dateien in OpenScad-Dateien um\n"
		"\n"
		"Aufruf:  " + program_name + " [Optionen] <Eingangsdatei> [Ausgabedatei]\n"
		"\n"
		"    <Eingangsdatei> und/oder <Ausgabedatei> kann \"-\" gesetzt werden\n"
		"    und entspricht stdin/stdout.\n"
		"\n"
		"    -?, --help               Hilfetext anzeigen lassen\n"
		"    -f, --force              Ausgabedatei überschreiben, wenn sie existiert\n"
		"        --language <Kürzel>  Stellt die angezeigte Sprache ein, entsprechend\n"
		"                             der Locale-Bezeichnung\n"
		"        --language-list      Auflisten aller Sprachen\n"
	;}
	std::string help_short () const override { return std::string() +
		program_name + " - Wandelt STL-Dateien in OpenScad-Dateien um\n"
		"\n"
		"Aufruf:  " + program_name + " [Optionen] <Eingangsdatei> [Ausgabedatei]\n"
		"\n"
		"versuche dies:\n"
		"    --help oder -?       Hilfetext anzeigen lassen\n"
	;}
	std::string error () const override { return
		"Ein Fehler ist aufgetreten.\n"
	;}
	std::string error_other (std::string text) const override { return std::string() +
		"Ein Fehler ist aufgetreten: " + text + "\n"
	;}
	std::string unknown_argument (std::string argument) const override { return std::string() +
		"Unbekannte Option: " + argument + "\n"
		"Hilfetext anzeigen lassen mit:\n"
		+ program_name + " --help\n"
	;}
	std::string file_input_not_found (std::string filename) const override { return std::string() +
		"Kann die Eingabedatei \"" + filename + "\" nicht finden.\n"
	;}
	std::string file_output_found (std::string filename) const override { return std::string() +
		"Die Ausgabegabedatei \"" + filename + "\" existiert bereits.\n"
		"Probiere die Option -f um die Datei zu überschreiben.\n"
	;}
	std::string file_output_not_open (std::string filename) const override { return std::string() +
		"Kann die Ausgabedatei \"" + filename + "\" nicht öffnen.\n"
	;}
	std::string input_type_unknown (std::string filename) const override { return std::string() +
		"Die Eingabedatei \"" + filename + "\" ist keine STL-Datei.\n"
	;}
	std::string input_type_undefined (std::string filename) const override { return std::string() +
		"Kann den Inhalt der Eingabedatei \"" + filename + "\" (noch) nicht verarbeiten.\n"
	;}
	std::string input_empty (std::string filename) const override { return std::string() +
		"Die Eingabedatei \"" + filename + "\" enthält kein 3D Modell.\n"
	;}
} text_DE;

#endif // _text_de_h_
