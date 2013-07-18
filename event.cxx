//==========================================================================
//                                 Clone.cxx
//    Kyle Strand - kstrand12@winona.edu
//    27 June 2013
//
//    This file will read a ProMC file and clone it to another file
//
//==========================================================================

#include "FortPro/src/promcfortran.h"

int main(int argc, char **argv) {

	//Help input
	for (int i = 1; i < argc; i++) {
		const string arg = argv[i];
		if (arg == "--help" || arg == "-h") {
			cout << argv[0] << ": Reads a ProMC file and clones it to another ProMC file" << endl;
			cout << "Command structure: " << argv[0] << "<input ProMC file> <output ProMC file>" << endl;
			cout << "<output ProMC file> will be an identical clone." << endl;
			exit(0);
		}
	}

	// Select input and output files and divisions from command line
	string infile("-"), outfile("-");
	//string div;
	if (argc == 3) {
		infile = argv[1];
		outfile = argv[2];
	//	div = atoi(argv[3].c_str());
	} else if (argc !=3) {
		cerr << "Command structure: " << argv[0] << " <input ProMC file> <output ProMC file" << endl;
		exit(1);
	}


	// Initializing new output file
	ProMCBook *epbook1 = new ProMCBook(outfile.c_str(),"w");

	// Reading ProMC file
	
	ProMCBook *epbook = new ProMCBook(infile.c_str(),"r");
	// get ProMC version
	int h = epbook->getVersion();
	cout << "ProMC Version = " << h << endl;
   // get description of file
   string d = epbook->getDescription();
	cout << "Description " << d << endl;
	unsigned int nev = epbook->getEvents();
	cout << "Number of events: " << nev << endl;
   epbook1->setDescription(nev, d);


	// get the header information
	ProMCHeader header = epbook->getHeader();
	cout << "Cross section: " << header.cross_section() << endl;
	double kEV = (double)(header.momentumunit());
	double kLe = (double)(header.lengthunit());
	cout << "Energy units: " << kEV << endl;
	cout << "Length units: " << kLe << endl;

	// set header in new file
	epbook1->setHeader(header);

	ProMCEvent test = epbook->event(6349);
	epbook1->write(test);

        ProMCStat stat = epbook->getStatistics();
        epbook1->setStatistics(stat);
        
	epbook->close();
	epbook1->close();

	return 0;

}
