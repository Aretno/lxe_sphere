#include "nain4.hh"
#include "g4-mandatory.hh"
#include "n4_ui.hh"
#include "n4-volumes.hh"

#include "geometry.hh"
#include "generator.hh"

#include <G4SystemOfUnits.hh>   // physical units such as `m` for metre
#include <G4GenericMessenger.hh>

#include <FTFP_BERT.hh>         // our choice of physics list
#include <G4EmStandardPhysics_option4.hh>
#include <G4OpticalPhysics.hh>
#include <G4RadioactiveDecayPhysics.hh>

#include <cstdlib>

void verify_number_of_args(int argc){
  if (argc != 2) {
    std::cerr << "Wrong number of arguments: " << argc
              << "\nUsage:\n./n4app <number of events>" << std::endl;
    std::exit(EXIT_FAILURE);
  }
}

n4::actions* create_actions(unsigned& n_event) {
  auto my_stepping_action = [&] (const G4Step* step) {
    auto pt = step -> GetPreStepPoint();
    auto volume_name = pt -> GetTouchable() -> GetVolume() -> GetName();
    if (volume_name == "straw" || volume_name == "bubble") {
      auto pos = pt -> GetPosition();
      std::cout << volume_name << " " << pos << std::endl;
    }
  };

  auto my_event_action = [&] (const G4Event*) {
     n_event++;
     std::cout << "end of event " << n_event << std::endl;
  };

  return (new n4::        actions{      my_generator})
 -> set( (new n4::   event_action{                  }) -> end(my_event_action) )
 -> set(  new n4::stepping_action{my_stepping_action} );
}


int main(int argc, char** argv)
{
	unsigned n_event = 0;
	
  // Physics lists
	G4int verbosity=0;
    auto physics_list = new FTFP_BERT{verbosity};
    physics_list ->  ReplacePhysics(new G4EmStandardPhysics_option4());
    physics_list -> RegisterPhysics(new G4OpticalPhysics{});
    physics_list -> RegisterPhysics(new G4RadioactiveDecayPhysics);

  // Messenger
	auto geom_messenger = new G4GenericMessenger(nullptr,"/geom/", "Geometry parameters");
	double sphere_rad_ = 20.*cm;
	geom_messenger->DeclareProperty("sphere_rad", sphere_rad_, "Sphere radius");
  //Lambda function to create geometry constructor with input parameters
  auto build_geometry = [sphere_rad_] () { return semisphere(sphere_rad_); };

  // Execute
	auto run_manager = n4::run_manager::create()	
	.physics(physics_list)
	.geometry(build_geometry)
  .actions(create_actions(n_event));

  	n4::ui(argc, argv);
}