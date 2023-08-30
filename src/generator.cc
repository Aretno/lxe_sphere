#include <G4RandomDirection.hh> // for launching particles in random directions
#include <G4Event.hh>           // needed to inject primary particles into an event

#include "nain4.hh"
#include "g4-mandatory.hh"
#include "n4_ui.hh"
#include "n4-utils.hh"
#include "n4-volumes.hh"

void my_generator(G4Event* event) {
  auto geantino = n4::find_particle("opticalphoton");
  auto vertex   = new G4PrimaryVertex();
  auto r = G4RandomDirection();
  vertex -> SetPrimary(new G4PrimaryParticle(geantino, r.x(), r.y(), r.z()));
  event  -> AddPrimaryVertex(vertex);
}