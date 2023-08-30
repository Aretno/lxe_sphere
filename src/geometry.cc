#include "geometry.hh"

G4PVPlacement* semisphere(double sphere_rad) {
  //double sphere_rad   = 20.*cm;
  double vessel_thick = 5  *mm;
  double el_rad       = 38.*mm;
  double el_gap       = 5  *mm;
  double shield_gap   = 5  *mm;
  double sensor_gap   = 1  *cm;

  auto liquid     = n4::material("G4_lXe");
  auto air        = n4::material("G4_AIR");
  auto steel      = n4::material("G4_STAINLESS-STEEL");
  auto world      = n4::box("world").cube(0.5*m).volume(air);

  double vessel_rad = sphere_rad + vessel_thick;
  auto vessel = n4::sphere("vessel").r(vessel_rad)
                                    .r_inner(sphere_rad)
                                    .phi_delta(180*deg)
                                    .volume(steel);


  double holder_w = el_gap+shield_gap+sensor_gap;
  double holder_y = holder_w/2;
  auto   holder   = n4::tubs("holder").r(el_rad+vessel_thick)
                                      .r_inner(el_rad)
                                      .z(holder_w)
                                      .place(steel)
                                      .in(world)
                                      .rotate_x(90*deg)
                                      .at_y(holder_y)
                                      .check_overlaps()
                                      .now();

  double cap_w = vessel_thick;
  double cap_y = cap_w/2;
  auto cap = n4::tubs("cap").r(vessel_rad)
                            .r_inner(el_rad+vessel_thick)
                            .z(cap_w)
                            .place(steel)
                            .in(world)
                            .rotate_x(90*deg)
                            .at_y(cap_y)
                            .check_overlaps()
                            .now();

  n4::place(vessel).in(world)
                   .rotate_z(180*deg)
                   .at(0, 0, 0)
                   .now();

  n4::sphere("target").r(sphere_rad)
                      .phi_delta(180*deg)
                      .place(liquid)
                      .in(world)
                      .rotate_z(180*deg)
                      .at(0, 0, 0)
                      .now();

  double el_pos = el_gap/2;
  n4::tubs("el_gap").r(el_rad)
                    .z(el_gap)
                    .place(liquid)
                    .in(world)
                    .rotate_x(90*deg)
                    .at_y(el_pos)
                    .check_overlaps()
                    .now();

  double shield_pos = el_pos + el_gap/2 + shield_gap/2;
  n4::tubs("shield_gap").r(el_rad)
                        .z(shield_gap)
                        .place(liquid)
                        .in(world)
                        .rotate_x(90*deg)
                        .at_y(shield_pos)
                        .check_overlaps()
                        .now();

  double sensor_pos = shield_pos + shield_gap/2 + sensor_gap/2;
  n4::tubs("sensor_gap").r(el_rad)
                        .z(sensor_gap)
                        .place(liquid)
                        .in(world)
                        .rotate_x(90*deg)
                        .at_y(sensor_pos)
                        .check_overlaps()
                        .now();

  return n4::place(world).now();
}