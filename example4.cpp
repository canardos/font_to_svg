// added by github user canardos

#include "font_to_svg.hpp"
#include <string>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <sys/stat.h>

bool dirExists(char * dirPath) {
    struct stat st;
    return ((stat(dirPath, &st) == 0) && (((st.st_mode) & S_IFMT) == S_IFDIR));
}

void writeGlyph(char* ttf, std::string glyphId, char* dirPath) {

	font2svg::glyph g( ttf, glyphId);

    std::string destPath = dirPath
        ? std::filesystem::path(dirPath).append("out_" + glyphId + ".svg").string()
        : ("out_" + glyphId + ".svg");

    std::ofstream outfile;
    outfile.open(destPath);
    outfile << g.svgheader(glyphId) << g.outline() << g.svgfooter(false);
	g.free();
}

int main( int argc, char * argv[] )
{
	if (argc!=3 && argc!=4) {
		std::cerr << std::endl <<
                     "Processes multiple comma separated code points." << std::endl <<
                     "Writes minimal SVG with outline only, no transform, and id=glyph_id." << std::endl <<
                     "viewBox needs to be set manually." << std::endl <<
                     std::endl <<
                     "Output filename is out_[glyph id].svg (e.g. out_0x0042.svg)" << std::endl <<
                     std::endl <<
		             "Usage: " << argv[0] << " font.ttf comma-separated-hex-code-points [output_directory]" << std::endl <<
                     "     : e.g." << std::endl <<
		             "     : " << argv[0] << " my-roman.ttf 0x0042,0x0045,0x0022 ./output_dir" << std::endl << std::endl;
		exit(1);
	}
    if (argc==4 && !dirExists(argv[3])) {
        std::cerr << std::endl << "Output directory (" << argv[3] << ") does not exist." << std::endl << std::endl;
        exit (1);
    }

    std::stringstream ss(argv[2]);
    while (ss.good()) {
        std::string glyphId;
        getline(ss, glyphId, ',');
        writeGlyph(argv[1], glyphId, argc==4 ? argv[3] : NULL);
    }

  return 0;
}
