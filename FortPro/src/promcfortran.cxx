//----------------------------------------------------------------------
//                             ProMCFortran
//  Author:  Kyle Strand
//  Date:    28 June 2013
//
//  Cloning promc file.
//  This implements the RootTuple class.
//
//----------------------------------------------------------------------

#include "promcfortran.h"

//--------------------------------------------------------------
//                        Public methods
//--------------------------------------------------------------
string getEnvVar( std::string const & key ) {
  char * val = getenv( key.c_str() );
  return val == NULL ? std::string("") : std::string(val);
}	// for use with readPDG()

void readPDG( ProMCHeader * header  ) {

  string temp_string;
  istringstream curstring;


  string  PdgTableFilename  = getEnvVar("PROMC")+"/data/particle.tbl";
  if (PdgTableFilename.size()<2) {
  cout <<"**        ERROR: PROMC variable not set. Did you run source.sh" <<
         "      **" << endl;
    exit(1);
  }     

  ifstream fichier_a_lire(PdgTableFilename.c_str());
  if(!fichier_a_lire.good()) {
	cout << "**      	ERROR: PDG Table ("<< PdgTableFilename << ") not found!			**" << endl;
	exit(1);
	return;
  }      

  getline(fichier_a_lire,temp_string);
  getline(fichier_a_lire,temp_string);
  getline(fichier_a_lire,temp_string);
  while (getline(fichier_a_lire,temp_string)) {
	curstring.clear();
	curstring.str(temp_string);
	long int ID; std::string name; int charge; float mass; float width; float lifetime;

	curstring >> ID >> name >> charge >> mass >> width >> lifetime;

	ProMCHeader_ParticleData* pp= header->add_particledata();
	pp->set_id(ID);
	pp->set_mass(mass);
	pp->set_name(name);
	pp->set_width(width);
	pp->set_lifetime(lifetime);
	pp->set_charge(charge);
  }
 
}	// fuction to gather particle data
PromcFortran::PromcFortran()
{
}//Constructor
PromcFortran::~PromcFortran()
{
}//Destructor

void PromcFortran::Initialise()
{
	const double kEV=1000*100;
	const double uL=1000.;
	
       //Define promc and create headers

       epbook = new ProMCBook("Pythia6.promc","w");
       cout << "Test." << endl;
       epbook->setDescription(1000, "PYTHIA6 Monte Carlo Simulation Data");
     
       ProMCHeader header;
	// define length and momentum units
	// momentum in 0.01*MeV (px, py, pz)
	// length in cm*1000
       header.set_momentumunit((int)kEV);
       header.set_lengthunit((int)uL); 
       header.set_id1(0);
       header.set_id2(0);
       header.set_pdf1(0);
       header.set_pdf2(0);
       header.set_x1(0);
       header.set_x2(0);
       header.set_scalepdf(0);
       header.set_weight(0);
       header.set_name("PYTHIA6 events");
       header.set_code(0);
       header.set_ecm(0);
       header.set_s(0);
       header.set_cross_section(0);	
       header.set_cross_section_error(0);

	//read in predefined header data
       readPDG( &header );

       epbook->setHeader(header);
 
}//Initialise

void PromcFortran::AddEvent(int proid, double scale)
{

	// Check vectors sizes consistent
	if ((int)m_Px.size() != (int)m_barcode.size() ||
		(int)m_Py.size() != (int)m_barcode.size() ||
		(int)m_Pz.size() != (int)m_barcode.size() ||
		(int)m_E.size()  != (int)m_barcode.size())
		std::cout << "RootTuple:: Warning: Inconsistent vector sizes" << std::endl;

	//Reset vectors
	ClearVectors();

	ProMCEvent promc;

	//fill event information
	ProMCEvent_Event *eve=promc.mutable_event();
	eve->set_number(1);		// all these zeroes to be filled later
	eve->set_process_id(proid);
	eve->set_scale(scale);
	eve->set_alpha_qed(0);
 	eve->set_alpha_qcd(0);
 	eve->set_scale_pdf(0);
 	eve->set_weight(m_weight);
 	eve->set_x1(0);
 	eve->set_x2(0);
 	eve->set_id1(0);
	eve->set_id2(0);


         // fill truth particle information 

	ProMCEvent_Particles *pa=promc.mutable_particles();

	for (int i=0; i<1000; i++) {

              //cout  << " " << m_Px[3] << endl;
              pa->add_pdg_id(m_barcode[i]);
              pa->add_status(m_status[i]);
              pa->add_px((int)(m_Px[i]*kEV));
              pa->add_py((int)(m_Py[i]*kEV));
              pa->add_pz((int)(m_Pz[i]*kEV));
              pa->add_mass((int)(m_mass[i]*kEV)); // we also keep masses in header 
              pa->add_mother1(m_mother1[i]);
              pa->add_mother2(m_mother2[i]);
              pa->add_daughter1(m_daughter1[i]);
              pa->add_daughter2(m_daughter2[i]);
              pa->add_barcode(i); // dummy 
              pa->add_id(0);
              pa->add_x(m_X[i]);
              pa->add_y(m_Y[i]);
              pa->add_z(m_Z[i]);
              pa->add_t(m_T[i]);
	}	

	epbook->write(promc);

}//AddEvent

void PromcFortran::Close()
{
        epbook->close();
}//Close

void PromcFortran::AddParticle(int barcode, double px, double py, double pz, double e, double mass, int status, int mother1, int mother2, int daughter1, int daughter2, double x, double y, double z, double t)
{
	m_barcode.push_back(barcode);
	m_Px.push_back(px);
	m_Py.push_back(py);
	m_Pz.push_back(pz);
	m_E.push_back(e);
	m_mass.push_back(mass);
	m_status.push_back(status);
	m_mother1.push_back(mother1);
	m_mother2.push_back(mother2);
	m_daughter1.push_back(daughter1);
	m_daughter2.push_back(daughter2);
        m_X.push_back(x);
        m_Y.push_back(y);
        m_Z.push_back(z);
	m_T.push_back(t);

}//AddParticle in Root file: promc is defined in AddEvent

void PromcFortran::SetStats(double cross, double crosserr, double lumi, int events) 
{
        ProMCStat stat;
        stat.set_cross_section_accumulated(cross); //pb
        stat.set_cross_section_error_accumulated(crosserr);
        stat.set_luminosity_accumulated(lumi);
        stat.set_ntried(events);
        stat.set_nselected(events);
        stat.set_naccepted(events);
        epbook->setStatistics(stat);
}

void PromcFortran::SetWeight(double weight)
{
	m_weight = weight;
}//SetWeight

//--------------------------------------------------------------
//                       Private methods
//--------------------------------------------------------------

void PromcFortran::ClearVectors()
{
	m_barcode.clear();
	m_Px.clear();
	m_Py.clear();
	m_Pz.clear();
	m_E.clear();
	m_mass.clear();
	m_status.clear();
	m_mother1.clear();
	m_mother2.clear();
	m_daughter1.clear();
	m_daughter2.clear();
        m_X.clear();
        m_Y.clear();
        m_Z.clear();
	m_T.clear();

}//ClearVectors



