#ifndef LASERBARCODE_H_
#define LASERBARCODE_H_

#define PLAYER_ENABLE_TRACE 0
#define PLAYER_ENABLE_MSG 0

#include <errno.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>  // for atoi(3)
#include <netinet/in.h>  /* for htons(3) */
#include <unistd.h>

#include "libplayercore/playercore.h"

namespace Player{
// The laser barcode detector.
class LaserBarcode : public Driver
{
  // Constructor
  public: LaserBarcode();

  // Setup/shutdown routines
  //
  public: virtual int Setup();
  public: virtual int Shutdown();

  // Process incoming messages from clients
  int ProcessMessage (player_laser_data_t * data);

  // Main function for device thread
  //private: virtual void Main(void);

  // Get the laser data
  //private: int ReadLaser();

  // Analyze the laser data and return beacon data
  private: void FindBeacons(const player_laser_data_t *laser_data,
                            player_fiducial_data_t *beacon_data);

  // Analyze the candidate beacon and return its id (0 == none)
  private: int IdentBeacon(int a, int b, double ox, double oy, double oth,
                           const player_laser_data_t *laser_data);

  // Write fidicual data
  private: void WriteFiducial();

  // Magic numbers
  private: int bit_count;
  private: double bit_width;
  private: double max_depth;
  private: double accept_thresh, zero_thresh, one_thresh;

  // Current laser data
  private: player_laser_data_t laser_data;
  private: struct timeval laser_timestamp;

  // Current fiducial data
  public: player_fiducial_data_t data;
  unsigned int fdata_allocated;
};

}/* namespace Player */

#endif /* LASERBARCODE_H_ */
